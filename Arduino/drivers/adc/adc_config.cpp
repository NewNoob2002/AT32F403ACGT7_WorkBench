#include "adc_config.h"

adc_device_t ADC1_device = 
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
	}
};