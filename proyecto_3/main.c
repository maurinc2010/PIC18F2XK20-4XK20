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
#include "SPI.h"
#include "type.h"
#include "filtro.h"
#define  _XTAL_FREQ 32000000
char *pointer_out;
unsigned char a=0x01,s1=0x01,s2=0b10000000,jj=0;
unsigned char d=0,ss=0,sec=0;
unsigned char x=0,periodo=1;
float in_v,out_v;
unsigned char num[5];
char *pointer_in;

float in_t=0;
float out_t=0;

void transmitir();

u8 flag_uart=0;


unsigned char msm1[]="   FILTRO PASABAJOS          \n \r";
unsigned char msm2[]=" VIN: \t\t VOUT: \n \r";

unsigned char msm3[]="           \r";

unsigned est=0;

char  *printFloat(float number, u8 digits,u8 base)
{ 
	u8  toPrint;
	u16 int_part;
	float rounding, remainder;
    static char buf[8 * sizeof(long)]; 
	u16 i = 0;
    

	
	if (number < 0.0)
	{
		//trans_dato('-');
		number = -number;
	}

	
	rounding = 0.5;
	for (i=0; i<digits; ++i)
		rounding /= 10.0;

	number += rounding;

	
	int_part = (u16)number;
	remainder = number - (float)int_part;
    
    

	if (int_part == 0)
	{
        buf[i++]='0';
	} 

	while (int_part > 0)
	{
        //lcd_write((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
		buf[i++] =((char) (int_part % base)+0x30);
		int_part /= base;
	}
    buf[i++]='.';
	//entero=printNumber(int_part, 10);

	
	//if (digits > 0)
	///	trans_dato('.'); 

	
	while (digits-- > 0)
	{
		remainder *= 10.0;
		toPrint = (unsigned int)remainder; 
		buf[i++] =((char) (toPrint)+0x30);
		remainder -= toPrint; 
	}
    buf[i++]='\r';
    return (buf); 
}

void uart_print(char *msm){
    u8 a=0;
    do{
        trans_dato(msm[a++]);
    }while (msm[a]!='\r');
    trans_dato('\n');
    trans_dato('\r');
}


void set_ph(){
    /*Configura el Timer y puertos*/
    set_leds();
    timer_1_conf();
    timer_1_set(0x17,0xFC);
    timer_1_star();
    conf_adc();
    set_adc(1);
    config_tx_aurt();
    activacion_tx(1);
    activacion_rx(1);
    set_spi();
}

int main(int argc, char** argv) { 
    set_ph();
    u8 time_TM=0;
    u8 time_UART=0;
    u16 in=0;
    u16 out=0;
    
    send_msm(msm1);
    send_msm(msm2);
    while(1){       
        if (flag_timer_1()){
            PORTD=~PORTD;
            time_UART++;
            time_TM++;
        }

        if (time_TM==1){
            star_convercion();
            time_TM=0;
        }
        if (time_UART==250){
           flag_uart=1;
           time_UART=0;
           in_t=0.001*in;
           out_t=0.001*out;
        }
        if (final_convercion()){
                in=3*dato_16();
                out=filtro(in);
            }
        if (flag_uart){
            transmitir();
        }
    }
    

    return (EXIT_SUCCESS);
}


void transmitir(){
    
    switch (est){
        case 0:
            pointer_in=printFloat(in_t,3,10);
            est=1;
            break;
        
        case 1:
            if (*pointer_in!='\r'){
                trans_dato(*pointer_in);
                pointer_in++;
            }
            else{
                //uart_print(msm3);
                trans_dato('\t');
                trans_dato('\t');
                est=2;
            }
            break;
        case 2:
            pointer_out=printFloat(out_t,3,10);
            est=3;
            break;
        case 3:
            if (*pointer_out!='\r')
                trans_dato(*pointer_out++);
            else{
                est=0;
                trans_dato('\n');
                trans_dato('\r');
                flag_uart=0;
            }
            break;
        default:
            break;
            
        }

}