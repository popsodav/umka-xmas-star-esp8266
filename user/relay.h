void ICACHE_FLASH_ATTR relay_Init();
void ICACHE_FLASH_ATTR relay_switchOn();
void ICACHE_FLASH_ATTR relay_switchOff();
int ICACHE_FLASH_ATTR relay_GetState();
void ICACHE_FLASH_ATTR relay_updateConfig(unsigned int value);

typedef struct {
	unsigned int relayValue;
} relayConfig;
