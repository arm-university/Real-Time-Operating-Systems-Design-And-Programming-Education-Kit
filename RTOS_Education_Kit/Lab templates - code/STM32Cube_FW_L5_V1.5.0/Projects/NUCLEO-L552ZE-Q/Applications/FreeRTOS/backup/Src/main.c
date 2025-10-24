/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Queues/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
// Scheduling mode (only one should be set to 1)
#define SCHED_PRE_PRI   0
#define SCHED_NPRE_PRI  0
#define SCHED_NPRE_NPRI 1
#define Delay(ms) osDelay(ms)


#if (SCHED_PRE_PRI || SCHED_NPRE_PRI)
#define SET_INTERVAL(x) osDelay(x)
#define TASK_SUSPEND osDelay(1)
#endif

#if SCHED_NPRE_NPRI
#define SET_INTERVAL(x)
#define TASK_SUSPEND return;
#endif

// LED definitions for STM32 BSP (adjust if using custom board)
#define RED   LED3
#define GREEN LED1
#define BLUE  LED2

#define TASK_PERIOD     1000    // in ms (formerly 30000 cycles)
#define TASK_DURATION   300      // in ms
#define TOP_PRIORITY    osPriorityHigh
#define PS_PRIORITY     osPriorityLow
#define RED_PRIORITY    osPriorityAboveNormal
#define BLUE_PRIORITY   osPriorityNormal
#define GREEN_PRIORITY  osPriorityBelowNormal
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
// Thread handles
osThreadId_t t_PSHandle;
osThreadId_t t_TRLHandle;
osThreadId_t t_TBLHandle;
osThreadId_t t_TGLHandle;

// Thread attributes
const osThreadAttr_t task_PS_attr = {
    .name = "PollSwitch",
    .priority = PS_PRIORITY,
    .stack_size = 128 * 4
};

const osThreadAttr_t task_R_attr = {
    .name = "RedLED",
    .priority = RED_PRIORITY,
    .stack_size = 128 * 4
};

const osThreadAttr_t task_B_attr = {
    .name = "BlueLED",
    .priority = BLUE_PRIORITY,
    .stack_size = 128 * 4
};

const osThreadAttr_t task_G_attr = {
    .name = "GreenLED",
    .priority = GREEN_PRIORITY,
    .stack_size = 128 * 4
};



void Poll_Switch(void) {
    while (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET) {
        BSP_LED_On(RED);
        BSP_LED_On(BLUE);
        BSP_LED_On(GREEN);
        Delay(200);
        BSP_LED_Off(RED);
        BSP_LED_Off(BLUE);
        BSP_LED_Off(GREEN);
				Delay(200);
    }
}

void Task_Poll_Switch(void *argument) {
    (void)argument;
    SET_INTERVAL(1);
    for (;;) {
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
        Poll_Switch();
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), PS_PRIORITY);
#endif
        TASK_SUSPEND;
    }
}

void ControlRedLED(void) {
    BSP_LED_On(RED);
    osDelay(TASK_DURATION);
    BSP_LED_Off(RED);
}

void Task_ControlRedLED(void *argument) {
    (void)argument;
    SET_INTERVAL(TASK_PERIOD);
    for (;;) {
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
        ControlRedLED();
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), RED_PRIORITY);
#endif
        TASK_SUSPEND;
    }
}

void ControlBlueLED(void) {
    BSP_LED_On(BLUE);
    osDelay(TASK_DURATION);
    BSP_LED_Off(BLUE);
}

void Task_ControlBlueLED(void *argument) {
    (void)argument;
    SET_INTERVAL(TASK_PERIOD);
    for (;;) {
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
        ControlBlueLED();
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), BLUE_PRIORITY);
#endif
        TASK_SUSPEND;
    }
}

void ControlGreenLED(void) {
    BSP_LED_On(GREEN);
    osDelay(TASK_DURATION);
    BSP_LED_Off(GREEN);
}

void Task_ControlGreenLED(void *argument) {
    (void)argument;
    SET_INTERVAL(TASK_PERIOD);
    for (;;) {
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), TOP_PRIORITY);
#endif
        ControlGreenLED();
#if SCHED_NPRE_PRI
        osThreadSetPriority(osThreadGetId(), GREEN_PRIORITY);
#endif
        TASK_SUSPEND;
    }
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);

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
  BSP_LED_Init(RED);
  BSP_LED_Init(GREEN);
  BSP_LED_Init(BLUE); 
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
	
  /* USER CODE END 2 */

  /* Init scheduler */
  
	osKernelInitialize();
  /* USER CODE BEGIN RTOS_MUTEX */
 
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of osQueue */
#if (SCHED_PRE_PRI || SCHED_NPRE_PRI)


    t_PSHandle = osThreadNew(Task_Poll_Switch, NULL, &task_PS_attr);
    t_TRLHandle = osThreadNew(Task_ControlRedLED, NULL, &task_R_attr);
    t_TBLHandle = osThreadNew(Task_ControlBlueLED, NULL, &task_B_attr);
    t_TGLHandle = osThreadNew(Task_ControlGreenLED, NULL, &task_G_attr);
    osKernelStart();
		

#elif SCHED_NPRE_NPRI
    
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		//HAL_Delay(100);
    Poll_Switch();
    ControlRedLED();
    ControlBlueLED();
    ControlGreenLED();
    /* USER CODE BEGIN 3 */
  }
#endif

	while (1) { }
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
