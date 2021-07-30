#include <stdlib.h>
#include <ConnectionHandler.h>
#include <readInput.h>
#include <frameSTOMP.h>
 #include  <boost/thread/mutex.hpp>
 #include <boost/thread/thread.hpp>
 #include <Frame.h>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/


int main () {
		bool terminate=false;
		ConnectionHandler * connectionHandler=0;
		frameSTOMP protocol;
		boost::thread* threadPointer=0;
		readInput* taskPointer=0;
	
		while(!terminate){
		
					const short bufsize = 1024;
					char buf[bufsize];	 
					std::cin.getline(buf, bufsize);
					std::string line(buf);
					Frame frame =protocol.executeCommandAndGetFrame(line);
					if(frame.getCommand()=="EXIT"){
						std::cout<<"exit after ERROR"<<std::endl;
						break;
					}
					if(frame.getCommand()=="CONNECT"){
							connectionHandler= new ConnectionHandler(protocol.getHost(), protocol.getPort());
							if (!connectionHandler->connect()) 
										break;
							taskPointer=new readInput(*connectionHandler,protocol);
							threadPointer=new boost::thread(&readInput::run, *taskPointer);
					}
					if(frame.getCommand()!="NULL"){
							if (!connectionHandler->sendFrameSTOMP(frame)) {
									break;
							}
					}
					if(frame.getCommand()=="DISCONNECT"){
							threadPointer->join();
							break;
					}
		}
	
	delete threadPointer;
	delete taskPointer;
	delete connectionHandler;
	
    return 0;
}
