#include <pic18f46k20.h>

void set_clk(){
    //OSCCONbits.IRCF=011;
    OSCCONbits.OSTS=1;
    OSCCONbits.IOFS=1;
    OSCCONbits.SCS=00;
}
void set_leds(){   
    PORTD=0;
    LATD=0;
    PORTB=0;
    TRISB=0;
    TRISD=0;
    ANSEL=0XE0;
    TRISA=0XFF;
}

void port(unsigned char c){
    LATD &=0XF0;
    LATB &= 0XF0;
    LATD |=(c & 0x0F);
    LATB |=(c>>4);
}
