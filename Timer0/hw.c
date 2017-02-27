#include <pic18f46k20.h>


void set_leds(){   
    PORTD=0;    
    LATD=0;
    PORTB=0;
    TRISB=0xF0;  //PARTE ALTA DEL PORTB COMO ENTRADA
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
