#include "io.h"
int dec_to_ascii(char *str, int dec) {

   int max_digits = 1;
   int digits = 1;
   while(dec / max_digits > 9) {
       digits += 1;
       max_digits *= 10; 
   }

   if(max_digits == 1) {
       str[0] = 0x30 + dec;
       return 1;
   }
   
   for(int i = 0; i < max_digits; i++) {
       int number_at_n_digits = dec / max_digits;

       str[i] = 0x30 + number_at_n_digits; 
       dec -= number_at_n_digits * max_digits;
       max_digits = max_digits / 10;
   }

   return max_digits;
}

int sprintf(char *copy, char *format, ...) {
    va_list list;
    va_start(list, format);
    int i = 0, j = 0;
    while(format[i] != '\0') {
        if(format[i] == '%') {
           i++;
 
           if(format[i] == 'd') {
               int number = va_arg(list, int);
               int digits = dec_to_ascii(&copy[j], number);
               // copy[j] なので digits進める
               // format[i] は dの1文字分
               i++;
               j += digits;
           } 

        } else {
           copy[j] = format[i];

           i++;
           j++;
        }
    }

    va_end(list);
    copy[j++] = '\0';
    return 0;
}


