/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "lpc21xx.h"
#include "Queue.h"
#include "event_groups.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )
/*-----------------------------------------------------------*/

TaskHandle_t Task1Handler = NULL;
TaskHandle_t Task2Handler = NULL;

int Task1Start=0,Task1End=0,Task1ETime=0;
int Task2Start=0,Task2End=0,Task2ETime=0;
int T1Start,T1End,T2Start,T2End;
volatile int Task_1_Deadline_Detector =0,Task_2_Deadline_Detector =0;
int SystemTime , CpuLoad;
char runTimeStatsBuff[200];

static void prvSetupHardware( void );
 

void Task1(void * pvParameters){
	
 TickType_t xLastWakeTime = xTaskGetTickCount();	
	int i =0;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );// Set Task Tag to 1 to use it in trace fun. in Config File 
	
	while(1){
		
T1Start=xTaskGetTickCount();//Get Time instance when task 1 start
		
for(i=0;i<135000;i++) i=i; // Simulate Execution time = 20 Ms
		
T1End=xTaskGetTickCount();//Get Time instance when task 2 end		
if(T1End-T1Start>50)Task_1_Deadline_Detector++;//Check if Task 1 Misses its Deadline 
		
vTaskDelayUntil( &xLastWakeTime, 50 );
 }
}

void Task2(void * pvParameters){
	
 TickType_t xLastWakeTime = xTaskGetTickCount();	
	int i =0;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );// Set Task Tag to 2 to use it in trace fun. in Config File 
	
	while(1){	
		
T2Start=xTaskGetTickCount();//Get Time instance when task 2 start
		
for(i=0;i<125000;i++) i=i;	// Simulate Execution time = 20 Ms
		
T2End=xTaskGetTickCount();//Get Time instance when task 2 End		
if(T2End-T2Start>80)Task_2_Deadline_Detector++;//Check if Task2 Misses its Deadline 
		 
vTaskDelayUntil( &xLastWakeTime, 80 );
		
vTaskGetRunTimeStats(runTimeStatsBuff);
xSerialPutChar('\n');				
vSerialPutString(	runTimeStatsBuff,200);
		
 }
}

void vApplicationTickHook( void ){	//(Tick Indicator) 
GPIO_write(PORT_0,PIN0,PIN_IS_HIGH);
GPIO_write(PORT_0,PIN0,PIN_IS_LOW);
}

void vApplicationIdleHook( void ){
GPIO_write(PORT_0,PIN3,PIN_IS_HIGH); //When Idle Task get called it will Set Pin3 high (Idle Task Detector) 
}

int main( void )
	{
	prvSetupHardware();
	 
	xTaskPeriodicCreate(Task1,"Task 1",100,( void * ) 0,2,&Task1Handler,50 );// Create New Task With Periodicity & Deadline 50 ms 	
	xTaskPeriodicCreate(Task2,"Task 2",100,( void * ) 0,1,&Task2Handler,80 );// Create New Task With Periodicity & Deadline 80 ms 
		
	vTaskStartScheduler();

while(1){}
}

void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}
static void prvSetupHardware( void )
{
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);
	GPIO_init();
	configTimer1();
	VPBDIV = mainBUS_CLK_FULL;
}




