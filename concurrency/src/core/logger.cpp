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
#include <iomanip>
#include <ostream>
#include <string>
#include <thread>

#include "concurrency/core/logger.h"

namespace concurrency::core {
const Logger::steady_clock Logger::steady_startup_ =
    std::chrono::steady_clock::now();
const Logger::system_clock Logger::system_startup_ =
    std::chrono::system_clock::now();

Logger::Logger(const std::string& level, const std::string& msg,
               bool print_thread_id)
    : level_{level}
    , msg_{msg}
    , print_thread_id_{print_thread_id}
    , timestamp_{std::chrono::steady_clock::now()}

{
}

std::ostream& operator<<(std::ostream& os, const Logger& log)
{
    std::time_t t_c = std::chrono::system_clock::to_time_t(
        Logger::system_startup_ - (log.timestamp_ - Logger::steady_startup_));

    os << std::put_time(std::localtime(&t_c), "%F %T") << " [" << log.level_
       << "] ";

    if(log.print_thread_id_) {
        os << "[log thread id:" << std::this_thread::get_id() << "] ";
    }

    os << log.msg_;

    return os;
}

} // namespace concurrency::core
