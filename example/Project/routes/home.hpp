#ifndef _Project_home_route
#define _Project_home_route

#include <webframe.hpp>

auto home = webframe::core::router().route("/home", []() { return "This is my home page"; });

#endif