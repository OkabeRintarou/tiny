#include "token.h"
#include <iomanip>

namespace tiny{
	TokenLocation::TokenLocation() :filename_(""), line_(1), column_(0)
	{}

	TokenLocation::TokenLocation(std::string filename, int line, int column)
		: filename_(filename), line_(line), column_(column)
	{}

	std::string TokenLocation::to_string()const
	{
		return filename_ + ":" + std::to_string(line_) + ":" + std::to_string(column_) + ":";
	}

	Token::Token() :
		type_(TokenType::UNKNOWN),
		value_(TokenValue::UNRESERVED),
		location_(TokenLocation(std::string(""), 0, 0)),
		name_(std::string("")),
		precedence_(-1)
	{}

	Token::Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc, std::string name, int precedence) :
		type_(token_type),
		value_(token_value),
		location_(token_loc),
		name_(name),
		precedence_(precedence)
	{}

	Token::Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc, std::string const & str_value, std::string name) :
		type_(token_type),
		value_(token_value),
		location_(token_loc),
		name_(name),
		str_value_(str_value),
		precedence_(-1)
	{}

	Token::Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc, long int_value, std::string name) :
		type_(token_type),
		value_(token_value),
		location_(token_loc),
		name_(name),
		int_value_(int_value),
		precedence_(-1)
	{}

	Token::Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc, double real_value, std::string name) :
		type_(token_type),
		value_(token_value),
		location_(token_loc),
		name_(name),
		real_value_(real_value)
	{}

	std::string Token::type_description()const
	{
		std::string buffer;
		switch (type_)
		{
		case TokenType::BOOLEAN:
			buffer = "boolean";
			break;
		case TokenType::CHAR:
			buffer = "char";
			break;
		case TokenType::DELIMITER:
			buffer = "delimiter";
			break;
		case TokenType::END_OF_FILE:
			buffer = "end of file";
			break;
		case TokenType::IDENTIFIER:
			buffer = "identifier";
			break;
		case TokenType::INTEGER:
			buffer = "integer";
			break;
		case TokenType::KEYWORDS:
			buffer = "keywords";
			break;
		case TokenType::OPERATORS:
			buffer = "operators";
			break;
		case TokenType::REAL:
			buffer = "real";
			break;
		case TokenType::STRING_LITERAL:
			buffer = "string literal";
			break;
		case TokenType::UNKNOWN:
			buffer = "unknow";
			break;
		default:
			break;
		}
		return buffer;
	}

	void Token::dump(std::ostream & os /* = std::cout */)const
	{
		os << location_.to_string() << std::setiosflags(std::ios::left) 
			<<  std::setw(10) << type_description() << "\t" << name_ << "\t\t" 
			<< get_token_precedence() << std::endl;
	}
}