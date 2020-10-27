/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )
	
/*	-----------------------------------Task 1 -----------------------------------

	TaskHandle_t LedTaskHandler = NULL;

-------------------------------------------------------------------------------*/

/*	-----------------------------------Task 2 -----------------------------------

	TaskHandle_t Led1TaskHandler = NULL;
	TaskHandle_t Led2TaskHandler = NULL;
	TaskHandle_t Led3TaskHandler = NULL;

-------------------------------------------------------------------------------*/


/*	-----------------------------------Task 3 ----------------------------------- */
	TaskHandle_t LedTaskHandler = NULL;
	TaskHandle_t ButtonTaskHandler = NULL;

	pinState_t ButtonState;  				 		 // Variable to Save Button State
	TickType_t OldTime,PressedTime; 		// Variable to Save Ticks 
	#define Button_Pressed 1			     // Flag to indicate that the button is pressed 
	#define Button_Released 0		      // Flag to indicate that the button is released 
	

/*-------------------------------------------------------------------------------*/

	
/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
/* -----------------------------------Task 1 ----------------------------------- 

void LedTask(void * pvParameters){
while(1){

	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
	vTaskDelay(1000);
	GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
	vTaskDelay(1000);
 }
}
----------------------------------------------------------------------------------------------------------------------------*/
/* -----------------------------------Task 2 ----------------------------------- 

void Led1_Task(void * pvParameters){
while(1){

	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
	vTaskDelay(100);
	GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
	vTaskDelay(100);
 }
}

void Led2_Task(void * pvParameters){
while(1){

	GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
	vTaskDelay(500);
	GPIO_write(PORT_0,PIN2,PIN_IS_LOW);
	vTaskDelay(500);

 }
}

void Led3_Task(void * pvParameters){
while(1){

	GPIO_write(PORT_0,PIN3,PIN_IS_HIGH);
	vTaskDelay(1000);
	GPIO_write(PORT_0,PIN3,PIN_IS_LOW);
	vTaskDelay(1000);

 }
}
----------------------------------------------------------------------------------------------------------------------------*/
	
/*	-----------------------------------Task 3 ----------------------------------- */

/**
 * this Task is responsable for controling led 
 */
void Led_Task(void * pvParameters){
while(1){
	
if(PressedTime>2000 && PressedTime<4000){ //chech if Pressed Time of the button is between 2 and 4 seconds 
	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
	vTaskDelay(400);
	GPIO_write(PORT_0,PIN1,PIN_IS_LOW); // Toggle the led every 400 ms
	vTaskDelay(400);}

else if(PressedTime>4000){     //chech if Pressed Time of the button is greater than 4 seconds 
	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH); 
	vTaskDelay(100);
	GPIO_write(PORT_0,PIN1,PIN_IS_LOW);// Toggle the led every 100 ms
	vTaskDelay(100);}

else if(PressedTime<2000){ //chech if Pressed Time of the button is less than 2 seconds 
		GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
} //turn th led off
	vTaskDelay(50);

 }
}

void Button_Task(void * pvParameters){
while(1){
  if (ButtonState == Button_Released && GPIO_read(PORT_0,PIN0)== PIN_IS_HIGH){ 		// Read the button state 
			OldTime=xTaskGetTickCount();																							 // Save current tick (when we pressed the button)
			ButtonState = Button_Pressed;
	}
	if (ButtonState == Button_Pressed &&  GPIO_read(PORT_0,PIN0)== PIN_IS_LOW ){ // Read the button state 
  PressedTime=xTaskGetTickCount()-OldTime;// Save current tick (when we Released the button) - works as timer to indicate how much time we pressed
		ButtonState = Button_Released;
	}
	vTaskDelay(50);
 }
}
/*-------------------------------------------------------------------------------*/


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	/*	-----------------------------------Task 1 ----------------------------------- 

	xTaskCreate(Led_Task,"Led Task",100,( void * ) 0,1,&LedTaskHandler );
		-------------------------------------------------------------------------------*/


/*	-----------------------------------Task 2 ----------------------------------- 

	xTaskCreate(Led1_Task,"Led 1 Task",100,( void * ) 0,1,&Led1TaskHandler );
	xTaskCreate(Led2_Task,"Led 2 Task",100,( void * ) 0,1,&Led2TaskHandler );
	xTaskCreate(Led3_Task,"Led 3 Task",100,( void * ) 0,1,&Led3TaskHandler );
	-------------------------------------------------------------------------------*/
	
	
	/*	-----------------------------------Task 3 ----------------------------------- */

	xTaskCreate(Led_Task,"Led Task",100,( void * ) 0,1,&LedTaskHandler );
	xTaskCreate(Button_Task,"Button Task",100,( void * ) 0,1,&ButtonTaskHandler );
										
	/*-------------------------------------------------------------------------------*/
    /* Create Tasks here */


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
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


