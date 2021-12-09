

#include <RTL.h>
#include <gpio.h>


/*Modified the function to a task*/
void Turn_GreenLed_On(void){

		for(;;){
			gpio_set(LED[GREEN],LED_ON);
		}

}

/*Modified the function to a task*/
void Turn_GreenLed_Off(void){

		for(;;){
			gpio_set(LED[GREEN],LED_OFF);
		}

}

/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	
	/*Create Tasks here*/
  os_tsk_delete_self (); // Delete the init(self) task
}

int main(void)
{
		gpio_set_mode(LED[GREEN],Output);
		gpio_set_mode(LED[RED],Output);
		gpio_set(LED[RED],LED_ON);
		os_sys_init(init);
	
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
