/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can.h"
#include "sdev.h"
#include <lely/co/dev.h>
#include <lely/can/net.h>
#include <lely/co/nmt.h>
#include <lely/co/sdo.h>
#include <lely/co/tpdo.h>
#include <lely/co/time.h>

#include <time.h>
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
CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */
static int last_button_state = 1; // 1 = Released (GPIO_PIN_SET)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
static int on_can_send(const struct can_msg *msg, void *data);
static void on_nmt_cs(co_nmt_t *nmt, co_unsigned8_t cs, void *data);
static void on_time(co_time_t *time, const struct timespec *tp, void *data);
static co_unsigned32_t on_dn_2000_00(co_sub_t *sub, struct co_sdo_req *req,
		void *data);
static co_unsigned32_t on_up_2001_00(const co_sub_t *sub,
		struct co_sdo_req *req, void *data);
static co_unsigned32_t on_dn_led_control(co_sub_t *sub,
		struct co_sdo_req *req, void *data);

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
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  // Initialize our custom CAN abstraction layer.
  can_init();

  // Initialize the CAN network interface.
  can_net_t *net = can_net_create();
  // assert(net); // We can add asserts later if needed

  // Set the function to be called by the Lely stack for transmitting frames.
  can_net_set_send_func(net, &on_can_send, NULL);

  // Create a dynamic object dictionary from the static one.
  co_dev_t *dev = co_dev_create_from_sdev(&slave_sdev); // Use our 'slave_sdev'
  // assert(dev);

  // Create the CANopen NMT service.
  co_nmt_t *nmt = co_nmt_create(net, dev);
  // assert(nmt);

  // Start the NMT service by resetting the node (triggers boot-up message).
  co_nmt_cs_ind(nmt, CO_NMT_CS_RESET_NODE);

  // Set the NMT indication function to handle commands from the master.
  co_nmt_set_cs_ind(nmt, &on_nmt_cs, NULL);

  // Set the TIME indication function.
  co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);

  // Register our custom functions to be called on SDO access to specific objects.
  co_sub_set_dn_ind(co_dev_find_sub(dev, 0x2000, 0x00), &on_dn_2000_00, NULL);
  co_sub_set_up_ind(co_dev_find_sub(dev, 0x2001, 0x00), &on_up_2001_00, NULL);

  // Register the download indication function for the LED control object.
  co_sub_set_dn_ind(co_dev_find_sub(dev, 0x2100, 0x00), &on_dn_led_control, NULL);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // Update the Lely stack's internal clock.
	  // HAL_GetTick() returns milliseconds since startup.
	  uint32_t ms = HAL_GetTick();
	  struct timespec now;
	  // Convert milliseconds to a timespec struct (seconds and nanoseconds)
	  now.tv_sec = ms / 1000;
	  now.tv_nsec = (ms % 1000) * 1000000;
	  can_net_set_time(net, &now);

	  struct can_msg msg;

	  // Continuously check our driver's buffer for new messages.
	  if (can_recv(&msg, 1)) {
		  // If a message is received, pass it to the Lely stack for processing.
		  can_net_recv(net, &msg);
	  }

	  // Read the current state of the user button (PA0).
	  // Note: Button pressed = pin is LOW (GPIO_PIN_RESET).
	  int current_button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

	  // Check if there is a change from the last stable state.
	  if (current_button_state != last_button_state)
	  {
		  // Wait for a short time to debounce the mechanical switch.
		  HAL_Delay(50); // 50ms delay for debouncing

		  // Read the state again after the delay.
		  current_button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

		  // If the state is still different, it's a valid press/release.
		  if (current_button_state != last_button_state)
		  {
			  last_button_state = current_button_state;

			  // Convert GPIO state to our application logic value (1=pressed, 0=released)
			  uint8_t od_value = (current_button_state == GPIO_PIN_RESET) ? 1 : 0;

			  // Find the button object (0x2110) in our Object Dictionary.
			  co_sub_t *sub = co_dev_find_sub(dev, 0x2110, 0x00);
			  if (sub) {
				  // Update the value in the local Object Dictionary.
				  co_sub_set_val_u8(sub, od_value);

				  // Trigger a TPDO event for this object.
				  // Lely will check if this object is mapped to a TPDO and send it.
				  co_dev_tpdo_event(dev, 0x2110, 0x00);
			  }
		  }
	  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 21;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD2_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Audio_SCL_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(Audio_SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
 * @brief Bridge function for Lely's can_net to use our driver's send function.
 * @note  This function mirrors the implementation in the LPC17xx reference project.
 */
static int on_can_send(const struct can_msg *msg, void *data)
{
	(void)data; // Suppress unused parameter warning
	return (can_send(msg, 1) == 1) ? 0 : -1;
}

/**
 * @brief NMT command service indication function.
 * @note  Mirrors the LPC project, but replaces exit(0) with a HAL_NVIC_SystemReset().
 */
static void on_nmt_cs(co_nmt_t *nmt, co_unsigned8_t cs, void *data)
{
	(void)data;
	(void)nmt; // Suppress unused parameter warning for now

	switch (cs) {
	case CO_NMT_CS_START:
		// Logic for 'Start' command can be added here.
		break;
	case CO_NMT_CS_STOP:
		// Logic for 'Stop' command can be added here.
		break;
	case CO_NMT_CS_ENTER_PREOP:
		// Logic for 'Enter Pre-Operational' can be added here.
		break;
	case CO_NMT_CS_RESET_NODE:
		// Perform a software reset of the microcontroller.
		HAL_NVIC_SystemReset();
		break;
	case CO_NMT_CS_RESET_COMM:
		// Logic for 'Reset Communication' can be added here.
		break;
	}
}

/**
 * @brief TIME stamp service indication function.
 * @note  This is a placeholder, as the Discovery board does not have a
 *        battery-backed Real-Time Clock to be set.
 */
static void on_time(co_time_t *time, const struct timespec *tp, void *data)
{
	(void)time;
	(void)tp;
	(void)data;
}

/**
 * @brief Custom SDO download (write) indication function for object 0x2000.
 * @note This function mirrors the implementation in the LPC17xx reference project.
 *       It only accepts the value 42 and returns a parameter error otherwise.
 */
static co_unsigned32_t on_dn_2000_00(co_sub_t *sub, struct co_sdo_req *req, void *data)
{
	(void)data;

	co_unsigned32_t ac = 0;
	co_unsigned16_t type = co_sub_get_type(sub);
	union co_val val;

	if (co_sdo_req_dn_val(req, type, &val, &ac) == -1)
		return ac;

	// Check if the received value is valid.
	if (val.u32 != 42) {
		ac = CO_SDO_AC_PARAM;
		goto error;
	}

	// Write the value to the local object dictionary.
	co_sub_dn(sub, &val);

error:
	co_val_fini(type, &val);
	return ac;
}

/**
 * @brief Custom SDO upload (read) indication function for object 0x2001.
 * @note This function mirrors the implementation in the LPC17xx reference project.
 *       It always returns the static value 42.
 */
static co_unsigned32_t on_up_2001_00(const co_sub_t *sub, struct co_sdo_req *req, void *data)
{
	(void)sub;
	(void)data;

	co_unsigned16_t type = CO_DEFTYPE_UNSIGNED32;
	co_unsigned32_t val = 42;

	co_unsigned32_t ac = 0;
	co_sdo_req_up_val(req, type, &val, &ac);
	return ac;
}

/**
 * @brief SDO download indication function for the LED control object (0x2100).
 *        This function is also triggered when a mapped RPDO updates the object.
 * @note  Mirrors the LPC project, adapted for STM32 HAL and three LEDs.
 */
static co_unsigned32_t on_dn_led_control(co_sub_t *sub, struct co_sdo_req *req, void *data)
{
	(void)data; // Unused parameter
	co_unsigned32_t ac = 0;
	co_unsigned16_t type = co_sub_get_type(sub);
	union co_val val;

	// This function extracts the value from the SDO/PDO request.
	if (co_sdo_req_dn_val(req, type, &val, &ac) == -1)
		return ac;

	// Control the LEDs based on the received 8-bit value.
	// HAL_GPIO_WritePin takes GPIO_PIN_SET (HIGH) to turn LED ON (for this board).
	// We will map:
	// Bit 0 -> Green LED (PD12)
	// Bit 1 -> Orange LED (PD13)
	// Bit 2 -> Red LED (PD14)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (val.u8 & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, (val.u8 & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, (val.u8 & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// Write the new value to the local object dictionary.
	co_sub_dn(sub, &val);

	co_val_fini(type, &val);
	return ac;
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
