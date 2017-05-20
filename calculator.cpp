#include <iostream>
#include <string>
#include <regex>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "calculator.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		setlocale(LC_ALL, "Russian");
		std::cerr << "ошибка, недостаточно аргументов\n";
		return 1;
	}
	//std::string s = "-1 + 5 - 3";
	//std::string s = "-10 + (8 * 2.5) - (3 / 1,5)";
	//std::string s = "1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)";
	//std::string s = "1.1 + 2.1 + abc";
	std::string s = argv[1];
	
	try
	{
		std::cout << Parse(s) << '\n';
	}
	catch(boost::bad_lexical_cast& e)
	{
		setlocale(LC_ALL, "Russian");
		std::cerr << "синтаксическая ошибка в выражении\n";
		return 1;	
	}
	//---------------------------------------------

	return 0;
}
