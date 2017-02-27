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
#define  _XTAL_FREQ 32000000
unsigned char a=1,ope=0;
int d=0;

/*EL SW1 MANEJA D1 Y D8
  EL SW2 MANEJA D2 Y D7
  EL SW3 MANEJA D3 Y D6,D4*/

int main(int argc, char** argv) { 
    set_leds();
    timer_0_conf();
    timer_0_set(0x2D,0x48);
    timer_0_star();
    while(1){
        if (flag_timer_0())
            sel_ope(ope);
        else{
            if (PORTAbits.RA5==0){ //Espera el cambio de estado del S2 de 1 a 0
                while (!PORTAbits.RA5) //Espera el cambio de S2 de 0 a 1
                   if(flag_timer_0())
                      sel_ope(ope); //Llama a la selecion de operacion
                //a=1;
                ope=ope+1; //incrementa la variable de estado para cambio de operacion
                if (ope==6) //Reinicia la variable de estado
                 ope=1;
            }  
        }
    }

    return (EXIT_SUCCESS);
}

