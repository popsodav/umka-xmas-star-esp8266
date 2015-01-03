#include "espmissingincludes.h"
#include "user_interface.h"
#include "user_config.h"
#include "stdout.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "cgi.h"
#include "cgiwifi.h"
#include "wifi.h"
#include "cgilpd6803.h"
#include "lpd6803.h"
#include "httpdespfs.h"
#include "discovery.h"
#include "relay.h"

HttpdBuiltInUrl builtInUrls[]={
	{"/wifi", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/wifi/connect.cgi", cgiWiFiConnect, NULL},
	{"/wifi/setmode.cgi", cgiWifiSetMode, NULL},
	{"/lpd6803/setmode.cgi", cgiLPD6803SetMode, NULL},
	{"/lpd6803/getmode.cgi", cgiLPD6803GetMode, NULL},
	{"/lpd6803/setpixel.cgi", cgiLPD6803SetPixelColor, NULL},
	{"/lpd6803/setcolor.cgi", cgiLPD6803SetColor,NULL},
	{"/lpd6803/getcolor.cgi", cgiLPD6803GetColor,NULL},
	{"/relay.cgi", cgiRelay,NULL},
	{"/relaystate.cgi", cgiRelayGetState,NULL},
	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};

//Init function 
void ICACHE_FLASH_ATTR
user_init() {
	system_timer_reinit();
	stdoutInit();
	initWiFi();
	httpdInit(builtInUrls, 80);
	discoveryInit(5533);
	relay_Init();

	lpd6803_init();
	lpd6803_show();

	lpd6803_startRainbow();
}
