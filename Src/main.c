#include "main.h"
#include "usb_device.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

#define NROWS   4
#define NCOLS   3

GPIO_PinState led_matrix[NCOLS][NROWS] = {
  {0,0,1,1},
  {1,1,0,0},
  {0,0,1,1}
};

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} _port_pin_t;

typedef struct {
  uint8_t         current_col;
  GPIO_PinState   matrix[NCOLS][NROWS];
} _led_states_t;

_led_states_t led_states = {
  0,
  {
    {0,0,1,1},
    {1,1,0,0},
    {0,0,1,1}
  }
};

/* Columns are cathodes (-) */
const _port_pin_t led_col_pins[NCOLS] = {
  {GPIOA, GPIO_PIN_0},
  {GPIOA, GPIO_PIN_1}, 
  {GPIOA, GPIO_PIN_2}
};

/* Rows are anodes (+) */
const _port_pin_t led_row_pins[NROWS] = {
  {GPIOA, GPIO_PIN_3},
  {GPIOA, GPIO_PIN_4},
  {GPIOA, GPIO_PIN_5},
  {GPIOA, GPIO_PIN_6}
};

const _port_pin_t key_col_pins[NCOLS] = {
  {GPIOB, GPIO_PIN_3},
  {GPIOB, GPIO_PIN_4},
  {GPIOB, GPIO_PIN_5}
};

const _port_pin_t key_row_pins[NROWS] = {
  {GPIOB, GPIO_PIN_6},
  {GPIOB, GPIO_PIN_7},
  {GPIOB, GPIO_PIN_8},
  {GPIOB, GPIO_PIN_9}
};


typedef struct {
  uint8_t         current_col;
  GPIO_PinState   matrix[NCOLS][NROWS];
} _key_states_t;

_key_states_t key_states = {
  0,
  {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  }
};

/*
  Draw the next column from the "current_col". This allows us for making seemless redrawing
  alongside with executing other tasks inbetween refreshes.
  We assume here that the previous column has been already drawn.
*/
inline void draw_next_col(){
  int cc = led_states.current_col;
  HAL_GPIO_WritePin(led_col_pins[cc].port, led_col_pins[cc].pin, GPIO_PIN_SET); // <- deactivate ROW
  // rewind if reached the last column
  if (++led_states.current_col == NCOLS){
    led_states.current_col = 0;
  }
  cc = led_states.current_col;
  for (int nr=0; nr<NROWS; nr++){
      HAL_GPIO_WritePin(led_row_pins[nr].port, led_row_pins[nr].pin , led_states.matrix[cc][nr]);
  };
  HAL_GPIO_WritePin(led_col_pins[cc].port, led_col_pins[cc].pin, GPIO_PIN_RESET); // <- activate ROW
  HAL_Delay(2);
};


/*
  Scan the next column of the keyboard.
  The previous column should have been scanned on the previous pass.
*/
inline void rescan_keys(){
  // these are helpers
  inline void _pin_to_output(_port_pin_t pair){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pair.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pair.port, &GPIO_InitStruct);
  };
  inline void _pin_to_input(_port_pin_t pair){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pair.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pair.port, &GPIO_InitStruct);
  };

  // rewind if we reached the last column
  if (++key_states.current_col == NCOLS){
    key_states.current_col = 0;
  }
  // just for convenience
  #define cc  key_states.current_col
  _pin_to_output(key_col_pins[cc]);
  HAL_GPIO_WritePin(key_col_pins[cc].port, key_col_pins[cc].pin, GPIO_PIN_RESET);
  GPIO_PinState _key = GPIO_PIN_SET;
  for (int nr=0; nr<NROWS; nr++){
    _key = HAL_GPIO_ReadPin(key_row_pins[nr].port, key_row_pins[nr].pin);
    if (_key == GPIO_PIN_RESET) {
      led_states.matrix[cc][nr] ^= 1;
    };
  };
  _pin_to_input(key_col_pins[cc]); // we finished, move the pin to the input mode (high-z)
  #undef cc
};

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  //MX_USB_DEVICE_Init();
  while (1)
  { 
    for (uint8_t i=0; i<100; i++) draw_next_col();
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
    rescan_keys();
    //HAL_Delay(10);
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BLINKER_GPIO_Port, BLINKER_Pin, GPIO_PIN_RESET);

   for (int nc=0; nc<NCOLS; nc++){
    HAL_GPIO_WritePin(led_col_pins[nc].port, led_col_pins[nc].pin, GPIO_PIN_SET); // <- deactivate ROW
  }

  /*Configure GPIO pins : LEDCOL1_Pin LEDCOL2_Pin LEDCOL3_Pin LEDROW1_Pin 
                           LEDROW2_Pin LEDROW3_Pin LEDROW4_Pin */
  GPIO_InitStruct.Pin = LEDCOL1_Pin|LEDCOL2_Pin|LEDCOL3_Pin|LEDROW1_Pin 
                          |LEDROW2_Pin|LEDROW3_Pin|LEDROW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BLINKER_Pin */
  memset(&GPIO_InitStruct, '\0', sizeof(GPIO_InitTypeDef));
  GPIO_InitStruct.Pin = BLINKER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLINKER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO Keyboard colmun pins as INPUTS w/o pulling */
  memset(&GPIO_InitStruct, '\0', sizeof(GPIO_InitTypeDef));
  GPIO_InitStruct.Pin = KEYCOL1_Pin|KEYCOL2_Pin|KEYCOL3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO Keyboard row pins as INPUTS with pullup */
  memset(&GPIO_InitStruct, '\0', sizeof(GPIO_InitTypeDef));
  GPIO_InitStruct.Pin = KEYROW1_Pin|KEYROW2_Pin|KEYROW3_Pin|KEYROW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */