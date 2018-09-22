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

#include <debug/debug.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const char *enableFlags = NULL; // controls which DEBUG messages are printed

void debugInit(const char *flagList) {
    enableFlags = flagList;
}

bool debugIsEnabled(char flag) {
    if (enableFlags != NULL)
       return (strchr(enableFlags, flag) != 0)
    || (strchr(enableFlags, '+') != 0);
    else
      return false;
}

void DEBUG(char flag, const char *format, ...) {
  if (debugIsEnabled(flag)) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stdout, format, ap);
    fflush(stdout);
    va_end(ap);
  }
}

bool isDebugParam(char *param) {
  if (strcmp(param, "c") == 0) {
    return true;
  }
  if (strcmp(param, "a") == 0) {
    return true;
  }
  if (strcmp(param, "s") == 0) {
    return true;
  }
  if (strcmp(param, "g") == 0) {
	return true;
  }
  return false;
}

void ERROR(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  const char *error_string = "Error: ";
  char *new_format = new char [sizeof(char) * strlen(error_string) + strlen(format) + 1];
  strcpy(new_format, error_string);
  strcat(new_format, format);
  vfprintf(stderr, new_format, ap);
  fprintf(stderr,"\n");
  fflush(stderr);
  va_end(ap);
  delete []new_format;
  exit(EXIT_FAILURE);
}

void ERROR_UNLESS(bool condition, const char *format, ...) {
  if(!condition) {
    va_list ap;
    va_start(ap, format);
    ERROR(format, ap);
    va_end(ap);
  }
}

void WARNING(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  const char *error_string = "Warning: ";
  char *new_format = new char [sizeof(char) * strlen(error_string) + strlen(format) + 1];
  strcpy(new_format, error_string);
  strcat(new_format, format);
  vfprintf(stderr, new_format, ap);
  fprintf(stderr,"\n");
  fflush(stderr);
  delete []new_format;
  va_end(ap);
}

void WARNING_UNLESS(bool condition, const char *format, ...) {
  if(!condition) {
    va_list ap;
    va_start(ap, format);
    WARNING(format, ap);
    va_end(ap);
  }
}


