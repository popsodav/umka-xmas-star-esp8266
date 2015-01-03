#include "flash_param.h"
#include "lpd6803.h"
#include "xmasstar_state.h"

/*
void ICACHE_FLASH_ATTR restoreState() {
	xmasStarConfig config;
	load_flash_param(ESP_PARAM_SAVE_1, (uint32 *) &config,
			sizeof(xmasStarConfig));

	switch (config.mode) {
		case (LPD6803_MODE_RAINBOW):
			lpd6803_startRainbow();
			break;
		case (LPD6803_MODE_RUNNING_LINE):
			lpd6803_startRunningLine(config.a_color);
			break;
		case (LPD6803_MODE_RUNNING_PIXEL):
			lpd6803_startRunningPixel(config.a_color);
			break;
		default:
			lpd6803_setPixelColorByColor(0, config.a_color);
			lpd6803_setPixelColorByColor(1, config.b_color);
			lpd6803_setPixelColorByColor(2, config.c_color);
			lpd6803_setPixelColorByColor(3, config.d_color);
			lpd6803_setPixelColorByColor(4, config.e_color);
			lpd6803_show();
			break;
	}
}

void ICACHE_FLASH_ATTR saveState() {
	xmasStarConfig config;
	config.mode = lpd6803_getMode();
	switch (config.mode) {
		case (LPD6803_MODE_RAINBOW):
			break;
		case (LPD6803_MODE_RUNNING_LINE):
			config.a_color =
			break;
		case (LPD6803_MODE_RUNNING_PIXEL):
			lpd6803_startRunningPixel(config.a_color);
			break;
		default:
			lpd6803_setPixelColorByColor(0, config.a_color);
			lpd6803_setPixelColorByColor(1, config.b_color);
			lpd6803_setPixelColorByColor(2, config.c_color);
			lpd6803_setPixelColorByColor(3, config.d_color);
			lpd6803_setPixelColorByColor(4, config.e_color);
			lpd6803_show();
			break;
	}
}
*/
