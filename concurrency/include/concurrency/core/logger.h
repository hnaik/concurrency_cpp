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
#include <ostream>
#include <string>

namespace concurrency::core {
/**
 * Custom @c Logger class to demonstrate some of the threading concepts
 */
class Logger {
public:
    Logger(const std::string& level, const std::string& msg,
           bool print_thread_id = false);

    /**
     * Overload of the << operator
     */
    friend std::ostream& operator<<(std::ostream&, const Logger&);

private:
    std::string level_;
    std::string msg_;
    bool print_thread_id_;

    template <typename T>
    using tp = std::chrono::time_point<T>;
    using steady_clock = std::chrono::time_point<std::chrono::steady_clock>;
    using system_clock = std::chrono::time_point<std::chrono::system_clock>;

    steady_clock timestamp_;

    static const steady_clock steady_startup_;
    static const system_clock system_startup_;
};

} // namespace concurrency::core
