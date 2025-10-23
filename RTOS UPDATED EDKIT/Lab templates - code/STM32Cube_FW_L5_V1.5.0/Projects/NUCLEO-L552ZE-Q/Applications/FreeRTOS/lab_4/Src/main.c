/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define SCHED_PRE_PRI   0
#define SCHED_NPRE_PRI  1
#define SCHED_NPRE_NPRI 0

#define RED   LED3
#define GREEN LED1
#define BLUE  LED2
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define Thread_PERIOD     1000     // in ms 
#define Thread_DURATION   300      // in ms
#define TOP_PRIORITY    osPriorityHigh
#define PS_PRIORITY     osPriorityHigh
#define RED_PRIORITY    osPriorityAboveNormal
#define BLUE_PRIORITY   osPriorityNormal
#define GREEN_PRIORITY  osPriorityBelowNormal
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */
osThreadId_t t_PSHandle;
osThreadId_t t_TRLHandle;
osThreadId_t t_TBLHandle;
osThreadId_t t_TGLHandle;
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */
__IO uint32_t TimeCounter = 0;
/* USER CODE END PV */
const osThreadAttr_t Thread_PS_attr = {
    .name = "PollSwitch",
    .priority = PS_PRIORITY,
    .stack_size = 128 * 4
};

const osThreadAttr_t Thread_R_attr = {
    .name = "RedLED",
    .priority = RED_PRIORITY,
    .stack_size = 128 * 4
};

const osThreadAttr_t Thread_B_attr = {
    .name = "BlueLED",
    .priority = BLUE_PRIORITY,
    .stack_size = 128 * 4
};

const osThreadAttr_t Thread_G_attr = {
    .name = "GreenLED",
    .priority = GREEN_PRIORITY,
    .stack_size = 128 * 4
};

void DelayBusy(uint32_t ms)
{
    uint32_t count = ms * 100000;
    while (count--) {
        __NOP();
    }
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
void ToggleLEDsThread(void *argument);
void osTimerCallback(void *argument);
void Thread_Poll_Switch(void *argument);
void Thread_ControlRedLED(void *argument);
void Thread_ControlBlueLED(void *argument);
void Thread_ControlGreenLED(void *argument);

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
	// USER Button - GPIOC PIN13 (input, pulled down)
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;  // or GPIO_PULLDOWN if needed
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Initialize LEDs */
	BSP_LED_Init(RED);
  BSP_LED_Init(GREEN);
  BSP_LED_Init(BLUE);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of LEDTimer */
	t_PSHandle = osThreadNew(Thread_Poll_Switch, NULL, &Thread_PS_attr);
  t_TRLHandle = osThreadNew(Thread_ControlRedLED, NULL, &Thread_R_attr);
  t_TBLHandle = osThreadNew(Thread_ControlBlueLED, NULL, &Thread_B_attr);
  t_TGLHandle = osThreadNew(Thread_ControlGreenLED, NULL, &Thread_G_attr);
  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
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

/* USER CODE END 4 */

void Poll_Switch(void) {
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
        BSP_LED_On(RED);
        BSP_LED_On(BLUE);
        BSP_LED_On(GREEN);
        DelayBusy(10);
        BSP_LED_Off(RED);
        BSP_LED_Off(BLUE);
        BSP_LED_Off(GREEN);
        DelayBusy(10);
    }
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), PS_PRIORITY);
#endif
}

void ControlRedLED(void) {
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
    BSP_LED_On(RED);
    DelayBusy(Thread_DURATION);
    BSP_LED_Off(RED);
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), RED_PRIORITY);
#endif
}

void ControlBlueLED(void) {
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
    BSP_LED_On(BLUE);
    DelayBusy(Thread_DURATION);
    BSP_LED_Off(BLUE);
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), BLUE_PRIORITY);
#endif
}

void ControlGreenLED(void) {
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
    BSP_LED_On(GREEN);
    DelayBusy(Thread_DURATION);
    BSP_LED_Off(GREEN);
#if SCHED_NPRE_PRI
    osThreadSetPriority(osThreadGetId(), GREEN_PRIORITY);
#endif
}

void Thread_Poll_Switch(void *argument) {
    (void)argument;
#if (SCHED_PRE_PRI || SCHED_NPRE_PRI)
    for (;;) {
        Poll_Switch();
        osDelay(1);
    }
#elif SCHED_NPRE_NPRI
    Poll_Switch();
    osThreadExit();
#endif
}

void Thread_ControlRedLED(void *argument) {
    (void)argument;
#if (SCHED_PRE_PRI || SCHED_NPRE_PRI)
    for (;;) {
        ControlRedLED();
        osDelay(Thread_PERIOD);
    }
#elif SCHED_NPRE_NPRI
    ControlRedLED();
    osThreadExit();
#endif
}

void Thread_ControlBlueLED(void *argument) {
    (void)argument;
#if (SCHED_PRE_PRI || SCHED_NPRE_PRI)
    for (;;) {
        ControlBlueLED();
        osDelay(Thread_PERIOD);
    }
#elif SCHED_NPRE_NPRI
    ControlBlueLED();
    osThreadExit();
#endif
}

void Thread_ControlGreenLED(void *argument) {
    (void)argument;
#if (SCHED_PRE_PRI || SCHED_NPRE_PRI)
    for (;;) {
        ControlGreenLED();
        osDelay(Thread_PERIOD);
    }
#elif SCHED_NPRE_NPRI
    ControlGreenLED();
    osThreadExit();
#endif
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
