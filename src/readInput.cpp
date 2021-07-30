#include <stdlib.h>

#include <readInput.h>
#include <Frame.h>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
	 void readInput::terminateThread(){
		this->terminate=true;
	}

		
	 readInput::readInput(ConnectionHandler & connectionHandler, frameSTOMP & protocol):connectionHandler(connectionHandler),terminate(false),protocol(protocol){}
	 void readInput::run() {
			bool error=true;
			while(!terminate){
				Frame received=connectionHandler.getFrameSTOMP(error);
				Frame toSend=protocol.process(received);
				if(toSend.getCommand()=="CLOSE"){ //in case of logout or error
					connectionHandler.close();
					break;
				}
				else if(toSend.getCommand()!="NULL"){ //if there is a frame to send
					if (!connectionHandler.sendFrameSTOMP(toSend)) {
						break;
					}
				}
			}
	}
		
		
		
		
		
		
		
		
		
		
		
	
	

