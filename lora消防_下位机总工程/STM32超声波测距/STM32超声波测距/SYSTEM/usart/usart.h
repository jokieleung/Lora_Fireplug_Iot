/**
  ******************************************************************************
  * @author  ???? ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART??H??
  ******************************************************************************
  * @attention
  *
  * ??	:	http://www.ashining.com
  * ??	:	https://shop105912646.taobao.com
  * ????:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __DRV_UART_H__
#define __DRV_UART_H__


#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"


//??????
#define UART_PORT					USART1
#define UART_PORT_CLK				RCC_APB2Periph_USART1
#define UART_PORT_AF				GPIO_Remap_USART1

//??????
#define UART_TX_GPIO_PORT			GPIOA
#define UART_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define UART_TX_GPIO_PIN			GPIO_Pin_9

#define UART_RX_GPIO_PORT			GPIOA
#define UART_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define UART_RX_GPIO_PIN			GPIO_Pin_10


void drv_uart_init( uint32_t UartBaudRate );
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer );



#endif



