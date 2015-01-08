#include <iostream>
#include <fstream>
#include "scanner.h"

using namespace tiny;

int main()
{
	Scanner lex(std::string("sample.tny"));
	lex.get_next_token();

	while (lex.get_token().get_token_type() != tiny::TokenType::END_OF_FILE){

		if (lex.get_error_flag()){
			return 1;
		}
		lex.get_token().dump();
		lex.get_next_token();
	}
	return 0;
}