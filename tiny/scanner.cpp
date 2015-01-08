
#include <cctype>
#include "scanner.h"
#include "error.h"

namespace tiny{
	Scanner::Scanner(std::string const & srcfile)
		: filename_(srcfile), line_(1), column_(0),
		current_char_(0), error_flag_(false),
		state_(State::NONE)
	{
		input_.open(srcfile);
		if (input_.fail()){
			errorToken(srcfile + " open failed.");
			error_flag_ = true;
		}
	}

	Scanner::~Scanner()
	{
		if (input_.is_open()){
			input_.close();
		}
	}

	void Scanner::get_next_char()
	{
		current_char_ = input_.get();
		if (current_char_ == '\n'){
			line_++;
			column_ = 0;
		}
		else{
			column_++;
		}
	}

	char Scanner::peek()
	{
		return input_.peek();
	}

	Token Scanner::get_next_token()
	{
		bool matched = false;

		do{

			if (state_ != State::NONE){
				matched = true;
			}
			
			switch (state_)
			{
			case State::NONE:
				get_next_char();
				break;
			case State::END_OF_FILE:
				handle_eof_state();
				break;
			case State::IDENTIFIRE:
				handle_identifier_state();
				break;
			case State::NUMBER:
				handle_number_state();
				break;
			case State::STRING:
				handle_string_state();
				break;
			case State::OPERATION:
				handle_operation_state();
				break;
			default:
				errorToken("Match token state error.");
				error_flag_ = true;
				break;
			}

			if (state_ == State::NONE){
				preprocess();

				if (input_.eof()){
					state_ = State::END_OF_FILE;
				}
				else{
					if (std::isalpha(current_char_)){
						state_ = State::IDENTIFIRE;
					}
					else if (std::isdigit(current_char_) || current_char_ == '$'){
						state_ = State::NUMBER;
					}
					else if (current_char_ == '\"'){
						state_ = State::STRING;
					}
					else{
						state_ = State::OPERATION;
					}
				}
			}

		} while (!matched);

		return token_;
	}

	void Scanner::preprocess()
	{
		do
		{
			while (std::isspace(current_char_))
			{
				get_next_char();
			}

			handle_line_comment();
			handle_block_comment();
		} while (std::isspace(current_char_));
	}

	void Scanner::handle_line_comment()
	{
		location_ = get_token_location();

		if (current_char_ == '(' && peek() == '*'){
			get_next_char();
			get_next_char();

			while (!(current_char_ == '*' && peek() == ')')){
				get_next_char();
				if (input_.eof()){
					errorToken(location_.to_string() + "end of file happended in comment, *) is excepted!,but found " + current_char_);
					error_flag_ = true;
					break;
				}
			}

			if (!input_.eof()){
				get_next_char();
				get_next_char();
			}
		}
	}

	void Scanner::handle_block_comment()
	{
		location_ = get_token_location();

		if (current_char_ == '{'){

			do 
			{
				get_next_char();
				if (input_.eof()){
					errorToken(location_.to_string() + "end of file happended in comment,} is excepted!,but found " + current_char_);
					error_flag_ = true;
					break;
				}
			} while (current_char_ != '}');

			if (!input_.eof()){
				get_next_char();
			}
		}
	}

	void Scanner::handle_eof_state()
	{
		location_ = get_token_location();
		make_token(TokenType::END_OF_FILE, TokenValue::UNRESERVED,
			location_, std::string("END OF FILE"), -1);
		buffer_.clear();
	}

	void Scanner::handle_identifier_state()
	{
		location_ = get_token_location();

		add_to_buffer(current_char_);
		get_next_char();

		while (std::isalnum(current_char_) || '_' == current_char_){
			add_to_buffer(current_char_);
			get_next_char();
		}

		auto token_info = dictionary_.lookup(buffer_);
		make_token(std::get<0>(token_info), std::get<1>(token_info), location_, buffer_, std::get<2>(token_info));

	}

	void Scanner::handle_number_state()
	{
		location_ = get_token_location();
		bool is_float = false;
		int base = 10;
		
		enum class NumberState
		{
			INTEGER,
			FRACTION,
			EXPONENT,
			DONE
		};

		NumberState number_state = NumberState::INTEGER;

		do{
			switch (number_state)
			{
				case NumberState::INTEGER:
					handle_integer();
					break;
				case NumberState::FRACTION:
					handle_fraction();
					is_float = true;
					break;
				case NumberState::EXPONENT:
					handle_exponent();
					is_float = true;
					break;
				case NumberState::DONE:
					break;
				default:
					errorToken("Match number state error!");
					error_flag_ = true;
					break;
			}

			if (current_char_ == '.'){
				number_state = NumberState::FRACTION;
			}
			else if (current_char_ == 'E' || current_char_ == 'e'){
				number_state = NumberState::EXPONENT;
			}
			else{
				number_state = NumberState::DONE;
			}

		} while (number_state != NumberState::DONE);

		if (!error_flag_){
			if (is_float){
				make_token(TokenType::REAL, TokenValue::UNRESERVED, location_, std::stod(buffer_), buffer_);
			}
			else{
				make_token(TokenType::INTEGER, TokenValue::UNRESERVED, location_, std::stol(buffer_, 0, base), buffer_);
			}
		}
		else{
			buffer_.clear();
			state_ = State::NONE;
		}
	}

	void Scanner::handle_string_state()
	{
		location_ = get_token_location();

		get_next_char();

		while (current_char_ != '\"'){
			add_to_buffer(current_char_);
			get_next_char();
		}

		// current_char_ = '\"'
		get_next_char();
		make_token(TokenType::STRING_LITERAL, TokenValue::UNRESERVED, location_,
			buffer_, -1);

	}

	void Scanner::handle_operation_state()
	{
		location_ = get_token_location();
		
		add_to_buffer(current_char_);
		add_to_buffer(peek());

		if (dictionary_.have_token(buffer_)){
			get_next_char();
		}
		else{
			reduce_buffer();
		}

		auto token_info = dictionary_.lookup(buffer_);
		make_token(std::get<0>(token_info), std::get<1>(token_info), location_, buffer_, std::get<2>(token_info));

		get_next_char();
	}

	void Scanner::handle_integer()
	{
		add_to_buffer(current_char_);
		get_next_char();

		while (std::isdigit(current_char_)){
			add_to_buffer(current_char_);
			get_next_char();
		}
		// current_char_ is not digit,but may be '.','E' or 'e'
		
	}

	void Scanner::handle_fraction()
	{
		add_to_buffer(current_char_);
		get_next_char();

		while (std::isdigit(current_char_)){
			add_to_buffer(current_char_);
			get_next_char();
		}

	}

	void Scanner::handle_exponent()
	{
		add_to_buffer(current_char_);
		get_next_char();

		if (current_char_ == '+' || current_char_ == '-'){
			add_to_buffer(current_char_);
			get_next_char();
		}

		while (std::isdigit(current_char_)){
			add_to_buffer(current_char_);
			get_next_char();
		}
	}

	void Scanner::add_to_buffer(char c)
	{
		buffer_.push_back(c);
	}

	void Scanner::reduce_buffer()
	{
		buffer_.pop_back();
	}

	void Scanner::make_token(TokenType token_type, TokenValue token_value, 
		TokenLocation const & token_loc, std::string name, int precedence)
	{
		token_ = Token(token_type, token_value, token_loc, name, precedence);
		buffer_.clear();
		state_ = State::NONE;
	}

	void Scanner::make_token(TokenType token_type, TokenValue token_value,
		TokenLocation const & token_loc, long int int_value, std::string name)
	{
		token_ = Token(token_type, token_value, token_loc, int_value, name);
		buffer_.clear();
		state_ = State::NONE;
	}

	void Scanner::make_token(TokenType token_type, TokenValue token_value,
		TokenLocation const & token_loc, double real_value, std::string name)
	{
		token_ = Token(token_type, token_value, token_loc, real_value, name);
		buffer_.clear();
		state_ = State::NONE;
	}

	Token Scanner::get_token()const
	{
		return token_;
	}

	bool Scanner::get_error_flag()const
	{
		return error_flag_;
	}

	TokenLocation Scanner::get_token_location()const
	{
		return TokenLocation(filename_, line_, column_);
	}
}
