#include "fx2lp_reg.h"

//USB interrupts
void SUDAV (void);
void SOF (void);
void SUTOC (void);
void SUSPEND (void);
void USB_RESET (void);
void HISPEED (void);
void EP0ACK (void);
void _reserved_ (void);
void EP0IN (void);
void EP0OUT (void);
void EP1IN (void);
void EP1OUT (void);
void EP2 (void);
void EP4 (void);
void EP6 (void);
void EP8 (void);
void IBN (void);
void _reserved2_ (void);
void EP0PING (void);
void EP1PING (void);
void EP2PING (void);
void EP4PING (void);
void EP6PING (void);
void EP8PING (void);
void ERRLIMIT (void);
void _reserved3_ (void);
void _reserved4_ (void);
void _reserved5_ (void);
void EP2ISOERR (void);
void EP4ISOERR (void);
void EP6ISOERR (void);
void EP8ISOERR (void);




extern void sync_delay(void);

extern __bit flagAltUsb;
extern __bit flagCfgUsb;
extern __bit flagStallEP0;
extern __bit flagGetDesc;
__xdata unsigned char *ptr_dat = (__xdata unsigned char *) xdata_temp;// указатель на буфер
__code unsigned char *ptr_descriptor;
extern __code unsigned char configuration0[];
extern __code unsigned char descr[];
extern __code unsigned char string0[];
extern __code unsigned char string1[];
extern __code unsigned char string2[];
extern __code unsigned char *pCode;
void send_byte_uart_9600 (unsigned char byte);
void write_string_to_uart(char *);
void nop(void);


void int0 (void) __interrupt (0){ __asm__("reti");}
 void tim0 (void) __interrupt (1){__asm__("reti");}// interrupt T0 overflow
 void int1 (void) __interrupt (2){__asm__("reti");}// interrupt int1#
 void tim1 (void) __interrupt (3){__asm__("reti");}// interrupt T1 overflow
 void usart0 (void) __interrupt (4){__asm__("reti");}// interrupt usart0
 void tim2 (void) __interrupt (5){__asm__("reti");}// interrupt T2 overflow
 void res (void) __interrupt (6){__asm__("reti");}// interrupt resume
 void usart1 (void) __interrupt (7){__asm__("reti");}// interrupt usart1
 void usb_interrupt (void) __interrupt (8) { __asm__("ljmp _std_func");}
void i2c (void) __interrupt (9) {__asm__("reti");}
void int4 (void) __interrupt (10) {__asm__("reti");}
void int5 (void) __interrupt (11) {__asm__("reti");}
void int6 (void) __interrupt (12) {__asm__("reti");}


#pragma codeseg USB_VECTORS
volatile void vectors (void) __naked
{
    __asm
    ljmp _SUDAV
    nop

    ljmp _SOF
    nop

    ljmp _SUTOC
    nop
    __endasm;
}




__xdata unsigned char *ptr_setupdat;// глобальный указатель на setup в памяти данных


void validEP (unsigned char adrr)
{
    if (adrr == 0x01)
    {

    }
}


void get_status (void) {
    return;}
void clear_feature (void) {
    return;}
void reserve (void)
{
    flagStallEP0 = 1;
    return;
}
void set_feature (void) {
    return;}
void set_adr (void) {
    return;}
void get_descriptor (void)
{
   if (ptr_dat[bmR] != 0x80)// проверка типа запроса, что запрос типа in
   {
    flagStallEP0 = 1;
    return;// выход с ошибкой
   }
   if ((ptr_dat[wVH] != 0x01) && (ptr_dat[wVH] != 0x02) && (ptr_dat[wVH] != 0x03))// проверка на get descriptor
   {
    flagStallEP0 = 1;
    return;// выход с ошибкой
   }
    if (ptr_dat[wVH] == 0x01)// дескриптор device
    {
        if ((ptr_dat[wVL] | ptr_dat[wIL] | ptr_dat[wIH]))// проверка на корректный формат требований
        {
           flagStallEP0 = 1;
            return;// выход с ошибкой
        }
        else
        {

            SUDPTRCTL = 0x01;// должна быть 1 даже для ручной передачи дескрипторов (page 331)

            __xdata unsigned char *pData = (__xdata unsigned char *) EP0BUF;
            for (unsigned char i = 0; i < descr[0]; i++)// переписываем дескриптор в ep0buf
            {
                pData[i] = descr[i];
            }
            sync_delay();
            EP0BCH = 0x00;
            sync_delay();
            EP0BCL = ptr_setupdat[wLL];
            sync_delay();
            SUDPTRCTL = 0x00;
            flagGetDesc = 1;
            return;
        }
    }
    if (ptr_dat[wVH] == 0x02)// дескриптор configuration
    {
        SUDPTRCTL = 0x01;
        if (ptr_dat[wVL] | ptr_dat[wIH] | ptr_dat[wIL])// проверили на равенство 0
        {
            flagStallEP0 = 1;
            return;// выход с ошибкой
        }
        else
        {
            SUDPTRL = (unsigned char) configuration0 & 0xff;
            SUDPTRH =  ((unsigned short) configuration0) >> 8;
            flagGetDesc = 1;
            return;
        }
    }
    if (ptr_dat[wVH] == 0x03)
    {
        if ((!(ptr_dat[wIH])) && (!(ptr_dat[wIL])))// проверка требования 0-й строки
            {
                SUDPTRL = (unsigned char) string0 & 0xff;
                SUDPTRH =  ((unsigned short) string0) >> 8;
                flagGetDesc = 1;
                return;
            }
        if (ptr_dat[wVL] == 0x01)// строка производителя
            {
                SUDPTRL = (unsigned char) string1 & 0xff;
                SUDPTRH =  ((unsigned short) string1) >> 8;
                flagGetDesc = 1;
                return;
            }
        if (ptr_dat[wVL] == 0x02)// строка производителя
            {
                SUDPTRL = (unsigned char) string2 & 0xff;
                SUDPTRH =  ((unsigned short) string2) >> 8;
                flagGetDesc = 1;
                return;
            }
        flagStallEP0 = 1;
        return;// выход с ошибкой
    }
}

void set_descriptor (void) {
    flagStallEP0 = 1;
    return;}
void get_configurated (void) {return;}
void set_configurated (void)
    {
     if (ptr_setupdat[bmR] | ptr_setupdat[wLL] | ptr_setupdat[wLH] | ptr_setupdat[wIL] | ptr_setupdat[wIH]
           | ptr_setupdat[wVH])// проверка корректности запроса
     {
         flagStallEP0 = 1;
         return;// выход с ошибкой
     }
     if (ptr_setupdat[wVL])// проверка конфигурации, пока поддерживается только 0-я и 1-я
    {
         if (ptr_setupdat[wVL] != 0x01)
         {
            flagStallEP0 = 1;
            return;// выход с ошибкой
         }
         else
         {
            flagCfgUsb = 1;
            ptr_setupdat[wVL] = 0;
            flagAltUsb = 0;

         }
     }
    }
void get_interface (void) {return;}
void set_interface (void) {return;}
void synch_frame (void) {return;}

void (*std_func[13]) (void) = {get_status, clear_feature, reserve, set_feature, reserve, set_adr, get_descriptor,
                    set_descriptor, get_configurated, set_configurated, get_interface, set_interface, synch_frame};

void SUDAV (void)
{

    EXIF &= 0xEF;// снимаем прерывание USB
    USBIRQ = (1<<bSUDAV); // снимаем прерывание sudav




    // копируем данные из буфера setup
    ptr_setupdat = (__xdata unsigned char *)setupdat;
    for (int i = 0; i < setupdat_size; i++)
    {
        ptr_dat[i] = ptr_setupdat[i];
    }







    flagStallEP0 = 0;// очистили флаг ошибки EP0

    if (!(ptr_dat[bmR] & 0x60) && (ptr_dat[bR] < 0x0D))// проверяем что требование стандартное и попадает в диапазон стандартных требований (13)
    {

        std_func[ptr_dat[bR]]();// функция обработки требования

        if (flagStallEP0)
        {
            EP0CS = (unsigned char) ((1<<HSNAK) | (1<<STALL));// устанавливаем stall, снимаем snak
            __asm__("reti");

        }
        EP0CS |= (1<<HSNAK);


        if (!(ptr_dat[bmR] & 0x80))// если направление OUT
        {
            __asm__("reti");

        }
        if (flagGetDesc)// если надо передать дескриптор с помощью системы автоматической передачи дескрипторов (адреса мы прописали в подпрограмме)
        {
            flagGetDesc = 0;
            __asm__("reti");
        }
        else// это заготовка для передачи данных в ручном режиме
        {
            sync_delay();
            EP0BCH = ptr_dat[wLH];
            sync_delay();
            EP0BCL = ptr_dat[wLL];
            sync_delay();

        }

    }
    else
    {
        flagStallEP0 = 1;
        EP0CS = (unsigned char) ((1<<HSNAK) | (1<<STALL));// устанавливаем stall, снимаем snak
        __asm__("reti");
    }
    __asm__("reti");
}
void SOF (void) {}
void SUTOC (void) {}
void SUSPEND (void) {}
void USB_RESET (void) {}
void HISPEED (void) {}
void EP0ACK (void) {}
void _reserved_ (void) {}
void EP0IN (void) {}
void EP0OUT (void) {}
void EP1IN (void) {}
void EP1OUT (void) {}
void EP2 (void) {}
void EP4 (void) {}
void EP6 (void) {}
void EP8 (void) {}
void IBN (void) {}
void _reserved2_ (void) {}
void EP0PING (void) {}
void EP1PING (void) {}
void EP2PING (void) {}
void EP4PING (void) {}
void EP6PING (void) {}
void EP8PING (void) {}
void ERRLIMIT (void) {}
void _reserved3_ (void) {}
void _reserved4_ (void) {}
void _reserved5_ (void) {}
void EP2ISOERR (void) {}
void EP4ISOERR (void) {}
void EP6ISOERR (void) {}
void EP8ISOERR (void) {}

void send_byte_uart_9600 (unsigned char byte)
{
        TMOD = (0<<TMOD0 | 1<<TMOD1);// 8bit counter0 with auto-reload T0
        CKCON = (0<<CKCON3);// CLK/12
        TH0 = 0x30;// 48 T0
        TL0 = 0x00;
        PA0 = 0;

        TR0 = 1;// run timer0
        while (!TF0);
        TF0 = 0;
        for (unsigned char i = 0; i < 8; i++)
        {
            PA0 = ((byte >> i) & 0x01);
            while (!TF0);
            TF0 = 0;
        }
        PA0 = 1;
        while (!TF0);
        TF0 = 0;
        while (!TF0);
        TF0 = 0;
        TR0 = 0;
        TH0 = 0;
        TL0 = 0;
        return;

}

void write_string_to_uart(char * str) {
    for (int i = 0; *(str + i) != '\0'; i++)
    {
        send_byte_uart_9600(*(str + i));
    }
}
