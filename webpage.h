#ifndef _WEBPAGE_H
#define _WEBPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum webpage_type
{
	STA_Mode = 0,
	AP_Mode,	
	webpage_end
};

void webpage_init(void);
void webpage_handleClient(void);
void webpage_launchWeb(int webtype);
void webpage_setupAP(void);
void webpage_createWebServer(int webtype);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_WEBPAGE_H
