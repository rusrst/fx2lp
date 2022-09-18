#include "fx2lp_reg.h"


extern void int0 (void) __interrupt (0);
extern void tim0 (void) __interrupt (1);
extern void int1 (void) __interrupt (2);
extern void tim1 (void) __interrupt (3);
extern void usart0 (void) __interrupt (4);
extern void tim2 (void) __interrupt (5);
extern void res (void) __interrupt (6);
extern void usart1 (void) __interrupt (7);
extern void usb_interrupt (void) __interrupt (8);
extern void i2c (void) __interrupt (9);
extern void int4 (void) __interrupt (10);
extern void int5 (void) __interrupt (11);
extern void int6 (void) __interrupt (12);

void sync_delay (void) __naked
{
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
   ret
    __endasm;
}

extern void write_string_to_uart(char *);
extern void nop(void);
extern void send_byte_uart_9600 (unsigned char byte);
extern void test_timer_1(void);

__code unsigned char *pCode;
unsigned char cntTmp;// счетчик общего назначения
__bit flagStallEP0;
__bit flagGetDesc;
__bit flagCfgUsb;
__bit flagAltUsb;
void main(void)
{
    // IO
    OEA = 0xFF; // PA1 input
    PA0 = 1;// UART TX
    //PA1 UART RX
    PA0=1;
    PA1=1;


    EP0BCH = 0x00;
    USBCS |= (1 << bDISCON);// отключение от линии USB
    for (volatile char i = 0; i < 255; i++)
    {
        for (volatile char j = 0; j < 255; j++)
        {
            __asm
            nop
            __endasm;

        }
    }// задержка обнаружения хостом

    INTSETUP = (1<<bAV2EN);// разрешение для модуля USB
    USBIE = (1<<bSUDAV) ;
    EUSB = 1; // разрешение общего прерывания для USB
    USBCS = (1<<bRENUM);// перехват работы с USB
    EA = 1;// разрешение прерываний




    // CPU clock
    if (!(CPUCS & 0x08))
        (CPUCS |= 0x08); // проверяем частоту, если не 24, то устанавливаем 24

    SUDPTRCTL = 0x00; // включаем систему автоматической передачи дескрипторов


write_string_to_uart("START");
    while (1);

}
