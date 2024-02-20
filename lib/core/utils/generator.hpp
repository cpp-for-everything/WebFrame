#pragma once

#include <experimental/coroutine>

namespace webframe::utils {

    template<typename T>
    struct generator {
        
        struct promise_type;
        using handle_type = std::experimental::coroutine_handle<promise_type>;
        
        generator(handle_type h): coro(h) {}                     

        handle_type coro;
        
        ~generator() {  
            if ( coro ) coro.destroy();
        }
        generator(const generator&) = delete;
        generator& operator = (const generator&) = delete;
        generator(generator&& oth): coro(oth.coro) {
            oth.coro = nullptr;
        }
        generator& operator = (generator&& oth) {
            coro = oth.coro;
            oth.coro = nullptr;
            return *this;
        }
        T getNextValue() {
            coro.resume();
            return coro.promise().current_value;
        }
        struct promise_type {
            promise_type() {}                              
            
            ~promise_type() {}
            
            std::experimental::suspend_always initial_suspend() {            
                return {};
            }
            std::experimental::suspend_always final_suspend() noexcept {
                return {};
            }
            auto get_return_object() {      
                return generator{handle_type::from_promise(*this)};
            }
        
            std::experimental::suspend_always yield_value(const T value) {    
                current_value = value;
                return {};
            }
            void return_void() {}
            void unhandled_exception() {
                std::exit(1);
            }

            T current_value;
        };

    };
}