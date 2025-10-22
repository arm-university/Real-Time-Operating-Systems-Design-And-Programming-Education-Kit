/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_SemaphoreFromISR/Src/main.c
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

#define COEFFICIENT_RAM_GEN 0
#define ML 1

#define RMIN (SystemCoreClock / 120000)
#define RMAX (SystemCoreClock / 110000)
#define RDIV 20
#define RAMT ((RMAX - RMIN) / RDIV)

#define MAX_COUNT 10000
#define MAX_LED_NUM 3
#define LED_NUM 3

#if ML
#define TRAIN_COUNT 2000
#endif

osThreadId_t tskID_Tsk_Dispatcher;
osThreadId_t tskID_Tsk_LED[LED_NUM];

int count = 0;
uint32_t end_time;

int LED_priority[3] = {1, 3, 2};
int LED_preference[3] = {3, 2, 1};
const uint32_t LED_index[3] = {LED1, LED2, LED3};

const osThreadAttr_t Tsk_Dispatcher_attr = {
    .name = "Dispatcher",
    .priority = osPriorityNormal,
    .stack_size = 512 * 4
};

const osThreadAttr_t Tsk_LED_attr = {
    .name = "LED_Task",
    .priority = osPriorityLow,
    .stack_size = 512 * 4
};

int GetRandomDelay(void) {
    return (rand() % (RDIV * RAMT)) + RMIN;
}

void Tsk_LED(void *argument)
{
    //int id = *(int *)argument;   /* 0,1,2 */
		int id = (int)(uintptr_t)argument;


    for (;;)
    {
        BSP_LED_On (LED_index[id]);
        osDelay(GetRandomDelay() * LED_preference[id]);

        BSP_LED_Off(LED_index[id]);
        osDelay(GetRandomDelay() * LED_preference[id]);
        /* back to beginning – no explicit yield needed when we call osDelay */
    }
}

void Tsk_Dispatcher(void *argument)
{
    int i, j;
#if ML
    uint32_t current_cycle_time = 0, last_cycle_time = 0;
    int      temp1[3]          = {0};
    int      temp2[3]          = {0};
    double   record_priority[3]= {0};
#endif

    for (; count < MAX_COUNT; count++)
    {
#if ML
        if (count < TRAIN_COUNT)
        {
            current_cycle_time = osKernelGetTickCount();
            for (i = 0; i < 3; i++)
            {
                record_priority[i] += LED_priority[i] * (current_cycle_time - last_cycle_time);
            }
            for (i = 0; i < 3; i++) temp1[i] = rand() % 1000;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    if (temp1[i] >= temp1[j]) temp2[i]++;
                }
                LED_priority[i] = temp2[i];
                temp2[i]        = 0;
            }
            last_cycle_time = current_cycle_time;
        }
        if (count == TRAIN_COUNT)
        {
            for (i = 0; i < 3; i++)
            {
                temp2[i] = 4;
                for (j = 0; j < 3; j++)
                {
                    if (record_priority[i] >= record_priority[j]) temp2[i]--;
                }
                LED_priority[i] = temp2[i];
            }
        }
#endif /* ML */

        for (i = 0; i < LED_NUM && i < MAX_LED_NUM; i++)
        {
            osThreadSetPriority(tskID_Tsk_LED[i],
                                (osPriority_t)(osPriorityLow + LED_priority[i]));
        }

        osDelay(1U);   /* -------? IMPORTANT: yield to LED threads        */
    }

    end_time = osKernelGetTickCount();
    for (i = 0; i < LED_NUM && i < MAX_LED_NUM; i++)
    {
        osThreadTerminate(tskID_Tsk_LED[i]);
    }
    osThreadExit();
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* Definitions for SEM_Thread */

/* USER CODE BEGIN PV */
__IO uint32_t OsStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
void SemaphoreTest(void *argument);
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
  /* Initialize LED */
	BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
	srand(HAL_GetTick());
	


  /* Initialize buttons */
  //BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
 
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of osSemaphore */
  //osSemaphoreHandle = osSemaphoreNew(1, 1, &osSemaphore_attributes);
	// Create Dispatcher thread
  tskID_Tsk_Dispatcher = osThreadNew(Tsk_Dispatcher, NULL, &Tsk_Dispatcher_attr);

    // Create LED threads
  static int led_ids[LED_NUM];
  for (int i = 0; i < LED_NUM && i < MAX_LED_NUM; i++) {
		//led_ids[i] = i;
    //tskID_Tsk_LED[i] = osThreadNew(Tsk_LED, &led_ids[i], &Tsk_LED_attr);
		tskID_Tsk_LED[i] = osThreadNew(Tsk_LED, (void *)(uintptr_t)i, &Tsk_LED_attr);

  }
  /* USER CODE BEGIN RTOS_SEMAPHORES */

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */

  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
 
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of SEM_Thread */
  //SEM_ThreadHandle = osThreadNew(SemaphoreTest, NULL, &SEM_Thread_attributes);

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
/**
  * @brief  EXTI line detection callbacks
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */


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
