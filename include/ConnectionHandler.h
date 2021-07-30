#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <Frame.h> 
 #include  <boost/thread/mutex.hpp>
 #include <boost/thread/with_lock_guard.hpp>
  #include <boost/bind.hpp>

using boost::asio::ip::tcp;

class ConnectionHandler {

	
public:
	
    ConnectionHandler(std::string host, int port);
  
	ConnectionHandler(const ConnectionHandler &toCopy);
    // Connect to the remote machine
    bool connect();
 
    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is closed before bytesToRead bytes can be read.
    bool getBytes(char bytes[], unsigned int bytesToRead);
 
	// Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);
	
    
    bool getUntilDelimiter(std::string &frame, char delimiter);
	
	
   
    Frame getFrameSTOMP(bool & error);
 
    // Send a message to the remote host.
    // Returns false in case connection is closed before all the data is sent.
    bool sendFrameSTOMP(Frame frame);
	
    // Close down the connection properly.
    void close();
 
 
 private:
		
	
	boost::mutex mutex;
	 std::string host_;
	 int port_;
	boost::asio::io_service io_service_;  
	boost::asio::ip::tcp::socket  socket_;
 
 
 
}; 
 
#endif