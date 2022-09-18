

volatile __xdata __at (0xE600) unsigned char CPUCS;// CPU control and settings (clock)
volatile __xdata __at (0xE680) unsigned char USBCS;// USB control and settings
#define bHSM 7 // hi-speed mode (only read)
#define bDISCON 3
#define bNOSYNSOF 2
#define bRENUM 1 // renumerate
#define bSIGRSUME 0
volatile __xdata __at (0xE668) unsigned char INTSETUP;// INT2 and INT4 setup register
#define bAV2EN 3 // INT2 autuvector enable (USB)
#define bINT4SRC 1 // INT4 Source
#define bAV4EN 0 // INT4 autovector enable
volatile __xdata __at (0xE65C) unsigned char USBIE;// USB interrupt enable register
#define bEP0ACK 6
#define bHSGRANT 5
#define bURES 4
#define bSUSP 3
#define bSUTOK 2
#define bSOF 1
#define bSUDAV 0 //
volatile __xdata __at (0xE65D) unsigned char USBIRQ;// USB interrupt request register
volatile __xdata __at (0xE6A0) unsigned char EP0CS;// EP0 control status
#define HSNAK 7
#define BUSY 1
#define STALL 0
volatile __xdata __at (0xE6B5) unsigned char SUDPTRCTL;
volatile __xdata __at (0xE6B3) unsigned char SUDPTRH;
volatile __xdata __at (0xE6B4) unsigned char SUDPTRL;
volatile __xdata __at (0xE68A) unsigned char EP0BCH;
volatile __xdata __at (0xE68B) unsigned char EP0BCL;

volatile __xdata __at (0xE65E) unsigned char EPIE;
volatile __xdata __at (0xE65F) unsigned char EPIRQ;
#define bEP8 7
#define bEP6 6
#define bEP4 5
#define bEP2 4
#define bEP1OUT 3
#define bEP1IN 2
#define bEP0OUT 1
#define bEP0IN 0
volatile __xdata __at (0xE610) unsigned char EP1OUTCFG;
volatile __xdata __at (0xE611) unsigned char EP1INCFG;
#define bVALID 7
#define bTYPE1 5
#define bTYPE0 4



__sfr __at (0x92) _XPAGE;
__sfr __at (0x92) MPAGE;
__sfr __at (0xB2) OEA;// IO
__sfr __at (0x80) IOA;// бит адресуемый регистр IO
__sfr __at (0x89) TMOD;// настройка таймера
__sfr __at (0x8E) CKCON;// контрольскорости тактирования
__sfr __at (0x88) TCON;// timer config bit-adrres
__sfr __at (0x8A) TL0;
__sfr __at (0x8C) TH0;
__sfr __at (0x8B) TL1;
__sfr __at (0x8D) TH1;
__sfr __at (0xA8) IE;// bit adressing
__sfr __at (0xD0) PSW;
__sfr __at (0xE0) ACC;
__sfr __at (0xE8) EIE; //interrupt, bit adressing
__sfr __at (0x91) EXIF;//external interrupt register
#define bEXIF7 7
#define bEXIF6 6
#define bEXIF5 5
#define bEXIF4 4
#define bEXIF3 3
#define bEXIF2 2
#define bEXIF1 1
#define bEXIF0 0


//EIE sbit
__sbit __at (0xE8 + 0) EUSB; // usb interrupt enable
__sbit __at (0xE8 + 1) EI2C; // i2c interrupt enable
__sbit __at (0xE8 + 2) EX4; // int4 interrupt enable
__sbit __at (0xE8 + 3) EX5; // int5 interrupt enable
__sbit __at (0xE8 + 4) EX6; // int6 interrupt enable


// IOA sbit
__sbit __at (0x80) PA0;
__sbit __at (0x81) PA1;


//IE SBIT
__sbit __at (0xA8 + 0x07) EA;// global interrupt enable
__sbit __at (0xA8 + 0x06) ES1;// serial
__sbit __at (0xA8 + 0x05) ET2;// T2 overflow
__sbit __at (0xA8 + 0x04) ES0;// serial
__sbit __at (0xA8 + 0x03) ET1;// T1 overflow
__sbit __at (0xA8 + 0x02) EX1;// external int1#
__sbit __at (0xA8 + 0x01) ET0;// T0 overflow
__sbit __at (0xA8 + 0x00) EX0;// int0#



//TCON sbit
__sbit __at (0x88) IT0;
__sbit __at (0x89) IE0;
__sbit __at (0x8A) IT1;
__sbit __at (0x8B) IE1;
__sbit __at (0x8C) TR0;// timer0 run control, 1 - enable run timer0
__sbit __at (0x8D) TF0;// timer0 overflow flag, set to 1 overflow
__sbit __at (0x8E) TR1;
__sbit __at (0x8F) TF1;

#define TMOD7 7
#define TMOD6 6
#define TMOD5 5
#define TMOD4 4
#define TMOD3 3
#define TMOD2 2
#define TMOD1 1
#define TMOD0 0

#define CKCON3 3
#define CKCON4 4
#define CKCON5 5

#define xdata_temp 0xE100
#define xdata_start 0xE000
#define setupdat 0xE6B8
#define setupdat_size  0x08
#define EP0BUF 0xE740
#define EP0BUF_SIZE 64

// описание пакета setup
#define bmR 0
#define bR 1
#define wVL 2
#define wVH 3
#define wIL 4
#define wIH 5
#define wLL 6
#define wLH 7





#define VID 0x3112
#define PID 0x1973
#define DID 0x0001

