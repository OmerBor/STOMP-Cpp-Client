#include <frameSTOMP.h>

	frameSTOMP::frameSTOMP():host(),port(0),username(),password(),subscriptionId(0),booksIneventoryMap(),
			genreIdMap(),recieptIdMap(),booksLendingMap(),waitingForBorrow(),logoutId(-1),terminate(false){}

	
	std::vector<std::string> stringToVector(std::string  frameString){
		  std::vector<std::string> output;
		  std::string toAdd;
		  std::istringstream ss(frameString); 
		  while(ss>>toAdd)
			  output.push_back(toAdd);
		    return output;
	}
		std::string vectorToString(std::vector<std::string> frameVector,unsigned int index){
			std::string output;
			for(unsigned int m=index;m<frameVector.size();++m){
				if (m==index) output+=frameVector.at(m);
				else output+=(" "+frameVector.at(m));
				
			}
			return output;
	}
	std::string vectorToString(std::vector<std::string> frameVector,unsigned int index,std::string toStop){
			std::string output;
			for(unsigned int m=index;m<frameVector.size();++m){
				if(frameVector.at(m)==toStop) return output;
				if (m==index) output+=frameVector.at(m);
				else output+=(" "+frameVector.at(m));
			}
			return output;
	}

	
	bool frameSTOMP::hasBook(std::string book,std::string destination){
		bool output=false;
		auto search=booksIneventoryMap.find(destination);
		 if (search != booksIneventoryMap.end()) {
				auto search2=booksIneventoryMap[destination].find(book);
				if (search2 != booksIneventoryMap[destination].end()){
										output=true;
				}
		}
		if(!output) output=hasLendBook(book, destination);
		return output;
		
	}
	
	bool frameSTOMP::hasLendBook(std::string book,std::string destination){
		bool output=false;
		auto search=booksLendingMap.find(destination);
		 if (search != booksLendingMap.end()) {
				auto search2=booksLendingMap[destination].find(book);
				if (search2 != booksLendingMap[destination].end()){
										output=true;
				}
		}
		return output;
		
	}
	
	
	
	
	
	std::string frameSTOMP::findGenreById(int subscriptionId){
				for(auto it = genreIdMap.begin(); it != genreIdMap.end(); ++it) {
							if(it->second == subscriptionId) 
									return it->first;
				}
		 return "not found";
	}
		
		std::string frameSTOMP::findGenreByRecieptId(int recieptId){
				for(auto it = recieptIdMap.begin(); it != recieptIdMap.end(); ++it) {
							if(it->first == recieptId) 
									return it->second;
				}
		 return "not found";
	}

	Frame dontSend(){
		std::vector<std::string> headers;
		return Frame("NULL",headers);
	}
	
	Frame dontSendExit(){
		std::vector<std::string> headers;
		return Frame("EXIT",headers);
	}
	
	std::string frameSTOMP::findUserNameFromVector(std::vector<std::string> findFrom,std::string until)
	{
		std::string output="not found";
		for (unsigned int i=0;i<findFrom.size();++i) 
				if(findFrom[i]==until) 
					if(unsigned(i+1)<findFrom.size())
						return findFrom[i+1] ;
		return output;
	}
	
	Frame frameSTOMP::process(Frame frame){
		std::string command=frame.getCommand();
		std::vector <std::string> headers=frame.getHeaders();
		std::string body=frame.getBody();
		std::string subId;
		std::string destination;
		std::vector <std::string> vectorBody=stringToVector(frame.getBody());
			if(command== "MESSAGE") {
						for(unsigned int i=0;i<headers.size();++i)
								if(headers.at(i).find("subscription")!=std::string::npos)
										subId=headers.at(i).substr(headers.at(i).find(":")+1,std::string::npos);
							std::string destination=findGenreById(std::stoi(subId));
							std::cout<<destination<<":"<<body<<std::endl; //print as required {topic} {content}
							if(	body.find(":")!=std::string::npos ) return dontSend();
							if(vectorBody.at(0)=="Taking") {
								std::string sentUser= findUserNameFromVector(vectorBody,"from");
								if(sentUser==username){
										std::string book=vectorToString(vectorBody,1,"from");
										for(auto it = booksIneventoryMap[destination].begin(); it != booksIneventoryMap[destination].end();++it ) {
														if(it->first == book){
																it = booksIneventoryMap[destination].erase(it);
																			break;
														}
										}
								}
							}
							if(vectorBody.at(0)=="Returning") {
									std::string sentUser= findUserNameFromVector(vectorBody,"to");
									if(sentUser==username){
									std::string book=vectorToString(vectorBody,1,"to");
									booksIneventoryMap[destination][book]="owned"; 
									}
							}
							if((body=="Book status")|(body=="book status")) {
									return executeCommandAndGetFrame("print "+destination);
							}
							if((vectorBody.size()>1)&&( vectorBody.at(1) == "wish") ){
									std::string book = vectorToString(vectorBody,4);
									return executeCommandAndGetFrame("has "+destination+" "+book);  
							}
							if((vectorBody.size()>2)&&(vectorBody.at(1) == "has" )&(vectorBody.at(2) != "added" )){
									std::string book=vectorToString(vectorBody,2);
									if((vectorBody.at(0)!=username)&(waitingForBorrow==book)){
											waitingForBorrow="";
											return executeCommandAndGetFrame("take "+destination+" "+vectorBody.at(0)+" "+book);
									}
							}
			}
			if(command== "RECEIPT") {
						std::string recId=headers[0].substr(headers[0].find(":")+1);
						if(recId == std::to_string(logoutId))
									return Frame("CLOSE",headers);
						std::string genre=findGenreByRecieptId(std::stoi(recId));
						if(genre!="not found")
									std::cout<<"Joined club "<<genre<<std::endl;
						else {
									genre=findGenreById(std::stoi(recId));
									if(genre!="not found"){
											std::cout<<"Exited club "<<genre<<std::endl;
											for(auto it = genreIdMap.begin(); it != genreIdMap.end(); it++) 
													if(it->first == genre)
													{
															it = genreIdMap.erase(it);
														break;
													}
									}
						}
			}
			
			if(command== "ERROR") {
					std::string errorMsg;
					for(unsigned int i=0;i<headers.size();++i)
									if(headers.at(i).find("message")!=std::string::npos)
											errorMsg=headers.at(i).substr(headers.at(i).find(":")+1,std::string::npos);
					std::cout<<errorMsg<<std::endl;
					terminate=true;
					return Frame("CLOSE",headers);
			}
			if(command== "CONNECTED") {
					std::cout << "Login successful"<< std::endl;
			}	
				return dontSend();	
	}
	
	
	 Frame frameSTOMP::executeCommandAndGetFrame(std::string  frameString){
		
		std::vector<std::string> frameVector=stringToVector(frameString);
		if(terminate){
			return dontSendExit();	
			
		}
			
		std::string frameCommand=frameVector.at(0);
		if(frameCommand=="login") {
				return makeConnectFrame(frameVector);
		}
				else if((frameCommand=="add")|(frameCommand=="borrow")|(frameCommand=="return")|(frameCommand=="status")|(frameCommand=="take")|
					(frameCommand=="print")|(frameCommand=="has")) {
					return makeSendFrame(frameVector);
				}
					else if(frameCommand=="join") {
						return makeSubscribeFrame(frameVector);
					}
						else if(frameCommand=="exit") {
							return makeUnsubscribeFrame(frameVector);
						}
							else if(frameCommand=="logout") {
								return makeDisconnectFrame(frameVector);
							}
							else return Frame();
	}
	
	
			 std::string frameSTOMP::getHost(){return this->host;}
			 int frameSTOMP::getPort(){return this->port;}
			 std::string frameSTOMP::getUsername(){return this->username;}
			 std::string frameSTOMP::getPassword(){return this->password;}
		
			Frame frameSTOMP::makeConnectFrame(std::vector<std::string>  frameVector){
				
				std::string command="CONNECT";
				std::vector <std::string> headers;
				headers.push_back("accept-version:1.2");
				headers.push_back("host:stomp.cs.bgu.ac.il");
				std::string hostAndPort=frameVector.at(1);
				int indexSplit =hostAndPort.find(':');
				this->host=hostAndPort.substr(0,indexSplit);
				this->port=std::stoi(hostAndPort.substr(indexSplit+1,hostAndPort.size()));
				this->username=frameVector.at(2);
				this->password=frameVector.at(3);
				headers.push_back("login:"+username);
				headers.push_back("passcode:"+password);
				return Frame(command,headers);
			}
						
	
		Frame frameSTOMP::makeSendFrame(std::vector<std::string>  frameVector){
				//add | borrow | return | status | take - for borrow reply | has- for borrow reply  | print -for status reply  
				std::string command="SEND";
				std::vector <std::string> headers;
				std::string body;
				std::string destination=frameVector.at(1);
				headers.push_back("destination:"+destination);
				if(frameVector.at(0)=="add"){
					std::string book = vectorToString(frameVector,2);
					body=username+" has added the book "+book;
					booksIneventoryMap[destination][book]="owned";
				}
				else if(frameVector.at(0)=="borrow"){
							std::string book=vectorToString(frameVector,(2));
							body=username+" wish to borrow "+book;
							waitingForBorrow=book;
				}
				else if(frameVector.at(0)=="return"){ 
						
						std::string book=vectorToString(frameVector,(2));
						 if(hasLendBook(book,destination)){ //if was lended
							body="Returning "+book+" to "+booksLendingMap[destination][book];
							for(auto it = booksLendingMap[destination].begin(); it != booksLendingMap[destination].end();++it ) {
									if(it->first == book){
											it = booksLendingMap[destination].erase(it);
												break;
									}
							}
							 if(hasBook(book,destination)){
							 for(auto it = booksIneventoryMap[destination].begin(); it != booksIneventoryMap[destination].end();++it ) {
									if(it->first == book){
											it = booksIneventoryMap[destination].erase(it);
												break;
									}	
							 }
							 }	
						 }
						 else if(hasBook(book,destination)){
							 body="Returning "+book+" to "+booksIneventoryMap[destination][book];
							 for(auto it = booksIneventoryMap[destination].begin(); it != booksIneventoryMap[destination].end();++it ) {
									if(it->first == book)
											it = booksIneventoryMap[destination].erase(it);
												break;
							}
							 
						}
						
						else return dontSend();	
				}
				else if(frameVector.at(0)=="status"){
							body="book status";
				
				}
				else if(frameVector.at(0)=="has"){ //has {genre} {book}
						
						std::string book=vectorToString(frameVector,2);
						if(hasBook(book,destination))
								body=username+" has "+book;
						else return dontSend();				
									
				}
				else if(frameVector.at(0)=="take"){ //take {genre} {other username) {book} 
						std::string book=vectorToString(frameVector,3);
						if(!hasBook(book,destination)){
							body="Taking "+book+" from "+frameVector.at(2);
							booksIneventoryMap[destination][book]=frameVector.at(2); //adding book
							booksLendingMap[destination][book]=frameVector.at(2);
						}
						else return dontSend();
				}
				else if(frameVector.at(0)=="print"){ //print {genre}
						std::string output=username+":";
						if(booksIneventoryMap.count(destination)){
						for (auto it=booksIneventoryMap[destination].begin(); it!=booksIneventoryMap[destination].end(); ++it)
								if(it==booksIneventoryMap[destination].begin())
										output+=it->first;
								else 											
									output+=","+ it->first ;
						}
						body=output;
						
				}
				
				return Frame(command,headers,body);
		}
				
						
		
		Frame frameSTOMP::makeSubscribeFrame(std::vector<std::string>  frameVector){
				std::string command="SUBSCRIBE";
				std::vector <std::string> headers;
				std::string destination=frameVector.at(1);
				auto search=genreIdMap.find(destination);
				if (search == genreIdMap.end()) {
				headers.push_back("destination:"+destination);
				genreIdMap[destination]=subscriptionId;
				headers.push_back("id:"+std::to_string(subscriptionId));
				++subscriptionId;
				recieptIdMap[subscriptionId]=destination;
				headers.push_back("receipt:"+std::to_string(subscriptionId));
				++subscriptionId;
				return  Frame(command,headers);
				}
				else return dontSend();				
			}
				
	
		
		Frame frameSTOMP::makeUnsubscribeFrame(std::vector<std::string>  frameVector){
				std::vector <std::string> headers;
				std::string command="UNSUBSCRIBE";
				std::string genre=frameVector.at(1);
				auto search=genreIdMap.find(genre);
				if (search != genreIdMap.end()) {
					std::string id=std::to_string(genreIdMap[genre]);
					headers.push_back("id:"+id);
					headers.push_back("receipt:"+id);
					return Frame(command,headers);
				}
				else return dontSend();
		}
					

			Frame frameSTOMP::makeDisconnectFrame(std::vector<std::string>  frameVector){
				std::string command="DISCONNECT";
				std::vector <std::string> headers;
				headers.push_back("receipt:"+std::to_string(subscriptionId));
				logoutId=subscriptionId;
				++subscriptionId;
				return Frame(command,headers);
			}
			
