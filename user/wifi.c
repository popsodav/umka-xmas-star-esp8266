#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "wifi.h"

static char ssid[33];
static char password[33];
static uint8_t macaddr[6];

void ICACHE_FLASH_ATTR initWiFi()
{
	struct softap_config apConfig;

	if (wifi_get_opmode() != STATION_MODE && false)
	{
		struct ip_info info;

		IP4_ADDR(&info.ip, 10, 10, 10, 1);
		IP4_ADDR(&info.gw, 10, 10, 10, 1);
		IP4_ADDR(&info.netmask, 255, 255, 255, 0);
		wifi_set_ip_info(SOFTAP_IF, &info);

		wifi_set_opmode(0x3);
		wifi_get_macaddr(SOFTAP_IF, macaddr);

		os_memset(apConfig.password, 0, sizeof(apConfig.password));
		os_sprintf(password, "%s", WIFI_APPASSWORD);
		os_memcpy(apConfig.password, password, os_strlen(password));

		os_memset(apConfig.ssid, 0, sizeof(apConfig.ssid));
		os_sprintf(ssid, "%s%02x", WIFI_APSSID, macaddr[5]);
		os_memcpy(apConfig.ssid, ssid, os_strlen(ssid));

		apConfig.authmode = AUTH_WPA_WPA2_PSK;
		//apConfig.authmode = AUTH_OPEN;
		apConfig.channel = 7;
		apConfig.max_connection = 10;
		apConfig.ssid_hidden = 0;
		wifi_softap_set_config(&apConfig);
	} else {
		wifi_set_opmode(STATION_MODE);
	}
}

