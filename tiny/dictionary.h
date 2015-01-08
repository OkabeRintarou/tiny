#ifndef TINY_DICTIONARY_H_
#define TINY_DICTIONARY_H_

#include <map>
#include <tuple>
#include <string>
#include "token.h"

namespace tiny{

	class Dictionary
	{
	public:
		Dictionary();
		std::tuple<TokenType, TokenValue, int> lookup(std::string const & name)const;
		bool have_token(std::string const & name)const;
	private:
		void add_token(std::string name, std::tuple<TokenType, TokenValue, int> token);
	private:
		std::map<std::string, std::tuple<TokenType, TokenValue, int>> dictionary_;
	};
}
#endif