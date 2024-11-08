#include <stdlib.h>

#include <core/core.hpp>
#include <iostream>
#include <sstream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	constexpr long long fasten = webframe::core::application::init();
	long long pass = fasten;
	webframe::core::application app;
	app.route("/{number}/2",
	          [&](long long steps) {
		          for (unsigned long long i = 0; i < (1ull << steps); i++) {
			          pass = pass + rand();
		          }
		          return "Hello World!";
	          })
	    .route("/{number}/1", [&](long long steps) {
		    volatile unsigned long long int test = 0;
		    for (unsigned long long i = 0; i < (1ull << steps); i++) {
			    test++;
		    }
		    return "Hello World!";
	    });
	const char* port = argv[1];
	std::cout << port << std::endl;
	app.run(port, 1).wait_end(port);
}
