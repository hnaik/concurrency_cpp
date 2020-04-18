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

#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

#include "concurrency/core/utility.h"

namespace concurrency::core::utility {
constexpr const char* message = R"GNU_PREAMBLE(
Copyright (C) 2020  Harish Naik <harishgnaik@gmail.com>
This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
This is free software, and you are welcome to redistribute it
under certain conditions; type `show c' for details.
)GNU_PREAMBLE";

void print_preamble(const std::string& program_name, std::ostream& out)
{
    out << std::filesystem::path(program_name) << " " << message << "\n";
}

std::string thread_id_s()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

} // namespace concurrency::core::utility
