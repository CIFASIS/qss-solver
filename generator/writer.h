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

#include "../util/util.h"

namespace MicroModelica {
  namespace Generator {
    
    /**
    *
    */
    #define SECTIONS 	72
    namespace WRITER {
      /**
      *
      */
      typedef enum
      {
        Alloc_Matrix,           //!< WR_Alloc_LD
        Alloc_Matrix_SD,        //!< WR_Alloc_LD_SD
        Alloc_Matrix_Alg_SD,        //!< WR_Alloc_LD_ALG_SD
        Alloc_Matrix_SZ,        //!< WR_Alloc_LD_SZ
        Alloc_Matrix_Alg_SZ,        //!< WR_Alloc_LD_ALG_SZ
        Alloc_Matrix_ZS,        //!< WR_Alloc_LD_ZS
        Alloc_Matrix_Alg_ZS,        //!< WR_Alloc_LD_ALG_ZS
        Alloc_Matrix_DS,        //!< WR_Alloc_LD_DS
        Alloc_Matrix_Alg_DS,        //!< WR_Alloc_LD_ALG_DS
        Alloc_Matrix_HD,        //!< WR_Alloc_LD_HD
        Alloc_Matrix_HZ,        //!< WR_Alloc_LD_HZ
        Alloc_Matrix_DD,        //!< WR_Alloc_LD_DD
        Alloc_Matrix_DH,        //!< WR_Alloc_LD_DH
        Alloc_State_Handlers,  //!< WR_ALLOC_STATE_HANDLERS
        Alloc_Event_LHSST,  //!< WR_ALLOC_EVENT_LHSST
        Alloc_Event_RHSST,  //!< WR_ALLOC_EVENT_RHSST
        Alloc_Event_Alg_RHSST,  //!< WR_ALLOC_EVENT_ALG_RHSST
        Alloc_Event_Discretes,    //!< WR_ALLOC_EVENT_DSC
        Alloc_Output_States,       //!< WR_Alloc_Output_StateS
        Alloc_Output_Alg_States,       //!< WR_ALLOC_OUTPUT_ALG_StateS
        Alloc_Output_Discretes,       //!< WR_ALLOC_OUTPUT_Discretes
        Alloc_Output_Alg_Discretes,       //!< WR_ALLOC_OUTPUT_ALG_Discretes
        Include,             //!< INCLUDE_
        Init_Matrix,            //!< WR_Init_LD
        Init_Matrix_SD,         //!< WR_Init_LD_SD
        Init_Matrix_Alg_SD,         //!< WR_Init_LD_ALG_SD
        Init_Matrix_SZ,         //!< WR_Init_LD_SZ
        Init_Matrix_Alg_SZ,         //!< WR_Init_LD_ALG_SZ
        Init_Matrix_ZS,         //!< WR_Init_LD_ZS
        Init_Matrix_Alg_ZS,         //!< WR_Init_LD_ALG_ZS
        Init_Matrix_DS,         //!< WR_Init_LD_DS
        Init_Matrix_Alg_DS,         //!< WR_Init_LD_ALG_DS
        Init_Matrix_HD,         //!< WR_Init_LD_HD
        Init_Matrix_HZ,         //!< WR_Init_LD_HZ
        Init_Matrix_DD,         //!< WR_Init_LD_DD
        Init_Matrix_DH,         //!< WR_Init_LD_DH
        Init_Event,         //!< WR_INIT_EVENT
        Init_State_Handlers,   //!< WR_INIT_STATE_HANDLERS
        Init_Event_LHSST,   //!< WR_INIT_EVENT_LHSST
        Init_Event_RHSST,   //!< WR_INIT_EVENT_RHSST
        Init_Event_Alg_RHSST,   //!< WR_INIT_EVENT_ALG_RHSST
        Init_Event_Discretes,     //!< WR_INIT_EVENT_DSC
        Init_Output,       //!< WR_INIT_OUTPUT
        Init_Output_States, //!< WR_INIT_OUTPUT_STATES
        Init_Output_Alg_States, //!< WR_INIT_OUTPUT_ALG_STATES
        Init_Output_Discretes,    //!< WR_INIT_OUTPUT_DSC
        Init_Output_Alg_Discretes,    //!< WR_INIT_OUTPUT_ALG_DSC
        Init_Time,          //!< WR_INIT_TIME
        Init_Code,          //!< WR_INIT_CODE
        Model,       //!< WR_MODEL_SIMPLE
        Model_Simple,       //!< WR_MODEL_SIMPLE
        Model_Generic,      //!< WR_MODEL_GENERIC
        Model_Deps_Simple,  //!< WR_MODEL_DEPS_SIMPLE
        Model_Deps,  //!< WR_MODEL_DEPS_SIMPLE
        Model_Deps_Generic, //!< WR_MODEL_DEPS_GENERIC
        Zero_Crossing,          //!< WR_ZC_SIMPLE
        ZC_Simple,          //!< WR_ZC_SIMPLE
        ZC_Generic,         //!< WR_ZC_GENERIC
        Handler_Pos,
        Handler_Pos_Simple, //!< WR_HANDLER_POS_SIMPLE
        Handler_Pos_Generic, //!< WR_HANDLER_POS_GENERIC
        Handler_Neg,
        Handler_Neg_Simple, //!< WR_HANDLER_NEG_SIMPLE
        Handler_Neg_Generic, //!< WR_HANDLER_NEG_GENERIC
        Jacobian,      //!< WR_MODEL_JACOBIAN
        Output,      //!< WR_Output_SIMPLE
        Output_Simple,      //!< WR_Output_SIMPLE
        Output_Generic,     //!< WR_Output_GENERIC
        Function_Header,    //!< WR_FUNCTION_HEADER
        Function_Code,      //!< WR_FUNCTION_CODE
        Model_Header,
        Empty                //!< WR_NULL
      } Section;

      typedef enum
      {
        Append_Simple,
        Append_Generic,
        Prepend
      } Insert;
    }
    
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
        write(std::string str, WRITER::Section section, WRITER::Insert it = WRITER::Prepend) = 0;
        /**
        *
        * @param s
        * @param section
        * @param clean
        */
        virtual void
        write(std::stringstream& s, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend) = 0;
        virtual void
        removeFromSection(std::string str, WRITER::Section section) = 0;
        /**
        *
        * @param block
        * @param section
        */
        virtual void
        writeBlock(std::list<std::string> block, WRITER::Section section) = 0;
        /**
        *
        * @param section
        */
        virtual void
        print(WRITER::Section section) = 0;
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
        isEmpty(WRITER::Section section) = 0;
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
        beginBlock(WRITER::Section s) = 0;
        /**
        *
        */
        virtual void
        endBlock() = 0;
        /**
        *
        */
        virtual void
        endBlock(WRITER::Section s) = 0;
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
        clear(WRITER::Section section) = 0;
        /**
        *
        * @param section
        */
        virtual void
        newLine(WRITER::Section section) = 0;
        virtual void 
        write(Util::SymbolTable symbols, WRITER::Section s) =  0;
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
        write(std::string str, WRITER::Section section, WRITER::Insert it = WRITER::Prepend);
        /**
        *
        * @param s
        * @param section
        * @param clean
        */
        void
        write(std::stringstream& s, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend);
        void
        removeFromSection(std::string str, WRITER::Section section);
        /**
        *
        * @param block
        * @param section
        */
        void
        writeBlock(std::list<std::string> block, WRITER::Section section);
        /**
        *
        * @param section
        */
        void
        print(WRITER::Section section);
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
        isEmpty(WRITER::Section section);
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
        beginBlock(WRITER::Section s);
        /**
        *
        */
        void
        endBlock();
        /**
        *
        */
        void
        endBlock(WRITER::Section s);
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
        newLine(WRITER::Section section);
        /**
        *
        * @param section
        */
        void
        clear(WRITER::Section section);
        void 
        write(Util::SymbolTable symbols, WRITER::Section s);
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
        write(std::string str, WRITER::Section section, WRITER::Insert it = WRITER::Prepend);
        /**
        *
        * @param s
        * @param section
        * @param clean
        */
        void
        write(std::stringstream& s, WRITER::Section section, bool clean = true, WRITER::Insert it = WRITER::Prepend);
        void
        removeFromSection(std::string str, WRITER::Section section);
        /**
        *
        * @param block
        * @param section
         */
        void
        writeBlock(std::list<std::string> block, WRITER::Section section);
        /**
        *
        * @param section
        */
        void
        print(WRITER::Section section);
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
        isEmpty(WRITER::Section section);
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
        beginBlock(WRITER::Section s);
        /**
        *
        */
        void
        endBlock();
        /**
        *
        */
        void
        endBlock(WRITER::Section s);
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
        newLine(WRITER::Section section);
        /**
        *
        * @param section
        */
        void
        clear(WRITER::Section section);
        void 
        write(Util::SymbolTable symbols, WRITER::Section s);
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
