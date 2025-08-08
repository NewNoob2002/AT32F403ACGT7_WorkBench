#ifndef __MCU_TYPE_H
#define __MCU_TYPE_H
#include "at32f403a_407.h"
#define F_CPU                       SystemCoreClock
#define CYCLES_PER_MICROSECOND      (F_CPU / 1000000U)

typedef gpio_type                   GPIO_TypeDef;
#define GPIO_Pin_0                  GPIO_Pins_0
#define GPIO_Pin_1                  GPIO_Pins_1
#define GPIO_Pin_2                  GPIO_Pins_2
#define GPIO_Pin_3                  GPIO_Pins_3
#define GPIO_Pin_4                  GPIO_Pins_4
#define GPIO_Pin_5                  GPIO_Pins_5
#define GPIO_Pin_6                  GPIO_Pins_6
#define GPIO_Pin_7                  GPIO_Pins_7
#define GPIO_Pin_8                  GPIO_Pins_8
#define GPIO_Pin_9                  GPIO_Pins_9
#define GPIO_Pin_10                 GPIO_Pins_10
#define GPIO_Pin_11                 GPIO_Pins_11
#define GPIO_Pin_12                 GPIO_Pins_12
#define GPIO_Pin_13                 GPIO_Pins_13
#define GPIO_Pin_14                 GPIO_Pins_14
#define GPIO_Pin_15                 GPIO_Pins_15
#define GPIO_Pin_All                GPIO_Pins_All

typedef uint16_t gpio_pin_t;
#endif