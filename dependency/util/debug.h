/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/


// This debugging routines allow the user to turn on selected
// debugging messages, controllable from the command line arguments
// passed to modelicacc after de -d option.


/**
 * Initialize so that only DEBUG messages with a flag in flagList
 * will be printed.
 *
 * If the flag is "+", we enable all DEBUG messages.
 *
 * "flagList" is a string of characters for whose DEBUG messages are to be enabled.
 */
void debugInit(const char* flags);

/**
 * Print an INFO debug message, if flag is enabled.
 **/
void DEBUG (char flag, const char* format, ...);

bool isDebugParam(char *param);
bool debugIsEnabled(char);

/*
 * Print an ERROR message. Then exits with EXIT_FAILURE status.
 */
void ERROR(const char *format, ...);

/**
 * Print an ERROR message if condition is false.
 * Then exits with EXIT_FAILURE status.
 */
void ERROR_UNLESS(bool condition, const char *format, ...);

/*
 * Print an Warning message. 
 */
void WARNING(const char *format, ...);

/**
 * Print an WARNING message if condition is false.
 * Then continues with the execution.
 */
void WARNING_UNLESS(bool condition, const char *format, ...);
