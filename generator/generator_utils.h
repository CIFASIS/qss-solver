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

#include <fstream>
#include <list>
#include <string>

#include "generator_types.h"

using namespace std;

/**
 *
 */
#define SECTIONS 	69
/**
 *
 */
typedef enum
{
  WR_ALLOC_LD,           //!< WR_ALLOC_LD
  WR_ALLOC_LD_SD,        //!< WR_ALLOC_LD_SD
  WR_ALLOC_LD_ALG_SD,        //!< WR_ALLOC_LD_ALG_SD
  WR_ALLOC_LD_SZ,        //!< WR_ALLOC_LD_SZ
  WR_ALLOC_LD_ALG_SZ,        //!< WR_ALLOC_LD_ALG_SZ
  WR_ALLOC_LD_ZS,        //!< WR_ALLOC_LD_ZS
  WR_ALLOC_LD_ALG_ZS,        //!< WR_ALLOC_LD_ALG_ZS
  WR_ALLOC_LD_DS,        //!< WR_ALLOC_LD_DS
  WR_ALLOC_LD_ALG_DS,        //!< WR_ALLOC_LD_ALG_DS
  WR_ALLOC_LD_HD,        //!< WR_ALLOC_LD_HD
  WR_ALLOC_LD_HZ,        //!< WR_ALLOC_LD_HZ
  WR_ALLOC_LD_DD,        //!< WR_ALLOC_LD_DD
  WR_ALLOC_LD_DH,        //!< WR_ALLOC_LD_DH
  WR_ALLOC_STATE_HANDLERS,  //!< WR_ALLOC_STATE_HANDLERS
  WR_ALLOC_EVENT_LHSST,  //!< WR_ALLOC_EVENT_LHSST
  WR_ALLOC_EVENT_RHSST,  //!< WR_ALLOC_EVENT_RHSST
  WR_ALLOC_EVENT_ALG_RHSST,  //!< WR_ALLOC_EVENT_ALG_RHSST
  WR_ALLOC_EVENT_DSC,    //!< WR_ALLOC_EVENT_DSC
  WR_ALLOC_OUTPUT_STATES,       //!< WR_ALLOC_OUTPUT_STATES
  WR_ALLOC_OUTPUT_ALG_STATES,       //!< WR_ALLOC_OUTPUT_ALG_STATES
  WR_ALLOC_OUTPUT_DSC,       //!< WR_ALLOC_OUTPUT_DSC
  WR_ALLOC_OUTPUT_ALG_DSC,       //!< WR_ALLOC_OUTPUT_ALG_DSC
  WR_GLOBAL_VARS,        //!< WR_GLOBAL_VARS
  WR_HEADER,             //!< WR_HEADER
  WR_INIT_LD,            //!< WR_INIT_LD
  WR_INIT_LD_SD,         //!< WR_INIT_LD_SD
  WR_INIT_LD_ALG_SD,         //!< WR_INIT_LD_ALG_SD
  WR_INIT_LD_SZ,         //!< WR_INIT_LD_SZ
  WR_INIT_LD_ALG_SZ,         //!< WR_INIT_LD_ALG_SZ
  WR_INIT_LD_ZS,         //!< WR_INIT_LD_ZS
  WR_INIT_LD_ALG_ZS,         //!< WR_INIT_LD_ALG_ZS
  WR_INIT_LD_DS,         //!< WR_INIT_LD_DS
  WR_INIT_LD_ALG_DS,         //!< WR_INIT_LD_ALG_DS
  WR_INIT_LD_HD,         //!< WR_INIT_LD_HD
  WR_INIT_LD_HZ,         //!< WR_INIT_LD_HZ
  WR_INIT_LD_DD,         //!< WR_INIT_LD_DD
  WR_INIT_LD_DH,         //!< WR_INIT_LD_DH
  WR_INIT_EVENT,         //!< WR_INIT_EVENT
  WR_INIT_STATE_HANDLERS,   //!< WR_INIT_STATE_HANDLERS
  WR_INIT_EVENT_LHSST,   //!< WR_INIT_EVENT_LHSST
  WR_INIT_EVENT_RHSST,   //!< WR_INIT_EVENT_RHSST
  WR_INIT_EVENT_ALG_RHSST,   //!< WR_INIT_EVENT_ALG_RHSST
  WR_INIT_EVENT_DSC,     //!< WR_INIT_EVENT_DSC
  WR_INIT_OUTPUT,       //!< WR_INIT_OUTPUT
  WR_INIT_OUTPUT_STATES, //!< WR_INIT_OUTPUT_STATES
  WR_INIT_OUTPUT_ALG_STATES, //!< WR_INIT_OUTPUT_ALG_STATES
  WR_INIT_OUTPUT_DSC,    //!< WR_INIT_OUTPUT_DSC
  WR_INIT_OUTPUT_ALG_DSC,    //!< WR_INIT_OUTPUT_ALG_DSC
  WR_INIT_TIME,          //!< WR_INIT_TIME
  WR_INIT_TIME_STRUCT,   //!< WR_INIT_TIME_STRUCT
  WR_INIT_CODE,          //!< WR_INIT_CODE
  WR_START_CODE,          //!< WR_START_CODE
  WR_INIT_VARS,          //!< WR_INIT_VARS
  WR_MODEL_SIMPLE,       //!< WR_MODEL_SIMPLE
  WR_MODEL_GENERIC,      //!< WR_MODEL_GENERIC
  WR_MODEL_DEPS_SIMPLE,  //!< WR_MODEL_DEPS_SIMPLE
  WR_MODEL_DEPS_GENERIC, //!< WR_MODEL_DEPS_GENERIC
  WR_ZC_SIMPLE,          //!< WR_ZC_SIMPLE
  WR_ZC_GENERIC,         //!< WR_ZC_GENERIC
  WR_HANDLER_POS_SIMPLE, //!< WR_HANDLER_POS_SIMPLE
  WR_HANDLER_POS_GENERIC, //!< WR_HANDLER_POS_GENERIC
  WR_HANDLER_NEG_SIMPLE, //!< WR_HANDLER_NEG_SIMPLE
  WR_HANDLER_NEG_GENERIC, //!< WR_HANDLER_NEG_GENERIC
  WR_OUTPUT_SIMPLE,      //!< WR_OUTPUT_SIMPLE
  WR_MODEL_JACOBIAN,      //!< WR_MODEL_JACOBIAN
  WR_OUTPUT_GENERIC,     //!< WR_OUTPUT_GENERIC
  WR_FUNCTION_HEADER,    //!< WR_FUNCTION_HEADER
  WR_FUNCTION_CODE,      //!< WR_FUNCTION_CODE
  WR_NULL                //!< WR_NULL
} WR_Section;

typedef enum
{
  WR_APPEND_SIMPLE,
  WR_APPEND_GENERIC,
  WR_PREPEND
} WR_InsertType;

class MMO_Writer_
{
  public:
    /**
     *
     */
    virtual
    ~MMO_Writer_()
    {
    }
    ;
    /**
     *
     * @param fname
     */
    virtual void
    setFile(string fname) = 0;
    /**
     *
     */
    virtual void
    clearFile() = 0;
    /**
     *
     * @param str
     * @param section
     */
    virtual void
    write(string str, WR_Section section, WR_InsertType it = WR_PREPEND) = 0;
    /**
     *
     * @param s
     * @param section
     * @param clean
     */
    virtual void
    write(stringstream *s, WR_Section section, bool clean = true,
        WR_InsertType it = WR_PREPEND) = 0;
    virtual void
    removeFromSection(string str, WR_Section section) = 0;
    /**
     *
     * @param block
     * @param section
     */
    virtual void
    writeBlock(list<string> block, WR_Section section) = 0;
    /**
     *
     * @param section
     */
    virtual void
    print(WR_Section section) = 0;
    /**
     *
     * @param s
     */
    virtual void
    print(stringstream *s) = 0;
    /**
     *
     * @param s
     */
    virtual void
    print(string s) = 0;
    /**
     *
     * @param section
     * @return
     */
    virtual bool
    isEmpty(WR_Section section) = 0;
    /**
     *
     * @param block
     */
    virtual void
    printBlock(list<string> block) = 0;
    /**
     *
     * @param n
     */
    virtual void
    setIndent(int n) = 0;
    /**
     *
     * @return
     */
    virtual string
    indent() = 0;
    /**
     *
     * @param n
     * @return
     */
    virtual string
    indent(int n) = 0;
    /**
     *
     */
    virtual void
    beginBlock() = 0;
    /**
     *
     */
    virtual void
    endBlock() = 0;
    /**
     *
     * @return
     */
    virtual string
    block() = 0;
    /**
     *
     * @param section
     */
    virtual void
    clear(WR_Section section) = 0;
    /**
     *
     * @param section
     */
    virtual void
    newLine(WR_Section section) = 0;
};

/**
 *
 */
class MMO_MemoryWriter_: public MMO_Writer_
{
  public:
    /**
     *
     */
    MMO_MemoryWriter_();
    /**
     *
     */
    ~MMO_MemoryWriter_();
    /**
     *
     * @param fname
     */
    void
    setFile(string fname);
    /**
     *
     */
    void
    clearFile();
    /**
     *
     * @param str
     * @param section
     */
    void
    write(string str, WR_Section section, WR_InsertType it = WR_PREPEND);
    /**
     *
     * @param s
     * @param section
     * @param clean
     */
    void
    write(stringstream *s, WR_Section section, bool clean = true,
        WR_InsertType it = WR_PREPEND);
    void
    removeFromSection(string str, WR_Section section);
    /**
     *
     * @param block
     * @param section
     */
    void
    writeBlock(list<string> block, WR_Section section);
    /**
     *
     * @param section
     */
    void
    print(WR_Section section);
    /**
     *
     * @param s
     */
    void
    print(stringstream *s);
    /**
     *
     * @param s
     */
    void
    print(string s);
    /**
     *
     * @param section
     * @return
     */
    bool
    isEmpty(WR_Section section);
    /**
     *
     * @param block
     */
    void
    printBlock(list<string> block);
    /**
     *
     * @param n
     */
    void
    setIndent(int n);
    /**
     *
     * @return
     */
    string
    indent();
    /**
     *
     * @param n
     * @return
     */
    string
    indent(int n);
    /**
     *
     */
    void
    beginBlock();
    /**
     *
     */
    void
    endBlock();
    /**
     *
     * @return
     */
    string
    block();
    /**
     *
     * @param section
     */
    void
    newLine(WR_Section section);
    /**
     *
     * @param section
     */
    void
    clear(WR_Section section);
    private:
    list<string> _sections[SECTIONS];
    ofstream _file;
    string _indentStr;
    string _block;
    int _indent;
    int _blockIndent;
    list<string>::iterator _removeIt;
};
/**
 *
 * @return
 */
MMO_MemoryWriter
newMMO_MemoryWriter();
/**
 *
 * @param m
 */
void
deleteMMO_MemoryWriter(MMO_MemoryWriter m);

/**
 *
 */
class MMO_FileWriter_: public MMO_Writer_
{
  public:
    /**
     *
     */
    MMO_FileWriter_();
    /**
     *
     */
    ~MMO_FileWriter_();
    /**
     *
     * @param fname
     */
    void
    setFile(string fname);
    /**
     *
     */
    void
    clearFile();
    /**
     *
     * @param str
     * @param section
     */
    void
    write(string str, WR_Section section, WR_InsertType it = WR_PREPEND);
    /**
     *
     * @param s
     * @param section
     * @param clean
     */
    void
    write(stringstream *s, WR_Section section, bool clean = true,
        WR_InsertType it = WR_PREPEND);
    void
    removeFromSection(string str, WR_Section section);
    /**
     *
     * @param block
     * @param section
     */
    void
    writeBlock(list<string> block, WR_Section section);
    /**
     *
     * @param section
     */
    void
    print(WR_Section section);
    /**
     *
     * @param s
     */
    void
    print(stringstream *s);
    /**
     *
     * @param s
     */
    void
    print(string s);
    /**
     *
     * @param section
     * @return
     */
    bool
    isEmpty(WR_Section section);
    /**
     *
     * @param block
     */
    void
    printBlock(list<string> block);
    /**
     *
     * @param n
     */
    void
    setIndent(int n);
    /**
     *
     * @return
     */
    string
    indent();
    /**
     *
     * @param n
     * @return
     */
    string
    indent(int n);
    /**
     *
     */
    void
    beginBlock();
    /**
     *
     */
    void
    endBlock();
    /**
     *
     * @return
     */
    string
    block();
    /**
     *
     * @param section
     */
    void
    newLine(WR_Section section);
    /**
     *
     * @param section
     */
    void
    clear(WR_Section section);
    private:
    ofstream _sections[SECTIONS];
    ofstream _file;
    string _indentStr;
    string _block;
    int _indent;
    int _blockIndent;
};
/**
 *
 * @return
 */
MMO_FileWriter
newMMO_FileWriter();
/**
 *
 * @param m
 */
void
deleteMMO_FileWriter(MMO_FileWriter m);

#endif  /* MMO_WRITER_H_ */
