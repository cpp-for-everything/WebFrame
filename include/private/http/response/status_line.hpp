#pragma once

#include <string>

#include "../predef.hpp"

namespace webframe::core {
	/**
	 *  @brief   Type of the status line of the response
	 *  @details This type represents the HTTP version and the HTTP code of the
	 *reponse
	 ***********************************************/
	struct status_line {
	public:
	private:
		std::string http;
		std::string code;
		std::string output;

		void rebuild_string();

	public:
		explicit status_line(const std::string& _code);

		status_line(const std::string& _http, const std::string& _code);

		const std::string& to_string() const;

		friend class response;
	};
}  // namespace webframe::core

namespace webframe::core {

	void status_line::rebuild_string() {
		output = "HTTP/" + this->http + " " + this->code + " " +
		         std::string(http_codes::get_reason_by_code(this->code.c_str())) + end_line.data();
	}

	status_line::status_line(const std::string& _code) : http("1.1"), code(_code) { rebuild_string(); }

	status_line::status_line(const std::string& _http, const std::string& _code) : http(_http), code(_code) {
		rebuild_string();
	}

	const std::string& status_line::to_string() const { return output; }

}  // namespace webframe::core
