#include <pic18f46k20.h>

void timer_1_conf(){
    T1CONbits.RD16=1;
    T1CONbits.T1RUN=0;
    T1CONbits.T1CKPS=11;
    T1CONbits.T1SYNC=0;
    T1CONbits.TMR1CS=0;
}


void timer_1_set(unsigned char low,unsigned char high){
    TMR1H=high;
    TMR1L=low;
}
void timer_1_star(){
    T1CONbits.TMR1ON=1;
}

int flag_timer_1(){
    if(PIR1bits.TMR1IF){
        PIR1bits.TMR1IF=0;
        timer_1_set(0x3B,0xF6);
        timer_1_star();
        return 1;
    }
    else
        return 0;
        
        
}

void timer_0_conf(){
    T0CONbits.T0PS=7;
    T0CONbits.T08BIT=0;
    T0CONbits.PSA=0;
    T0CONbits.T0CS=0;
}

void timer_0_set(unsigned char low,unsigned char high){
    TMR0H=high;
    TMR0L=low;
}

void timer_0_star(){
    T0CONbits.TMR0ON=1;
}

int flag_timer_0(){
    if(INTCONbits.T0IF){
        INTCONbits.T0IF=0;
        timer_0_set(0x6F,0xC3);
        timer_0_star();
        return 1;
    }
    else
        return 0;
        
        
}