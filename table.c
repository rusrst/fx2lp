#include "fx2lp_reg.h"

#pragma constseg TABLE

__code unsigned char descr[18] = { 18, 1, 1, 1, 0xff, 0xff, 0xff, 64,
        (VID & 0xff), ((VID >> 8) & 0xff), (PID & 0xff), ((PID >> 8) & 0xff), (DID & 0xff), ((DID >> 8) & 0xff),
        1, 2, 0, 1};
__code unsigned char configuration0[] = {9, 2, 32, 0, 1, 1, 0, 0x80, 0x30,// configuration закончился
                                         9, 4, 0, 0, 2, 0xff, 0xff, 0xff, 0,// if0 закончился
                                         7, 5, 1, 2, 64, 0, 0,// ep1out закончился
                                         7, 5, 0x81, 2, 64, 0, 0 // ep1in закончился
                                        };
__code unsigned char string0[] = { 4,// размер
 3,// тип запроса - строка
  9, 4// langid
  };

__code unsigned char string1[] = { 12, 3, 'M', 0, 'A', 0, 'X', 0, 'I', 0, 'M', 0};
__code unsigned char string2[] = { 28, 3, 'M', 0, 'A', 0, 'X', 0, 'I', 0, 'M', 0, 0x20, 0,'F', 0, 'I', 0, 'L', 0, 'A', 0, 'T', 0, 'O', 0, 'V', 0};
