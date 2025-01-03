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


#pragma once

#include <fstream>
#include <string>

namespace MicroModelica {
namespace Util {

#define LOG Logger::instance().log

class Logger {
  public:
  static Logger& instance()
  {
    static Logger _instance;
    return _instance;
  }
  ~Logger();

  void setFile(std::string file_name);

  std::string getLoggerFile() const;
  std::string getLogsPath() const;

  std::ofstream log;
  
  protected:
  std::string _log_file;
  std::string _logs_path;

  private:
  Logger();
};
}  // namespace Util
}  // namespace MicroModelica