#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdexcept>

class Exception : public std::runtime_error {
	public:
		Exception(const std::string & s) throw () : std::runtime_error(s) {}
};

#endif
