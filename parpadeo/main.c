/* 
 * File:   main.c
 * Author: game
 *
 * Created on 5 de noviembre de 2016, 08:35 AM
 */

#include <pic18f46k20.h>
#include "configreg.h"

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define  _XTAL_FREQ 32000000
unsigned char a=1,ope=0;
int d=0;

void rotar_derecha_0(){ //rota a derecha un cero
    unsigned char c;
    LATD &=0XF0;
    LATB &= 0XF0;
    LATD |=(c & 0x0F);
    LATB |=(c>>4);
    c=0xFF & ~ a;
    if (a==128)
        a=1;
    else
        a=a<<1;  
}

void rotar_izquierda_0(){ //rota a izquierda un cero
    unsigned char c;
    LATD &=0XF0;
    LATB &= 0XF0;
    LATD |=(c & 0x0F);
    LATB |=(c>>4);
    c=0xFF & ~ a;
    if (a==1)
        a=128;
    else
        a=a>>1;  
}


void rotar_derecha(){  //rota a derecha un 1
    LATD &=0XF0;
    LATB &= 0XF0;
    LATD |=(a & 0x0F);
    LATB |=(a>>4);
    if (a==128)
        a=1;       
    else
        a=a<<1;
}

void rotar_izquierda(){   //rota a izquierda un 1
    LATD &=0XF0;
    LATB &= 0XF0;
    LATD |=(a & 0x0F);
    LATB |=(a>>4);
    if (a==1)
        a=128; 
    else
        a=a>>1;
}
void rotar(){ //rotacion izquierda-derecha 1
    LATD &=0XF0;
    LATB &= 0XF0;
    LATD |=(a & 0x0F);
    LATB |=(a>>4);
    if (d==0){
        a=a<<1;
        if (a==128)
            d=1;
    }
    else{
        a=a>>1;
        if (a==1){
            d=0;
                  }
    }    
}

void sel_ope(int ope){ //Seleciona el tipo de rotación
    switch (ope){
            case 1:
                rotar();
                break;
            case 2:
                rotar_derecha();
                break;
            case 3:
                rotar_izquierda();
                break;
            case 4:
                rotar_derecha_0();
                break;
            case 5:
                rotar_izquierda_0();
                break;
        }


}

int main(int argc, char** argv) {
    
    PORTD=0;
    LATD=0;
    PORTB=0;
    TRISB=0;
    TRISD=0;
    ANSEL=0XE0;
    TRISA=0XFF;
    
    
    while(1){
        
        //LATDbits.LATD0=~LATDbits.LATD0;//rutinas a ejecutar
        __delay_ms(22);
        __delay_ms(22);
        __delay_ms(22);
        __delay_ms(22);
        //PORTDbits.RD0=~PORTDbits.RD0;
        //LATD &=0XF0;
        //LATD |=(a & 0x0F);
        //LATB &= 0XF0;
        //LATB |=(a>>4);
        //a=a+1;
        //PORTD=PORTA>>4;
        if (PORTAbits.RA5==0){ //Espera el cambio de estado del S2 de 1 a 0
            while (!PORTAbits.RA5){ //Espera el cambio de S2 de 0 a 1
                sel_ope(ope); //Llama a la selecion de operacion
                __delay_ms(22); //Se evita que se bloquie el led al tener presionado S2
                __delay_ms(22);
                __delay_ms(22);
                __delay_ms(22);
            }
            ope=ope+1; //incrementa la variable de estado para cambio de operacion
            if (ope==6) //Reinicia la variable de estado
                ope=1;
            }
        sel_ope(ope); //Llama a la selecion de operacion.
        
        
       
    }

    return (EXIT_SUCCESS);
}

