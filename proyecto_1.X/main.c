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

#define d1 0x81
#define d2 0x42
#define d3 0x24
unsigned char a=0xFF,ope=0,m=0;
unsigned char d=0;
/*EL SW1 MANEJA D1 Y D8
  EL SW2 MANEJA D2 Y D7
  EL SW3 MANEJA D3 Y D6*/

void signal(unsigned char x,unsigned char p){
    switch (m){
            case 0:
                port(x & a); 
                m=1;
                break;
            case 1:
                port(x & a); 
                m=0;
                ope=p;
                break;                  
            default:
                ope=0;
                break;
            
    }
}

void sw2(unsigned char a,unsigned char b){
    /*Decide cuando la dos sw son activo*/
    switch (ope){
        case 0:
            signal(a,1);
            break;
        case 1:
            signal(b,0);
            break; 
        default:
            ope=0;
            break;
    }
    
}

void sw3(){
    /*Decide la secuencia para sw3 activo*/
    switch (ope){
        case 0:
            signal(d1,1);
            break;
        case 1:
            signal(d2,2);
            break;
        case 2:
            signal(d3,0);
            break;
        default:
            ope=0;
            break;
    }  
}
void sw(unsigned char d){
    /*decide que operación realizar, toma el swich para decidir*/
    switch (d){
            case 1:
                port(d1 & a); //manda los datos a los  leds D1, D8
                break;
            case 2:
                port(d2 & a); //manda los datos a los  leds D1, D8
                break; 
            case 3:
                sw2(d1,d2);
                break;
            case 4:
                port(d3 & a); //manda los datos a los  leds D1, D8
                break;
            case 5:
                sw2(d3,d1);
                break;
            case 6:
                sw2(d3,d2);
                break;
            case 7:
                sw3();
                break;
            default:
                port(0XFF & a);
                break;
            }
}


void set_ph(){
    /*Configura el Timer y puertos*/
    set_leds();
    timer_0_conf();
    timer_0_set(0x6F,0xC3);
    timer_0_star();
}

int main(int argc, char** argv) { 
    set_ph();
    while(1){
        d=(PORTD & 0x70)>>4;  //Captura y trunca el dato de entrada
        if (flag_timer_0()){  //Espera que la bandera sea uno para realizar las operaciones
            sw(d);            //
            a=~a;             //inveierte la variable, lleva el estado de las señales
        }
    }

    return (EXIT_SUCCESS);
}

