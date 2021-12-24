#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
extern "C" {
#endif

#define _baudrate	115200

void uart_init_0(void);
void uart_handle(void);
void uart_command(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif 
