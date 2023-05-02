#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "StackMacros.h"
#include "FreeRTOSConfig.h"
#include "tm4c123gh6pm.h"
//   Function Prototypes
void PortF_Init(void);
void Delay(void);
#define TOGGLE_BIT(REG,BIT) (REG ^=(1<<BIT))
#define CLEAR_BIT(REG,BIT) (REG &=(~(1<<BIT)))
#define mainSOFTWARE_INTERRUPT_PRIORITY      (5)
#define mainSW_INTERRUPT_ID ((IRQn_Type)0) //gives default to be port A
#define mainTRIGGER_INTERRUPT() NVIC_SetPendingIRQ(mainSW_INTERRUPT_ID)
#define mainCLEAR_INTERRUPT() NVIC_ClearPendingIRQ(mainSW_INTERRUPT_ID)
#define GET_BIT(REG,BIT) ( ( REG &(1<<BIT) ) >> BIT )
void fun1();
void fun2();
//void vSoftwareInterruptHandler( void );
static void prvSetupSoftwareInterrupt();
xSemaphoreHandle xCountingSemaphore;
xQueueHandle xQueue;
static int counter=0;
portBASE_TYPE xstatus;

///void vSoftwareInterruptHandler( void )
//{
	//portBASE_TYPE xHigherPriorityTaskWoken =pdFALSE;
	//xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	//xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
	//clear the software interrupt bit 
	//mainCLEAR_INTERRUPT();
	//apply context switching 
	//portEND_SWITCHING_ISR(&xHigherPriorityTaskWoken);
//}
static void prvSetupSoftwareInterrupt(void){
	NVIC_SetPriority(mainSW_INTERRUPT_ID,mainSOFTWARE_INTERRUPT_PRIORITY);
	NVIC_EnableIRQ(mainSW_INTERRUPT_ID);
}
void fun1()
{
	for( ;; )
	{
    //GPIO_PORTF_DATA_R = 0x02;  
		// LED is red
	   if((GET_BIT(GPIO_PORTF_DATA_R,0))==0){
			 counter++;
			 xstatus= xQueueSendToBack(xQueue,&counter,0);
		   xSemaphoreGive(xCountingSemaphore);
		 }
		
		//TOGGLE_BIT(GPIO_PORTF_DATA_R,1);
		//vTaskDelay(1000);
	}
}

void fun2()
{
	  static int arr[20];
		static int i=0;
		int ptr;
	xSemaphoreTake(xCountingSemaphore,0);
	for( ;; )
	{
    //GPIO_PORTF_DATA_R = 0x04;
		// LED is blue
		
	  xSemaphoreTake(xCountingSemaphore,portMAX_DELAY);
		xQueueReceive(xQueue,&ptr,0);
		arr[i]=ptr;
		i++;
		//vTaskDelay(2000);
		//mainTRIGGER_INTERRUPT();
		//TOGGLE_BIT(GPIO_PORTF_DATA_R,2);
		
	}
}

/*-----------------------------------------------------------*/
int main(void){    
  PortF_Init();        // Call initialization of port PF4, PF3, PF2, PF1, PF0  
	xQueue=xQueueCreate(3,sizeof(long));
	xCountingSemaphore=xSemaphoreCreateCounting(3,0);
	//if(xCountingSemaphore != NULL){
		//enable Interrupts
	//prvSetupSoftwareInterrupt();
	xTaskCreate( fun1, "Task1",240, NULL, 1, NULL );
	//xTaskCreate( fun2, "Task2",240, NULL, 2, NULL );
	xTaskCreate( fun2, "Task2",240, NULL, 2, NULL );
	/* Start the scheduler. */
	vTaskStartScheduler();
	while(1){
	}
}
	//}

void PortF_Init(void){
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;
}
