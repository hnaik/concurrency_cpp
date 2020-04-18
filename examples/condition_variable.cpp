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

#include "concurrency/core/utility.h"

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

class Logger {
public:
    Logger(const std::string& level, const std::string& msg);

    friend std::ostream& operator<<(std::ostream&, const Logger&);

private:
    std::string level_;
    std::string msg_;

    template <typename T>
    using tp = std::chrono::time_point<T>;

    tp<std::chrono::steady_clock> timestamp_;

    static const tp<std::chrono::steady_clock> steady_startup_;
    static const tp<std::chrono::system_clock> system_startup_;
};

const Logger::tp<std::chrono::steady_clock> Logger::steady_startup_ =
    std::chrono::steady_clock::now();
const Logger::tp<std::chrono::system_clock> Logger::system_startup_ =
    std::chrono::system_clock::now();

Logger::Logger(const std::string& level, const std::string& msg)
    : level_{level}
    , msg_{msg}
    , timestamp_{std::chrono::steady_clock::now()}
{
}

std::ostream& operator<<(std::ostream& os, const Logger& log)
{
    std::time_t t_c = std::chrono::system_clock::to_time_t(
        Logger::system_startup_ - (log.timestamp_ - Logger::steady_startup_));

    os << std::put_time(std::localtime(&t_c), "%F %T") << " [" << log.level_
       << "] [tid:" << std::this_thread::get_id() << "] " << log.msg_;
    return os;
}

template <typename Q>
void produce_logs(Q& q)
{
    for(int i = 0; i < 10; ++i) {
        std::stringstream ss;
        const auto msg =
            "[tid:" + util::thread_id_s() + "] message " + std::to_string(i);
        q.add(Logger{"INFO", msg});
    }
}

int main(int argc, char** argv)
{
    util::print_preamble(argv[0], std::cout);

    Spsc_q<Logger> log_q;
    std::thread th_c{[&log_q] { log_q.process(); }};
    std::thread th_p{[&log_q] { produce_logs(log_q); }};

    th_p.join();
    th_c.join();
}
