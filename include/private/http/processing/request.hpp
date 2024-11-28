/**
 *  @file   base.hpp
 *  @brief  Basic utils to handle web requests and reponses
 *  @author Alex Tsvetanov
 *  @date   2022-03-07
 ***********************************************/

#pragma once

#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>

#include "../predef.hpp"
#include "../request/method.hpp"
#include "loading_state.hpp"

namespace webframe::core {
	/**
	 *  @brief   Type of the request
	 *  @details This type represents the HTTP version, the URL, the URL variables,
	 *all headers, and the body of the request
	 *  @see     webframe::path_vars
	 ***********************************************/
	class request {
	private:
		LoadingState loading;
		std::string remaining_to_parse;

		/**
		 *  @brief   Type of request prameters
		 *  @details This is the type of all parameters passed as request paramertes
		 *  @note    Ex. In 'url?param=value' param and value will be saved in the map
		 *as key and value.
		 ***********************************************/
		using req_vars = std::map<std::string, std::string>;

	public:
		method m;
		std::string uri;
		req_vars request_params;
		std::string http;
		std::map<std::string, std::string> header;
		std::string body;

	public:
		request();
		request(method _m, const std::string& h, const std::map<std::string, std::string>& m, const std::string& _body);

		LoadingState getState() const;

		LoadingState loadMore(const char* buff, const size_t n);
	};
}  // namespace webframe::core

namespace webframe::core {
	request::request()
	    : loading(LoadingState::NOT_STARTED), remaining_to_parse(""), m(method::undefined), uri(""), request_params({}),
	      http(""), header({}), body("") {}

	request::request(method _m, const std::string& h, const std::map<std::string, std::string>& m,
	                 const std::string& _body)
	    : loading(LoadingState::NOT_STARTED), m(_m), http(h), header(m), body(_body) {}

	LoadingState request::getState() const { return loading; }

	LoadingState request::loadMore(const char* buff, const size_t n) {
		if (n != 0) {
			size_t i = 0;
			if (loading == LoadingState::NOT_STARTED) {
				for (; i < n; i++) {
					if (buff[i] == ' ') {
						m = string_to_method(remaining_to_parse.c_str());
						loading = LoadingState::METHOD;
						remaining_to_parse.clear();
						i++;
						break;
					}
					remaining_to_parse += buff[i];
				}
			}
			if (loading == LoadingState::METHOD) {
				for (; i < n; i++) {
					if (buff[i] == ' ') {
						uri = std::move(remaining_to_parse);
						loading = LoadingState::HTTP_IN_PROGRESS;
						remaining_to_parse.clear();
						i++;
						break;
					}
					if (buff[i] == '?') {
						uri = std::move(remaining_to_parse);
						loading = LoadingState::PARAM_KEY;
						remaining_to_parse.clear();
						i++;
						break;
					}
					remaining_to_parse += buff[i];
				}
			}
			if (loading == LoadingState::PARAM_KEY or loading == LoadingState::PARAM_VALUE) {
				std::string var = "", val = "";
				std::string* x = nullptr;
				if (loading == LoadingState::PARAM_VALUE) x = &var;
				if (loading == LoadingState::PARAM_KEY) x = &val;
				if (x == nullptr) {
					throw std::logic_error("Null pointer exception.");
				}
				for (; i < n; i++) {
					if (buff[i] == '=') {
						(*x) = std::move(remaining_to_parse);
						remaining_to_parse.clear();
						loading = LoadingState::PARAM_KEY;
						x = &val;
						continue;
					}
					if (buff[i] == '&' || buff[i] == ' ') {
						(*x) = std::move(remaining_to_parse);
						remaining_to_parse.clear();
						loading = LoadingState::PARAM_VALUE;
						request_params[var] = val;
						var = val = "";
						x = &var;
					}
					if (buff[i] == ' ') {
						loading = LoadingState::HTTP_IN_PROGRESS;
						remaining_to_parse.clear();
						break;
					}
					remaining_to_parse += buff[i];
				}
			}
			if (loading == LoadingState::HTTP_IN_PROGRESS) {
				// skip " HTTP/" to reach "1.1"
				for (; i < n && !(buff[i] >= '0' && buff[i] <= '9') && buff[i] != '.'; i++) {
				}
				// read version
				for (; i < n; i++) {
					if (buff[i] == '\r') {
						http = std::move(remaining_to_parse);
						remaining_to_parse.clear();
						loading = LoadingState::HTTP_LOADED;
						i++;
						break;
					}
					remaining_to_parse += buff[i];
				}
			}
			if (loading == LoadingState::HTTP_LOADED || loading == LoadingState::HEADER_ROW) {
				if (buff[i] == '\n') i++;
				size_t splitter = remaining_to_parse.find(':');
				for (; i < n; i++) {
					loading = LoadingState::HEADER_ROW;
					for (; i < n; i++) {
						if (buff[i] == '\r') {
							i++;
							break;
						}
						if (buff[i] != ' ') remaining_to_parse += buff[i];
						if (buff[i] == ':') splitter = remaining_to_parse.size() - 1;
					}
					if (remaining_to_parse == "") {
						loading = LoadingState::BODY;
						break;
					}
					if (i != n) {
						header[remaining_to_parse.substr(0, splitter)] = remaining_to_parse.substr(splitter + 1);
						remaining_to_parse.clear();
					}
				}
			}
			if (loading == LoadingState::BODY) {
				body = std::move(remaining_to_parse) + std::string((char*)(buff + i + 1));
				remaining_to_parse.clear();
				if (header.find("Content-Length") == header.end() ||
				    body.size() >= (unsigned long long)atoll(header["Content-Length"].c_str()))
					loading = LoadingState::LOADED;
			}
		}
		return loading;
	}

}  // namespace webframe::core
