/**
 *  @file   server_status.hpp
 *  @brief  The class will manage the status of the server ran on each port
 *  @author Alex Tsvetanov
 *  @date   2022-03-07
 ***********************************************/

#pragma once
#include "../http/predef.hpp"
#include <shared_mutex>

namespace webframe::utils {
class server_status {
public:
  server_status() {
    for (int i = 0; i < 65536; i++) {
      start[i] = dead[i] = nullptr;
    }
  }

private:
  using mutex = std::mutex;
  
  std::shared_ptr<mutex> start[65536];
  std::shared_ptr<mutex> dead[65536];
  std::shared_ptr<std::optional<size_t>> number_of_requests[65536];

public:
  void initiate(const char *PORT, std::optional<size_t> requests = std::nullopt) {
    if (this->get_start_ptr(PORT))
      throw std::ios_base::failure("start mutex is not cleaned up");
    if (this->get_end_ptr(PORT))
      throw std::ios_base::failure("end mutex is not cleaned up");
    this->get_start_ptr(PORT) = std::make_shared<mutex>();
    this->get_end_ptr(PORT) = std::make_shared<mutex>();
    this->get_number_of_requests_ptr(PORT) = std::make_shared<std::optional<size_t>>(requests);
    this->lock_working(PORT);
    this->lock_dead(PORT);
  }

  void alert_start(const char *PORT) { this->unlock_working(PORT); }

  void alert_end(const char *PORT) {
    if (this->get_remaining_number_of_requests(PORT).has_value()) 
      this->get_remaining_number_of_requests(PORT).value() = 0;
    else
      this->get_remaining_number_of_requests(PORT) = std::make_optional<size_t>(0);
  }

  void wait_end(const char *PORT) {
    this->unlock_dead(PORT);
  }

  bool is_over(const char *PORT) {
    bool locked = this->get_end(PORT).try_lock();
    if (!locked)
      return false;
    this->get_end(PORT).unlock();
    return true;
  }

  std::optional<size_t> &get_remaining_number_of_requests(const char *PORT) {
    return *this->number_of_requests[_compile_time::string_to_uint(PORT)];
  }

  mutex &get_start(const char *PORT) {
    return *this->start[_compile_time::string_to_uint(PORT)];
  }

  mutex &get_end(const char *PORT) {
    return *this->dead[_compile_time::string_to_uint(PORT)];
  }

  void reset(const char *PORT) {
    this->get_start_ptr(PORT) = nullptr;
    this->get_end_ptr(PORT) = nullptr;
  }

  std::shared_ptr<std::optional<size_t>>& get_number_of_requests_ptr(const char *PORT) {
    return this->number_of_requests[_compile_time::string_to_uint(PORT)];
  }

private:

  std::shared_ptr<mutex>& get_start_ptr(const char *PORT) {
    return this->start[_compile_time::string_to_uint(PORT)];
  }

  std::shared_ptr<mutex>& get_end_ptr(const char *PORT) {
    return this->dead[_compile_time::string_to_uint(PORT)];
  }

  void lock_working(const char *PORT) {
    this->start[_compile_time::string_to_uint(PORT)]->lock();
  }

  void unlock_working(const char *PORT) {
    this->start[_compile_time::string_to_uint(PORT)]->unlock();
  }

  void lock_dead(const char *PORT) {
    this->dead[_compile_time::string_to_uint(PORT)]->lock();
  }

  void unlock_dead(const char *PORT) {
    this->dead[_compile_time::string_to_uint(PORT)]->unlock();
  }
};
} // namespace webframe::utils