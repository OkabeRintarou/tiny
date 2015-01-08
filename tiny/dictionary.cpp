#include "dictionary.h"

namespace tiny{

	Dictionary::Dictionary(){
		add_token("if",		std::make_tuple(TokenType::KEYWORDS, TokenValue::IF, -1));
		add_token("then",	std::make_tuple(TokenType::KEYWORDS, TokenValue::THEN, -1));
		add_token("else",	std::make_tuple(TokenType::KEYWORDS, TokenValue::ELSE, -1));
		add_token("end",	std::make_tuple(TokenType::KEYWORDS, TokenValue::END, -1));
		add_token("repeat", std::make_tuple(TokenType::KEYWORDS, TokenValue::REPEAT, -1));
		add_token("until",	std::make_tuple(TokenType::KEYWORDS, TokenValue::UNTIL, -1));
		add_token("read",	std::make_tuple(TokenType::KEYWORDS, TokenValue::READ, -1));
		add_token("write",	std::make_tuple(TokenType::KEYWORDS, TokenValue::WRITE, -1));

		add_token("+",		std::make_tuple(TokenType::OPERATORS, TokenValue::PLUS,					10));
		add_token("-",		std::make_tuple(TokenType::OPERATORS, TokenValue::MINUS,				10));
		add_token("*",		std::make_tuple(TokenType::OPERATORS, TokenValue::MULTIPLY,				20));
		add_token("/",		std::make_tuple(TokenType::OPERATORS, TokenValue::DIVIDE,				20));
		add_token("(",		std::make_tuple(TokenType::DELIMITER, TokenValue::LEFT_PARAM,			-1));
		add_token(")",		std::make_tuple(TokenType::DELIMITER, TokenValue::RIGHT_PARAM,			-1));
		add_token(";",		std::make_tuple(TokenType::DELIMITER, TokenValue::SEMICOLON,			-1));
		add_token(":=",		std::make_tuple(TokenType::OPERATORS, TokenValue::ASIGN,				 0));
		add_token("=",		std::make_tuple(TokenType::OPERATORS, TokenValue::EQUAL,				 1));
		add_token("<>",		std::make_tuple(TokenType::OPERATORS, TokenValue::NOT_EQUAL,			2));
		add_token("<=",		std::make_tuple(TokenType::OPERATORS, TokenValue::LESS_OR_EQUAL,		2));
		add_token("<",		std::make_tuple(TokenType::OPERATORS, TokenValue::LESS_THAN,			2));
		add_token(">=",		std::make_tuple(TokenType::OPERATORS, TokenValue::GREATER_OR_EQUAL,		2));
		add_token(">",		std::make_tuple(TokenType::OPERATORS, TokenValue::GREATER_THAN,			2));
	}

	void Dictionary::add_token(std::string name, std::tuple<TokenType, TokenValue, int> token)
	{
		dictionary_.insert(std::pair<decltype(name), decltype(token)>(name, token));
	}

	std::tuple<TokenType, TokenValue, int> Dictionary::lookup(std::string const & name)const
	{
		TokenType token_type = TokenType::IDENTIFIER;
		TokenValue token_value = TokenValue::UNRESERVED;
		int        precedence = -1;

		auto itr = dictionary_.find(name);
		if (itr != dictionary_.end()){
			token_type = std::get<0>(itr->second);
			token_value = std::get<1>(itr->second);
			precedence = std::get<2>(itr->second);
		}

		return std::make_tuple(token_type, token_value, precedence);
	}

	bool Dictionary::have_token(std::string const & name)const
	{
		return dictionary_.find(name) != dictionary_.end();
	}
}