/*
 * uart_enable.h
 *
 *  Created on: 11-Mar-2025
 *      Author:Sourish
 */

#ifndef UART_ENABLE_H_
#define UART_ENABLE_H_

#define  LED_RED    (GPIO_PORTF_DATA_R=0x02)
#define  LED_BLUE   (GPIO_PORTF_DATA_R=0x04)
#define  LED_GREEN  (GPIO_PORTF_DATA_R=0x08)
#define  LED_WHITE  (GPIO_PORTF_DATA_R=0x0E)
#define  LED_DARK   (GPIO_PORTF_DATA_R=0x00)
#define  LED_YELLOW (GPIO_PORTF_DATA_R=0x0A)
#define  LED_SKYBLUE (GPIO_PORTF_DATA_R=0x0C)
#define  LED_VIOLET  (GPIO_PORTF_DATA_R=0x06)

//extern int state ;

extern void UART0_init(void);
extern void UART0_Int_Enable(void);
extern void extract_blinkrate_info(char * buffer, char * command, char * blinkrate);
extern void upd_buffer(char curr_val, char * buffer, char * command , char * blinkrate ,  int * state , int * rtc_data, uint8_t *buffer_updated);
extern void perform_action(char * buffer, int * state, int * rtc_data);
extern void removeNonAlphanumeric(char *str);
extern void UARTSend(const char *pucBuffer);
extern void UARTCharSend(const char *pucBuffer);
extern void UARTSendHex(uint32_t value);
#endif /* UART_ENABLE_H_ */
