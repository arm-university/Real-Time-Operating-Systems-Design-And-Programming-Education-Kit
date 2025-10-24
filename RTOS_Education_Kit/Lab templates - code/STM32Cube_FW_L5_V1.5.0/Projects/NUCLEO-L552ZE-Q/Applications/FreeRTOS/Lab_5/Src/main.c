/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define RMIN      (SystemCoreClock / 20)
#define RMAX      (SystemCoreClock / 10)
#define RDIV      5
#define RAMT      ((RMAX - RMIN) / RDIV)
#define RANDOM_SEED 0

#define BUFFER_SIZE 10
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
/* USER CODE END PM */
int buffer[BUFFER_SIZE];
int producer_datum;
int consumer_datum;
int i = 0;
int tickets;

osThreadId_t ThreadID1;
osThreadId_t ThreadID2;

osMutexId_t mutexID;
osSemaphoreId_t semID;
osSemaphoreId_t semID1;
osSemaphoreId_t semID2;
osSemaphoreId_t semIDA;
osSemaphoreId_t semIDB;
/* Private variables ---------------------------------------------------------*/

/* Definitions for SEM_Thread1 */
const osMutexAttr_t mutexAttr = {
    .name = "mutex"
};

const osSemaphoreAttr_t semAttr = {
    .name = "sem"
};

void Delay(uint32_t nCount) {
    while(nCount--) {
        __NOP();
    }
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
void general_init_sem(int initial_value);
void Producer_0(void *arg);
void Consumer_0(void *arg);
void Producer_1(void *arg);
void Consumer_1(void *arg);
void Producer_2(void *arg);
void Consumer_2(void *arg);
void Producer_3(void *arg);
void Consumer_3(void *arg);
void Producer_4(void *arg);
void Consumer_4(void *arg);
void Producer_5(void *arg);
void Consumer_5(void *arg);
void Producer_6(void *arg);
void Consumer_6(void *arg);


/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

	BSP_LED_Init(LED3); // RED (unused in this version)
  BSP_LED_Init(LED1); // GREEN
  BSP_LED_Init(LED2); // BLUE
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Initialize LEDs */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
	srand(RANDOM_SEED);
  Delay(RMIN);
	
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of osSemaphore */
  mutexID = osMutexNew(&mutexAttr);
  semID = osSemaphoreNew(BUFFER_SIZE, 0, &semAttr);
  semID1 = osSemaphoreNew(1, 1, &semAttr);
  semID2 = osSemaphoreNew(1, 0, &semAttr);

  general_init_sem(0);

  ThreadID1 = osThreadNew(Producer_0, NULL, NULL);
  ThreadID2 = osThreadNew(Consumer_0, NULL, NULL);
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/* USER CODE BEGIN 4 */
int GetRandomDelay() {
    return (rand() % (RDIV * RAMT)) + RMIN;
}

int produce(void) {
    Delay(GetRandomDelay());
    return rand() % 20;
}

void consume(int datum) {
    Delay(GetRandomDelay());
}

void append(int datum) {
    buffer[i] = datum;
    Delay(GetRandomDelay());
    i++;
}

int extract(void) {
    i--;
    Delay(GetRandomDelay());
    return buffer[i];
}

void general_init_sem(int initial_value) {
    semIDA = osSemaphoreNew(1, 1, &semAttr);
    semIDB = osSemaphoreNew(1, MIN(1, initial_value), &semAttr);
    tickets = initial_value;
}

void general_wait_sem(void) {
    osSemaphoreAcquire(semIDB, osWaitForever);
    osSemaphoreAcquire(semIDA, osWaitForever);
    tickets--;
    if (tickets > 0) {
        osSemaphoreRelease(semIDB);
    }
    osSemaphoreRelease(semIDA);
}

void general_send_sem(void) {
    osSemaphoreAcquire(semIDA, osWaitForever);
    tickets++;
    if (tickets == 1) {
        osSemaphoreRelease(semIDB);
    }
    osSemaphoreRelease(semIDA);
}



// Producer & Consumer 0: No Sync
void Producer_0(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        producer_datum = produce();
        append(producer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}

void Consumer_0(void *arg) {
    for (;;) {
        BSP_LED_On(LED1);
        consumer_datum = extract();
        consume(consumer_datum);
        BSP_LED_Off(LED1);
        Delay(GetRandomDelay());
    }
}

// Producer & Consumer 1: Mutex
void Producer_1(void *arg) {
    for (;;) {
        BSP_LED_On(LED3);
        producer_datum = produce();
        osMutexAcquire(mutexID, osWaitForever);
        append(producer_datum);
        osMutexRelease(mutexID);
        BSP_LED_Off(LED3);
        Delay(GetRandomDelay());
    }
}

void Consumer_1(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        osMutexAcquire(mutexID, osWaitForever);
        consumer_datum = extract();
        osMutexRelease(mutexID);
        consume(consumer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}

// Producer & Consumer 2: 1 Semaphore
void Producer_2(void *arg) {
    for (;;) {
        BSP_LED_On(LED3);
        producer_datum = produce();
        append(producer_datum);
        osSemaphoreRelease(semID);
        BSP_LED_Off(LED3);
        Delay(GetRandomDelay());
    }
}

void Consumer_2(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        osSemaphoreAcquire(semID, osWaitForever);
        consumer_datum = extract();
        consume(consumer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}

// Producer & Consumer 3: Two Semaphores
void Producer_3(void *arg) {
    for (;;) {
        BSP_LED_On(LED3);
        producer_datum = produce();
        osSemaphoreAcquire(semID1, osWaitForever);
        append(producer_datum);
        osSemaphoreRelease(semID1);
        osSemaphoreRelease(semID2);
        BSP_LED_Off(LED3);
        Delay(GetRandomDelay());
    }
}

void Consumer_3(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        osSemaphoreAcquire(semID2, osWaitForever);
        osSemaphoreAcquire(semID1, osWaitForever);
        consumer_datum = extract();
        osSemaphoreRelease(semID1);
        consume(consumer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}

// Producer & Consumer 4: Swapped sem_send
void Producer_4(void *arg) {
    for (;;) {
        BSP_LED_On(LED3);
        producer_datum = produce();
        osSemaphoreAcquire(semID1, osWaitForever);
        append(producer_datum);
        osSemaphoreRelease(semID2);
        osSemaphoreRelease(semID1);
        BSP_LED_Off(LED3);
        Delay(GetRandomDelay());
    }
}

void Consumer_4(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        osSemaphoreAcquire(semID2, osWaitForever);
        osSemaphoreAcquire(semID1, osWaitForever);
        consumer_datum = extract();
        osSemaphoreRelease(semID1);
        consume(consumer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}

// Producer & Consumer 5: Swapped sem_wait with deadlock
void Producer_5(void *arg) {
    for (;;) {
        BSP_LED_On(LED3);
        producer_datum = produce();
        osSemaphoreAcquire(semID1, osWaitForever);
        append(producer_datum);
        osSemaphoreRelease(semID1);
        osSemaphoreRelease(semID2);
        BSP_LED_Off(LED3);
        Delay(GetRandomDelay());
    }
}

void Consumer_5(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        osSemaphoreAcquire(semID1, osWaitForever);
        Delay(GetRandomDelay());
        Delay(GetRandomDelay());
        osSemaphoreAcquire(semID2, osWaitForever);
        consumer_datum = extract();
        osSemaphoreRelease(semID1);
        consume(consumer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}

// Producer & Consumer 6: Mutex + Semaphore
void Producer_6(void *arg) {
    for (;;) {
        BSP_LED_On(LED3);
        producer_datum = produce();
        osMutexAcquire(mutexID, osWaitForever);
        append(producer_datum);
        osMutexRelease(mutexID);
        osSemaphoreRelease(semID);
        BSP_LED_Off(LED3);
        Delay(GetRandomDelay());
    }
}

void Consumer_6(void *arg) {
    for (;;) {
        BSP_LED_On(LED2);
        osSemaphoreAcquire(semID, osWaitForever);
        osMutexAcquire(mutexID, osWaitForever);
        consumer_datum = extract();
        osMutexRelease(mutexID);
        consume(consumer_datum);
        BSP_LED_Off(LED2);
        Delay(GetRandomDelay());
    }
}
/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
