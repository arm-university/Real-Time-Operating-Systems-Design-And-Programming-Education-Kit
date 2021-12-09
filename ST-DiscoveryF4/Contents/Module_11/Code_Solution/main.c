

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

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
 
int buffer [BUFFER_SIZE];
int producer_datum;
int consumer_datum;
int i = 0;

OS_TID taskID1;                        
OS_TID taskID2;
OS_MUT mutexID;
OS_SEM semID;
OS_SEM semID1;
OS_SEM semID2;

OS_SEM semIDA;
OS_SEM semIDB;
int tickets;


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



//Original Version
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

//Mutex Version
__task void Producer_1(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();			
			os_mut_wait (mutexID, 0xFFFF);
			append(producer_datum);
			os_mut_release (mutexID);			
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer_1(void){
		while(1){
			//Green for Consumer
			gpio_set(LED[GREEN],1);	
			os_mut_wait (mutexID, 0xFFFF);
			consumer_datum=extract();
			os_mut_release (mutexID);		
			consume(consumer_datum);
			gpio_set(LED[GREEN],0);	
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

//Signaling Version
__task void Producer_2(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();
			append(producer_datum);
			os_sem_send (semID);
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer_2(void){
		while(1){
			//Green for Consumer
			gpio_set(LED[GREEN],1);
			os_sem_wait (semID, 0xFFFF );
			consumer_datum=extract();	
			consume(consumer_datum);
			gpio_set(LED[GREEN],0);	
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

//Two Semaphores Version
__task void Producer_3(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();
			os_sem_wait (semID1, 0xFFFF);
			append(producer_datum);
			os_sem_send (semID1);
			os_sem_send (semID2);
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer_3(void){
		while(1){
			//Green for Consumer
			gpio_set(LED[GREEN],1);
			os_sem_wait (semID2, 0xFFFF );
			os_sem_wait (semID1, 0xFFFF );
			consumer_datum=extract();
			os_sem_send (semID1);
			consume(consumer_datum);
			gpio_set(LED[GREEN],0);	
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

//Swapped sem_send in Producer Version
__task void Producer_4(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();
			os_sem_wait (semID1, 0xFFFF);
			append(producer_datum);
			os_sem_send (semID2);			
			os_sem_send (semID1);
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer_4(void){
		while(1){
			//Green for Consumer
			gpio_set(LED[GREEN],1);
			os_sem_wait (semID2, 0xFFFF );
			os_sem_wait (semID1, 0xFFFF );
			consumer_datum=extract();
			os_sem_send (semID1);
			consume(consumer_datum);
			gpio_set(LED[GREEN],0);	
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

//Swapped sem_wait in Consumer Version
__task void Producer_5(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();
			os_sem_wait (semID1, 0xFFFF);
			append(producer_datum);
			os_sem_send (semID1);
			os_sem_send (semID2);			
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer_5(void){
		while(1){
			//Green for Consumer
			gpio_set(LED[GREEN],1);
			os_sem_wait (semID1, 0xFFFF );
			//The delay here is deliberately inserted to invite the deadlock
			Delay(rand() % RDIV*RAMT + RMIN);
			Delay(rand() % RDIV*RAMT + RMIN);
			os_sem_wait (semID2, 0xFFFF );
			consumer_datum=extract();
			os_sem_send (semID1);
			consume(consumer_datum);
			gpio_set(LED[GREEN],0);	
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

//Mutex Signaling Version
__task void Producer_6(void){
		while(1){
			//Blue for Producer
			gpio_set(LED[BLUE],1);
			producer_datum=produce();
			os_mut_wait (mutexID, 0xFFFF);
			append(producer_datum);
			os_mut_release (mutexID);
			os_sem_send (semID);
			gpio_set(LED[BLUE],0);
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

__task void Consumer_6(void){
		while(1){
			//Green for Consumer
			gpio_set(LED[GREEN],1);
			os_sem_wait (semID, 0xFFFF );
			os_mut_wait (mutexID, 0xFFFF);
			consumer_datum=extract();
			os_mut_release (mutexID);
			consume(consumer_datum);
			gpio_set(LED[GREEN],0);	
			Delay(rand() % RDIV*RAMT + RMIN);
		}
}

//Solution for the general semaphore
//Semaphore A is actually a mutex
//Semaphore B is keeping track of if there is still any ticket.
void general_init_sem(int inital_value){
		os_sem_init (semIDA, 1);
		os_sem_init (semIDB, MIN(1,inital_value));
		tickets = inital_value;
}
void general_wait_sem(void){
	os_sem_wait(semIDB,0xFFFF);
	os_sem_wait(semIDA,0XFFFF);	
	tickets--;
	if(tickets>0){	
		os_sem_send(semIDB);	
	}
	os_sem_send(semIDA);	
}
void general_send_sem(void){
	os_sem_wait(semIDA,0xFFFF);
	tickets++;
	if(tickets==1){
			os_sem_send(semIDB);
	}
	os_sem_send(semIDA);
}


/*----------------------------------------------------------------------------
  The first task run by the OS and should do the initialization for other tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
	os_mut_init (mutexID);
	os_sem_init (semID, 0);
	os_sem_init (semID1, 1);
	os_sem_init (semID2, 0);
	general_init_sem(0);
	taskID1=os_tsk_create(Producer_6,0);
	taskID2=os_tsk_create(Consumer_6,0);
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
