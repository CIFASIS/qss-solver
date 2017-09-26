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

#ifndef ERROR_H_
#define ERROR_H_

#include <map>
#include <string>

using namespace std;

/**
 *
 */
#define EM_MODULE 	0xffff0000
/**
 *
 */
#define EM_MESSAGE 	0x0000ffff
/**
 *
 */
#define EM_AST 		0x00010000
/**
 *
 */
#define EM_IR 		0x00020000
/**
 *
 */
#define EM_CG 		0x00030000
/**
 *
 */
#define EM_PP 		0x00040000
/**
 *
 */
#define EM_ERROR 	0x00050000
/**
 *
 */
#define EM_GRM 		0x00060000
/**
 *
 */
#define EM_VARIABLE_NOT_FOUND 		0x00000001
/**
 *
 */
#define EM_CANT_OPEN_FILE 			0x00000002
/**
 *
 */
#define EM_PARSE_FILE 				0x00000003
/**
 *
 */
#define EM_WRONG_SCANNER 			0x00000004
/**
 *
 */
#define EM_VARIABLE_NOT_DEFINED 	0x00000005
/**
 *
 */
#define EM_WRONG_TYPE 				0x00000006
/**
 *
 */
#define EM_WRONG_EXP 				0x00000007
/**
 *
 */
#define	EM_UNKNOWN_OPER 			0x00000008
/**
 *
 */
#define EM_UNKNOWN_OPERATION 		0x00000009
/**
 *
 */
#define EM_UNKNOWN_EXP 				0x0000000a
/**
 *
 */
#define EM_UNKNOWN_TYPE 			0x0000000b
/**
 *
 */
#define EM_ARRAY_INIT 				0x0000000c
/**
 *
 */
#define EM_ARRAY_INIT_VAL 			0x0000000d
/**
 *
 */
#define EM_ARRAY_CTE 				0x0000000e
/**
 *
 */
#define EM_ARRAY_SIZE 				0x0000000f
/**
 *
 */
#define EM_ALG_INIT 				0x00000010
/**
 *
 */
#define EM_ALG_INDEX 				0x00000020
/**
 *
 */
#define EM_OUTPUT_INIT 				0x00000030
/**
 *
 */
#define EM_FOR_VAR 					0x00000040
/**
 *
 */
#define EM_FOR_DEF 					0x00000050
/**
 *
 */
#define EM_CALL_EXP 				0x00000060
/**
 *
 */
#define EM_RANGE_EXP 				0x00000070
/**
 *
 */
#define EM_INDEX_RANGE 				0x00000080
/**
 *
 */
#define EM_SIZE_ZERO 				0x00000090
/**
 *
 */
#define EM_GEN_DEF 					0x000000a0
/**
 *
 */
#define EM_EVENT_DEF 				0x000000b0
/**
 *
 */
#define EM_SYMBOL 					0x000000c0
/**
 *
 */
#define EM_SYM_DER 					0x000000d0
/**
 *
 */
#define EM_EQ_DEF 					0x000000e0
/**
 *
 */
#define EM_NO_EQ 					0x000000f0
/**
 *
 */
#define EM_UNKNOWN_ODE 				0x00000100
/**
 *
 */
#define EM_EVENT_FOUND 				0x00000200
/**
 *
 */
#define EM_UNKNOWN_ERROR 			0x00000300
/**
 *
 */
#define EM_SAMPLED_DEF 				0x00000400
/**
 *
 */
#define EM_OUTPUT_DEF 				0x00000500
/**
 *
 */
#define EM_ARGUMENTS 				0x00000600
/**
 *
 */
#define EM_SAMPLED_OUT 				0x00000700
/**
 *
 */
#define EM_HANDLER 					0x00000800
/**
 *
 */
#define EM_CONSTANT_FOR				0x00000900
/**
 *
 */
#define EM_DEFINITION_NOT_ALLOWED 	0x00000a00
/**
 *
 */
#define EM_CLASS_DEFINITION 		0x00000b00
/**
 *
 */
#define EM_INIT_EXP 				0x00000c00
/**
 *
 */
#define EM_IMPLICIT_CAST 			0x00000d00
/**
 *
 */
#define EM_WRONG_VARIABLE_TYPE		0x00000e00
/**
 *
 */
#define EM_INDEX_EXP				0x00000f00
/**
 *
 */
#define EM_FUNCTION_NOT_FOUND 		0x00001000
/**
 *
 */
#define EM_ANNOTATION_NOT_FOUND 	0x00002000
/**
 *
 */
#define EM_ANNOTATION_TYPE 			0x00003000

/**
 *
 */
typedef enum
{
  ER_Warning, //!< ER_Warning
  ER_Error,   //!< ER_Error
  ER_Fatal    //!< ER_Fatal
} ER_Type;

/**
 *
 */
class Error
{
  public:
    /**
     *
     * @return
     */
    static Error *
    getInstance();
    /**
     *
     */
    ~Error();
    /**
     *
     * @param pos
     * @param code
     * @param t
     * @param message
     */
    void
    add(int pos, unsigned int code, ER_Type t, const string message, ...);
    /**
     *
     */
    void
    show();
    /**
     *
     * @param className
     */
    void
    setClassName(string className);
    /**
     *
     * @return
     */
    string
    className();
    /**
     *
     * @param s
     */
    void
    setFile(string s);
    /**
     *
     * @return
     */
    string
    file();
    /**
     *
     * @return
     */
    int
    errors();
    private:
    Error();
    static Error *_instance;
    string
    _typeString(ER_Type t);
    string
    _printCode(int code);
    map<unsigned int, string> _errors;
    map<unsigned int, string> _warnings;
    int _numErrors;
    int _numWarnings;
    string _stdDef;
    string _className;
};

#endif  /* ERROR_H_ */
