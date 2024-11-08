#pragma once

#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std::literals::string_view_literals;

namespace webframe::core {
	enum class method { undefined, GET, HEAD, POST, PUT, DDELETE, CONNECT, OPTIONS, TRACE, PATCH };

	/**
	 *  @brief    Casts webframe::method to const char*
	 *  @see      webframe::method
	 ***********************************************/
	const char* method_to_string(method m);

	/**
	 *  @brief    Casts const char* to webframe::method
	 *  @see      webframe::method
	 ***********************************************/
	method string_to_method(const char* m);
}  // namespace webframe::core