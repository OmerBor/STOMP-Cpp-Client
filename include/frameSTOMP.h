#ifndef FRAME_STOMP__
#define FRAME_STOMP__


#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <Frame.h>

 class frameSTOMP{
	
		
	
	public:
	
	frameSTOMP();
	 Frame executeCommandAndGetFrame(std::string frameString);
	 std::string getHost();
	 int getPort();
	 std::string getUsername();
	 std::string getPassword();
	Frame process(Frame frame);
	
	private:
	Frame makeConnectFrame(std::vector<std::string>  frameVector);
	Frame makeSendFrame(std::vector<std::string>  frameVector);
	Frame makeSubscribeFrame(std::vector<std::string>  frameVector);
	Frame makeUnsubscribeFrame(std::vector<std::string>  frameVector);
	Frame makeDisconnectFrame(std::vector<std::string>  frameVector);
	bool hasBook(std::string book,std::string destination);
	bool hasLendBook(std::string book,std::string destination);
	std::string findGenreById(int subscriptionId);
	std::string findGenreByRecieptId(int recieptId);
	std::string findUserNameFromVector(std::vector<std::string> findFrom,std::string until);
		
		//fields----------------
		std::string host ;
		int port;
		std::string username;
		std::string password;
		int subscriptionId;
		std::unordered_map <std::string,std::unordered_map<std::string,std::string>> booksIneventoryMap;
		std::unordered_map <std::string,int> genreIdMap;
		std::unordered_map <int,std::string> recieptIdMap;
		std::unordered_map <std::string,std::unordered_map<std::string,std::string>> booksLendingMap;
		std::string waitingForBorrow;
		int logoutId;
		bool terminate;
	};
		
		
	
	



#endif