#pragma once

#include <stdexcept>
#include <string>
#include <vector>

namespace webframe::core {
	/**
	 *  @brief   Type of path prameters
	 *  @details This is the type of all variables passed as part of the unique URL
	 *  @note    Ex. In 'url/variable' the value of the variable will be saved in
	 *the variable.
	 ***********************************************/
	struct path_vars {
		struct var {
			std::string value;
			std::string type;

			var();

			explicit var(const std::string& _value);

			var(const std::string& _value, const std::string& _type);
			var(const std::pair<std::string, std::string>& details);

			const std::string& get() const;

			explicit operator int() const;
			explicit operator long long() const;
			explicit operator const char*() const;
			explicit operator char() const;
			explicit operator std::string() const;
			explicit operator const std::string&() const;
			template <typename T>
			explicit operator T&() const;
			template <typename T>
			explicit operator T() const;
		};
		path_vars();
		path_vars(std::initializer_list<var> l);

	private:
		std::vector<var> vars;

	public:
		const var operator[](long long unsigned int ind) const;
		path_vars& operator+=(const var& v);
		size_t size() const;
	};
}  // namespace webframe::core

namespace webframe::core {
	path_vars::var::var() : value(""), type("string") {}
	path_vars::var::var(const std::string& _value) : value(_value), type("string") {}
	path_vars::var::var(const std::string& _value, const std::string& _type) : value(_value), type(_type) {}

	path_vars::var::var(const std::pair<std::string, std::string>& details)
	    : value(details.first), type(details.second) {}

	const std::string& path_vars::var::get() const { return value; }

	path_vars::var::operator int() const {
		if (value.size() == 0) throw std::invalid_argument("path_vars::var::value is empty.");
		int ans = 0;
		if (value[0] == '-') ans = -(value[1] - '0');
		for (size_t i = (value[0] == '-'); i < value.size(); i++)
			if (value[i] >= '0' && value[i] <= '9')
				ans = ans * 10 + value[i] - '0';
			else
				throw std::invalid_argument(
				    "path_vars::var::value is not matching "
				    "path_vars::var::type (not integer)");
		return ans;
	}

	path_vars::var::operator long long() const {
		if (value.size() == 0) throw std::invalid_argument("path_vars::var::value is empty.");
		int ans = 0;
		if (value[0] == '-') ans = -(value[1] - '0');
		for (size_t i = (value[0] == '-'); i < value.size(); i++)
			if (value[i] >= '0' && value[i] <= '9')
				ans = ans * 10 + value[i] - '0';
			else
				throw std::invalid_argument(
				    "path_vars::var::value is not matching "
				    "path_vars::var::type (not integer)");
		return ans;
	}

	path_vars::var::operator const char*() const {
		if (value.size() == 0) throw std::invalid_argument("path_vars::var::value is empty.");
		return value.c_str();
	}

	path_vars::var::operator char() const {
		if (value.size() == 0) throw std::invalid_argument("path_vars::var::value is empty.");
		if (value.size() != 1) throw std::invalid_argument("path_vars::var::value is too long.");
		return value[0];
	}

	path_vars::var::operator std::string() const {
		if (value.size() == 0) throw std::invalid_argument("path_vars::var::value is empty.");
		return value;
	}

	path_vars::var::operator const std::string&() const {
		if (value.size() == 0) throw std::invalid_argument("path_vars::var::value is empty.");
		return value;
	}

	template <typename T>
	path_vars::var::operator T&() const {
		return T(value);
	}

	template <typename T>
	path_vars::var::operator T() const {
		return T(value);
	}

	path_vars::path_vars() {}
	path_vars::path_vars(std::initializer_list<path_vars::var> l) : vars(l) {}

	const path_vars::var path_vars::operator[](long long unsigned int ind) const { return vars[ind]; }

	path_vars& path_vars::operator+=(const var& v) {
		vars.push_back(v);
		return *this;
	}

	size_t path_vars::size() const { return vars.size(); }

}  // namespace webframe::core