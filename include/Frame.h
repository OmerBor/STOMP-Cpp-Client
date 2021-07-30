#ifndef FRAME__
#define FRAME__
#include <iostream>
#include <vector>
#include <string>



 class Frame{
	
		
	
	public:
	Frame();
	Frame(std::string command, std::vector<std::string> headers);
	Frame(std::string command, std::vector<std::string> headers, std::string body);
	std::string getCommand();
	std::vector<std::string> getHeaders();
	std::string getBody();
	
	
	private:
		std::string command;
		std::vector<std::string> headers;
		std::string body;
				
	};
		
				



#endif