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

#ifndef MMO_WRITER_H_
#define MMO_WRITER_H_

#include <boost/variant/variant.hpp>
#include <fstream>
#include <list>
#include <string>

#include "../util/util.h"

namespace MicroModelica {
namespace Generator {

/**
 *
 */
#define SECTIONS 36
namespace WRITER {
/**
 *
 */
typedef enum {
  Prologue,
  Epilogue,
  Alloc_Data,
  Init_Data,
  Alloc_Output,
  Include,
  Init_Output,
  Init_Code,
  Input,
  Model,
  Model_Simple,
  Model_Generic,
  Model_Deps,
  Model_Deps_Simple,
  Model_Deps_Generic,
  Model_Bdf,
  Model_Bdf_Simple,
  Zero_Crossing,
  ZC_Simple,
  ZC_Generic,
  Handler_Pos,
  Handler_Pos_Simple,
  Handler_Pos_Generic,
  Handler_Neg,
  Handler_Neg_Simple,
  Handler_Neg_Generic,
  Jacobian,
  Jacobian_Simple,
  Jacobian_Generic,
  Output,
  Output_Simple,
  Output_Generic,
  Function_Header,
  Function_Code,
  Model_Header,
  Empty
} Section;

typedef enum { Append_Simple, Append_Generic, Prepend } Insert;
}  // namespace WRITER

class Writer {
  public:
  virtual ~Writer(){};
  virtual void setFile(std::string fname) = 0;
  virtual void clearFile() = 0;

  template <class W>
  void write(W out, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend)
  {
    std::stringstream buffer;
    buffer << out;
    write(buffer, section, clean, it);
  }

  virtual void write(std::string str, WRITER::Section section, WRITER::Insert it = WRITER::Prepend) = 0;
  virtual void write(std::stringstream &s, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend) = 0;
  virtual void removeFromSection(std::string str, WRITER::Section section) = 0;
  virtual void writeBlock(std::list<std::string> block, WRITER::Section section) = 0;
  virtual void print(WRITER::Section section) = 0;
  virtual void print(std::stringstream &s) = 0;
  virtual void print(std::string s) = 0;
  virtual bool isEmpty(WRITER::Section section) = 0;
  virtual void printBlock(std::list<std::string> block) = 0;
  virtual void setIndent(int n) = 0;
  virtual std::string indent() = 0;
  virtual std::string indent(int n) = 0;
  virtual void beginBlock() = 0;
  virtual void beginBlock(WRITER::Section s) = 0;
  virtual void endBlock() = 0;
  virtual void endBlock(WRITER::Section s) = 0;
  virtual std::string block() = 0;
  virtual void clear(WRITER::Section section) = 0;
  virtual void newLine(WRITER::Section section) = 0;
  virtual void write(Util::SymbolTable symbols, WRITER::Section s) = 0;
};

typedef std::shared_ptr<Writer> WriterPtr;

/**
 *
 */
class MemoryWriter : public Writer {
  public:
  MemoryWriter();
  ~MemoryWriter();
  void setFile(std::string fname);
  void clearFile();
  void write(std::string str, WRITER::Section section, WRITER::Insert it = WRITER::Prepend);
  void write(std::stringstream &s, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend);
  void removeFromSection(std::string str, WRITER::Section section);
  void writeBlock(std::list<std::string> block, WRITER::Section section);
  void print(WRITER::Section section);
  void print(std::stringstream &s);
  void print(std::string s);
  bool isEmpty(WRITER::Section section);
  void printBlock(std::list<std::string> block);
  void setIndent(int n);
  std::string indent();
  std::string indent(int n);
  void beginBlock();
  void beginBlock(WRITER::Section s);
  void endBlock();
  void endBlock(WRITER::Section s);
  std::string block();
  void newLine(WRITER::Section section);
  void clear(WRITER::Section section);
  void write(Util::SymbolTable symbols, WRITER::Section s);

  private:
  std::list<std::string> _sections[SECTIONS];
  std::ofstream _file;
  std::string _indentStr;
  std::string _block;
  int _indent;
  int _blockIndent;
  std::list<std::string>::iterator _removeIt;
};

/**
 *
 */
class FileWriter : public Writer {
  public:
  FileWriter();
  ~FileWriter();
  void setFile(std::string fname);
  void clearFile();
  void write(std::string str, WRITER::Section section, WRITER::Insert it = WRITER::Prepend);
  void write(std::stringstream &s, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend);
  void removeFromSection(std::string str, WRITER::Section section);
  void writeBlock(std::list<std::string> block, WRITER::Section section);
  void print(WRITER::Section section);
  void print(std::stringstream &s);
  void print(std::string s);
  bool isEmpty(WRITER::Section section);
  void printBlock(std::list<std::string> block);
  void setIndent(int n);
  std::string indent();
  std::string indent(int n);
  void beginBlock();
  void beginBlock(WRITER::Section s);
  void endBlock();
  void endBlock(WRITER::Section s);
  std::string block();
  void newLine(WRITER::Section section);
  void clear(WRITER::Section section);
  void write(Util::SymbolTable symbols, WRITER::Section s);

  private:
  std::ofstream _sections[SECTIONS];
  std::ofstream _file;
  std::string _indentStr;
  std::string _block;
  int _indent;
  int _blockIndent;
};

typedef boost::variant<MemoryWriter, FileWriter> WriterType;
}  // namespace Generator
}  // namespace MicroModelica

#endif /* MMO_WRITER_H_ */
