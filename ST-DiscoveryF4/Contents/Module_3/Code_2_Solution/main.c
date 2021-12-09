

#include <RTL.h>
#include <gpio.h>

OS_TID taskID1;                        
OS_TID taskID2; 

/*Modified the function to a task*/
__task void Turn_GreenLed_On(void){

		for(;;){
			gpio_set(LED[GREEN],LED_ON);
			//os_tsk_pass (); 
		}
		
}

/*Modified the function to a task*/
__task void Turn_GreenLed_Off(void){

		for(;;){
			gpio_set(LED[GREEN],LED_OFF);
			//os_tsk_pass (); 
		}

}

/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	
	taskID1 = os_tsk_create( Turn_GreenLed_On, 0);
	taskID2 = os_tsk_create( Turn_GreenLed_Off, 0);
	
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
