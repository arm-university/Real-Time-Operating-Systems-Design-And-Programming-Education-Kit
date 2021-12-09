

#include <stdlib.h>
#include <RTL.h>
#include <gpio.h>

#define RMIN CLK_FREQ/20
#define RMAX CLK_FREQ/10
#define RDIV 5
#define RAMT (RMAX-RMIN)/RDIV

OS_TID taskID0;                        
OS_TID taskID1;                        

void Delay (uint32_t nCount)
{
	while(nCount--)
	{
			__NOP();
	}
}

void Access(int colour){
		gpio_set(LED[colour],LED_ON);
}

void Release(int colour){
		gpio_set(LED[colour],LED_OFF);
}

int Check(int colour){
	return gpio_get(LED[colour]);
}



__task void Task0(void){
	
	  /*Critical section starts from here*/
		gpio_set(LED[BLUE],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);
		gpio_set(LED[BLUE],LED_OFF);
		/*Critical section ends here*/

	  os_tsk_delete_self ();
}

__task void Task1(void){
	
		/*Critical section starts from here*/
		gpio_set(LED[GREEN],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);		
		gpio_set(LED[GREEN],LED_OFF);
		/*Critical section ends here*/
	
	  os_tsk_delete_self ();
}


/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	
	taskID0 = os_tsk_create( Task0, 0);
	//taskID1 = os_tsk_create( Task1, 0);

  os_tsk_delete_self (); // Delete the init(self) task
}

int main(void)
{
	
		gpio_set_mode(LED[RED],Output);
		gpio_set_mode(LED[GREEN],Output);		
		gpio_set_mode(LED[BLUE],Output);
		Delay(RMIN);
		os_sys_init(init);
	
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
