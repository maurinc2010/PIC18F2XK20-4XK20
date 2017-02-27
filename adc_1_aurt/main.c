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

struct conversion{
    unsigned int dato;
    unsigned char digitos[5];
    unsigned char digitos2[5];
};

struct conversion conversion16;
unsigned char msm[]="Valor conversion \r";
unsigned est=0,i=0;
void transmitir();
unsigned char inicio_transmision=0;

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
    unsigned char contador=0,contador1=0;
    
    while(1){
        if (flag_timer_0()){
            contador1++;
            contador++;
            if(contador==2){
                contador=0;
                star_convercion();
            }
            if(contador1==125){
                contador1==0;
                inicio_transmision=1;
            }
        }
        if(final_convercion()){
            conversion16.dato=dato_16();
            port(conversion16.dato);
        }
        if(inicio_transmision){
            transmitir();
        }
    }
    

    return (EXIT_SUCCESS);
}

void int_ascii(){
    
    unsigned char j;
    unsigned int aux1,aux2;
    aux1=conversion16.dato;
    for(j=0;j<=5;j++){
        conversion16.digitos[j]=0;
        conversion16.digitos2[j]='0';
        
    }
    j=0;
    while(j<5){
        aux2=aux1%10;
        conversion16.digitos[4-j]=aux2;
        conversion16.digitos2[4-j]=aux2+0x30;
        aux1/=10;
        if (aux1==0)
            j=5;
        else
            j++;
        
    }
}

void transmitir(){
    switch (est){
        case 0:
            int_ascii();
            est=1;
            break;
        case 1:
            if (msm[i]!='\r')
                trans_dato(msm[i++]);
            else{
                i=0;
                est=2;
            }
            break;
        case 2:
            if (i<=4){
                trans_dato(conversion16.digitos2[i]);
                i++;
            }
            else{
                i=0;
                est=0;
                trans_dato('\n');
                trans_dato('\r');
            }
            break;    
        }

}