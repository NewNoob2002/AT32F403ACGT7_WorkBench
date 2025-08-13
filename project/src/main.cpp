/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"
#include "wk_debug.h"
#include "wk_system.h"
/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "Arduino.h"
#include "SparkFun_Extensible_Message_Parser.h"
#include "lwmem/lwmem.h"
#include "MillisTaskManager.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Fonts/FreeMono12pt7b.h"

typedef Adafruit_ST7789 SCREEN_CLASS;

static SCREEN_CLASS screen(
	PA15,
	PB4,
	PB6);
/* add user code end private includes */
uint8_t region1_data[1024 * LWMEM_SIZE];

static lwmem_region_t
	regions[] = {
		{region1_data, sizeof(region1_data)},
		/* Add more regions if needed */
		{nullptr, 0}};

SEMP_PARSE_ROUTINE const rtkParserTable[] = {
	sempNmeaPreamble,
};

const int rtkParserCount = sizeof(rtkParserTable) / sizeof(rtkParserTable[0]);

const char *const rtkParserNames[] = {
	"NMEA",
};

const int rtkParserNameCount = sizeof(rtkParserNames) / sizeof(rtkParserNames[0]);

SEMP_PARSE_STATE *rtkParse = nullptr;

static MillisTaskManager taskManager;

static uint32_t numSv = 0;
static uint32_t interval = 0;

void processUart1Message(SEMP_PARSE_STATE *parse, uint16_t type)
{
	switch (type)
	{
	case 0:
		printf("NEMA\n");
	}
	char *responsePointer = strstr((char *)parse->buffer, "GNGGA");
	if (responsePointer != nullptr) // Found
	{
		char gngga[100];
		strncpy(gngga, (const char *)parse->buffer, parse->length - 1); // Make copy before strtok
		char *pt;
		pt = strtok(gngga, ",");
		int counter = 0;
		while (pt != NULL)
		{
			int spotValue = atoi(pt);
			if (counter++ == 7)
			{
				numSv = spotValue;
				interval = 1000 / (numSv + 1);
			}
			pt = strtok(NULL, ",");
		}
	}
}

void led_task()
{
	if (interval > 0)
	{
		const uint32_t now = millis();
		static uint32_t last = 0;
		if (now - last >= interval)
		{
			last = now;
			digitalToggle(PA2);
		}
	}
}

void parser_task()
{
	uint8_t rx_buf[512];
	if (Serial.available())
	{
		int bytesIncoming = Serial.readBytes(rx_buf, sizeof(rx_buf));
		for (int x = 0; x < bytesIncoming; x++)
		{
			sempParseNextByte(rtkParse, rx_buf[x]);
		}
	}
}
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
	Serial.begin(115200);
	pinMode(PA2, OUTPUT);
	pinMode(PB7, OUTPUT);
	digitalWrite(PA2, LOW);
	lwmem_assignmem(regions);
	
	screen.init(135, 240);
	screen.setRotation(1);
	screen.setTextSize(3);
	screen.fillScreen(RED);
	screen.print("HelloWorld\n");
	rtkParse = sempBeginParser(rtkParserTable, rtkParserCount,
							   rtkParserNames, rtkParserNameCount,
							   0, 3000, processUart1Message, "NemaParser");
	taskManager.Register(led_task, 50);
	taskManager.Register(parser_task, 10);
	/* add user code end 2 */

	while (1)
	{
		/* add user code begin 3 */
		taskManager.Running(millis());
		/* add user code end 3 */
	}
}
