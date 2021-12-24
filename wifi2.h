#ifndef _WIFI2_H
#define _WIFI2_H

#ifdef __cplusplus
extern "C" {
#endif

void wifi2_init(void);
void wifi2_handle(void);
bool wifi2_connect(void);
void wifi2_WifiCheck(unsigned short usDummyParam);
bool wifi_get_connection(void);
void wifi_ping_test(void);
bool wifi_ping(char* ip);


#ifdef __cplusplus
} // extern "C"
#endif

#endif 
