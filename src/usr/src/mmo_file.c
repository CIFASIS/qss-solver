#include <stdio.h>

#include "mmo_file.h"

static FILE *_save = NULL;

double
mmo_save(double v)
{
  if(_save == NULL)
  {
    _save = fopen("/tmp/data.log", "w+");
  }
  fprintf(_save, "%.16lf\n", v);
  return v;
}
