#ifndef CGI_H
#define CGI_H

#include "httpd.h"

int cgiLed(HttpdConnData *connData);
void tplLed(HttpdConnData *connData, char *token, void **arg);
void tplDHT(HttpdConnData *connData, char *token, void **arg);
int cgiReadFlash(HttpdConnData *connData);
void tplCounter(HttpdConnData *connData, char *token, void **arg);
int ICACHE_FLASH_ATTR cgiRelay(HttpdConnData *connData);
int ICACHE_FLASH_ATTR cgiRelayGetState(HttpdConnData *connData);
#endif
