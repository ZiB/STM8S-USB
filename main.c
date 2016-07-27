/*
 * File: main.c
 * Date: 02.01.2013
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

@inline static void gpio_init(void)
{
	// перевод всех линий на вход с подтяжкой к плюсу
	GPIOA->CR1 = 0xFF;
	GPIOB->CR1 = 0xFF;
	GPIOC->CR1 = 0xFF;
	GPIOD->CR1 = 0xFF;
	GPIOE->CR1 = 0xFF;
	GPIOF->CR1 = 0xFF;

	// Входные линии USB
	GPIOC->CR1 = 0;
	GPIOC->CR2 = 0;
	GPIOC->DDR = 0x3F;
	GPIOC->ODR = 0;
}

@inline static void clock_init(void)
{
	// после сброса микроконтроллер работает от встроенного HSI-генератора
	// с делителем по умолчанию 8, меняем его на 1
	CLK->CKDIVR = 0;

	// переключаемся на внешний кварцевый резонатор
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

@inline static void timers_init(void)
{
	CLK->PCKENR1 |= CLK_PCKENR1_TIM1;

	// таймер 1 - "захват" первого импульса USB посылки
	TIM1_TimeBaseInit(0, TIM1_PSCRELOADMODE_UPDATE, 1000, 0);
	// захват сигнала по линии USB D-
	TIM1_ICInit(TIM1_CHANNEL_2, TIM1_ICPOLARITY_FALLING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);
	TIM1_SelectInputTrigger(TIM1_TS_TI2FP2);
	TIM1_SelectSlaveMode(TIM1_SLAVEMODE_TRIGGER);
	TIM1_ClearFlag(TIM1_FLAG_CC2);
	TIM1_ITConfig(TIM1_IT_CC2, ENABLE);
}

void main(void)
{
	disableInterrupts();

	gpio_init();

	clock_init();

	timers_init();

	enableInterrupts();

	while(1)
	{
		usb_process();
	}
}
