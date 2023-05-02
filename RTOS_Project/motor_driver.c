#include "tm4c123gh6pm.h"
#define EnableA PF_1
#define Input1 PD_1
#define Input2 PD_2
#define Button PD_3
#define GET_BIT(REG,BIT) ( ( REG &(1<<BIT) ) >> BIT )
void Init(void);
void Turn_oneDirection(void);
void Turn_OppositeDir(void);
void Delay_ms(int time_ms);
void DebouncingSwitch(void);
static int pressed= 0; //false not pressed
/*void Init(void){
	pinMode(EnableA,OUTPUT);
	pinMode(Input1,OUTPUT);
	pinMode(Input2,OUTPUT);
	pinMode(Button,INPUT);
}
*/
//Spin motor in one direction by giving IN1 and IN2 signals to L298N
void Turn_oneDirection(void)
{
			//while(digitalRead(Button)==true){
			
		  if(pressed==1){
     SYSCTL_RCGC2_R  |= 0x01;   /* enable clock to PORTF */
	   GPIO_PORTA_DIR_R |= (1<<3)|(1<<2);            /* pin digital */
     GPIO_PORTA_DEN_R|= (1<<3)|(1<<2);            /* pin digital */
     GPIO_PORTA_DATA_R |=(1<<2);
		 GPIO_PORTA_DATA_R &= ~(1<<3);
		  Delay_ms(20);
	}
}
			//}
//Spin motor in one direction by giving IN1 and IN2 signals to L298N
void Turn_OppositeDir(void)
{
	   //while(digitalRead(Button)==true){
			
		  if(pressed==0){
     SYSCTL_RCGC2_R  |= 0x01;   /* enable clock to PORTF */
	   GPIO_PORTA_DIR_R |= (1<<3)|(1<<2);            /* pin digital */
     GPIO_PORTA_DEN_R|= (1<<3)|(1<<2);            /* pin digital */
     GPIO_PORTA_DATA_R |=(1<<2);
		 GPIO_PORTA_DATA_R &= ~(1<<2);
		 Delay_ms(20);
       }
			}
		//}
void Delay_ms(int time_ms)
{
    int i, j;
    for(i = 0 ; i < time_ms; i++)
        for(j = 0; j < 3180; j++)
            {
							/* excute NOP for 1ms */
						}  
}
void DebouncingSwitch(void){
	//if (digitalRead(Button)==true){
		pressed=!pressed;
	//}
	
}
