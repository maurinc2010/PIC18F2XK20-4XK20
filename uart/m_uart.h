  
#ifndef M_UART_H
#define	M_UATY_H

//cofigurar uart
void config_tx_aurt();
//activar transmicion
void activacion_tx(unsigned char);
//test de bits de transmicion
unsigned char test_tx(void);
//transmitir dato
void trans_dato(unsigned char);
void activacion_rx(unsigned char );
unsigned char read_dato();
void send_msm(char *msm);


#endif	/* XC_HEADER_TEMPLATE_H */

