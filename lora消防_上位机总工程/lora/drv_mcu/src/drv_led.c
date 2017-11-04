/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   LED配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_led.h"



/**
  * @brief :LED初始化
  * @param :无
  * @note  :无
  * @retval:无
  */ 
void drv_led_init( void )
{
	GPIO_InitTypeDef	GpioInitStructer;
	
	//使能口线时钟
	RCC_APB2PeriphClockCmd( LED_RED_GPIO_CLK | LED_BLUE_GPIO_CLK, ENABLE );	//打开端口时钟
	
	GpioInitStructer.GPIO_Mode = GPIO_Mode_Out_PP;						
	GpioInitStructer.GPIO_Speed = GPIO_Speed_2MHz;		
	
	GpioInitStructer.GPIO_Pin = LED_RED_GPIO_PIN;		
	GPIO_Init( LED_RED_GPIO_PORT, &GpioInitStructer );			//初始化红色LED引脚
	GPIO_SetBits( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN );		//初始状态置高，红色LED初始化状态默认为灭
	
	GpioInitStructer.GPIO_Pin = LED_BLUE_GPIO_PIN;		
	GPIO_Init( LED_BLUE_GPIO_PORT, &GpioInitStructer );			//初始化蓝色LED引脚
	GPIO_SetBits( LED_BLUE_GPIO_PORT, LED_BLUE_GPIO_PIN );		//初始状态置高，蓝色LED初始化状态默认为灭
	
}

/**
  * @brief :LED亮
  * @param :
  *			@LedPort:LED选择，红色或蓝色
  * @note  :无
  * @retval:无
  */
void drv_led_on( LedPortType LedPort )
{
	if( LED_RED == LedPort )	//LED_RED
	{
		GPIO_ResetBits( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN );		//红色LED引脚置低，红色LED亮
	}
	else						//LED_BLUE
	{
		GPIO_ResetBits( LED_BLUE_GPIO_PORT, LED_BLUE_GPIO_PIN );	//蓝色LED引脚置低，蓝色LED亮
	}
	
}

/**
  * @brief :LED灭
  * @param :
  *			@LedPort:LED选择，红色或蓝色
  * @note  :无
  * @retval:无
  */
void drv_led_off( LedPortType LedPort )
{
	if( LED_RED == LedPort )	//LED_RED
	{
		GPIO_SetBits( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN );		//红色LED引脚置高，红色LED灭
	}
	else						//LED_BLUE
	{
		GPIO_SetBits( LED_BLUE_GPIO_PORT, LED_BLUE_GPIO_PIN );		//蓝色LED引脚置高，蓝色LED灭
	}
	
}

/**
  * @brief :LED闪烁
  * @param :
  *			@LedPort:LED选择，红色或蓝色
  * @note  :无
  * @retval:无
  */
void drv_led_flashing( LedPortType LedPort )
{
	
	if( LED_RED == LedPort )
	{
		LED_RED_GPIO_PORT->ODR ^= ( uint32_t)LED_RED_GPIO_PIN;
	}
	else
	{
		LED_BLUE_GPIO_PORT->ODR ^= ( uint32_t)LED_BLUE_GPIO_PIN;
	}
}
