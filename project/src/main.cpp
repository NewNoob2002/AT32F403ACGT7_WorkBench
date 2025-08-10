/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"
#include "wk_debug.h"
#include "wk_system.h"
/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "Arduino.h"
/* add user code end private includes */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* system clock config. */
  wk_system_clock_config();

  /* init debug function. */
  wk_debug_config();

  Delay_Init();

  /* nvic config. */
  wk_nvic_config();

  /* add user code begin 2 */
	pinMode(PA0, OUTPUT);
  Serial.begin(115200);
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
		digitalToggle(PA0);
		delay_ms(1000);
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
