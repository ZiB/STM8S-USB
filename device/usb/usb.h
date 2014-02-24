/*
 * File: usb.h
 * Date: 07.02.2014
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef USB_H_
#define USB_H_

extern uint8_t usb_tx_count;

extern uint8_t usb_tx_buffer[16];

void usb_init(void);
void usb_process(void);

#endif /* USB_H_ */
