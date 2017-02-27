/* 
 * File:   main.c
 * Author: game
 *
 * Created on 5 de noviembre de 2016, 08:35 AM
 */

#include <pic18f46k20.h>
#include "configreg.h"
#include "timer.h"
#include "hw.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "adc.h"
#include "m_uart.h"
#define  _XTAL_FREQ 32000000
unsigned char a=0xFF,ope=0,m=0;
unsigned char d=0;
/*EL SW1 MANEJA D1 Y D8
  EL SW2 MANEJA D2 Y D7
  EL SW3 MANEJA D3 Y D6,D4*/


void set_ph(){
    /*Configura el Timer y puertos*/
    set_leds();
    timer_0_conf();
    timer_0_set(0x19,0xD3);
    timer_0_star();
    conf_adc();
    set_adc(1);
    config_tx_aurt();
    activacion_tx(1);
    
}

int main(int argc, char** argv) { 
    set_ph();
    unsigned char contador=0;
    
    while(1){
        if (flag_timer_0()){
            contador++;
            if(contador==25){
                contador=0;
                star_convercion();
            }
        }
        if(final_convercion())
            port(dato_8());
       
    }

    return (EXIT_SUCCESS);
}

