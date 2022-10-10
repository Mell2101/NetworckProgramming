#include <iostream>
#include <string>

#include "curlcpp/curl_easy.h"
#include "curlcpp/curl_form.h"
#include "curlcpp/curl_pair.h"
#include "curlcpp/curl_receiver.h"
#include "curlcpp/curl_exception.h"
#include "curlcpp/curl_sender.h"

using std::cout;
using std::endl;
using std::string;

using curl::curl_form;
using curl::curl_easy;
using curl::curl_sender;
using curl::curl_receiver;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

int main(int argc, const char* argv[]) {
    // Simple request
    string request = "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n";
    // Creation of easy object.
    curl_easy easy;
    try {
        easy.add<CURLOPT_URL>("http://<your_url_here>");
        // Just connect
        easy.add<CURLOPT_CONNECT_ONLY>(true);
        // Execute the request.
        easy.perform();

    }
    catch (curl_easy_exception& error) {
        // If you want to get the entire error stack we can do:
        curlcpp_traceback errors = error.get_traceback();
        // Otherwise we could print the stack like this:
        error.print_traceback();
    }

    // Creation of a sender. You should wait here using select to check if socket is ready to send.
    curl_sender<string> sender(easy);
    sender.send(request);
    // Prints che sent bytes number.
    cout << "Sent bytes: " << sender.get_sent_bytes() << endl;

    for (;;) {
        // You should wait here to check if socket is ready to receive
        try {
            // Create a receiver
            curl_receiver<char, 1024> receiver;
            // Receive the content on the easy handler
            receiver.receive(easy);
            // Prints the received bytes number.
            cout << "Receiver bytes: " << receiver.get_received_bytes() << endl;

        }
        catch (curl_easy_exception& error) {
            // If any errors occurs, exit from the loop
            break;
        }
    }
    return 0;
}