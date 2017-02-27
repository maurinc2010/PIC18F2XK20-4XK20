#include "type.h"
//#include "fdacoefs.h"
#include "SPI.h"

u16 u[100];
float y[100];
int k=0;
float a[4]={1.,-2.87435689,2.7564832,-0.88189313};
float b[4]={2.91464945e-05,8.74394834e-05,8.74394834e-05,2.91464945e-05};

u16 filtro(u16 inn){
    //in[k+4]=inn;
    u[k+3]=inn; 
	y[k+3]=(b[0]*u[k+3])+(b[1]*u[k+2])+(b[2]*u[k+1])+(b[3]*u[k]);
    y[k+3]=y[k+3]-(a[1]*y[k+2])-(a[2]*y[k+1])-(a[3]*y[k]); 
    k=k+1;
	if (k+3==100){ 
	 	 //u[3]=u[k+3]; 
	 	 u[2]=u[k+2]; 
	 	 u[1]=u[k+1]; 
	 	 u[0]=u[k+0]; 
	 	 //y[3]=y[k+3]; 
	 	 y[2]=y[k+2]; 
	 	 y[1]=y[k+1]; 
	 	 y[0]=y[k+0]; 
	 	 k=0; 
	} 
   // send_mcp(y[k+3]);//out[k+4]);
    return y[k+3];
}
