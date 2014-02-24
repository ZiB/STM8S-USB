/*
 * File: macros.h
 * Date: 28.01.2011
 * Denis Zheleznjakov @ ZiBlog.ru
 */

#ifndef MACROS_H_
#define MACROS_H_

#define BIT(NUMBER)            		(1UL << (NUMBER))

#define ARRAY_LENGHT(Value)			(sizeof(Value) / sizeof(Value[0]))

#define BYTES(value)    ((uint8_t *) & (value))

#define Bit_Set(Var, BitIdx)        ((Var) |=  BIT(BitIdx))
#define Bit_Clr(Var, BitIdx)        ((Var) &= ~BIT(BitIdx))
#define Bit_Get(Var, BitIdx)		((Var)  &  BIT(BitIdx))
#define Bit_Inv(Var, BitIdx)		((Var) ^=  BIT(BitIdx))
#define Bit_Is_Set(Var, BitIdx)		(Bit_Get(Var, BitIdx) == BIT(BitIdx))
#define Bit_Is_Clr(Var, BitIdx)		(Bit_Get(Var, BitIdx) == 0x00)
#define Set(FlagDef)				Bit_Set(FlagDef)
#define Clr(FlagDef)				Bit_Clr(FlagDef)
#define Get(FlagDef)           		Bit_Get(FlagDef)
#define Inv(FlagDef)				Bit_Inv(FlagDef)
#define Is_Set(FlagDef)        		Bit_Is_Set(FlagDef)
#define Is_Clr(FlagDef)				Bit_Is_Clr(FlagDef)

#define Bits_Set(Var, Mask)        ((Var) |=  (Mask))
#define Bits_Clr(Var, Mask)        ((Var) &= ~(Mask))
#define Bits_Inv(Var, Mask)        ((Var) ^=  (Mask))

void delay_ms_refresh(void);
void delay_ms(volatile uint16_t delay);
uint8_t get_random_byte(void);
void synchronization_10ms_refresh(void);
_Bool time_synchronization_10ms(void);

#endif /* MACROS_H_ */
