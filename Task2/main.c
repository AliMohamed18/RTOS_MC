/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

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


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

/*-------------------------------------------------Task 1 Defines Start------------------------------------------------- 
TaskHandle_t LedTaskHandler = NULL;
TaskHandle_t ButtonTaskHandler = NULL;
SemaphoreHandle_t xSemaphore;
pinState_t ButtonState,LedState;
	
#define Button_Pressed 1
#define Button_Released 0
#define Led_On  1
#define Led_Off 0

-------------------------------------------------Task 1 Defines End-------------------------------------------------*/


/*-------------------------------------------------Task 2 Defines Start------------------------------------------------- 
	TaskHandle_t Task1Handler = NULL;
	TaskHandle_t Task2Handler = NULL;

SemaphoreHandle_t xMutex;


const signed char Task1String[]=" Task 1 String ! \n";	
const signed char Task2String[]=" Task 2 String ! \n";
-------------------------------------------------Task 2 Defines End-------------------------------------------------*/

/*-------------------------------------------------Task 3 Defines Start------------------------------------------------- */

	TaskHandle_t Task1Handler = NULL;
	TaskHandle_t Task2Handler = NULL;
	TaskHandle_t Task3Handler = NULL;
	TaskHandle_t Task4Handler = NULL;

	QueueHandle_t xQueue;


	
	pinState_t FirstButtonState,SecondButtonState;

#define Button_1_Pressed	 PIN_IS_HIGH
#define Button_1_Released  PIN_IS_LOW
#define Button_2_Pressed	 PIN_IS_HIGH
#define Button_2_Released  PIN_IS_LOW

signed char TaskMessage[35];
 	
/*-------------------------------------------------Task 3 Defines End------------------------------------------------- */

/*-------------------------------------------------------------------------------*/

	
/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
/*-------------------------------------------------Task 1 Start-------------------------------------------------
void Led_Task(void * pvParameters){
while(1){
if(xSemaphore!=NULL){	
if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE ){		 
	LedState=!LedState; 
}
}
	if(ButtonState == Button_Released)	 GPIO_write(PORT_0,PIN1,LedState);

	 vTaskDelay(50);
  }
}


void Button_Task(void * pvParameters){
while(1){
	
  if (ButtonState == Button_Released && GPIO_read(PORT_0,PIN0)== PIN_IS_HIGH)	 ButtonState = Button_Pressed;
	if (ButtonState == Button_Pressed &&  GPIO_read(PORT_0,PIN0)== PIN_IS_LOW ){
		ButtonState = Button_Released;
		xSemaphoreGive(xSemaphore);
}
	vTaskDelay(100);
 }
}
-------------------------------------------------Task 1 End-------------------------------------------------*/


/*-------------------------------------------------Task 2 Start-------------------------------------------------
void Task1(void * pvParameters){
	char x ;
	uint32_t Counter = 0 ;

while(1){
if(xMutex!=NULL){	
if( xSemaphoreTake( xMutex, ( TickType_t ) 10 ) == pdTRUE ){	
for(x=0 ; x<10; x++){	
vSerialPutString(Task1String,18);
for(Counter=0;Counter<500000;Counter++){} 	

}
xSemaphoreGive( xMutex );
 }
}
vTaskDelay(100);
 }
}


void Task2(void * pvParameters){
uint32_t Counter = 0 ;
char y ;
while(1){
if(xMutex!=NULL){	
if( xSemaphoreTake( xMutex, ( TickType_t ) 10 ) == pdTRUE ){
for( y=0;y<10;y++){	
vSerialPutString(Task2String,18);	
for(Counter=0;Counter<1000000;Counter++){} 	
}
xSemaphoreGive( xMutex );
 }
}
vTaskDelay(500);
 }
}
-------------------------------------------------Task 2 End-------------------------------------------------*/


/*-------------------------------------------------Task 3 Start-------------------------------------------------*/


void Task1(void * pvParameters){
	 char Task1Message[25],CheckVariable;
while(1){
	
	CheckVariable=FirstButtonState;	
	
  if (FirstButtonState == Button_1_Released && GPIO_read(PORT_0,PIN0)== PIN_IS_HIGH){	 			
	FirstButtonState=Button_1_Pressed;
	strcpy(Task1Message," Button 1 Is Pressed \n");
	}
	else if (FirstButtonState == Button_1_Pressed &&  GPIO_read(PORT_0,PIN0)== PIN_IS_LOW ){
		FirstButtonState=Button_1_Released;
		strcpy(Task1Message," Button 1 Is Released \n");
	}
			if (FirstButtonState != CheckVariable){
		if(xQueueSend(xQueue,( void * )Task1Message, ( TickType_t ) portMAX_DELAY)==pdTRUE){}
		else xSerialPutChar('E');
		}
				vTaskDelay(50);

 }
}

void Task2(void * pvParameters){
	 char Task2Message[25],CheckVariable;
while(1){
		  CheckVariable=SecondButtonState;	
  if (SecondButtonState == Button_2_Released && GPIO_read(PORT_0,PIN1)== PIN_IS_HIGH){	 			
	SecondButtonState=Button_2_Pressed;
	strcpy(Task2Message," Button 2 Is Pressed \n");
	}
	else if (SecondButtonState == Button_2_Pressed &&  GPIO_read(PORT_0,PIN1)== PIN_IS_LOW ){
		SecondButtonState=Button_2_Released;
		strcpy(Task2Message," Button 2 Is Released \n");
	}
		if (SecondButtonState != CheckVariable){
		if(xQueueSend(xQueue,( void * )Task2Message, ( TickType_t ) portMAX_DELAY)==pdTRUE){}
		else xSerialPutChar('E');
		}
	vTaskDelay(50);
			
 }
}

void Task3(void * pvParameters){
signed char Task3String[]=" String to Be Sent Every 100 Ms \n";
while(1){
 		
if(xQueueSend(xQueue,( void * )Task3String, ( TickType_t ) portMAX_DELAY)==pdTRUE){}
else xSerialPutChar('E');
vTaskDelay(100);
 }
}

void Task4(void * pvParameters){
	signed char Buff[35];
	while(1){
if(xQueueReceive(xQueue,&Buff,( TickType_t ) portMAX_DELAY)==pdTRUE){
vSerialPutString(Buff,35);
		}
	vTaskDelay(150);

 }
}

/*-------------------------------------------------Task 3 End-------------------------------------------------*/

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
	{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	 
/*-------------------------------------------------Task 1 Main Start-------------------------------------------------
	xSemaphore=xSemaphoreCreateBinary();
	xTaskCreate(Led_Task,"Led Task",100,( void * ) 0,1,&LedTaskHandler );
	xTaskCreate(Button_Task,"Button Task",100,( void * ) 0,1,&ButtonTaskHandler );

-------------------------------------------------Task 1 Main End-------------------------------------------------*/


/*-------------------------------------------------Task 2 Main Start-------------------------------------------------
xMutex=xSemaphoreCreateMutex();
	
	xTaskCreate(Task1,"Task 1",100,( void * ) 0,2,&Task1Handler );
	xTaskCreate(Task2,"Task 2",100,( void * ) 0,1,&Task2Handler );
-------------------------------------------------Task 2 Main End-------------------------------------------------*/

/*-------------------------------------------------Task 2 Main Start-------------------------------------------------*/

	xQueue = xQueueCreate( 3, sizeof( TaskMessage) );
  

	xTaskCreate(Task1,"Task 1",100,( void * ) 0,1,&Task1Handler );
	xTaskCreate(Task2,"Task 2",100,( void * ) 0,1,&Task2Handler );
	xTaskCreate(Task3,"Task 3",100,( void * ) 0,1,&Task3Handler );
	xTaskCreate(Task4,"Task 4",100,( void * ) 0,1,&Task3Handler );
	vTaskStartScheduler();

/*-------------------------------------------------Task 3 Main End-------------------------------------------------*/

	for( ;; );
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


