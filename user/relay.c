#include "espmissingincludes.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "flash_param.h"
#include "relay.h"

static int relayState = 0;
static os_timer_t relayLoadConfigTimer;

void ICACHE_FLASH_ATTR relay_loadConfig() {
	relayConfig config;
	load_flash_param(ESP_PARAM_SAVE_0, (uint32 *)&config, sizeof(relayConfig));
	os_printf("Relay saved value: %d\n", config.relayValue);

	if(config.relayValue == 1)
	{
		GPIO_OUTPUT_SET(3, 1);
		relayState = 1;
	}
	else
	{
		GPIO_OUTPUT_SET(3, 0);
		relayState = 0;
	}
}


void ICACHE_FLASH_ATTR relay_Init()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3);

	os_timer_disarm(&relayLoadConfigTimer);
	os_timer_setfn(&relayLoadConfigTimer, (os_timer_func_t *) relay_loadConfig, NULL);
	os_timer_arm(&relayLoadConfigTimer, 1000, 0);
}

int ICACHE_FLASH_ATTR relay_GetState()
{
	return relayState;
}

void ICACHE_FLASH_ATTR relay_switchOn()
{
	GPIO_OUTPUT_SET(3, 1);
	relayState = 1;
	relay_updateConfig(relayState);
}

void ICACHE_FLASH_ATTR relay_switchOff()
{
	GPIO_OUTPUT_SET(3, 0);
	relayState = 0;
	relay_updateConfig(relayState);
}

void ICACHE_FLASH_ATTR relay_updateConfig(unsigned int value)
{
	relayConfig config;
	config.relayValue = value;
	save_flash_param(ESP_PARAM_SAVE_0, (uint32 *)&config, sizeof(relayConfig));
}

