#include "init.h"

/* Глобальные переменные для отслеживания на графиках в STMViewer */
uint8_t btn1_flag = 0;   // Состояние кнопки 1 (PC12)
uint8_t btn2_flag = 0;   // Состояние кнопки 2 (PA3)
uint8_t step_leds = 1;   // Заданное количество одновременно включаемых светодиодов (1..6)
uint8_t active_leds = 0; // Текущее количество включенных светодиодов (0..6)

/* Вспомогательная функция управления всеми 6 светодиодами */
void Update_LEDs(uint8_t count) {
    // 1-й светодиод (PB0) — прямое обращение к памяти (регистр BSRR)
    if (count >= 1) {
        *(uint32_t*)(0x40020400UL + 0x18UL) = 0x00000001UL; // BS0 (вкл)
    } else {
        *(uint32_t*)(0x40020400UL + 0x18UL) = 0x00010000UL; // BR0 (выкл)
    }

    // 2-й светодиод (PC6 вместо PB7) — собственные директивы и макросы
    if (count >= 2) {
        *(uint32_t*)(0x40020800UL + 0x18UL) = 0x00000040UL; // BS6 PC6 (вкл)
    } else {
        *(uint32_t*)(0x40020800UL + 0x18UL) = 0x00400000UL; // BR6 PC6 (выкл)
    }

    // 3-й светодиод (PC7 вместо PB14) — макросы библиотеки CMSIS
    if (count >= 3) {
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS7);
    } else {
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR7);
    }

    // 4-й светодиод (PA5) — CMSIS
    if (count >= 4) {
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5);
    } else {
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5);
    }

    // 5-й светодиод (PA6) — CMSIS
    if (count >= 5) {
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS6);
    } else {
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR6);
    }

    // 6-й светодиод (PA7) — CMSIS
    if (count >= 6) {
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS7);
    } else {
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR7);
    }
}

/* Программная задержка для подавления дребезга контактов */
void Delay_Debounce(void) {
    for (volatile uint32_t i = 0; i < 150000; i++) {
        __NOP();
    }
}

int main(void) {
    GPIO_Ini(); // Инициализация портов GPIO
    Update_LEDs(0);

    uint8_t btn1_prev = 0;
    uint8_t btn2_prev = 0;

    while (1) {
        /* Считываем текущее состояние обеих кнопок */
        uint8_t btn1_curr = (READ_BIT(GPIOC->IDR, GPIO_IDR_ID12) != 0) ? 1 : 0;
        uint8_t btn2_curr = (READ_BIT(GPIOA->IDR, GPIO_IDR_ID3) != 0) ? 1 : 0;

        btn1_flag = btn1_curr;
        btn2_flag = btn2_curr;

        /* Обработка нажатия Кнопки 1 (PC12) по переднему фронту (0 -> 1) */
        if (btn1_curr == 1 && btn1_prev == 0) {
            step_leds++;
            if (step_leds > 6) {
                step_leds = 1; // Циклический выбор шага от 1 до 6
            }
            // При смене шага сбрасываем текущие горящие светодиоды
            active_leds = 0;
            Update_LEDs(active_leds);
            Delay_Debounce();
        }
        btn1_prev = btn1_curr;

        /* Обработка нажатия Кнопки 2 (PA3) по переднему фронту (0 -> 1) */
        if (btn2_curr == 1 && btn2_prev == 0) {
            if (active_leds >= 6) {
                // Если все 6 светодиодов уже включены — выключаем их и начинаем сначала
                active_leds = 0;
            } else {
                // Иначе включаем следующее заданное количество светодиодов
                active_leds += step_leds;
                if (active_leds > 6) {
                    active_leds = 6;
                }
            }
            Update_LEDs(active_leds);
            Delay_Debounce();
        }
        btn2_prev = btn2_curr;
    }
}