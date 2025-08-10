#ifndef ADC_H
#define ADC_H
#include "adc_config.h"

class ADC
{
public:
	ADC(adc_config_t *adc_config);
	~ADC();

	bool addOrdinaryChannel(uint16_t channel);

	void begin();
	/**
	 * @brief Get the value of an ADC channel
	 * @param channel ADC channel number
	 * @return ADC value
	 * @note The channel number should be less than ADC1_MAX_CHANNEL_COUNT
	 * @return 0 if the channel is not initialized
	 */
	uint16_t getOrdinaryValue(uint16_t channel) const
	{
		if (channel < ADC1_MAX_CHANNEL_COUNT && this->isInitilized[channel])
		{
			return this->adc_ordinary_valuetab[channel];
		}
		return 0;
	}

private:
	adc_config_t *adc_config;

	bool clockInitialized = false;
	/**
	 * @brief ADC ordinary value table
	 * @note The size of this array is defined by ADC1_MAX_CHANNEL_COUNT
	 * This array holds the values read from the ADC channels.
	 * Each index corresponds to a channel number.
	 * @note The values are updated when the ADC is read.
	 * @note This array is initialized to zero in the constructor.
	 * 	*/
	__IO uint16_t adc_ordinary_valuetab[ADC1_MAX_CHANNEL_COUNT];
	__IO bool isInitilized[ADC1_MAX_CHANNEL_COUNT];

	uint16_t adc_ch_enable_count = 0; // Count of enabled ADC channels
};

#endif