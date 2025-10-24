/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

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
#define mutexSHORT_DELAY     ((uint32_t) 20)
#define mutexNO_DELAY        ((uint32_t) 0)
#define mutexTWO_TICK_DELAY  ((uint32_t) 2)
#define RDIV 10
#define RAMT 10
#define RMIN 100

volatile uint8_t token = 0;  // Start with Thread0_2 allowed

volatile uint8_t flag[2] = {0, 0};
volatile uint8_t turn = 0;

// Define color IDs
#define BLUE  2
#define GREEN 1
#define RED   3

// Define LED hardware mapping based on board
const uint32_t LEDS[] = {
    LED2,  // BLUE
    LED1,  // GREEN
    LED3   // RED
};

volatile uint8_t redLock = 0;

osThreadId_t Thread0_2Handle;
osThreadId_t Thread1_2Handle;
osThreadId_t Thread0_3Handle;
osThreadId_t Thread1_3Handle;
osThreadId_t Thread0_4Handle;
osThreadId_t Thread1_4Handle;
osThreadId_t Thread0_5Handle;
osThreadId_t Thread1_5Handle;

void Access(int colour)
{
    if (colour == RED)
        redLock = 1;

    BSP_LED_On(LEDS[colour]);
}

void Release(int colour)
{
    if (colour == RED)
        redLock = 0;

    BSP_LED_Off(LEDS[colour]);
}

uint8_t Check(int colour)
{
    return (colour == RED && redLock);
}



static uint32_t seed = 123456789;

uint32_t GetRandomDelay(void)
{
    seed = (1103515245 * seed + 12345) & 0x7FFFFFFF;
    return (seed % (RDIV * RAMT)) + RMIN;
}


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* Definitions for osMutex */
osMutexId_t osMutexHandle;
const osMutexAttr_t osMutex_attributes = {
  .name = "osMutex"
};

osThreadId_t Thread0Handle;
const osThreadAttr_t Thread0_attributes = {
  .name = "Thread0",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

osThreadId_t Thread1Handle;
const osThreadAttr_t Thread1_attributes = {
  .name = "Thread1",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

osThreadId_t Thread0_1Handle;
const osThreadAttr_t Thread0_1_attributes = {
  .name = "Thread0_1",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

osThreadId_t Thread1_1Handle;
const osThreadAttr_t Thread1_1_attributes = {
  .name = "Thread1_1",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread0_2_attributes = {
  .name = "Thread0_2",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread1_2_attributes = {
  .name = "Thread1_2",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread0_3_attributes = {
  .name = "Thread0_3",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread1_3_attributes = {
  .name = "Thread1_3",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread0_4_attributes = {
  .name = "Thread0_4",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread1_4_attributes = {
  .name = "Thread1_4",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread0_5_attributes = {
  .name = "Thread0_5",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t Thread1_5_attributes = {
  .name = "Thread1_5",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* USER CODE BEGIN PV */

/* Variables used to detect and latch errors */
__IO uint32_t HighPriorityThreadCycles = 0, MediumPriorityThreadCycles = 0, LowPriorityThreadCycles = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
void Thread0(void *argument);
void Thread1(void *argument);
void Thread0_1(void *argument);
void Thread1_1(void *argument);
void Thread0_2(void *argument);
void Thread1_2(void *argument);
void Thread0_3(void *argument);
void Thread1_3(void *argument);
void Thread0_4(void *argument);
void Thread1_4(void *argument);
void Thread0_5(void *argument);
void Thread1_5(void *argument);

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

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
	
	Thread0Handle = osThreadNew(Thread0, NULL, &Thread0_attributes);
	Thread1Handle = osThreadNew(Thread1, NULL, &Thread1_attributes);
	
	//Thread0_1Handle = osThreadNew(Thread0_1, NULL, &Thread0_1_attributes);
	//Thread1_1Handle = osThreadNew(Thread1_1, NULL, &Thread1_1_attributes);
	
	//Thread0_2Handle = osThreadNew(Thread0_2, NULL, &Thread0_2_attributes);
	//Thread1_2Handle = osThreadNew(Thread1_2, NULL, &Thread1_2_attributes);
	
	//Thread0_3Handle = osThreadNew(Thread0_3, NULL, &Thread0_3_attributes);
	//Thread1_3Handle = osThreadNew(Thread1_3, NULL, &Thread1_3_attributes);
	
	//Thread0_4Handle = osThreadNew(Thread0_4, NULL, &Thread0_4_attributes);
	//Thread1_4Handle = osThreadNew(Thread1_4, NULL, &Thread1_4_attributes);
	
	//Thread0_5Handle = osThreadNew(Thread0_5, NULL, &Thread0_5_attributes);
	//Thread1_5Handle = osThreadNew(Thread1_5, NULL, &Thread1_5_attributes);

  /* Create the mutex(es) */
  /* creation of osMutex */
  osMutexHandle = osMutexNew(&osMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  
  /* USER CODE END RTOS_QUEUES */

  /* USER CODE BEGIN RTOS_THREADS */
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

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


void Thread0(void *argument)
{
  (void)argument;
  BSP_LED_On(LED2);
	
	Access(RED);                 // Lock RED access (turn on RED LED)

  osDelay(GetRandomDelay());   // Wait random delay

  Release(RED);                // Unlock RED access (turn off RED LED)
  /* Critical section ends */

  BSP_LED_Off(LED2);
  osThreadExit();
}


void Thread1(void *argument)
{
  (void)argument;

  // Critical section starts
  BSP_LED_On(LED1); // GREEN

  Access(RED);                 // Lock RED access (turn on RED LED)

  osDelay(GetRandomDelay());   // Random delay

  Release(RED);       

  BSP_LED_Off(LED1);
  // Critical section ends

  osThreadExit(); // Terminate the thread
}

void Thread0_1(void *argument)
{
    (void)argument;

    while (Check(RED)) {}  // Wait until RED is free
		//osDelay(GetRandomDelay());

    BSP_LED_On(LED2);      // BLUE ON

    Access(RED);
    osDelay(GetRandomDelay());
    Release(RED);

    BSP_LED_Off(LED2);     // BLUE OFF

    osThreadExit();
}

void Thread1_1(void *argument)
{
    (void)argument;

    while (Check(RED)) {}  // Wait until RED is free
		//osDelay(GetRandomDelay());

    BSP_LED_On(LED1);      // GREEN ON

    Access(RED);
    osDelay(GetRandomDelay());
    Release(RED);

    BSP_LED_Off(LED1);     // GREEN OFF

    osThreadExit();
}

void Thread0_2(void *argument)
{
    (void)argument;

    while (token != 0) {
        osDelay(1); // avoid tight CPU loop
    }

    BSP_LED_On(LED2);     // BLUE ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED2);    // BLUE OFF

    token = 1 - token;    // Pass token to Thread1_2

    osThreadExit();
}

void Thread1_2(void *argument)
{
    (void)argument;

    while (token != 1) {
        osDelay(1); // avoid tight CPU loop
    }

    BSP_LED_On(LED1);     // GREEN ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED1);    // GREEN OFF

    token = 1 - token;    // Pass token to Thread0_2

    osThreadExit();
}

void Thread0_3(void *argument)
{
    (void)argument;
    const uint8_t id = 0;
    const uint8_t other_id = 1;

    flag[id] = 1;
    turn = other_id;
		//osDelay(GetRandomDelay());
    while (flag[other_id] && turn == other_id) {
    }

    // Critical Section
    BSP_LED_On(LED2);     // BLUE ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED2);    // BLUE OFF

    flag[id] = 0;

    osThreadExit();
}


void Thread1_3(void *argument)
{
    (void)argument;
    const uint8_t id = 1;
    const uint8_t other_id = 0;

    flag[id] = 1;
    turn = other_id;
		//osDelay(GetRandomDelay());
    while (flag[other_id] && turn == other_id) {
    }

    // Critical Section
    BSP_LED_On(LED1);     // GREEN ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED1);    // GREEN OFF

    flag[id] = 0;

    osThreadExit();
}

void Thread0_4(void *argument)
{
    (void)argument;
    const uint8_t id = 0;
    const uint8_t other_id = 1;

    flag[id] = 1;
    osDelay(GetRandomDelay());

    while (flag[other_id])
    {
        osDelay(GetRandomDelay());  // random backoff
        flag[id] = 0;
        osDelay(500);               // wait before retry
        flag[id] = 1;
    }

    // Critical Section
    BSP_LED_On(LED2);     // BLUE ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED2);    // BLUE OFF

    flag[id] = 0;

    osThreadExit();
}

void Thread1_4(void *argument)
{
    (void)argument;
    const uint8_t id = 1;
    const uint8_t other_id = 0;

    flag[id] = 1;
    osDelay(GetRandomDelay());

    while (flag[other_id])
    {
        osDelay(GetRandomDelay());  // random backoff
        flag[id] = 0;
        osDelay(500);               // wait before retry
        flag[id] = 1;
    }

    // Critical Section
    BSP_LED_On(LED1);     // GREEN ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED1);    // GREEN OFF

    flag[id] = 0;

    osThreadExit();
}

void Thread0_5(void *argument)
{
    (void)argument;
    const uint8_t id = 0;
    const uint8_t other_id = 1;

    flag[id] = 1;

    while (flag[other_id]) {
        if (token == other_id) {
            flag[id] = 0;
            while (token == other_id) {
                osDelay(1);
            }
            flag[id] = 1;
        }
    }

    // Critical Section
    BSP_LED_On(LED2);     // BLUE ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED2);    // BLUE OFF

    token = other_id;
    flag[id] = 0;

    osThreadExit();
}


void Thread1_5(void *argument)
{
    (void)argument;
    const uint8_t id = 1;
    const uint8_t other_id = 0;

    flag[id] = 1;

    while (flag[other_id]) {
        if (token == other_id) {
            flag[id] = 0;
            while (token == other_id) {
                osDelay(1);
            }
            flag[id] = 1;
        }
    }

    // Critical Section
    BSP_LED_On(LED1);     // GREEN ON
    Access(RED);          // RED ON
    osDelay(GetRandomDelay());
    Release(RED);         // RED OFF
    BSP_LED_Off(LED1);    // GREEN OFF

    flag[id] = 0;

    osThreadExit();
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
