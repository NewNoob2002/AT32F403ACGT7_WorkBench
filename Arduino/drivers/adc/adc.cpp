#include "adc.h"

ADC::ADC(adc_config_t *adc_config)
{
	this->adc_config = adc_config;
	memset((void *)this->adc_ordinary_valuetab, 0, ADC1_MAX_CHANNEL_COUNT);
	memset((void *)this->isInitilized, 0, ADC1_MAX_CHANNEL_COUNT);
}

bool ADC::addOrdinaryChannel(uint16_t channel)
{
	if (channel < ADC1_MAX_CHANNEL_COUNT && !this->isInitilized[channel])
	{
		this->isInitilized[channel] = true;
		this->adc_ch_enable_count++;
	}
	return true;
}

void ADC::begin()
{
	adc_base_config_type adc_base_struct;
	if (!this->clockInitialized)
	{
		crm_periph_clock_enable(this->adc_config->adc.clock_id, TRUE);
		crm_adc_clock_div_set(this->adc_config->adc.clk_div);
		this->clockInitialized = true;
	}
	adc_combine_mode_select(ADC_INDEPENDENT_MODE);
	adc_base_default_para_init(&adc_base_struct);
	if (this->adc_ch_enable_count == 1){
		adc_base_struct.sequence_mode = FALSE;
	}
	else{
		adc_base_struct.sequence_mode = TRUE;
	}
	
}

ADC::~ADC()
{
	// Destructor logic if needed
	// Currently, nothing to clean up as we are using simple types
	// and no dynamic memory allocation is done in this class.
	memset((void *)this->adc_ordinary_valuetab, 0, ADC1_MAX_CHANNEL_COUNT);
	memset((void *)this->isInitilized, 0, ADC1_MAX_CHANNEL_COUNT);
}