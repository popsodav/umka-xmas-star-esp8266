#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "lpd6803.h"
#include "cgi.h"
#include "cgilpd6803.h"
#include "espmissingincludes.h"

int ICACHE_FLASH_ATTR cgiLPD6803GetMode(HttpdConnData *connData) {
	int len;
	char buff[1024];
	os_printf("cgiLPD6803GetMode\n");

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	len =
			os_sprintf(buff,
					"{\n \"result\":  { \n\"Request\": \"getMode\", \n\"mode\": \"%d\"\n }\n}\n",
					lpd6803_getMode());
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiLPD6803SetMode(HttpdConnData *connData) {
	int len;
	char buffMode[2];
	char buff[1024];
	char r[3];
	char g[3];
	char b[3];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	len = httpdFindArg(connData->getArgs, "mode", buffMode, sizeof(buffMode));
	if (len != 0) {
		os_printf("cgiLPD6803SetMode: %s\n", buffMode);
		switch (atoi(buffMode)) {
		case (0):
			lpd6803_disableModes();
			break;
		case (1):
			httpdFindArg(connData->getArgs, "r", r, sizeof(r));
			httpdFindArg(connData->getArgs, "g", g, sizeof(g));
			httpdFindArg(connData->getArgs, "b", b, sizeof(b));
			lpd6803_startRunningPixel(lpd6803_Color(atoi(r), atoi(g), atoi(b)));
			break;
		case (2):
			httpdFindArg(connData->getArgs, "r", r, sizeof(r));
			httpdFindArg(connData->getArgs, "g", g, sizeof(g));
			httpdFindArg(connData->getArgs, "b", b, sizeof(b));
			lpd6803_startRunningLine(lpd6803_Color(atoi(r), atoi(g), atoi(b)));
			break;
		case (3):
			lpd6803_startRainbow();
			break;
		case (4):
			lpd6803_startRainbow2();
			break;
		case (5):
			lpd6803_startSnow();
			break;
		case (6):
			lpd6803_startRGB();
			break;
		}
	}
	len =
			os_sprintf(buff,
					"{\n \"result\":  { \n\"Request\": \"setMode\", \n\"mode\": \"%s\"\n }\n}\n",
					buffMode);
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiLPD6803SetPixelColor(HttpdConnData *connData) {
	int len;
	char num[3];
	char buff[1024];
	char r[4];
	char g[4];
	char b[4];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	len = httpdFindArg(connData->getArgs, "num", num, sizeof(num));
	if (len != 0) {
		os_printf("cgiLPD6803SetPixelColor: %s\n", num);
		len = httpdFindArg(connData->getArgs, "r", buff, sizeof(buff));
		if (len > 3)
			len = 3;
		os_strncpy(r, buff, len);
		r[len] = '\0';
		len = httpdFindArg(connData->getArgs, "g", buff, sizeof(buff));
		if (len > 3)
			len = 3;
		os_strncpy(g, buff, len);
		g[len] = '\0';
		len = httpdFindArg(connData->getArgs, "b", buff, sizeof(buff));
		if (len > 3)
			len = 3;
		os_strncpy(b, buff, len);
		b[len] = '\0';
		lpd6803_disableModes();
		lpd6803_setPixelColor(atoi(num), atoi(r), atoi(g), atoi(b));
		lpd6803_show();
	}
	len =
			os_sprintf(buff,
					"{\n \"result\": { \n\"Request\": \"setPixelColor\", \"PixelNum:\": \"%s\", \"R\": \"%s\", \"G\": \"%s\", \"B\": \"%s\" \n }\n}\n",
					num, r, g, b);
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiLPD6803SetColor(HttpdConnData *connData) {
	int len;
	char buff[1024];
	char r[4];
	char g[4];
	char b[4];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	os_printf("cgiLPD6803SetColor\n");
	len = httpdFindArg(connData->getArgs, "r", buff, sizeof(buff));
	if (len > 3)
		len = 3;
	os_strncpy(r, buff, len);
	r[len] = '\0';
	len = httpdFindArg(connData->getArgs, "g", buff, sizeof(buff));
	if (len > 3)
		len = 3;
	os_strncpy(g, buff, len);
	g[len] = '\0';
	len = httpdFindArg(connData->getArgs, "b", buff, sizeof(buff));
	if (len > 3)
		len = 3;
	os_strncpy(b, buff, len);
	b[len] = '\0';
	lpd6803_disableModes();
	lpd6803_setAllPixelColor(atoi(r), atoi(g), atoi(b));
	lpd6803_show();

	len =
			os_sprintf(buff,
					"{\n \"result\": { \n\"Request\": \"setColor\", \"R\": \"%s\", \"G\": \"%s\", \"B\": \"%s\" \n }\n}\n",
					r, g, b);
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiLPD6803GetColor(HttpdConnData *connData) {
	int len;
	char buff[1024];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	os_printf("cgiLPD6803GetColor\n");
	len = httpdFindArg(connData->getArgs, "num", buff, sizeof(buff));

	int pixel = atoi(buff);

	len =
			os_sprintf(buff,
					"{\n \"result\": { \n\"Request\": \"getColor\", \"num\": \"%d\", \"R\": \"%d\", \"G\": \"%d\", \"B\": \"%d\" \n }\n}\n",
					pixel, lpd6803_getPixelColorR(pixel),
					lpd6803_getPixelColorG(pixel),
					lpd6803_getPixelColorB(pixel));
	httpdSend(connData, buff, len);
	return HTTPD_CGI_DONE;
}
