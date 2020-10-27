

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
// this task take an action on led based on button state

void Led_Task(void * pvParameters){
while(1){
if(xSemaphore!=NULL){	
if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE ){		//check if i can take the semaphore 
	LedState=!LedState; 																						// toggle led 
}
}
	if(ButtonState == Button_Released)	 GPIO_write(PORT_0,PIN1,LedState); // if you released the button it will show the new state of the led

	 vTaskDelay(50);
  }
}

//this task read the button state
void Button_Task(void * pvParameters){
while(1){
	
  if (ButtonState == Button_Released && GPIO_read(PORT_0,PIN0)== PIN_IS_HIGH)	 ButtonState = Button_Pressed; // check if the button is pressed
	if (ButtonState == Button_Pressed &&  GPIO_read(PORT_0,PIN0)== PIN_IS_LOW ){ // check if the button is released
		ButtonState = Button_Released;
		xSemaphoreGive(xSemaphore);// give the semaphore 
}
	vTaskDelay(100);
 }
}
-------------------------------------------------Task 1 End-------------------------------------------------*/


/*-------------------------------------------------Task 2 Start-------------------------------------------------
//this task wirte on the uart 
void Task1(void * pvParameters){
	char x ;
	uint32_t Counter = 0 ;

while(1){
if(xMutex!=NULL){	//check if the mutex has been created successfully
if( xSemaphoreTake( xMutex, ( TickType_t ) 10 ) == pdTRUE ){	// try to take the mutex 
for(x=0 ; x<10; x++){	// send on uart 10 times 
vSerialPutString(Task1String,18);
for(Counter=0;Counter<500000;Counter++){} // Heavy load so we can see it on uart	

}
xSemaphoreGive( xMutex ); // give back the mutex so  any task can take it 
 }
}
vTaskDelay(100);
 }
}

//this task wirte on the uart 
void Task2(void * pvParameters){
uint32_t Counter = 0 ;
char y ;
while(1){
if(xMutex!=NULL){	//check if the mutex has been created successfully
if( xSemaphoreTake( xMutex, ( TickType_t ) 10 ) == pdTRUE ){// try to take the mutex 
for( y=0;y<10;y++){	// send on uart 10 times 
vSerialPutString(Task2String,18);	
for(Counter=0;Counter<1000000;Counter++){} // Heavy load so we can see it on uart 	
}
xSemaphoreGive( xMutex ); // give back the mutex so  any task can take it 
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
	
	CheckVariable=FirstButtonState;	// save current state of the button
	
  if (FirstButtonState == Button_1_Released && GPIO_read(PORT_0,PIN0)== PIN_IS_HIGH){	 //Check if button is pressed			
	FirstButtonState=Button_1_Pressed;
	strcpy(Task1Message," Button 1 Is Pressed \n"); // check if the button is pressed copy "Button 1 Is Pressed " to Task1Message array to put it in queue
	}
	else if (FirstButtonState == Button_1_Pressed &&  GPIO_read(PORT_0,PIN0)== PIN_IS_LOW ){//Check if button is Released	
		FirstButtonState=Button_1_Released;
		strcpy(Task1Message," Button 1 Is Released \n");// check if the button is Released copy "Button 1 Is Released " to Task1Message array to put it in queue
	}
			if (FirstButtonState != CheckVariable){// check if the button state has been changed
		if(xQueueSend(xQueue,( void * )Task1Message, ( TickType_t ) portMAX_DELAY)==pdTRUE){}//check if it can send the string array in the queue
		else xSerialPutChar('E');// if it cant it will put E on serial 
		}
				vTaskDelay(50);

 }
}

void Task2(void * pvParameters){
	 char Task2Message[25],CheckVariable;
while(1){
		  CheckVariable=SecondButtonState;	// save current state of the button
  if (SecondButtonState == Button_2_Released && GPIO_read(PORT_0,PIN1)== PIN_IS_HIGH){	 //Check if button is pressed	 			
	SecondButtonState=Button_2_Pressed;
	strcpy(Task2Message," Button 2 Is Pressed \n");// check if the button is pressed copy "Button 2 Is Pressed " to Task1Message array to put it in queue
	}
	else if (SecondButtonState == Button_2_Pressed &&  GPIO_read(PORT_0,PIN1)== PIN_IS_LOW ){//Check if button is Released	
		SecondButtonState=Button_2_Released;
		strcpy(Task2Message," Button 2 Is Released \n");// check if the button is Released copy "Button 2 Is Released " to Task1Message array to put it in queue
	}
		if (SecondButtonState != CheckVariable){// check if the button state has been changed
		if(xQueueSend(xQueue,( void * )Task2Message, ( TickType_t ) portMAX_DELAY)==pdTRUE){}//check if it can send the string array in the queue
		else xSerialPutChar('E');// if it cant it will put E on serial 
		}
	vTaskDelay(50);
			
 }
}
//this task send string every 100 ms
void Task3(void * pvParameters){
signed char Task3String[]=" String to Be Sent Every 100 Ms \n";
while(1){
 		
if(xQueueSend(xQueue,( void * )Task3String, ( TickType_t ) portMAX_DELAY)==pdTRUE){}//check if it can send the string array in the queue
else xSerialPutChar('E');// if it cant it will put E on serial 
vTaskDelay(100);
 }
}

void Task4(void * pvParameters){
	signed char Buff[35];
	while(1){
if(xQueueReceive(xQueue,&Buff,( TickType_t ) portMAX_DELAY)==pdTRUE){// receive the queue message and save it in Buff. array 
vSerialPutString(Buff,35);//send buff array on uart 
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
	xSemaphore=xSemaphoreCreateBinary();//create semaphore
	xTaskCreate(Led_Task,"Led Task",100,( void * ) 0,1,&LedTaskHandler );
	xTaskCreate(Button_Task,"Button Task",100,( void * ) 0,1,&ButtonTaskHandler );

-------------------------------------------------Task 1 Main End-------------------------------------------------*/


/*-------------------------------------------------Task 2 Main Start-------------------------------------------------
xMutex=xSemaphoreCreateMutex(); // create mutex
	
	xTaskCreate(Task1,"Task 1",100,( void * ) 0,2,&Task1Handler );
	xTaskCreate(Task2,"Task 2",100,( void * ) 0,1,&Task2Handler );
-------------------------------------------------Task 2 Main End-------------------------------------------------*/

/*-------------------------------------------------Task 2 Main Start-------------------------------------------------*/

	xQueue = xQueueCreate( 3, sizeof( TaskMessage) ); // create queue with TaskMessage array size
  

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


