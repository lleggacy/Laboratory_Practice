#include "init.h"

void GPIO_Ini(void) {
    // Включаем тактирование портов GPIOA (бит 0), GPIOB (бит 1) и GPIOC (бит 2)
    *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x07UL;  // RCC->AHB1ENR |= 0x07

    // Настройка PB0 (LED 1 - зеленый) напрямую через адреса памяти:
    *(uint32_t*)(0x40020400UL + 0x00UL) &= ~(0x03UL << 0);  // Очистка битов MODER0
    *(uint32_t*)(0x40020400UL + 0x00UL) |= 0x01UL;          // MODER0 = 01 (Output mode)
    *(uint32_t*)(0x40020400UL + 0x04UL) &= ~(0x01UL << 0);  // OTYPER0 = 0 (Push-Pull)
    *(uint32_t*)(0x40020400UL + 0x08UL) |= 0x01UL;          // OSPEEDR0 = 01 (Medium speed)
    *(uint32_t*)(0x40020400UL + 0x0CUL) &= ~(0x03UL << 0);  // PUPDR0 = 00 (No pull-up/pull-down)


    // Настройка PC6 на выход (MODER6 = 01)
    MY_GPIOC_MODER &= ~(0x03UL << 12);  // Очистка битов MODER6
    MY_GPIOC_MODER |= (0x01UL << 12);   // MODER6 = 01 (Output mode)
    
    // OTYPER6 = 0 (Push-Pull)
    MY_CLEAR_BIT(MY_GPIOC_OTYPER, (0x01UL << 6));
    
    // OSPEEDR6 = 01 (Medium speed)
    MY_GPIOC_OSPEEDR &= ~(0x03UL << 12);
    MY_GPIOC_OSPEEDR |= (0x01UL << 12);
    
    // PUPDR6 = 00 (No pull-up/pull-down)
    MY_GPIOC_PUPDR &= ~(0x03UL << 12);

    GPIOC->MODER &= ~(GPIO_MODER_MODER7);      // Очистка MODER7
    GPIOC->MODER |= GPIO_MODER_MODER7_0;       // MODER7 = 01 (Output)
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT7); // Push-Pull
    GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR7); // Очистка
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_0;  // Medium speed
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR7);  // No pull-up/pull-down

    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);
    SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR5_0 | GPIO_OSPEEDER_OSPEEDR6_0 | GPIO_OSPEEDER_OSPEEDR7_0);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);

    // PC12 по умолчанию в режиме входа (00), имеет внешний резистор стяжки на плате
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER12);

    // PA3 (внешняя кнопка 2): режим входа (00) + включаем внутреннюю стяжку к земле (Pull-Down = 10)
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER3);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPDR3_0);
    SET_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPDR3_1);
}