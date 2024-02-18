#pragma once

#include "../../c/host.h"

namespace webframe::core {
		struct thread_pool;
		struct procedure_thread;

		struct procedure_thread {
		private:
			mutable std::shared_mutex m;
			void unlock() { m.unlock(); }
			void lock() { m.lock(); }
			bool try_lock() { return m.try_lock(); }

		public:
			std::shared_ptr<SOCKET> requestor;

			procedure_thread() { requestor = std::make_shared<SOCKET>(); }

			void join(std::shared_ptr<std::function<void(SOCKET)>> f, SOCKET socket) {
				std::thread(
				    [this, f](SOCKET socket) {
					    const std::lock_guard<std::shared_mutex> lock_thread(
					        this->m);

					    f->operator()(socket);
				    },
				    socket)
				    .join();
			}

			void detach(std::shared_ptr<std::function<void(SOCKET)>> f,
			            SOCKET socket) {
				std::thread(
				    [this, f](SOCKET socket) {
					    const std::lock_guard<std::shared_mutex> lock_thread(
					        this->m);

					    f->operator()(socket);
				    },
				    socket)
				    .detach();
			}

			friend struct thread_pool;
		};

		struct thread_pool {
		private:
			size_t size;
			std::shared_ptr<std::vector<std::shared_ptr<procedure_thread>>>
			    pool;
			mutable std::shared_mutex extract;

		public:
			explicit thread_pool(size_t _size)
			    : size{_size},
			      pool{std::make_shared<
			          std::vector<std::shared_ptr<procedure_thread>>>(_size)} {
				for (size_t i = 0; i < _size; i++) {
					this->get(i) = std::make_shared<procedure_thread>();
				}
			}

			std::shared_ptr<procedure_thread>& get(const size_t index) const {
				std::lock_guard locker(this->extract);
				return pool->at(index);
			}

			std::shared_ptr<procedure_thread>& operator[](
			    const size_t index) const {
				return this->get(index);
			}

			std::optional<size_t> get_free_thread() {
				std::lock_guard locker(this->extract);
				for (size_t index = 0; index < this->size; index++) {
					if (pool->at(index)->try_lock()) {
						pool->at(index)->unlock();
						return index;
					}
				}
				return {};
			}
		};

}