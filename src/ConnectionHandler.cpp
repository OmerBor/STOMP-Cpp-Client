#include <ConnectionHandler.h>
 #include <stdlib.h>


using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler(string host, int port):mutex(),host_(host), port_(port), io_service_(), socket_(boost::asio::ip::tcp::socket (io_service_)){}

bool ConnectionHandler::connect() {
 
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Could not connect to server"<< std::endl;
        return false;
    }

    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    
	size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
	int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
Frame ConnectionHandler::getFrameSTOMP(bool & error) {
	std::string command;
	std::string tmp;
	std::vector <std::string> headers;
	std::string body;
	bool advanceToRead=false;
	while((!advanceToRead)&&(getUntilDelimiter(command,'\n'))) {
			if(command[0]!='\n') advanceToRead=true;
			command=command.substr(0, command.size()-1); //removes \n
	}
	while(getUntilDelimiter(tmp, '\n')) { 
			if(tmp[0]=='\n'){
				break;
			}
			tmp=tmp.substr(0, tmp.size()-1);//removes \n
			headers.push_back(tmp);
			tmp="";
	}
	std::string s;
	if(getUntilDelimiter(s, '\0')) {
		if((!(s.empty())) &(s[0]!='\0'))
			body+=s;
	}
	else error=false;
	return Frame(command,headers,body);
}

bool ConnectionHandler::sendFrameSTOMP(Frame frame) {
		
	std::string toBytes;
	std::string EOL= std::string (1, char(10));
	std::string nullChar=std::string(1, char(0));
	toBytes=frame.getCommand()+EOL;     //command EOL command EOL
	std::vector<std::string> headers=frame.getHeaders();
	 	for (unsigned int i=0;i<headers.size();++i) { // *( header EOL )
		toBytes+=headers[i];
		toBytes+=EOL;
	}
	toBytes+=EOL;
	if(! (frame.getBody()).empty()) // //    *OCTET
		toBytes+=frame.getBody();
	toBytes+=nullChar;  //   NULL
	bool output=boost::with_lock_guard(mutex,boost::bind(&ConnectionHandler::sendBytes,this,toBytes.c_str(),toBytes.size()));
   	return output;
}
 


 bool ConnectionHandler::getUntilDelimiter(std::string &frame, char delimiter) { //reading one byte at a time (blocking) until delimiter is read
		char ch;
    try {
	do{
		if(!getBytes(&ch, 1))
		{
			return false;
		}
		if(ch!='\0')  {
			frame.append(1, ch);
		}
		}while (delimiter != ch);
    } catch (std::exception& e) {
	std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
	return false;
    }
    return true;
}
 

 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
    }
}

