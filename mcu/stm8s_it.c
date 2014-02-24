/**
 ******************************************************************************
 * @file     stm8s_it.c
 * @author   MCD Application Team
 * @version  V2.1.0
 * @date     18-November-2011
 * @brief    Main Interrupt Service Routines.
 *           This file provides template for all peripherals interrupt service
 *           routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm8s_it.h"

#ifdef _COSMIC_
INTERRUPT_HANDLER(NonHandledInterrupt)
{
}
#endif /*_COSMIC_*/

INTERRUPT_HANDLER_TRAP( TRAP_IRQHandler)
{
}

INTERRUPT_HANDLER( TLI_IRQHandler)
{
}

INTERRUPT_HANDLER( AWU_IRQHandler)
{
}

INTERRUPT_HANDLER( CLK_IRQHandler)
{
}

INTERRUPT_HANDLER( EXTI_PORTA_IRQHandler)
{
}

INTERRUPT_HANDLER( EXTI_PORTB_IRQHandler)
{
}

INTERRUPT_HANDLER( EXTI_PORTC_IRQHandler)
{
}

INTERRUPT_HANDLER( EXTI_PORTD_IRQHandler)
{
}

INTERRUPT_HANDLER( EXTI_PORTE_IRQHandler)
{
}

#ifdef STM8S903
INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler)
{
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
INTERRUPT_HANDLER(CAN_RX_IRQHandler)
{
}

INTERRUPT_HANDLER(CAN_TX_IRQHandler)
{
}
#endif /*STM8S208 || STM8AF52Ax */

INTERRUPT_HANDLER( SPI_IRQHandler)
{
}

INTERRUPT_HANDLER( TIM1_UPD_OVF_TRG_BRK_IRQHandler)
{
}

//INTERRUPT_HANDLER( TIM1_CAP_COM_IRQHandler)
//{
//}

#ifdef STM8S903
INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler)
{
}

INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler)
{
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
INTERRUPT_HANDLER( TIM2_UPD_OVF_BRK_IRQHandler)
{
}

INTERRUPT_HANDLER( TIM2_CAP_COM_IRQHandler)
{
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler)
{
}

INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler)
{
}
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
INTERRUPT_HANDLER( UART1_TX_IRQHandler)
{
}

INTERRUPT_HANDLER( UART1_RX_IRQHandler)
{
}
#endif /*STM8S208 or STM8S207 or STM8S103 or STM8S903 or STM8AF62Ax or STM8AF52Ax */

INTERRUPT_HANDLER( I2C_IRQHandler)
{
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
INTERRUPT_HANDLER(UART2_TX_IRQHandler)
{
}

INTERRUPT_HANDLER(UART2_RX_IRQHandler)
{
}
#endif /* STM8S105 or STM8AF626x */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
INTERRUPT_HANDLER(UART3_TX_IRQHandler)
{
}

INTERRUPT_HANDLER(UART3_RX_IRQHandler)
{
}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
INTERRUPT_HANDLER(ADC2_IRQHandler)
{
}
#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */
INTERRUPT_HANDLER( ADC1_IRQHandler)
{
}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler)
{
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
 * @brief Timer4 Update/Overflow Interrupt routine.
 * @param  None
 * @retval None
 */

INTERRUPT_HANDLER( TIM4_UPD_OVF_IRQHandler)
{
}
#endif /*STM8S903*/

INTERRUPT_HANDLER( EEPROM_EEC_IRQHandler)
{
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
