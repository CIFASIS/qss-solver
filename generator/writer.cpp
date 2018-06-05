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

#include <sstream>
#include <iostream>

#include <generator/generator_utils.h>
#include "../util/util.h"

#define TAB "\t"

/* MMO_MemoryWriter class. */

MMO_MemoryWriter_::MMO_MemoryWriter_() :
    _indentStr(), _block(), _indent(0), _blockIndent(0)
{
  for(int i = 0; i < SECTIONS; i++)
  {
    _sections[i] = list<string>();
  }
}

void
MMO_MemoryWriter_::setFile(string fname)
{
  _file.open(fname.c_str());
  if(!_file.good())
  {
    cout << "Memory Writer: Can not open file " << fname << endl;
  }
}

void
MMO_MemoryWriter_::removeFromSection(string str, WR_Section section)
{
  list<string>::iterator it;
  list<string> rmv;
  _removeIt = _sections[section].end();
  for(it = _sections[section].begin(); it != _sections[section].end(); it++)
  {
    string fi = Util::getInstance()->trimString(*it);
    string cmp = Util::getInstance()->trimString(str);
    if(fi.compare("{") == 0)
    {
      _removeIt = it;
    }
    if(fi.compare(cmp) == 0)
    {
      rmv.push_back(*it);
      break;
    }
  }
  for(it = rmv.begin(); it != rmv.end(); it++)
  {
    _sections[section].remove(*it);
  }
}

void
MMO_MemoryWriter_::clear(WR_Section section)
{
  _sections[section].clear();
}

void
MMO_MemoryWriter_::clearFile()
{
  _file.close();
}

MMO_MemoryWriter_::~MMO_MemoryWriter_()
{
  _file.close();
}

void
MMO_MemoryWriter_::newLine(WR_Section section)
{
  _sections[section].push_back("");
}

void
MMO_MemoryWriter_::write(string str, WR_Section section, WR_InsertType it)
{
  if(!str.empty())
  {
    if(it == WR_PREPEND)
    {
      _sections[section].push_back(str);
    }
    else if(it == WR_APPEND_SIMPLE)
    {
      _sections[section].push_front(str);
    }
    else
    {
      _sections[section].insert(++_removeIt, str);
    }
  }
}

void
MMO_MemoryWriter_::write(stringstream *s, WR_Section section, bool clean,
    WR_InsertType it)
{
  if(!s->str().empty())
  {
    if(it == WR_PREPEND)
    {
      _sections[section].push_back(s->str());
    }
    else if(it == WR_APPEND_SIMPLE)
    {
      _sections[section].push_front(s->str());
    }
    else
    {
      _sections[section].insert(++_removeIt, s->str());
    }

    if(clean)
    {
      s->str("");
    }
  }
}

void
MMO_MemoryWriter_::writeBlock(list<string> block, WR_Section section)
{
  list<string>::iterator it;
  for(it = block.begin(); it != block.end(); it++)
  {
    _sections[section].push_back(*it);
  }
}

void
MMO_MemoryWriter_::print(WR_Section section)
{
  list<string>::iterator it;
  for(it = _sections[section].begin(); it != _sections[section].end(); it++)
  {
    _file << _block << _indentStr << *it << endl;
  }
}

void
MMO_MemoryWriter_::print(stringstream *s)
{
  _file << _block << _indentStr << s->str() << endl;
  s->str("");
}

void
MMO_MemoryWriter_::print(string s)
{
  _file << _block << _indentStr << s << endl;
}

bool
MMO_MemoryWriter_::isEmpty(WR_Section section)
{
  return _sections[section].empty();
}

void
MMO_MemoryWriter_::printBlock(list<string> block)
{
  list<string>::iterator it;
  for(it = block.begin(); it != block.end(); it++)
  {
    _file << _block << _indentStr << *it << endl;
  }
}

void
MMO_MemoryWriter_::setIndent(int n)
{
  stringstream s;
  _indent = n;
  for(int i = 0; i < _indent; i++)
  {
    s << TAB;
  }
  _indentStr = s.str();
}

string
MMO_MemoryWriter_::indent()
{
  return _indentStr;
}

string
MMO_MemoryWriter_::indent(int n)
{
  stringstream s;
  for(int i = 0; i < n; i++)
  {
    s << TAB;
  }
  return s.str();
}

void
MMO_MemoryWriter_::beginBlock()
{
  _block = indent(++_blockIndent);
}

void
MMO_MemoryWriter_::endBlock()
{
  _block = indent(--_blockIndent);
}

string
MMO_MemoryWriter_::block()
{
  return _block;
}

MMO_MemoryWriter
newMMO_MemoryWriter()
{
  return new MMO_MemoryWriter_();
}

void
deleteMMO_MemoryWriter(MMO_MemoryWriter m)
{
  delete m;
}
/* MMO_FileWriter class. */

MMO_FileWriter_::MMO_FileWriter_() :
    _indentStr(), _indent(0), _blockIndent(0)
{
  for(int i = 0; i < SECTIONS; i++)
  {
    stringstream name;
    name << "/tmp/temp" << i;
    _sections[i].open(name.str().c_str());
  }
}

MMO_FileWriter_::~MMO_FileWriter_()
{
  for(int i = 0; i < SECTIONS; i++)
  {
    stringstream name;
    name << "/tmp/temp" << i;
    _sections[i].close();
    remove(name.str().c_str());
  }
}

void
MMO_FileWriter_::removeFromSection(string str, WR_Section section)
{
  return;
}

void
MMO_FileWriter_::clear(WR_Section section)
{
  return;
}

void
MMO_FileWriter_::setFile(string fname)
{
  _file.open(fname.c_str());
}

void
MMO_FileWriter_::clearFile()
{
  _file.close();
}

void
MMO_FileWriter_::newLine(WR_Section section)
{
  _sections[section] << endl;
}

void
MMO_FileWriter_::write(string str, WR_Section section, WR_InsertType it)
{
  _sections[section] << str << endl;
}

void
MMO_FileWriter_::write(stringstream *s, WR_Section section, bool clean,
    WR_InsertType it)
{
  _sections[section] << s->str() << endl;
  if(clean)
  {
    s->str("");
  }
}

void
MMO_FileWriter_::writeBlock(list<string> block, WR_Section section)
{
  list<string>::iterator it;
  for(it = block.begin(); it != block.end(); it++)
  {
    _sections[section] << *it << endl;
  }
}

void
MMO_FileWriter_::print(WR_Section section)
{
  _file << _sections[section].rdbuf();
}

void
MMO_FileWriter_::print(stringstream *s)
{
  _file << s->str() << endl;
  s->str("");
}

void
MMO_FileWriter_::print(string s)
{
  _file << s << endl;
}

bool
MMO_FileWriter_::isEmpty(WR_Section section)
{
  return false;
}

void
MMO_FileWriter_::printBlock(list<string> block)
{
  list<string>::iterator it;
  for(it = block.begin(); it != block.end(); it++)
  {
    _file << *it << endl << endl;
  }
}

void
MMO_FileWriter_::setIndent(int n)
{
  stringstream s;
  _indent = n;
  for(int i = 0; i < _indent; i++)
  {
    s << TAB;
  }
  _indentStr = s.str();
}

string
MMO_FileWriter_::indent()
{
  return _indentStr;
}

string
MMO_FileWriter_::indent(int n)
{
  stringstream s;
  for(int i = 0; i < n; i++)
  {
    s << TAB;
  }
  return s.str();
}

void
MMO_FileWriter_::beginBlock()
{
  _block = indent(++_blockIndent);
}

void
MMO_FileWriter_::endBlock()
{
  _block = indent(--_blockIndent);
}

string
MMO_FileWriter_::block()
{
  return _block;
}

MMO_FileWriter
newMMO_FileWriter()
{
  return new MMO_FileWriter_();
}

void
deleteMMO_FileWriter(MMO_FileWriter m)
{
  delete m;
}
