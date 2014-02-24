/*
 *  File:   mcu_gpio.h
 *  Date:   23.01.2011
 *  Denis Zheleznjakov @ ZiBlog.ru
 */

#ifndef MCU_GPIO_H_
#define MCU_GPIO_H_

// PIN_TEST    A, 3, HIGH, INPUT_FLOATING_WITHOUT_ITERRUPT

#define INPUT_FLOATING_WITHOUT_ITERRUPT     INPUT_FLOATING_WITHOUT_ITERRUPT
#define INPUT_FLOATING_WITH_ITERRUPT        INPUT_FLOATING_WITH_ITERRUPT
#define INPUT_PULL_UP_WITHOUT_ITERRUPT      INPUT_PULL_UP_WITHOUT_ITERRUPT
#define INPUT_PULL_UP_WITH_ITERRUPT         INPUT_PULL_UP_WITH_ITERRUPT
#define OUTPUT_OPEN_DRAIN_NOSPEED_LIMIT     OUTPUT_OPEN_DRAIN_NOSPEED_LIMIT
#define OUTPUT_OPEN_DRAIN_SPEED_LIMIT_10MHZ OUTPUT_OPEN_DRAIN_SPEED_LIMIT_10MHZ
#define OUTPUT_PUSH_PULL_NOSPEED_LIMIT      OUTPUT_PUSH_PULL_NOSPEED_LIMIT
#define OUTPUT_PUSH_PULL_SPEED_LIMIT_10MHZ  OUTPUT_PUSH_PULL_SPEED_LIMIT_10MHZ

//------------------------------------------------------------------------------
#define GPIO_PIN_CONFIGURATION_OUTPUT_PUSH_PULL_SPEED_LIMIT_10MHZ(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_CONFIGURATION_OUTPUT_PUSH_PULL_NOSPEED_LIMIT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

#define GPIO_PIN_CONFIGURATION_OUTPUT_OPEN_DRAIN_SPEED_LIMIT_10MHZ(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_CONFIGURATION_OUTPUT_OPEN_DRAIN_NOSPEED_LIMIT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

#define GPIO_PIN_CONFIGURATION_INPUT_PULL_UP_WITH_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_CONFIGURATION_INPUT_PULL_UP_WITHOUT_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

#define GPIO_PIN_CONFIGURATION_INPUT_FLOATING_WITH_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_CONFIGURATION_INPUT_FLOATING_WITHOUT_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

//------------------------------------------------------------------------------
#define GPIO_PIN_CONFIGURATION(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO_PIN_CONFIGURATION_##MODE(PORT, PIN, LEVEL, MODE); \
		}

//------------------------------------------------------------------------------
#define GPIO_PIN_OFF_HIGH(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR &= (uint8_t)((uint8_t)(~(1 << PIN))); }

#define GPIO_PIN_OFF_LOW(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR |= (1 << PIN); }

#define GPIO_PIN_OFF(PORT, PIN, LEVEL, MODE) \
		{ GPIO_PIN_OFF_##LEVEL(PORT, PIN, LEVEL, MODE) }

//------------------------------------------------------------------------------
#define GPIO_PIN_ON_HIGH(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR |= (1 << PIN); }

#define GPIO_PIN_ON_LOW(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR &= (uint8_t)(~(1 << PIN)); }

#define GPIO_PIN_ON(PORT, PIN, LEVEL, MODE) \
		{ GPIO_PIN_ON_##LEVEL(PORT, PIN, LEVEL, MODE) }

//------------------------------------------------------------------------------
#define GPIO_PIN_TOGGLE(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR ^= (uint8_t)(1 << PIN); }

//------------------------------------------------------------------------------
#define GPIO_PIN_SIGNAL_HIGH(PORT, PIN, LEVEL, MODE) \
		( (GPIO##PORT->IDR & (1 << PIN)) == (1 << PIN) )

#define GPIO_PIN_SIGNAL_LOW(PORT, PIN, LEVEL, MODE) \
	( (GPIO##PORT->IDR & (1 << PIN)) != (1 << PIN) )

#define GPIO_PIN_SIGNAL(PORT, PIN, LEVEL, MODE) \
	( GPIO_PIN_SIGNAL_##LEVEL(PORT, PIN, LEVEL, MODE) )

//------------------------------------------------------------------------------
#define PIN_CONFIGURATION(PIN_DESCRIPTION) GPIO_PIN_CONFIGURATION(PIN_DESCRIPTION)
#define PIN_ON(PIN_DESCRIPTION) GPIO_PIN_ON(PIN_DESCRIPTION)
#define PIN_OFF(PIN_DESCRIPTION) GPIO_PIN_OFF(PIN_DESCRIPTION)
#define PIN_TOGGLE(PIN_DESCRIPTION) GPIO_PIN_TOGGLE(PIN_DESCRIPTION)
#define PIN_SIGNAL(PIN_DESCRIPTION) GPIO_PIN_SIGNAL(PIN_DESCRIPTION)

#endif /* MCU_GPIO_H_ */
