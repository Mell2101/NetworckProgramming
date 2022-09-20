#include <iostream>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sslSocket;

class Session
{
    sslSocket _socket;
    enum { maxLenght = 1024 };
    char _data[maxLenght];
public:
    Session(boost::asio::io_service& ioService, boost::asio::ssl::context& context)
        : _socket(ioService, context) {}

    sslSocket::lowest_layer_type& socket()
    {
        return _socket.lowest_layer();
    }

    void start()
    {
        _socket.async_handshake(boost::asio::ssl::stream_base::server,
            boost::bind(&Session::handleHandshake, this,
                boost::asio::placeholders::error));
    }

    void handleHandshake(const boost::system::error_code& error)
    {
        if (!error)
        {
            _socket.async_read_some(boost::asio::buffer(_data, maxLenght),
                boost::bind(&Session::handleRead, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }

    void handleRead(const boost::system::error_code& error,
        size_t bytesTransferred)
    {
        if (!error)
        {
            boost::asio::async_write(_socket, boost::asio::buffer(_data, bytesTransferred),
                boost::bind(&Session::handleWrite, this, 
                    boost::asio::placeholders::error));
        }
        else
        {
            delete this;
        }
    }
    
    void handleWrite(const boost::system::error_code& error)
    {
        if (!error)
        {
            _socket.async_read_some(boost::asio::buffer(_data, maxLenght),
                boost::bind(&Session::handleRead, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
    }
};

class Server 
{
    boost::asio::io_service& _io_service;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ssl::context _context;
public:
    Server(boost::asio::io_service& io_service, unsigned short port)
        : _io_service(io_service), _acceptor(io_service,
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        _context(io_service, boost::asio::ssl::context::sslv23)
    {
        _context.set_options(
            boost::asio::ssl::context::default_workarounds
            | boost::asio::ssl::context::no_sslv2
            | boost::asio::ssl::context::single_dh_use);
        _context.set_password_callback(boost::bind(&Server::get_password, this));
        _context.use_certificate_chain_file("server.pem");
        _context.use_private_key_file("server.pem", boost::asio::ssl::context::pem);
        _context.use_tmp_dh_file("dh512.pem");

        Session* newSession = new Session(_io_service, _context);
        _acceptor.async_accept(newSession->socket(), boost::bind(&Server::handleAccept, this,
            newSession, boost::asio::placeholders::error));
    }

    std::string get_password() const
    {
        return "test";
    }

    void handleAccept(Session* newSession, const boost::system::error_code& error) 
    {
        if (!error)
        {
            newSession->start();
            newSession = new Session(_io_service, _context);
            _acceptor.async_accept(newSession->socket(),
                boost::bind(&Server::handleAccept, this, newSession,
                    boost::asio::placeholders::error));
        }
        else
        {
            delete newSession;
        }
    }

};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        using namespace std;
        Server s(io_service, atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

