

#include <stdlib.h>
#include <RTL.h>
#include <gpio.h>


#define RMIN CLK_FREQ/20
#define RMAX CLK_FREQ/10
#define RDIV 5
#define RAMT (RMAX-RMIN)/RDIV

#define RANDOM_SEED 0

OS_TID taskID1;                        
OS_TID taskID2;    

#define BUFFER_SIZE 10  
 
int buffer [BUFFER_SIZE];
int producer_datum;
int consumer_datum;
int i = 0;



void Delay (uint32_t nCount)
{
	while(nCount--)
	{
			__NOP();
	}
}

/*----------------------------------------------------------------------------
  Produce a random number ranging from 0 to 19.
 *----------------------------------------------------------------------------*/
int produce(void){
	Delay(rand() % RDIV*RAMT + RMIN);
	return (rand()%20);
}
	
/*----------------------------------------------------------------------------
  Consume the datum, actually doing nothing.
 *----------------------------------------------------------------------------*/
void consume(int datum){	
	Delay(rand() % RDIV*RAMT + RMIN);
}	
	
/*----------------------------------------------------------------------------
  Append the new datum to the buffer
 *----------------------------------------------------------------------------*/
void append(int datum){
	buffer[i]=datum;	
	Delay(rand() % RDIV*RAMT + RMIN);
	i++;
}

/*----------------------------------------------------------------------------
  Extract datum from the buffer
 *----------------------------------------------------------------------------*/
int extract(void){
	i--;
	Delay(rand() % RDIV*RAMT + RMIN);
	return buffer[i];
}


__task void Producer(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();
			append(producer_datum);
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer(void){
		while(1){
		//Green for Consumer
		gpio_set(LED[GREEN],1);
		consumer_datum=extract();
		consume(consumer_datum);
		gpio_set(LED[GREEN],0);
		Delay(rand() % RDIV*RAMT + RMIN);
		}
}


/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	
	
	taskID1=os_tsk_create(Producer,0);
	//taskID2=os_tsk_create(Consumer,0);

  os_tsk_delete_self (); // Delete the init(self) task
}

int main(void)
{
		srand(RANDOM_SEED);
		gpio_set_mode(LED[RED],Output);
		gpio_set_mode(LED[GREEN],Output);		
		gpio_set_mode(LED[BLUE],Output);
		Delay(RMIN);
		os_sys_init(init);
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
