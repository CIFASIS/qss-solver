/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "logger.h"

#include <util/util.h>

using namespace std;

namespace MicroModelica {
namespace Util {
Logger::Logger() : _log_file(), _logs_path() {}

Logger::~Logger() { log.close(); }

void Logger::setFile(string file_name) 
{
    string build_path = Utils::instance().environmentVariable("MMOC_BUILD");
    string base_name = Utils::instance().baseFileName(file_name);
    _logs_path = build_path + SLASH + base_name + SLASH; 
    _log_file = _logs_path + base_name + ".log";
    log.open(_log_file, std::ofstream::out);
}

std::string Logger::getLoggerFile() const  { return _log_file; }

std::string Logger::getLogsPath() const  { return _logs_path; }

}  // namespace Util
}  // namespace MicroModelica
