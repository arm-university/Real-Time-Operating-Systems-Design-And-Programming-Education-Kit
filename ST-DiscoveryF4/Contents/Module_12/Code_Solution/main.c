
#include <stdlib.h>
#include <RTL.h>
#include <gpio.h>
#include <adc.h>
#include <setup.h>

#define COEFFICIENT_RAM_GEN 0
#define ML 1

#define RMIN CLK_FREQ/120000
#define RMAX CLK_FREQ/110000
#define RDIV 20
#define RAMT (RMAX-RMIN)/RDIV

#define MAX_COUNT 10000
#define MAX_LED_NUM 4

#if ML
#define TRAIN_COUNT 2000
#endif

OS_TID tskID_Tsk_Dispatcher;
OS_TID tskID_Tsk_LED[LED_NUM];

int count=0;
int end_time;

//Statically assign the priority
int LED_priority[4]={1,4,2,3};
//The coefficients reflect the program's preference 
int LED_preference[4]={4,3,2,1};


void Delay (uint32_t nCount)
{
	while(nCount--)
	{
			__NOP();
	}
}

__task void Tsk_LED(void *colour){
	
		while(1){
			//Turn on the LED
			gpio_set(LED[(*(int *)colour)],LED_ON);
			//Do some computation
			Delay((int)((rand() % RDIV*RAMT + RMIN))*(LED_preference[(*(int *)colour)]));
			//Turn off the LED
			gpio_set(LED[(*(int *)colour)],LED_OFF);
			//Do some computation
			Delay((int)((rand() % RDIV*RAMT + RMIN))*(LED_preference[(*(int *)colour)]));		
			//Readujust self priority
			os_tsk_prio_self(1);
			//Give up current control section
			os_tsk_pass();
		}
		
}

/*----------------------------------------------------------------------------
  The dispatcher task dispatches LED tasks
 *----------------------------------------------------------------------------*/
__task void Tsk_Dispatcher(){
	
	int i;
	int j;
	
#if ML
	int current_cycle_time;
	int last_cycle_time;
	int temp1[4];
	int temp2[4];
	double record_priority[4];
#endif
	
	while(count<MAX_COUNT){
		count++;

#if	ML	
		if(count<TRAIN_COUNT){
			current_cycle_time=os_time_get();
			for(i=0;i<4;i++){
				record_priority[i]=record_priority[i]+LED_priority[i]*(current_cycle_time-last_cycle_time);
			}			
			temp1[0]	= rand()%1000;
			temp1[1]	= rand()%1000;	
			temp1[2]	= rand()%1000;
			temp1[3]	= rand()%1000;					
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){
					if(temp1[i]>=temp1[j]){
						temp2[i]++;
					}						
				}												
				LED_priority[i]=temp2[i];
				temp2[i]=0;
			}
		}
		if(count==TRAIN_COUNT){
			for(i=0;i<4;i++){
				temp2[i]=5;
				for(j=0;j<4;j++){
					if(record_priority[i]>=record_priority[j]){
						temp2[i]--;
					}						
				}												
				LED_priority[i]=temp2[i];
			}	
		}
		last_cycle_time=current_cycle_time;
#endif
		
		//Dispatch tasks
		for(i=0;i<LED_NUM&&i<MAX_LED_NUM;i++){
			for(j=LED_priority[i];j>0;j--){
				os_tsk_prio (tskID_Tsk_LED[i], 10);
			}
		}
	}
		//Calculate the time to finish
		end_time=os_time_get();
		//Delete tasks
		for(i=0;i<LED_NUM&i<MAX_LED_NUM;i++){
			os_tsk_delete(tskID_Tsk_LED[i]);
		}
		os_tsk_delete_self ();
}



/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void){

		int led[LED_NUM];
		int i=0;
	
//Generate random preference	
#if COEFFICIENT_RAM_GEN
			int j=0;
			int temp1[4];
			int temp2[4];
			temp1[0]	= rand()%1000;
			temp1[1]	= rand()%1000;	
			temp1[2]	= rand()%1000;
			temp1[3]	= rand()%1000;					
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){
					if(temp1[i]>=temp1[j]){
						temp2[i]++;
					}						
				}												
				LED_preference[i]=temp2[i];
				temp2[i]=0;
			}
#endif

		os_tsk_prio_self(10);
		tskID_Tsk_Dispatcher=os_tsk_create(Tsk_Dispatcher,9);
		//Create LED tasks
		for(i=0;i<LED_NUM&&i<MAX_LED_NUM;i++){
			led[i]=i;
			tskID_Tsk_LED[i]=os_tsk_create_ex(Tsk_LED,0,&led[i]);
		}
		os_tsk_delete_self (); // Delete the init(self) task
}

int main(void)
{
		int i;
	
		SystemClock_Config(); // Configure the System Clock.
	
		adc_init();
		srand(adc_read());
		for(i=0;i<LED_NUM&i<4;i++){
			gpio_set_mode(LED[i],Output);
		}
		os_sys_init(init);
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
