#ifndef __MCU_TYPE_H
#define __MCU_TYPE_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "at32f403a_407.h"
#define F_CPU                       SystemCoreClock
#define CYCLES_PER_MICROSECOND      (F_CPU / 1000000U)

typedef gpio_type                   GPIO_TypeDef;

typedef uint16_t gpio_pin_t;
#endif