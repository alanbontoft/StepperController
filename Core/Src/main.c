/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "modbus.h"
#include "globals.h"
#include <stdlib.h>
/* USER CODE END Includes */

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
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
modbusHandler_t ModbusH;
uint16_t HoldingRegs[HR_SIZE] = { APP_FC_NOP, 0x0000, 0x0000, SPEED_1, 0x0000, STEP_1 };
uint16_t InputRegs[IR_SIZE] = { 100, 200, 300, 400 };
uint16_t pulses_remaining;
uint16_t pulse_width;
int32_t dir_pulses;

osThreadId_t pulseTaskHandle;
const osThreadAttr_t pulseTask_attributes = {
  .name = "pulseTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */
void StartPulseTask(void *argument);
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  /* Slave initialization */
  ModbusH.uModbusType = MB_SLAVE;
  ModbusH.port =  &huart2;
  ModbusH.u8id = 1;
  ModbusH.u16timeOut = 1000;
  ModbusH.EN_Port = NULL;
  ModbusH.u16HoldingRegs = HoldingRegs;
  ModbusH.u16HoldingRegSize= sizeof(HoldingRegs)/sizeof(HoldingRegs[0]);
  ModbusH.u16InputRegs = InputRegs;
  ModbusH.u16InputRegSize= sizeof(InputRegs)/sizeof(InputRegs[0]);
  ModbusH.xTypeHW = USART_HW;
  //Initialize Modbus library
  ModbusInit(&ModbusH);
  //Start capturing traffic on serial Port
  ModbusStart(&ModbusH);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 32000 - 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EN1_Pin|RST1_Pin|EN2_Pin|RST2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, STEP1_Pin|DIR1_Pin|STEP2_Pin|DIR2_Pin
                          |MS1_Pin|MS2_Pin|MS3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EN3_Pin|RST3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|STEP3_Pin|DIR3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : EN1_Pin RST1_Pin STEP1_Pin DIR1_Pin
                           EN2_Pin RST2_Pin STEP2_Pin DIR2_Pin
                           MS1_Pin MS2_Pin MS3_Pin */
  GPIO_InitStruct.Pin = EN1_Pin|RST1_Pin|STEP1_Pin|DIR1_Pin
                          |EN2_Pin|RST2_Pin|STEP2_Pin|DIR2_Pin
                          |MS1_Pin|MS2_Pin|MS3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : EN3_Pin RST3_Pin LD3_Pin STEP3_Pin
                           DIR3_Pin */
  GPIO_InitStruct.Pin = EN3_Pin|RST3_Pin|LD3_Pin|STEP3_Pin
                          |DIR3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void setStepSize()
{
	switch(HoldingRegs[HR_STEP_SIZE])
	{
		case STEP_1:
			HAL_GPIO_WritePin(GPIOA, MS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, MS2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, MS3_Pin, GPIO_PIN_RESET);
			break;
		case STEP_1_2:
			HAL_GPIO_WritePin(GPIOA, MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MS2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, MS3_Pin, GPIO_PIN_RESET);
			break;
		case STEP_1_4:
			HAL_GPIO_WritePin(GPIOA, MS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MS3_Pin, GPIO_PIN_RESET);
			break;
		case STEP_1_8:
			HAL_GPIO_WritePin(GPIOA, MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MS3_Pin, GPIO_PIN_RESET);
			break;
		case STEP_1_16:
			HAL_GPIO_WritePin(GPIOA, MS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, MS3_Pin, GPIO_PIN_SET);
			break;
	}

}

// calculate the number of pulses required to rotate through the requested angle
// positive value is CW, negative CCW
void calculatePulses()
{
	float angle, pulses;
	uint8_t factor;
	uint16_t pulses_per_rev;

	switch(HoldingRegs[HR_STEP_SIZE])
	{
		case STEP_1:
			factor = 1;
			break;
		case STEP_1_2:
			factor = 2;
			break;
		case STEP_1_4:
			factor = 4;
			break;
		case STEP_1_8:
			factor = 8;
			break;
		case STEP_1_16:
			factor = 16;
			break;
	}

	pulses_per_rev = STEPS_PER_REV * factor;

	angle = *((float*)&HoldingRegs[HR_ANGLE_LO]);

	pulses =  (angle / 360.0F) * pulses_per_rev;

	dir_pulses = (int32_t)round(pulses);
}

void startTimer(uint16_t numpulses)
{
	// set number of pulses
	pulses_remaining = 2 * numpulses;

	// set starting CCR value
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse_width - 1);

	// reset counter
	__HAL_TIM_SET_COUNTER(&htim1, 0);

	// start interrupt
	HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
}

void startPulseTask(uint16_t numpulses)
{
	// set number of pulses
	pulses_remaining = 2 * numpulses;

	// create and start task
	pulseTaskHandle = osThreadNew(StartPulseTask, NULL, &pulseTask_attributes);
}

void enableDriver(bool enable)
{

	GPIO_PinState PinState;

	PinState = enable ? GPIO_PIN_RESET : GPIO_PIN_SET;

	switch(HoldingRegs[HR_CHANNEL])
	{
		case 0: HAL_GPIO_WritePin(GPIOA, EN1_Pin, PinState);
		break;

		case 1: HAL_GPIO_WritePin(GPIOA, EN2_Pin, PinState);
		break;

		case 2: HAL_GPIO_WritePin(GPIOB, EN3_Pin, PinState);
		break;
	}
}

void setDirection(bool clockwise)
{

	GPIO_PinState PinState;

	PinState = clockwise ? GPIO_PIN_RESET : GPIO_PIN_SET;

	switch(HoldingRegs[HR_CHANNEL])
	{
		case 0: HAL_GPIO_WritePin(GPIOA, DIR1_Pin, PinState);
		break;

		case 1: HAL_GPIO_WritePin(GPIOA, DIR2_Pin, PinState);
		break;

		case 2: HAL_GPIO_WritePin(GPIOB, DIR3_Pin, PinState);
		break;
	}
}

void rotate()
{
	// int32_t pulses;
	// calculate pulses required
	calculatePulses();

	if (dir_pulses == 0) return;

	// set direction pin (+ve = CW)
	setDirection(dir_pulses > 0);

	// set step size
	setStepSize();

	// enable driver output
	enableDriver(true);

	// calculate required pulse width based on speed and angle
	// minimum width of pulse is 1ms (mark/space equal, therefore 2 ms per cycle)
	pulse_width = 2;

	// start timer
	// startTimer(abs(dir_pulses));

	// start pulse task
	 startPulseTask(abs(dir_pulses));

}





void HAL_TIM_OC_DelayElapsedCallback (TIM_HandleTypeDef * htim)
{
	if (htim == &htim1)
	{
		switch(HoldingRegs[HR_CHANNEL])
		{
			case 0: HAL_GPIO_TogglePin(GPIOA, STEP1_Pin);
			break;

			case 1: HAL_GPIO_TogglePin(GPIOA, STEP2_Pin);
			break;

			case 2: HAL_GPIO_TogglePin(GPIOB, STEP3_Pin);
			break;
		}

		// decrement pulse count
		pulses_remaining--;

		if (pulses_remaining == 0)
		{
			// stop timer, disable interrupt
			HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_1);
			// disable driver output
			enableDriver(false);
			return;
		}

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,
			__HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1) + pulse_width);

	}
}

void StartPulseTask(void *argument)
{
	while (pulses_remaining > 0)
	{
		switch(HoldingRegs[HR_CHANNEL])
		{
			case 0: HAL_GPIO_TogglePin(GPIOA, STEP1_Pin);
			break;

			case 1: HAL_GPIO_TogglePin(GPIOA, STEP2_Pin);
			break;

			case 2: HAL_GPIO_TogglePin(GPIOB, STEP3_Pin);
			break;
		}

		pulses_remaining--;

		osDelay(1);
	}

	// disabling the driver after rotation seems like the right thing to do
	// however, these seems to lead to slight overshoot at faster speeds
	// enableDriver(false);

	osThreadExit();
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
	  osDelay(500);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7)
  {
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
