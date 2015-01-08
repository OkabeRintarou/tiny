#ifndef _TINY_SCANNER_H_
#define _TINY_SCANNER_H_

#include <string>
#include <fstream>
#include "token.h"
#include "dictionary.h"

namespace tiny{
	class Scanner
	{
	public:
		explicit Scanner(std::string const & srcfile);
		~Scanner();
		Token  get_token()const;
		Token get_next_token();
		bool get_error_flag()const;
	private:
		void get_next_char();
		char peek();
		void preprocess();
		void handle_line_comment();
		void handle_block_comment();
		void handle_eof_state();
		void handle_identifier_state();
		void handle_number_state();
		void handle_string_state();
		void handle_operation_state();
		void handle_integer();
		void handle_fraction();
		void handle_exponent();
		void add_to_buffer(char c);
		void reduce_buffer();
		void make_token(TokenType token_type, TokenValue token_value, 
			TokenLocation const & token_loc, std::string name, int precedence);
		void make_token(TokenType token_type, TokenValue token_value,
			TokenLocation const & token_loc, long int int_value, std::string name);
		void make_token(TokenType token_type, TokenValue token_value,
			TokenLocation const & token_loc, double real_value, std::string name);

		TokenLocation get_token_location()const;
	public:
		enum class State
		{
			NONE,
			END_OF_FILE,
			IDENTIFIRE,
			NUMBER,
			STRING,
			OPERATION
		};
	private:

		std::string			filename_;
		std::fstream		input_;
		long				line_;
		long				column_;
		TokenLocation		location_;
		char				current_char_;
		bool				error_flag_;
		State				state_;
		Token				token_;
		Dictionary			dictionary_;
		std::string			buffer_;

	};
}
#endif
