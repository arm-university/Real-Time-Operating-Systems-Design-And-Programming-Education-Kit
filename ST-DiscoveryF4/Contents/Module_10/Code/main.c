

#include <RTL.h>
#include <gpio.h>

#define MASK(x) (1UL << (x))
                      
// Task IDs for the tasks
OS_TID t_PS;                        
OS_TID t_TRL;                        
OS_TID t_TGL;                        
OS_TID t_TBL;                        

// Select only one of the scheduling options 
#define SCHED_NPRE_NPRI 1
#define SCHED_NPRE_PRI 0
#define SCHED_PRE_PRI 0


#define TOP_PRIORITY (9)
#define PS_PRIORITY (1) // try 1 (for low priority) and 5 (for high priority)
#define RED_PRIORITY (4)
#define BLUE_PRIORITY (3)
#define GREEN_PRIORITY (2)


#define TASK_PERIOD (30000)
#define TASK_DURATION (30)

#if (SCHED_PRE_PRI | SCHED_NPRE_PRI)
#define SET_INTERVAL(x) os_itv_set(x)
#define TASK_SUSPEND os_itv_wait()
//#define TASK_SUSPEND os_tsk_pass ()
#endif

#if (SCHED_NPRE_NPRI)
#define SET_INTERVAL(x) 
#define TASK_SUSPEND return;
#endif


void Delay (uint32_t nCount)
{
	uint32_t count = nCount*100000;
	while(count--)
	{
			__NOP();
	}
}



void Poll_Switch (void) {
	while (gpio_get(SWITCH)) {		// flash all LEDs
		// Light up LEDs
		gpio_set(LED[RED],LED_ON);
		gpio_set(LED[BLUE],LED_ON);
		gpio_set(LED[GREEN],LED_ON);
		Delay(10);
		// Turn off LEDs
		gpio_set(LED[RED],LED_OFF);
		gpio_set(LED[BLUE],LED_OFF);
		gpio_set(LED[GREEN],LED_OFF);
	  Delay(10);	
	}
}


__task void Task_Poll_Switch (void) {
	SET_INTERVAL(1);
	for (;;) {
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(TOP_PRIORITY);
#endif
		Poll_Switch();
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(PS_PRIORITY);
#endif

		TASK_SUSPEND;
	}
}


void ControlRedLED(void) {
	gpio_set(LED[RED],LED_ON);
	Delay(TASK_DURATION);	
	gpio_set(LED[RED],LED_OFF);
}

__task void Task_ControlRedLED (void) {
	SET_INTERVAL(TASK_PERIOD);
  for (;;) {
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(TOP_PRIORITY);
#endif
		ControlRedLED();
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(RED_PRIORITY);
#endif
		TASK_SUSPEND;
  }
}



void ControlBlueLED(void) {
	gpio_set(LED[BLUE],LED_ON);
	Delay(TASK_DURATION);	
	gpio_set(LED[BLUE],LED_OFF);
}

__task void Task_ControlBlueLED (void) {
	SET_INTERVAL(TASK_PERIOD);
  for (;;) {
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(TOP_PRIORITY);
#endif
		ControlBlueLED();
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(BLUE_PRIORITY);
#endif
		TASK_SUSPEND;
		}
}


void ControlGreenLED (void) {
	gpio_set(LED[GREEN],LED_ON);
	Delay(TASK_DURATION);	
	gpio_set(LED[GREEN],LED_OFF);
}

__task void Task_ControlGreenLED (void) {
	SET_INTERVAL(TASK_PERIOD);
  for (;;) {
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(TOP_PRIORITY);
#endif
		ControlGreenLED();
#if SCHED_NPRE_PRI		
		os_tsk_prio_self(GREEN_PRIORITY);
#endif
		TASK_SUSPEND;
  }
}



/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	t_PS = os_tsk_create( Task_Poll_Switch, PS_PRIORITY);
	t_TRL = os_tsk_create( Task_ControlRedLED, RED_PRIORITY);
	t_TBL = os_tsk_create( Task_ControlBlueLED, BLUE_PRIORITY);	
	t_TGL = os_tsk_create( Task_ControlGreenLED, GREEN_PRIORITY);
  os_tsk_delete_self (); // Delete the init(self) task
}

int main(void)
{
	gpio_set_mode(SWITCH,PullDown);
  gpio_set_mode(LED[RED],Output);                            /* Initialize the LEDs           */
	gpio_set_mode(LED[BLUE],Output);
	gpio_set_mode(LED[GREEN],Output);
// 	Preemptive, prioritized with RTX
#if (SCHED_PRE_PRI | SCHED_NPRE_PRI)
  os_sys_init(init);                        /* Initialize RTX and start init */
#endif

# if SCHED_NPRE_NPRI
	while (1) {
		Poll_Switch();
		ControlRedLED();
		ControlBlueLED();
		ControlGreenLED();
	}
#endif
	
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
