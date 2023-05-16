

#include <stdlib.h>
#include <RTL.h>
#include <gpio.h>

#define RMIN CLK_FREQ/20
#define RMAX CLK_FREQ/10
#define RDIV 5
#define RAMT (RMAX-RMIN)/RDIV


OS_TID taskID0;                        
OS_TID taskID1;    

int token = 0 ;
int flag[2] = {0,0} ;

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

//First Scheme
__task void Task0_1(void){

		while(Check(RED)){}
		//Delay(rand() % RDIV*RAMT + RMIN);
	  /*Critical section starts from here*/
		gpio_set(LED[BLUE],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);
		gpio_set(LED[BLUE],LED_OFF);
		/*Critical section ends here*/
	
	  os_tsk_delete_self ();
}

__task void Task1_1(void){
	
		while(Check(RED)){}
		//Delay(rand() % RDIV*RAMT + RMIN);	
		/*Critical section starts from here*/
		gpio_set(LED[GREEN],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);		
		gpio_set(LED[GREEN],LED_OFF);	
		/*Critical section ends here*/

	  os_tsk_delete_self ();
}

//Second Scheme
__task void Task0_2(void){

	
		while(token!=0){}
	  /*Critical section starts from here*/
		gpio_set(LED[BLUE],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);
		gpio_set(LED[BLUE],LED_OFF);
		/*Critical section ends here*/
		token=1-token;
	
	  os_tsk_delete_self ();
}

__task void Task1_2(void){

		while(token!=1){}
		/*Critical section starts from here*/
		gpio_set(LED[GREEN],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);		
		gpio_set(LED[GREEN],LED_OFF);	
		/*Critical section ends here*/
		token=1-token;

	  os_tsk_delete_self ();
}

//Third Scheme
__task void Task0_3(void){

		flag[0]=1;
		Delay(rand() % RDIV*RAMT + RMIN);			
		while(flag[1]){}
	  /*Critical section starts from here*/
		gpio_set(LED[BLUE],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);
		gpio_set(LED[BLUE],LED_OFF);
		/*Critical section ends here*/
		flag[0]=0;
	
	  os_tsk_delete_self ();
}

__task void Task1_3(void){

		flag[1]=1;
		Delay(rand() % RDIV*RAMT + RMIN);	
		while(flag[0]){}
		/*Critical section starts from here*/
		gpio_set(LED[GREEN],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);		
		gpio_set(LED[GREEN],LED_OFF);	
		/*Critical section ends here*/
		flag[1]=0;

	  os_tsk_delete_self ();
}

//Fourth Scheme
__task void Task0_4(void){

		flag[0]=1;
		Delay(rand() % RDIV*RAMT + RMIN);			
		while(flag[1]){	
			Delay(rand() % RDIV*RAMT + RMIN);	
			flag[0]=0;
			Delay(500);	
			flag[0]=1;
		}
	  /*Critical section starts from here*/
		gpio_set(LED[BLUE],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);
		gpio_set(LED[BLUE],LED_OFF);
		/*Critical section ends here*/
		flag[0]=0;
	
	  os_tsk_delete_self ();
}

__task void Task1_4(void){

		flag[1]=1;
		Delay(rand() % RDIV*RAMT + RMIN);	
		while(flag[0]){
			Delay(rand() % RDIV*RAMT + RMIN);	
			flag[1]=0;	
			Delay(500);	
			flag[1]=1;		
		}
		/*Critical section starts from here*/
		gpio_set(LED[GREEN],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);		
		gpio_set(LED[GREEN],LED_OFF);	
		/*Critical section ends here*/
		flag[1]=0;

	  os_tsk_delete_self ();
}

//Dekker's Algorithm
__task void Task0_5(void){

		flag[0]=1;		
		while(flag[1]){	
			if(token==1){
				flag[0]=0;
				while(token==1){}
				flag[0]=1;
			}
		}
	  /*Critical section starts from here*/
		gpio_set(LED[BLUE],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);
		gpio_set(LED[BLUE],LED_OFF);
		/*Critical section ends here*/
		token=1-token;
		flag[0]=0;
	
	  os_tsk_delete_self ();
}

__task void Task1_5(void){

		flag[1]=1;
		while(flag[0]){
			if(token==0){	
				flag[1]=0;	
				while(token==1){}
				flag[1]=1;		
			}
		
		}			
		/*Critical section starts from here*/
		gpio_set(LED[GREEN],LED_ON);
		Access(RED);
		Delay(rand() % RDIV*RAMT + RMIN);
		Release(RED);		
		gpio_set(LED[GREEN],LED_OFF);	
		/*Critical section ends here*/
		flag[1]=0;

	  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	//Taskx_y is the xth scheme for task x
	taskID0 = os_tsk_create(Task0_5, 0);
	taskID1 = os_tsk_create(Task1_5, 0);
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
