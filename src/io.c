#include "io.h"

int dec_to_ascii(char *str, int dec, struct Padding_Info padding_info) {
   bool minus = false;
   if(dec < 0) {
       minus = true;
       dec = -dec;
   }

   int max_digits = 1;
   int digits = 1;
   while(dec / max_digits > 9) {
       digits += 1;
       max_digits *= 10; 
   }

   if(minus) {
       digits += 1;
   }

   int str_pos = fill_padding(str, digits, padding_info);

   if(minus) {
       str[str_pos] = '-';
       str_pos++;
       digits--;
   }

   for(int i = 0; i < digits; i++, str_pos++) {
       int number_at_n_digits = dec / max_digits;
       
       str[str_pos] = 0x30 + number_at_n_digits; 
       dec -= number_at_n_digits * max_digits;
       max_digits = max_digits / 10;
   }

   return str_pos;
}

int dec_to_big_hex(char *str, int dec, struct Padding_Info padding_info) {

   int max_digits = 1;
   int digits = 1;
   while(dec / max_digits > 15) {
       digits += 1;
       max_digits *= 16; 
   }

   int str_pos = fill_padding(str, digits, padding_info);
   for(int i = 0; i < digits; i++, str_pos++) {
       int number_at_n_digits = dec / max_digits;
       if(number_at_n_digits < 10) {
           str[str_pos] = 0x30 + number_at_n_digits; 
       } else {
           str[str_pos] = 0x41 + (number_at_n_digits - 10); 
       }
       dec -= number_at_n_digits * max_digits;
       max_digits = max_digits / 16;
   }

   return str_pos;
   
}

int fill_padding(char *str, int num_digits, struct Padding_Info padding_info) {
   int padding_length = padding_info.length - num_digits;
   int i = 0;
   for(; 0 < padding_length && i < padding_length;i++) {
       str[i] = padding_info.padding; 
   }
   return i; 
}

int parse_padding_info(char* format, struct Padding_Info* padding_info) {
    int str_pos = 0;
    if(format[str_pos] == '0') {
        str_pos++;
        padding_info->padding = '0';
    } else {
        padding_info->padding = ' ';
    }
    
    if('1' <= format[str_pos] && format[str_pos] <= '9') {
        padding_info->length = format[str_pos] - 0x30;
        return str_pos + 1;
    } else {
        padding_info->length = 0;
        return 0;
    }
}

int __sprintf(char *copy, char *format, va_list list) {
    int i = 0, j = 0;
    struct Padding_Info padding_info;
    while(format[i] != '\0') {
        if(format[i] == '%') {
           i++;
           i+= parse_padding_info(&format[i], &padding_info);

           if(format[i] == 'd') {
               int number = va_arg(list, int);
               int digits = dec_to_ascii(&copy[j], number, padding_info);
               // copy[j] なので digits進める
               // format[i] は dの1文字分
               i++;
               j += digits;
           } else if(format[i] == 'X') {
               int number = va_arg(list, int);
               int digits = dec_to_big_hex(&copy[j], number, padding_info);
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

    copy[j++] = '\0';
    return 0;
}


