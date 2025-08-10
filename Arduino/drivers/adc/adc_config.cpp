#include "adc_config.h"

adc_config_t ADC1_config = 
{
	.adc = {
		.register_base = ADC1,
		.clock_id = CRM_ADC1_PERIPH_CLOCK,
		.clk_div = CRM_ADC_DIV_4,
		.channel_count = ADC_CHANNEL_17,
	},
	.init_params = {
		.repeat = TRUE,
		.data_alignment = ADC_RIGHT_ALIGNMENT,
		.ordinary_channel_length = 1,
	},
	.dma_config = {
		.dma_ch = DMA1_CHANNEL1,
		.loop_mode_enable = TRUE,
	}
};