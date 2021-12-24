#ifndef _NVM_H
#define _NVM_H

#ifdef __cplusplus
extern "C" {
#endif

#define _ssid_len_ 32
#define _pw_len_ 64
#define MAGIC_NUMBER 0x5AA5


typedef struct
{
  int magic_number; //2 byte
  byte date;	// 1 byte
  byte spraytime;	// 1 byte 
  unsigned int boot_count;	// 2 byte 
  char eep_ssid[_ssid_len_];
  char eep_pass[_pw_len_];
  unsigned int id;
  int eep_intData;
  float Home_GPS[2];
  //char device_id[DEVICE_MAX][_id_len_]
} TDIC_SystemDictionary;


void nvm_init(void);
void nvm_handle(void);
void nvm_clean_network(void);
void nvm_clean_SprayData(void);
void nvm_set_save_flag(bool flag);
void nvm_set_HW_ID(unsigned int id);
unsigned int nvm_get_HW_ID(void);
unsigned int nvm_get_boot_count(void);
void nvm_set_ssid(char* ssid);
void nvm_get_ssid(char* ssid);
void nvm_set_password(char* pw);
void nvm_get_password(char* pw);
void nvm_clean_boot_flag(void);
bool nvm_get_boot_flag(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_NVM_H
