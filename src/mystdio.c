#include "mystdio.h"
#include <stdarg.h>

int sprintf(char *copy, char *format, ...) {
     va_list list;
     va_start(list, format);
     va_end(list);
     return __sprintf(copy, format, list);
}

