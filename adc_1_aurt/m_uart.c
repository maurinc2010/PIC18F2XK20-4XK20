#include <pic18f46k20.h>

void config_tx_aurt(){
    //SPBRGH=0x00;  //
    SPBRGH=0x3;
    //SPBRG=0X19; //
    SPBRG=0X40;
    TXSTAbits.SYNC=0;
    TXSTAbits.BRGH=1;
    BAUDCONbits.BRG16=1;
    TRISCbits.RC7=1;
    TRISCbits.RC6=1;
    RCSTAbits.SPEN=1;
    RCSTAbits.RX9=0;
    
    
    
}

void activacion_tx(unsigned char x){
    TXSTAbits.TXEN=x;
}

void activacion_rx(unsigned char x){
    RCSTAbits.CREN=x;
}

unsigned char test_tx(){
    if(TXSTAbits.TRMT)
        return 1;
    else
        return 0;
}

void trans_dato(unsigned char x){
     while (!TXSTAbits.TRMT);
        TXREG=x;  
}

unsigned char read_dato(){
    while(!PIR1bits.RCIF);
        return RCREG;  
}

unsigned char read_d(){
    if (PIR1bits.RCIF==1)
        return RCREG;  
    else
        return 0;
}



void send_msm(char *msm){
    unsigned char a=1;
    do{
        trans_dato(msm[a++]);
    }while (msm[a]!='\r');
    trans_dato('r');
}