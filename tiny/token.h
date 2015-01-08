#ifndef _TINY_TOKEN_H_
#define _TINY_TOKEN_H_

#include <iostream>
#include <string>

namespace tiny{
	enum class TokenType{
		INTEGER,		 // such as 123,456
		REAL,			 // such as 1.234
		BOOLEAN,		 // true or false
		CHAR,			 // such as 'a','b'
		STRING_LITERAL,	 // such as "hello world"

		IDENTIFIER,      // such as abc
		KEYWORDS,		 // such as if , else
		OPERATORS,       // such as +,-,*,/
		DELIMITER,		 // such as ,
		END_OF_FILE,     // end of file
		UNKNOWN
	};

	enum class TokenValue{
		// keyword
		IF,
		THEN,
		ELSE,
		END,
		REPEAT,
		UNTIL,
		READ,
		WRITE,

		// special symbols
		PLUS,				// +
		MINUS,				// -
		MULTIPLY,			// *
		DIVIDE,				// /
		LEFT_PARAM,			// (
		RIGHT_PARAM,		// )
		SEMICOLON,			// ;
		ASIGN,				// :=

		EQUAL,				// =
		NOT_EQUAL,			// <>
		LESS_OR_EQUAL,		// <=
		LESS_THAN,			// <
		GREATER_OR_EQUAL,	// >=
		GREATER_THAN,		// >

		UNRESERVED
	};

	class TokenLocation
	{
	public:
		TokenLocation();
		TokenLocation(std::string filename, int line, int column);

		std::string to_string()const;
	private:
		std::string filename_;
		int line_;
		int column_;
	};

	class Token
	{
	public:
		Token();
		Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc,
			std::string name, int precedence);
		Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc,
			std::string const & str_value, std::string name);
		Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc,
			long int_value, std::string name);
		Token(TokenType token_type, TokenValue token_value, TokenLocation token_loc,
			double real_value, std::string name);

		std::string type_description()const;
		void dump(std::ostream & os = std::cout)const;

		TokenType		get_token_type()const{ return type_; }
		TokenValue		get_token_value()const{ return value_; }
		TokenLocation	get_token_location()const{ return location_; }
		int				get_token_precedence()const{ return precedence_; }
		std::string		get_token_name()const{ return name_; }
		long			get_int_value()const{ return int_value_; }
		double			get_real_value()const{ return real_value_; }
		std::string		get_string_value()const{ return str_value_; }
	private:
		TokenType		type_;
		TokenValue		value_;
		TokenLocation	location_;
		int				precedence_;
		std::string		name_;

		long			int_value_;
		double			real_value_;
		std::string		str_value_;
	};
}

#endif