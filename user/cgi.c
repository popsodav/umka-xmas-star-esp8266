/*
 Some random cgi routines.
 */

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "cgi.h"
#include "espmissingincludes.h"
#include "relay.h"

//cause I can't be bothered to write an ioGetLed()
static char currLedState = 0;

int ICACHE_FLASH_ATTR cgiRelay(HttpdConnData *connData) {
	int len;
	char state[2];
	char buff[1024];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	len = httpdFindArg(connData->getArgs, "state", state, sizeof(state));
	if (len != 0) {
		os_printf("cgiRelay: %s\n", state);

		if(state[0] == '1')
		{
			relay_switchOn();
		} else {
			relay_switchOff();
		}
	}
	len =	os_sprintf(buff, "{\n \"result\": { \n\"Request\": \"setRelay\", \"state:\": \"%d\"\n }\n}\n", atoi(state));
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiRelayGetState(HttpdConnData *connData) {
	int len;
	int state;
	char buff[1024];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	state =  relay_GetState();

	len =	os_sprintf(buff, "{\n \"result\": { \n\"Request\": \"getRelay\", \"state:\": \"%d\"\n }\n}\n", state);
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}

//Template code for the led page.
void ICACHE_FLASH_ATTR tplLed(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token == NULL)
		return;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "ledstate") == 0) {
		if (currLedState) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
	}
	httpdSend(connData, buff, os_strlen(buff));
}

static long hitCounter = 0;

//Template code for the counter on the index page.
void ICACHE_FLASH_ATTR tplCounter(HttpdConnData *connData, char *token,
		void **arg) {
	char buff[128];
	if (token == NULL)
		return;

	if (os_strcmp(token, "counter") == 0) {
		hitCounter++;
		os_sprintf(buff, "%ld", hitCounter);
	}
	httpdSend(connData, buff, os_strlen(buff));
}


//Cgi that reads the SPI flash. Assumes 512KByte flash.
int ICACHE_FLASH_ATTR cgiReadFlash(HttpdConnData *connData) {
	int *pos=(int *)&connData->cgiData;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (*pos==0) {
		os_printf("Start flash download.\n");
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "application/bin");
		httpdEndHeaders(connData);
		*pos=0x40200000;
		return HTTPD_CGI_MORE;
	}
	//Send 1K of flash per call. We will get called again if we haven't sent 512K yet.
	espconn_sent(connData->conn, (uint8 *)(*pos), 1024);
	*pos+=1024;
	if (*pos>=0x40200000+(512*1024)) return HTTPD_CGI_DONE; else return HTTPD_CGI_MORE;
}

