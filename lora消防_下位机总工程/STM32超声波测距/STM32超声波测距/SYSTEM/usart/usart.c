/**
  ******************************************************************************
  * @author  ???? ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART??C??
  ******************************************************************************
  * @attention
  *
  * ??	:	http://www.ashining.com
  * ??	:	https://shop105912646.taobao.com
  * ????:	https://cdzeyao.1688.com
  ******************************************************************************
  */
	
	

#include "usart.h"


/**
  * @brief :?????
  * @param :
  *			@UartBaudRate:?????
  * @note  :?
  * @retval:?
  */
void drv_uart_init( uint32_t UartBaudRate )
{
	GPIO_InitTypeDef	UartGpioInitStructer;
	USART_InitTypeDef	UartinitStructer;
	
	//??????,???TX RX????????,??????,?????
	//?????TX RX ?? 
	RCC_APB2PeriphClockCmd( UART_TX_GPIO_CLK | UART_RX_GPIO_CLK, ENABLE );	//??TX RX ????
	
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	UartGpioInitStructer.GPIO_Speed = GPIO_Speed_2MHz;
	//TX
	UartGpioInitStructer.GPIO_Pin = UART_TX_GPIO_PIN;
	GPIO_Init( UART_TX_GPIO_PORT, &UartGpioInitStructer );		//???TX??  ???????
	//RX
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	UartGpioInitStructer.GPIO_Pin = UART_RX_GPIO_PIN;
	GPIO_Init( UART_RX_GPIO_PORT, &UartGpioInitStructer );		//???RX??  ?????
	
	//??USART??
	USART_DeInit( UART_PORT );		//????
	
	if( USART1 == UART_PORT )		//??????
	{
		RCC_APB2PeriphClockCmd( UART_PORT_CLK, ENABLE );			
	}																	//???USART????????APB???														
	else																//STM32F103?????USART1?APB2?,?????????????
	{
		RCC_APB1PeriphClockCmd( UART_PORT_CLK, ENABLE );
	}
	
	UartinitStructer.USART_BaudRate = UartBaudRate;						//?????
	UartinitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//??????
	UartinitStructer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//?????	
	UartinitStructer.USART_Parity = USART_Parity_No;					//????
	UartinitStructer.USART_StopBits = USART_StopBits_1;					//?????
	UartinitStructer.USART_WordLength = USART_WordLength_8b;			//8????
	
	USART_Cmd( UART_PORT, DISABLE );									//????
	USART_Init( UART_PORT, &UartinitStructer );							//?????
	USART_Cmd( UART_PORT, ENABLE );										//????	
}

/**
  * @brief :??????
  * @param :
  *			@TxBuffer:???????
  *			@Length:????
  * @note  :?
  * @retval:?
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( UART_PORT, USART_FLAG_TXE ));
		UART_PORT->DR = *TxBuffer;
		TxBuffer++;
	}
}

/**
  * @brief :??????
  * @param :
  *			@RxBuffer:???????
  * @note  :?
  * @retval:????????
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0x7FFF;
	
	while( l_UartRxTimOut-- )			//????????
	{
		
		if( RESET != USART_GetFlagStatus( UART_PORT, USART_FLAG_RXNE ))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			*RxBuffer = (uint8_t)UART_PORT->DR;
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0x7FFF;	//???????,??????
		}
		if( 100 == l_RxLength )
		{
			break;		//????100???
		}
	}

	return l_RxLength;					//????,??????
}

