#include <Frame.h>

	Frame::Frame():command(),headers(),body(){}
	
	Frame::Frame(std::string command, std::vector<std::string> headers):command(command),headers(headers),body(){}
	Frame::Frame(std::string command, std::vector<std::string> headers, std::string body):command(command),headers(headers),body(body){}
	std::string Frame::getCommand(){
		return this->command;
	}
	std::vector<std::string> Frame::getHeaders(){
		return this->headers;
	}
	std::string Frame::getBody(){
		return this->body;
	}
	
	
			
