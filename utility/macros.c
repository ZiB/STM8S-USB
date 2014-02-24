/*
 *	File:	macros.c
 *	Date:	09.01.2012
 *	Denis Zheleznjakov @ ZiBlog.ru
 */

#include  "main.h"

static volatile uint16_t delay_count;
static volatile _Bool delay_flag;
static volatile uint16_t random;
static _Bool is_ready;

void synchronization_10ms_refresh(void)
{
	static uint8_t counter_0_5_ms;

	if (is_ready == FALSE)
	{
		if (++counter_0_5_ms >= 20)
		{
			counter_0_5_ms = 0;
			is_ready = TRUE;
		}
	}
}

_Bool time_synchronization_10ms(void)
{
	if (is_ready == TRUE)
	{
		is_ready = FALSE;
		return TRUE;
	}

	return FALSE;
}

uint8_t get_random_byte(void)
{
	random *= 5;
	random += 1;
	random %= 256;

	return (uint8_t) random;
}

void delay_ms(volatile uint16_t delay)
{
	delay_count = delay << 1;
	delay_flag = TRUE;

	while (delay_flag == TRUE)
	{
	}
}

void delay_ms_refresh(void)
{
	if (delay_flag == TRUE)
	{
		if (--delay_count == 0U)
		{
			delay_flag = FALSE;
		}
	}
}
