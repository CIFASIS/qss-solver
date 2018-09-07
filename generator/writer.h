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
#include <boost/variant/variant.hpp>


namespace MicroModelica {
  namespace Generator {
    
    /**
    *
    */
    #define SECTIONS 	70
    /**
    *
    */
    typedef enum
    {
      ALLOC_LD,           //!< WR_ALLOC_LD
      ALLOC_LD_SD,        //!< WR_ALLOC_LD_SD
      ALLOC_LD_ALG_SD,        //!< WR_ALLOC_LD_ALG_SD
      ALLOC_LD_SZ,        //!< WR_ALLOC_LD_SZ
      ALLOC_LD_ALG_SZ,        //!< WR_ALLOC_LD_ALG_SZ
      ALLOC_LD_ZS,        //!< WR_ALLOC_LD_ZS
      ALLOC_LD_ALG_ZS,        //!< WR_ALLOC_LD_ALG_ZS
      ALLOC_LD_DS,        //!< WR_ALLOC_LD_DS
      ALLOC_LD_ALG_DS,        //!< WR_ALLOC_LD_ALG_DS
      ALLOC_LD_HD,        //!< WR_ALLOC_LD_HD
      ALLOC_LD_HZ,        //!< WR_ALLOC_LD_HZ
      ALLOC_LD_DD,        //!< WR_ALLOC_LD_DD
      ALLOC_LD_DH,        //!< WR_ALLOC_LD_DH
      ALLOC_STATE_HANDLERS,  //!< WR_ALLOC_STATE_HANDLERS
      ALLOC_EVENT_LHSST,  //!< WR_ALLOC_EVENT_LHSST
      ALLOC_EVENT_RHSST,  //!< WR_ALLOC_EVENT_RHSST
      ALLOC_EVENT_ALG_RHSST,  //!< WR_ALLOC_EVENT_ALG_RHSST
      ALLOC_EVENT_DSC,    //!< WR_ALLOC_EVENT_DSC
      ALLOC_OUTPUT_STATES,       //!< WR_ALLOC_OUTPUT_STATES
      ALLOC_OUTPUT_ALG_STATES,       //!< WR_ALLOC_OUTPUT_ALG_STATES
      ALLOC_OUTPUT_DSC,       //!< WR_ALLOC_OUTPUT_DSC
      ALLOC_OUTPUT_ALG_DSC,       //!< WR_ALLOC_OUTPUT_ALG_DSC
      GLOBAL_VARS,        //!< WR_GLOBAL_VARS
      INCLUDE,             //!< INCLUDE_
      INIT_LD,            //!< WR_INIT_LD
      INIT_LD_SD,         //!< WR_INIT_LD_SD
      INIT_LD_ALG_SD,         //!< WR_INIT_LD_ALG_SD
      INIT_LD_SZ,         //!< WR_INIT_LD_SZ
      INIT_LD_ALG_SZ,         //!< WR_INIT_LD_ALG_SZ
      INIT_LD_ZS,         //!< WR_INIT_LD_ZS
      INIT_LD_ALG_ZS,         //!< WR_INIT_LD_ALG_ZS
      INIT_LD_DS,         //!< WR_INIT_LD_DS
      INIT_LD_ALG_DS,         //!< WR_INIT_LD_ALG_DS
      INIT_LD_HD,         //!< WR_INIT_LD_HD
      INIT_LD_HZ,         //!< WR_INIT_LD_HZ
      INIT_LD_DD,         //!< WR_INIT_LD_DD
      INIT_LD_DH,         //!< WR_INIT_LD_DH
      INIT_EVENT,         //!< WR_INIT_EVENT
      INIT_STATE_HANDLERS,   //!< WR_INIT_STATE_HANDLERS
      INIT_EVENT_LHSST,   //!< WR_INIT_EVENT_LHSST
      INIT_EVENT_RHSST,   //!< WR_INIT_EVENT_RHSST
      INIT_EVENT_ALG_RHSST,   //!< WR_INIT_EVENT_ALG_RHSST
      INIT_EVENT_DSC,     //!< WR_INIT_EVENT_DSC
      INIT_OUTPUT,       //!< WR_INIT_OUTPUT
      INIT_OUTPUT_STATES, //!< WR_INIT_OUTPUT_STATES
      INIT_OUTPUT_ALG_STATES, //!< WR_INIT_OUTPUT_ALG_STATES
      INIT_OUTPUT_DSC,    //!< WR_INIT_OUTPUT_DSC
      INIT_OUTPUT_ALG_DSC,    //!< WR_INIT_OUTPUT_ALG_DSC
      INIT_TIME,          //!< WR_INIT_TIME
      INIT_TIME_STRUCT,   //!< WR_INIT_TIME_STRUCT
      INIT_CODE,          //!< WR_INIT_CODE
      START_CODE,          //!< WR_START_CODE
      INIT_VARS,          //!< WR_INIT_VARS
      MODEL_SIMPLE,       //!< WR_MODEL_SIMPLE
      MODEL_GENERIC,      //!< WR_MODEL_GENERIC
      MODEL_DEPS_SIMPLE,  //!< WR_MODEL_DEPS_SIMPLE
      MODEL_DEPS_GENERIC, //!< WR_MODEL_DEPS_GENERIC
      ZC_SIMPLE,          //!< WR_ZC_SIMPLE
      ZC_GENERIC,         //!< WR_ZC_GENERIC
      HANDLER_POS_SIMPLE, //!< WR_HANDLER_POS_SIMPLE
      HANDLER_POS_GENERIC, //!< WR_HANDLER_POS_GENERIC
      HANDLER_NEG_SIMPLE, //!< WR_HANDLER_NEG_SIMPLE
      HANDLER_NEG_GENERIC, //!< WR_HANDLER_NEG_GENERIC
      OUTPUT_SIMPLE,      //!< WR_OUTPUT_SIMPLE
      MODEL_JACOBIAN,      //!< WR_MODEL_JACOBIAN
      OUTPUT_GENERIC,     //!< WR_OUTPUT_GENERIC
      FUNCTION_HEADER,    //!< WR_FUNCTION_HEADER
      FUNCTION_CODE,      //!< WR_FUNCTION_CODE
      MODEL_HEADER,
      EMPTY                //!< WR_NULL
    } Section;

    typedef enum
    {
      APPEND_SIMPLE,
      APPEND_GENERIC,
      PREPEND
    } InsertType;


    class Writer
    {
      public:
        /**
        *
        */
        virtual
        ~Writer() {};
        /**
        *
        * @param fname
        */
        virtual void
        setFile(std::string fname) = 0;
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
        write(std::string str, Section section, InsertType it = PREPEND) = 0;
        /**
        *
        * @param s
        * @param section
        * @param clean
        */
        virtual void
        write(std::stringstream& s, Section section, bool clean = true, InsertType it = PREPEND) = 0;
        virtual void
        removeFromSection(std::string str, Section section) = 0;
        /**
        *
        * @param block
        * @param section
        */
        virtual void
        writeBlock(std::list<std::string> block, Section section) = 0;
        /**
        *
        * @param section
        */
        virtual void
        print(Section section) = 0;
        /**
        *
        * @param s
        */
        virtual void
        print(std::stringstream& s) = 0;
        /**
        *
        * @param s
        */
        virtual void
        print(std::string s) = 0;
        /**
        *
        * @param section
        * @return
        */
        virtual bool
        isEmpty(Section section) = 0;
        /**
        *
        * @param block
        */
        virtual void
        printBlock(std::list<std::string> block) = 0;
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
        virtual std::string
        indent() = 0;
        /**
        *
        * @param n
        * @return
        */
        virtual std::string
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
        beginBlock(Section s) = 0;
        /**
        *
        */
        virtual void
        endBlock() = 0;
        /**
        *
        */
        virtual void
        endBlock(Section s) = 0;
        /**
        *
        * @return
        */
        virtual std::string
        block() = 0;
        /**
        *
        * @param section
        */
        virtual void
        clear(Section section) = 0;
        /**
        *
        * @param section
        */
        virtual void
        newLine(Section section) = 0;
    };

    typedef std::shared_ptr<Writer> WriterPtr;

    /**
    *
    */
    class MemoryWriter: public Writer
    {
      public:
        /**
        *
        */
        MemoryWriter();
        /**
        *
        */
        ~MemoryWriter();
        /**
        *
        * @param fname
        */
        void
        setFile(std::string fname);
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
        write(std::string str, Section section, InsertType it = PREPEND);
        /**
        *
        * @param s
        * @param section
        * @param clean
        */
        void
        write(std::stringstream& s, Section section, bool clean = true, InsertType it = PREPEND);
        void
        removeFromSection(std::string str, Section section);
        /**
        *
        * @param block
        * @param section
        */
        void
        writeBlock(std::list<std::string> block, Section section);
        /**
        *
        * @param section
        */
        void
        print(Section section);
        /**
        *
        * @param s
        */
        void
        print(std::stringstream& s);
        /**
        *
        * @param s
        */
        void
        print(std::string s);
        /**
        *
        * @param section
        * @return
        */
        bool
        isEmpty(Section section);
        /**
        *
        * @param block
        */
        void
        printBlock(std::list<std::string> block);
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
        std::string
        indent();
        /**
        *
        * @param n
        * @return
        */
        std::string
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
        beginBlock(Section s);
        /**
        *
        */
        void
        endBlock();
        /**
        *
        */
        void
        endBlock(Section s);
        /**
        *
        * @return
        */
        std::string
        block();
        /**
        *
        * @param section
        */
        void
        newLine(Section section);
        /**
        *
        * @param section
        */
        void
        clear(Section section);
      private:
        std::list<std::string>            _sections[SECTIONS];
        std::ofstream                     _file;
        std::string                       _indentStr;
        std::string                       _block;
        int                               _indent;
        int                               _blockIndent;
        std::list<std::string>::iterator  _removeIt;
    };

    /**
    *
    */
    class FileWriter: public Writer
    {
      public:
        /**
        *
        */
        FileWriter();
        /**
        *
        */
        ~FileWriter();
        /**
        *
        * @param fname
        */
        void
        setFile(std::string fname);
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
        write(std::string str, Section section, InsertType it = PREPEND);
        /**
        *
        * @param s
        * @param section
        * @param clean
        */
        void
        write(std::stringstream& s, Section section, bool clean = true, InsertType it = PREPEND);
        void
        removeFromSection(std::string str, Section section);
        /**
        *
        * @param block
        * @param section
         */
        void
        writeBlock(std::list<std::string> block, Section section);
        /**
        *
        * @param section
        */
        void
        print(Section section);
        /**
        *
        * @param s
        */
        void
        print(std::stringstream& s);
        /**
        *
        * @param s
        */
        void
        print(std::string s);
        /**
         *
        * @param section
        * @return
        */
        bool
        isEmpty(Section section);
        /**
        *
        * @param block
        */
        void
        printBlock(std::list<std::string> block);
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
        std::string
        indent();
        /**
        *
        * @param n
        * @return
        */
        std::string
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
        beginBlock(Section s);
        /**
        *
        */
        void
        endBlock();
        /**
        *
        */
        void
        endBlock(Section s);
        /**
        *
        * @return
        */
        std::string
        block();
        /**
        *
        * @param section
        */
        void
        newLine(Section section);
        /**
        *
        * @param section
        */
        void
        clear(Section section);
      private:
        std::ofstream _sections[SECTIONS];
        std::ofstream _file;
        std::string _indentStr;
        std::string _block;
        int _indent;
        int _blockIndent;
    };

    typedef boost::variant<
      MemoryWriter,
      FileWriter 
      > WriterType;
  }
}

#endif  /* MMO_WRITER_H_ */
