#include "usart.h"
#include "usart_config.h"
#include "core_hooks.h"

#ifdef USART1_ENABLE
Usart Serial1(&USART1_config, SERIAL_RX_BUFFER_SIZE, SERIAL_TX_BUFFER_SIZE);
#endif

//
// IRQ register / unregister helper
//
inline void usart_irq_register(usart_interrupt_config_t &irq, const char *name)
{
    // register and enable irq
    uint32_t temp_priority = 0;

    /* encode priority */
    temp_priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), irq.preempt_priority, irq.sub_priority);
    NVIC_SetPriority(irq.interrupt_number, temp_priority);
    NVIC_EnableIRQ(irq.interrupt_number);
}

inline void usart_irq_resign(usart_interrupt_config_t &irq, const char *name)
{
    // disable interrupt and clear pending
    NVIC_DisableIRQ(irq.interrupt_number);
    NVIC_ClearPendingIRQ(irq.interrupt_number);
}

//
// debug print helpers
//
#define USART_REG_TO_X(reg) \
    reg == USART1   ? 1  \
    : reg == USART2 ? 2  \
    : reg == USART3 ? 3  \
    : reg == USART4 ? 4  \
                       : 0
#define USART_DEBUG_PRINTF(fmt, ...) \
    CORE_DEBUG_PRINTF("[USART%d] " fmt, USART_REG_TO_X(this->config->peripheral.register_base), ##__VA_ARGS__)

		Usart :: Usart(usart_config_t *usart_config, size_t rx_buffer_size, size_t tx_buffer_size)
		{
		}
		Usart :: ~Usart(){
		}