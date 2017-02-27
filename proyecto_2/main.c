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
unsigned char a=0x01,s1=0x01,s2=0b10000000,jj=0;
unsigned char d=0,ss=0,sec=0;
unsigned char x=0,periodo=1;

unsigned char msm1[]="       Lista de comandos          \n\r";
unsigned char msm2[]=" Rotar derecha pulsar tecla:      r    \n\r";
unsigned char msm3[]=" Rotar izquierda pulsar tecla:    l \n\r";
unsigned char msm4[]=" Vasculante pulsar tecla:         v    \n\r";
unsigned char msm5[]=" knight rider pulsar tecla:       s \n\r";
unsigned char msm6[]=" Rota un uno presionar:           t \n\r";
unsigned char msm7[]=" Rota un cero precionar           n \n\r";


unsigned est=0,i=0;
void transmitir();
unsigned char inicio_transmision=0;

int rotar_derecha(unsigned char x){
    if (x==1){
        port(a);
    }
    else{
        port(0xFF & ~a);
    }
    if (a==128){
        a=1;    
        return 1;
    }
    else
        a=a<<1;
    return  0;
}

int rotar_izquierda(unsigned char x){
    if (x==1){
        port(a);
    }
    else{
        port(0xFF & ~a);
    }
    if (a==1){
        a=128;
        return 0;
    }
    else
        a=a>>1;
    return 1;
}


void vasculante (unsigned char x){ //rotacion izquierda-derecha 1
    if (d==0){
        d=rotar_derecha(x);
        if (a==1)
            a=128;
    }
    else{
        d=rotar_izquierda(x);
        if (a==128)
            a=1;
    }    
}

void shit_raid(){
    unsigned char g;
    g=s2 | s1;
    if (x==1){
        port(g);
    }
    else{
        port(0xFF & ~g);
    }
    if (jj<3){
        s1<<=1;
        s2>>=1;
        jj++;
    }
    else if(jj>=3 && jj<6){
        s1>>=1;
        s2<<=1;
        jj++;
    }
    else if(jj==6){
        jj=0;
        s1=0x01;
        s2=0b10000000;
    }
            
}

unsigned char pulsador_s2(){
    unsigned char n=0;
    switch (ss){
        case 0:
            if (PORTAbits.RA5==0){
                ss=1;
                n=0;
            }
            break;
        case 1:
            if (PORTAbits.RA5==1){
                ss=0;
                n=1;
            }
            break;
        default:
            break;
    }
    return n;            
}

void set_secuencia(){
    switch(sec){
        case 0:
            rotar_derecha(x);
            break;
        case 1:
            rotar_izquierda(x);
            break;
        case 2:
            vasculante(x);
            break;
        case 3:
            shit_raid();
            break;
        default:
            sec=0;
            break;
    }
    
}

void set_periodo(int x){
    if (x<=200)
        periodo=10;
    else if (x>=201 && x<=500)
        periodo=20;
    else if (x>=501 && x<=700)
        periodo=30;
    else if (x>=701 && x<=900)
        periodo=40;
    else if (x>=901 && x<=1120)
        periodo=60;   
}

unsigned char set_comandos(unsigned char c){
    unsigned char d=0;
    switch (c){
        case 'r':
            sec=0;
            d=1;
            break;
        case 'l':
            sec=1;
            d=1;
            break;
        case 'v':
            sec=2;
            d=1;
            break;
        case 's':
            sec=3;
            d=1;
            break;
        case 'n':
            x=0;
            d=1;
            break;
        case 't':
            x=1;
            d=1;
            break;
        default:
            d=0;
            break;
    }
    return d;
}

void set_ph(){
    /*Configura el Timer y puertos*/
    set_leds();
    timer_1_conf();
    timer_1_set(0x3B,0xF6);
    timer_1_star();
    conf_adc();
    set_adc(1);
    config_tx_aurt();
    activacion_tx(1);
    activacion_rx(1);
    
}

int main(int argc, char** argv) { 
    set_ph();
    unsigned char time_adc=0,time_led=0,time_sw=0,time_comando=0;
    unsigned char flag_read=0,flag_5s=0;
    int time_star=0;
    
    while(1){       
        if (flag_timer_1()){
            time_star++;
            if (time_star>=2000){
                time_star=2000;
                time_adc++;
                time_led++;
                time_comando++;
                time_sw++;
            }
            else
                transmitir();
        }
        
        
        if (time_adc==200){
             star_convercion();
             time_adc=0;
        }
        
        if(time_sw==20){
            if (pulsador_s2())
                sec++;
            else if (sec==4)
                sec=0;
            time_sw=0;
        }
        
        if (time_led==periodo){
            set_secuencia();
            time_led=0;
        }
        if (time_comando==4){
            flag_read=1;
            time_comando=0;
        }
        
        if (flag_read){
            if (set_comandos(read_d())){
                flag_read=0;
            }      
        }
              
        
        if (final_convercion()){
            set_periodo(dato_16());
        }
    }
    

    return (EXIT_SUCCESS);
}


void transmitir(){
    switch (est){
        case 0:
            if (msm1[i]!='\r')
                trans_dato(msm1[i++]);
            else{
                i=0;
                est=1;
                trans_dato('\r');
            }
            break;
        case 1:
            if (msm2[i]!='\r')
                trans_dato(msm2[i++]);
            else{
                i=0;
                est=2;
                trans_dato('\r');
            }
            break;
        case 2:
            if (msm3[i]!='\r')
                trans_dato(msm3[i++]);
            else{
                i=0;
                est=3;
                trans_dato('\r');
            }
            break;  
        case 3:
            if (msm4[i]!='\r')
                trans_dato(msm4[i++]);
            else{
                i=0;
                est=4;
                trans_dato('\r');
            }
            break;
        case 4:
            if (msm5[i]!='\r')
                trans_dato(msm5[i++]);
            else{
                i=0;
                est=5;
                trans_dato('\r');
            }
            break;
        case 5:
            if (msm6[i]!='\r')
                trans_dato(msm6[i++]);
            else{
                i=0;
                est=6;
                trans_dato('\r');
            }
            break;
        case 6:
            if (msm7[i]!='\r')
                trans_dato(msm7[i++]);
            else{
                i=0;
                trans_dato('\r');
                est=9;
            }
            break;
        default:
            break;
            
        }

}