/* concurrency_cpp: Demonstration of modern multi-threading in C++
 * Copyright(C) 2020 Harish Naik <harishgnaik@gmail.com>
 *
 * This program is free software : you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <thread>
#include <utility>

#include <concurrency/concurrency>

namespace util = concurrency::core::utility;

template <typename DataType>
class Spsc_q {
public:
    void add(DataType&& data);
    void process();

private:
    mutable std::mutex m_;
    std::condition_variable cond_;
    std::queue<DataType> queue_;
};

template <typename DataType>
void Spsc_q<DataType>::add(DataType&& data)
{
    {
        std::lock_guard<std::mutex> lock{m_};
        queue_.push(std::forward<DataType>(data));
    }

    cond_.notify_one();
}

template <typename DataType>
void Spsc_q<DataType>::process()
{
    for(;;) {
        std::unique_lock<std::mutex> lock{m_};
        cond_.wait(lock, [this] { return !queue_.empty(); });
        auto data = queue_.front();
        queue_.pop();
        lock.unlock();

        std::cout << data << "\n";
    }
}

std::string make_message(int id)
{
    return "[producer thread id:" + util::thread_id_s() + "] message " +
           std::to_string(id);
}

template <typename Q>
void produce_logs(Q& q)
{
    for(int i = 0; i < 10; ++i) {
        q.add(concurrency::core::Logger{"INFO", make_message(i), true});
    }
}

int main(int argc, char** argv)
{
    util::print_preamble(argv[0], std::cout);

    Spsc_q<concurrency::core::Logger> log_q;
    std::thread th_c{[&log_q] { log_q.process(); }};
    std::thread th_p{[&log_q] { produce_logs(log_q); }};

    std::cout << "^C to exit ...\n";

    th_p.join();
    th_c.join();
}
