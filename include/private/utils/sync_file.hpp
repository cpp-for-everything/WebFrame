/**
 *  @file   file.hpp
 *  @brief  Multi-thread-safe file structure
 *  @author Alex Tsvetanov
 *  @date   2022-03-07
 ***********************************************/

#pragma once

#include <iomanip>
#include <string>
#include <iostream>
#include <mutex>
#include <sstream>

namespace webframe::utils {
	/**
	 *  @brief   Multi-thread-safe file class
	 *  @details This type handle multithreading write requests to a given output
	 *stream (inc. files)
	 ***********************************************/

	class SyncStream {
	public:
		SyncStream(std::ostream* os = nullptr) : stream(os) {}
		void set(std::ostream* os = nullptr) { stream = os; }
		SyncStream(SyncStream&&) = default;

		// Overloaded insertion operator
		template <typename T>
		SyncStream& operator<<(const T& value) {
			std::lock_guard<std::mutex> lock(mtx);
			if (stream == nullptr) return *this;
			buffer << value;  // Write to the internal buffer
			return *this;
		}

		// Special handling for std::endl
		SyncStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
			std::lock_guard<std::mutex> lock(mtx);
			if (stream == nullptr) return *this;
			if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
				(*stream) << buffer.str();  // Flush the internal buffer to the stream
				buffer.str("");             // Clear the buffer
				buffer.clear();             // Reset any error flags
				(*stream) << manip;         // Apply the manipulator (flush)
			} else if (manip != static_cast<std::ostream& (*)(std::ostream&)>(std::flush)) {
				buffer << manip;  // Apply the manipulator
			}
			return *this;
		}

	private:
		std::ostream* stream;       // Reference to the output stream
		std::ostringstream buffer;  // Thread-local buffer
		std::mutex mtx;             // Mutex for synchronization
	};
}  // namespace webframe::utils