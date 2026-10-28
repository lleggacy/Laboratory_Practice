#ifndef INIT_H
#define INIT_H

#include "stm32f4xx.h"
#include "stm32f429xx.h"

#define MY_GPIOC_MODER          (*(uint32_t*)(0x40020800UL + 0x00UL))
#define MY_GPIOC_OTYPER         (*(uint32_t*)(0x40020800UL + 0x04UL))
#define MY_GPIOC_OSPEEDR        (*(uint32_t*)(0x40020800UL + 0x08UL))
#define MY_GPIOC_PUPDR          (*(uint32_t*)(0x40020800UL + 0x0CUL))
#define MY_GPIOC_BSRR           (*(uint32_t*)(0x40020800UL + 0x18UL))

/* Константы битовых масок для 6-го пина порта C (PC6) */
#define MY_GPIOC_MODE_PIN6_OUT      0x00000000UL  /* Будет установлено через сдвиг */
#define MY_GPIOC_OTYPE_PIN6_PP      0x00000000UL  /* Push-Pull (бит = 0) */
#define MY_GPIOC_OSPEED_PIN6_MID    0x00001000UL  /* Medium speed для PC6 */
#define MY_GPIOC_PUPDR_PIN6_NOPUPD  0x00000000UL  /* No pull-up/pull-down */
#define MY_GPIOC_BSRR_PIN6_SET      0x00000040UL  /* BS6 */
#define MY_GPIOC_BSRR_PIN6_RESET    0x00400000UL  /* BR6 */

/* Пользовательские макросы работы с битами */
#define MY_SET_BIT(REG, BIT)        ((REG) |= (BIT))
#define MY_CLEAR_BIT(REG, BIT)      ((REG) &= ~(BIT))

/* Прототип функции инициализации периферии GPIO */
void GPIO_Ini(void);

#endif 