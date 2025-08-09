/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_system.c
  * @brief    work bench config program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

#include "wk_system.h"

#define STEP_DELAY_MS                    (uint32_t)(50)
#define TICK_COUNT_MAX                   (uint32_t)(0xFFFF)
#define TICK_COUNT_VALUE                 (TMR3->cval)

/* global variable */
volatile uint32_t ticks_count_us;

/**
  * @brief  this function provides minimum delay (in microsecond).
  * @param  delay: specifies the delay time length, in microsecond.
  * @retval none
  */
__WEAK void wk_delay_us(uint32_t delay)
{
  uint32_t delay_ticks, pre_ticks, cur_ticks, delta;
  delay_ticks = delay * ticks_count_us;

  pre_ticks = TICK_COUNT_VALUE;
  do
  {
    cur_ticks = TICK_COUNT_VALUE;
    /* count up */
    delta = (cur_ticks >= pre_ticks) ? (cur_ticks - pre_ticks) : ((TICK_COUNT_MAX - pre_ticks) + cur_ticks + 1);
  } while(delta < delay_ticks);
}

/**
  * @brief  this function provides minimum delay (in milliseconds).
  * @param  delay variable specifies the delay time length, in milliseconds.
  * @retval none
  */
__WEAK void wk_delay_ms(uint32_t delay)
{
  while(delay)
  {
    if(delay > STEP_DELAY_MS)
    {
      wk_delay_us(STEP_DELAY_MS * 1000);
      delay -= STEP_DELAY_MS;
    }
    else
    {
      wk_delay_us(delay * 1000);
      delay = 0;
    }
  }
}

/**
  * @brief  this function configures the source of the time base.
  * @param  none
  * @retval none
  */
__WEAK void wk_timebase_init(void)
{
  crm_clocks_freq_type crm_clocks;
  uint32_t frequency = 0;

  /* get crm_clocks */
  crm_clocks_freq_get(&crm_clocks);

  frequency = crm_clocks.apb1_freq * 2;

  /* time base configuration */
  tmr_base_init(TMR3, TICK_COUNT_MAX, ((frequency / 1000000U) - 1));
  ticks_count_us = 1U;
  /* config timer count up */
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  /* enable timer */
  tmr_counter_enable(TMR3, TRUE);
}

/**
  * @brief  config systick and enable interrupt.
  * @param  nHz: systick interrupt frequency.
  * @retval ERROR or SUCCESS.
  */
error_status systick_interrupt_config(uint32_t nHz)
{
  crm_clocks_freq_type crm_clocks;
  uint32_t frequency = 0, ticks = 0;

  /* get crm_clocks */
  crm_clocks_freq_get(&crm_clocks);

  frequency = crm_clocks.ahb_freq / 8;

  /* config systick clock source */
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_DIV8);

  ticks = frequency / nHz;
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return ERROR;
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);
  SysTick->VAL   = 0UL;
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk |
                   SysTick_CTRL_ENABLE_Msk;
  return SUCCESS;
}

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
  __asm (".global __use_no_semihosting\n\t");
  void _sys_exit(int x)
  {
    UNUSED(x);
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    UNUSED(ch);
  }
  FILE __stdout;
#else
 #ifdef __CC_ARM
  #pragma import(__use_no_semihosting)
  struct __FILE
  {
    int handle;
  };
  FILE __stdout;
  void _sys_exit(int x)
  {
    UNUSED(x);
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    UNUSED(ch);
  }
 #endif
#endif

#if defined (__GNUC__) && !defined (__clang__)
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
void io_putc(int ch);
/**
  * @brief  retargets the c library printf function to the usart.
  * @param  none
  * @retval none
  */
PUTCHAR_PROTOTYPE
{
#if !defined (__GNUC__) || defined (__clang__)
  UNUSED(f);
#endif
	io_putc(ch);
  return ch;
}

#if (defined (__GNUC__) && !defined (__clang__)) || (defined (__ICCARM__))
#if defined (__GNUC__) && !defined (__clang__)
int _write(int fd, char *pbuffer, int size)
#elif defined ( __ICCARM__ )
#pragma module_name = "?__write"
int __write(int fd, char *pbuffer, int size)
#endif
{
  UNUSED(fd);
  for(int i = 0; i < size; i ++)
  {
    while(usart_flag_get(USART1, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART1, (uint16_t)(*pbuffer++));
    while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
  }

  return size;
}
#endif

