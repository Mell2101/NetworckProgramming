// This file keeps a main class
#pragma once
#include "has.h"

class TalkToClient : public std::enable_shared_from_this<TalkToClient>, 
	boost::noncopyable
{	
// varibles	
	ip::tcp::socket _sock;
	enum { max_msg = 102};
	char _readBuffer[max_msg];
	char _writeBuffer[max_msg];
	bool _started;
	bool _clientChanged;
	std::string _username;
	deadline_timer _timer;
	boost::posix_time::ptime _lastPing;
//========
	typedef TalkToClient selfType;

	TalkToClient() : _sock(service), _started(false), _timer(service) {}

	std::size_t
		read_complete(const boost::system::error_code& err, std::size_t bytes)
	{
		if (err) return 0;

		bool found = std::find(_readBuffer, _readBuffer + bytes, '\n') 
			< _readBuffer + bytes;

		return found ? 0 : 1;
	}

public:
	typedef boost::system::error_code error_code;
	typedef std::shared_ptr<TalkToClient> ptr;

	static ptr _new()
	{
		ptr newClient(new TalkToClient);
		
		return newClient;
	}

	void start()
	{
		_started = true;
		clients.push_back(shared_from_this());
		_lastPing = boost::posix_time::microsec_clock::local_time();
		doRead();
	}

	void stop() 
	{
		if (!_started) return;
		_started = false;
		_sock.close();

		ptr self = shared_from_this();
		client_array::iterator it = std::find(clients.begin(), clients.end(), self);
		clients.erase(it);
		setClientChange();
	}

	bool get_started() const
	{
		return _started;
	}

	ip::tcp::socket& get_sock()
	{
		return _sock;
	}

	std::string get_username()
	{
		return _username;
	}

	void setClientChange() {
		_clientChanged = true;
	}


	void onRead(const error_code& err, std::size_t bytes)
	{
		if (err) stop();
		if (!get_started()) return;

		std::string msg(_readBuffer, bytes);

		if (msg.find("login") == 0) onLogin(msg);
		else if (msg.find("ping") == 0) onPing();
		else if (msg.find("Ask clients") == 0) onClients();
	}

	void onLogin(const std::string& msg)
	{
		std::istringstream in(msg);
		in >> _username >> _username;
		doWrite("login OK \n");
		setClientChange();
	}

	void onPing() {
		doWrite(_clientChanged ? "ping client list chahged \n" : "ping OK");
		_clientChanged = false;
	}
	
	void onClients()
	{
		std::string msg;
		for (client_array::const_iterator it = clients.begin(); it != clients.end(); ++it)
			msg += (*it)->get_username() + " ";
		doWrite("Clients" + msg + "\n");
	}

	void doPing(){
		doWrite("ping\n");
	}
	
	void doAskClients() {
		doWrite("ask_clients\n");
	}

	void onWrite(const error_code& err, std::size_t bytes)
	{
		doRead();
	}

	void doRead() 
	{
		async_read(_sock, buffer(_readBuffer),
			MEM_FN2(read_complete, std::placeholders::_1, std::placeholders::_2),
			MEM_FN2(onRead, std::placeholders::_1, std::placeholders::_2)
		);

		postCheckPing();
	}
	
	
	void doWrite(const std::string& msg)
	{
		if (!get_started()) return;

		std::copy(msg.begin(), msg.end(), _writeBuffer);
		_sock.async_write_some(buffer(_writeBuffer, msg.size()),
			MEM_FN2(onWrite, std::placeholders::_1, std::placeholders::_2));
	}

	void onCheckPing()
	{
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		
		if ((now - _lastPing).total_microseconds() > 5000) stop();
		_lastPing = boost::posix_time::microsec_clock::local_time();
	}

	void postCheckPing() {
		_timer.expires_from_now(boost::posix_time::milliseconds(5000));
		_timer.async_wait(MEM_FN(onCheckPing));
	}
};

