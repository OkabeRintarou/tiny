#include <iostream>
#include "error.h"

namespace tiny{
	void errorToken(std::string const & err){
		std::cerr << "Token Error : " << err << std::endl;
	}
}