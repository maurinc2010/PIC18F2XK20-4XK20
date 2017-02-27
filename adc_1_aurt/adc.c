/*
 * File:   adc.c
 * Author: game
 *
 * Created on 26 de noviembre de 2016, 09:05 AM
 */

#include <pic18f46k20.h>
#include "adc.h"


void conf_adc(void){
    TRISAbits.RA0=1;
    ANSELbits.ANS0=1;
    ADCON2bits.ADCS=2;
    ADCON2bits.ACQT=2;
    ADCON1=0;
    ADCON0bits.CHS=0;
    ADCON2bits.ADFM=izquierda;
    
}

void set_adc(unsigned char x){
    ADCON0bits.ADON=x;
}

void star_convercion(void){
    ADCON0bits.GO=1;
}

unsigned char final_convercion(void){
    if (PIR1bits.ADIF){
        PIR1bits.ADIF=0;
        return 1;
    }
    else
        return 0;
    
}

unsigned char dato_8(void){
    return (ADRESH);
    
}

int dato_16(){
    unsigned int y=0;
    y=ADRESH;
    y<<=8;
    y|=ADRESL;
    return y;
    
}