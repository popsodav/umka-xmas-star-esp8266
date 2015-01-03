#include "espmissingincludes.h"
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"

#include "espconn.h"
#include "discovery.h"

static int discoveryPort;

void ICACHE_FLASH_ATTR sendResponse(uint8 remote_ip0, uint8 remote_ip1, uint8 remote_ip2, uint8 remote_ip3, int port)
{
	struct espconn udpresp;
	esp_udp udpcl;
	udpresp.type = ESPCONN_UDP;
	udpresp.state = ESPCONN_NONE;
	udpresp.proto.udp = &udpcl;
	udpcl.remote_port = port;
	udpcl.remote_ip[0] = remote_ip0;
	udpcl.remote_ip[1] = remote_ip1;
	udpcl.remote_ip[2] = remote_ip2;
	udpcl.remote_ip[3] = remote_ip3;
	espconn_create(&udpresp);
	uint8_t macaddr[6];
	char str[1024];
	wifi_get_macaddr(SOFTAP_IF, macaddr);
	int l;
	l = os_sprintf(str, "{\"deviceName\": \"%s\", \"deviceType\": \"%s\", \"fwVersion\": \"%s\", \"mac\": \"%02x:%02x:%02x:%02x:%02x:%02x\"}", DEVICE_NAME, DEVICE_TYPE, FW_VERSION , macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	os_printf("Data: %s (len: %d)\n\r", str, l);
	espconn_sent(&udpresp, (uint8_t *)str, l);
	//espconn_delete(&udpresp);
	//restartServer();
}

static void ICACHE_FLASH_ATTR udp_recv(void *arg, char *pusrdata, unsigned short length) {
	struct espconn *conn = arg;
	char ipaddr[16];
	os_sprintf(ipaddr, "%d.%d.%d.%d", conn->proto.udp->remote_ip[0], conn->proto.udp->remote_ip[1], conn->proto.udp->remote_ip[2], conn->proto.udp->remote_ip[3]);
	int port = conn->proto.udp->remote_port;
	os_printf("Discovery msg: %s (%s:%d)\n", pusrdata, ipaddr, port);
	sendResponse(conn->proto.udp->remote_ip[0], conn->proto.udp->remote_ip[1], conn->proto.udp->remote_ip[2], conn->proto.udp->remote_ip[3], port);
}


void ICACHE_FLASH_ATTR restartServer()
{
	static struct espconn udpconn;
	static esp_udp udp;
	udpconn.type = ESPCONN_UDP;
	udpconn.state = ESPCONN_NONE;
	udpconn.proto.udp = &udp;
	udp.local_port=discoveryPort;
	udpconn.reverse = NULL;
	espconn_regist_recvcb(&udpconn, udp_recv);
	espconn_create(&udpconn);
}

void ICACHE_FLASH_ATTR discoveryInit(int port) {
		discoveryPort = port;
		restartServer();
}


