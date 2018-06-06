#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <stdbool.h>

struct Padding_Info {
  int length;
  char padding;
};

int dec_to_ascii(char *str, int dec, struct Padding_Info padding_info);
int dec_to_big_hex(char *str, int dec, struct Padding_Info padding_info);
int fill_padding(char *str, int num_digits, struct Padding_Info padding_info);
int parse_padding_info(char* format, struct Padding_Info* padding_info); 
int __sprintf(char *copy, char *format, va_list list);

#endif
