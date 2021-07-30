#ifndef READ_INPUT__
#define READ_INPUT__

#include <stdlib.h>
#include <ConnectionHandler.h>
#include <boost/thread/thread.hpp>
 #include <frameSTOMP.h>



 class readInput{
		
	public:
	readInput(ConnectionHandler &handler,frameSTOMP & protocol);
	
	void run();
	void terminateThread();		
	
  private:
  ConnectionHandler & connectionHandler;
	bool terminate;
	frameSTOMP & protocol;

};


#endif