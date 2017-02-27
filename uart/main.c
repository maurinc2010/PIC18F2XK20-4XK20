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
#include "m_uart.h"
#include <stdlib.h>
#include <xc.h>
#define  _XTAL_FREQ 1000000
unsigned char d=0;
unsigned char B=65;
unsigned char b=97;
unsigned char a=0;
char msm[]="buen dia /r";

void configg(){
    set_clk();
    config_tx_aurt();
    set_leds();
    activacion_tx(1);
    timer_1_conf();
    timer_1_set(0x8C,0xAA);
    timer_1_star();
    activacion_rx(1);
}

void send_leter(unsigned char a){
    switch (a){
                case 0:
                    trans_dato(B);
                    if (B==90)
                        B=65;
                    else
                        B=B+1;
                    break;
                case 255:
                    trans_dato(b);
                    if (b==122)
                        b=97;
                    else
                        b=b+1;
                    break;
                default:
                    break;
                }
}

int main(int argc, char** argv) { 
    unsigned char m=0;
    configg();
    while(1){
        m=read_d();
        if (m!=0){
            if (m>=48 & m<=57){
                a=~a;
                B=65;
                b=97;
            }
            port(m);
        }
        if (flag_timer_1()){
            send_leter(a);
        }
    }
    return (EXIT_SUCCESS);
}

