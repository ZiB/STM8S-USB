/*
 * File: usb.c
 * Date: 07.02.2014
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

extern void usb_tx(void);

uint8_t usb_rx_buffer[16];

uint8_t * usb_tx_buffer_pointer;

uint8_t usb_tx_count;
uint8_t usb_rx_count;

enum usb_settings
{
	USB_VENDOR_ID_LOW = 0x11, //
	USB_VENDOR_ID_HIGH = 0x22,
	USB_DEVICE_ID_LOW = 0x33, //
	USB_DEVICE_ID_HIGH = 0x44,
	USB_DEVICE_VERSION_LOW = 0x55, //
	USB_DEVICE_VERSION_HIGH = 0x66
};

const uint8_t usb_device_descriptor[] = { 18, // размер дескриптора (байт)
		1, // тип дескриптора - устройство = 1
		0x00, 0x01, // номер спецификации USB (USB 1.1 как 0x0110 и USB 1.0 как 0x0100)
		0xFF, // код класса назначен вендором !
		0x00, // код подкласса
		0, // код протокола
		8, // максимальный размер пакета, для LS 8 байт
		USB_VENDOR_ID_LOW, USB_VENDOR_ID_HIGH, // VID
		USB_DEVICE_ID_LOW, USB_DEVICE_ID_HIGH,// PID
		USB_DEVICE_VERSION_LOW, USB_DEVICE_VERSION_HIGH, // версия устройства
		0, // индекс строки описания производителя
		0, // индекс строки описания продукта
		0, // индекс строки серийного номера
		1, // кол-во конфигураций
		};

const uint8_t usb_configuration_descriptor[] = { 9, // размер дескриптора (байт)
		2, // тип дескриптора - конфигурация = 2
		18, 0, // полная длина возвращаемых данных в байтах
		1, // количество интерфейсов
		1, // номер конфигурации
		0, // индекс строки описания конфигурации
		0x80, // аттрибуты (D7 всегда 1, D6 - питание не от USB, D5 - пробуждение)
		50, // максимальный ток потребления в мА деленный на 2
		};

const uint8_t usb_interface_descriptor[] = { 9, // размер дескриптора (байт)
		4, // тип дескриптора - интерфейс = 4
		1, // количество интерфейсов
		0, // альтернативная установка (не понял для чего)
		1, // кол-во точек используемых интерфейсом
		0, // код класса
		0, // код подкласса
		0, // код протокола
		0, // индекс строки описания интерфейса
		};

const uint8_t usb_endpoint_descriptor[] = { 7, // размер дескриптора (байт)
		5, // тип дескриптора - конечная точка = 5
		0x81, // адрес и направление конечной точки (IN 0x81)
		0x03, // аттрибут - тип передач прерывание
		8, 0, // максимальный размер пакета, для LS 8 байт
		10 // интервал опроса конечной точки в фреймах (для LS - 1 мс)
		};

enum usb_events_list
{
	USB_EVENT_NO = 0, //
	USB_EVENT_RECEIVE_SETUP_DATA = 1, // приняты данные токена SETUP
	USB_EVENT_READY_DATA_IN = 2, // готовы данные для передачи в ПК
	USB_EVENT_WAIT_DATA_IN = 3, // ожидание готовности данных для передачи в ПК
	USB_EVENT
};

enum usb_states_list
{
	USB_STATE_IDLE = 0, //
	USB_STATE_SETUP = 1,
	USB_STATE_IN = 2,
	USB_STATE_OUT = 3,
	USB_STATE_DATA = 4,
	USB_STATE_ADDRESS_CHANGE = 5
};

enum usb_packets_id_list
{
	USB_PID_SETUP = 0x2D, //
	USB_PID_DATA0 = 0xC3,
	USB_PID_DATA1 = 0x4B,
	USB_PID_IN = 0x69, // данные для ПК
	USB_PID_OUT = 0xE1, // данные от ПК
	USB_PID_ACK = 0xD2,
	USB_PID_NACK = 0x5A,
	USB_PID_STALL = 0x1E
};

enum usb_request_standart_type_list
{
	USB_REQUEST_TYPE_TO_DEVICE = 0x00, //
	USB_REQUEST_TYPE_TO_INTERFACE = 0x01, //
	USB_REQUEST_TYPE_TO_ENDPOINT = 0x02, //
	USB_REQUEST_TYPE_FROM_DEVICE = 0x80, //
	USB_REQUEST_TYPE_FROM_INTERFACE = 0x81, //
	USB_REQUEST_TYPE_FROM_ENDPOINT = 0x82
};

enum usb_request_list
{
	USB_REQUEST_GET_STATUS = 0x00, //
	USB_REQUEST_SET_ADDRESS = 0x05,
	USB_REQUEST_GET_DESCRIPTOR = 0x06,
	USB_REQUEST_SET_DESCRIPTOR = 0x07,
	USB_REQUEST_GET_CONFIGURATION = 0x08,
	USB_REQUEST_SET_CONFIGURATION = 0x09
};

struct usb_type
{
	volatile uint8_t state;
	volatile uint8_t event;
	volatile uint8_t device_address;

	uint8_t endpoint_number;
	uint8_t setup_address;
	uint8_t setup_endpoint;

	uint8_t rx_buffer[16];
	uint8_t rx_lenght;

	uint8_t tx_buffer[16];
	uint8_t tx_lenght;
	uint8_t tx_is_all;
} usb;

void usb_init(void)
{
	usb.state = USB_STATE_IDLE;
	usb.event = USB_EVENT_NO;
	usb.device_address = 0;
	usb.tx_is_all = TRUE;
}

void usb_send_nack(void)
{
	uint8_t data[2];

	GPIOC->ODR = 0x80;
	GPIOC->CR1 = 0xFF;
	GPIOC->CR2 = 0xFF;
	GPIOC->DDR = 0xFF;

	data[0] = 0x80;
	data[1] = USB_PID_NACK;

	usb_tx_count = 2;
	usb_tx_buffer_pointer = &data[0];

	usb_tx();

	nop();
	nop();
	nop();
	nop();
	nop();
	nop();

	GPIOC->CR2 = 0x00;
	GPIOC->CR1 = 0x00;
	GPIOC->DDR = 0x3F;
}

void usb_send_ack(void)
{
	uint8_t data[2];

	GPIOC->ODR = 0x80;
	GPIOC->CR1 = 0xFF;
	GPIOC->CR2 = 0xFF;
	GPIOC->DDR = 0xFF;

	data[0] = 0x80;
	data[1] = USB_PID_ACK;

	usb_tx_count = 2;
	usb_tx_buffer_pointer = &data[0];

	usb_tx();

	nop();
	nop();
	nop();
	nop();
	nop();
	nop();

	GPIOC->CR2 = 0x00;
	GPIOC->CR1 = 0x00;
	GPIOC->DDR = 0x3F;
}

uint8_t count = 0;

@inline void usb_send_answer(void)
{
	GPIOC->ODR = 0x80;
	GPIOC->CR1 = 0xFF;
	GPIOC->CR2 = 0xFF;
	GPIOC->DDR = 0xFF;

	usb_tx_count = usb.tx_lenght;
	usb_tx_buffer_pointer = &usb.tx_buffer[0];

	usb_tx();

	nop();
	nop();
	nop();
	nop();
	nop();
	nop();

	GPIOC->CR2 = 0x00;
	GPIOC->CR1 = 0x00;
	GPIOC->DDR = 0x3F;
}

void usb_rx_ok(void)
{
	switch (usb_rx_buffer[1])
	{
		case (USB_PID_SETUP):
		{
			usb.state = USB_STATE_SETUP;

			break;
		}
		case (USB_PID_OUT):
		{
			usb.state = USB_STATE_OUT;

			break;
		}
		case (USB_PID_IN):
		{
			if (usb.event == USB_EVENT_READY_DATA_IN)
			{
				usb_send_answer();

				usb.event = USB_EVENT_WAIT_DATA_IN;
			}
			else
			{
				usb_send_nack();
			}

			usb.state = USB_STATE_IN;

			break;
		}
		case (USB_PID_DATA0):
		{
			if (usb.state == USB_STATE_SETUP)
			{
				usb.rx_buffer[0] = usb_rx_buffer[0];
				usb.rx_buffer[1] = usb_rx_buffer[1];
				usb.rx_buffer[2] = usb_rx_buffer[2];
				usb.rx_buffer[3] = usb_rx_buffer[3];
				usb.rx_buffer[4] = usb_rx_buffer[4];
				usb.rx_buffer[5] = usb_rx_buffer[5];
				usb.rx_buffer[6] = usb_rx_buffer[6];
				usb.rx_buffer[7] = usb_rx_buffer[7];
				usb.rx_buffer[8] = usb_rx_buffer[8];
				usb.rx_buffer[9] = usb_rx_buffer[9];
				usb.rx_buffer[10] = usb_rx_buffer[10];
				usb.rx_buffer[11] = usb_rx_buffer[11];
				usb.rx_buffer[12] = usb_rx_buffer[12];
				usb.rx_buffer[13] = usb_rx_buffer[13];
				usb.rx_buffer[14] = usb_rx_buffer[14];
				usb.rx_buffer[15] = usb_rx_buffer[15];
				
				usb_send_ack();

				usb.event = USB_EVENT_RECEIVE_SETUP_DATA;
			}
			else if (usb.state == USB_STATE_OUT)
			{
				usb_send_ack();

				usb.event == USB_EVENT_NO;
			}

			break;
		}
		case (USB_PID_DATA1):
		{
			if (usb.state == USB_STATE_OUT)
			{
				usb_send_ack();

				usb.event == USB_EVENT_NO;
			}

			break;
		}
		case (USB_PID_ACK):
		{
			break;
		}
		case (USB_PID_NACK):
		{
			break;
		}
		default:
		{
			usb.state = USB_STATE_IDLE;

			break;
		}
	}
}

void usb_calc_crc16(uint8_t * buffer, uint8_t lenght)
{
	uint16_t crc = 0xFFFF;
	uint8_t index;
	uint8_t i;

	for (index = 0; index < lenght; index++)
	{
		crc ^= *buffer++;

		for (i = 8; i--;)
		{
			if ((crc & BIT(0)) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	crc = ~crc;

	*buffer++ = (uint8_t) crc;
	*buffer = (uint8_t) (crc >> 8);
}

void usb_send_data(uint8_t * buffer, uint8_t lenght)
{
	uint8_t data_sync = USB_PID_DATA1;
	uint8_t index;

	while (lenght > 0)
	{
		if (lenght >= 8)
		{
			usb.tx_lenght = 12;

			usb.tx_buffer[0] = 0x80;
			usb.tx_buffer[1] = data_sync;

			for (index = 2; index < 10; index++)
				usb.tx_buffer[index] = *buffer++;

			lenght -= 8;
		}
		else
		{
			usb.tx_lenght = (uint8_t) (4 + lenght);

			usb.tx_buffer[0] = 0x80;
			usb.tx_buffer[1] = data_sync;

			for (index = 2; index < 2 + lenght; index++)
				usb.tx_buffer[index] = *buffer++;

			lenght = 0;
		}

		// расчитываем контрольную сумму пакета
		usb_calc_crc16(&usb.tx_buffer[2], (uint8_t) (usb.tx_lenght - 4));

		// обновляем пакет синхронизации
		if (data_sync == USB_PID_DATA1)
			data_sync = USB_PID_DATA0;
		else
			data_sync = USB_PID_DATA1;

		// сообщаем о готовности данных
		usb.event = USB_EVENT_READY_DATA_IN;

		while (usb.event == USB_EVENT_READY_DATA_IN)
		{
			if (usb.state != USB_STATE_IN)
				return;
		}
	}
}

uint8_t counter_a = 0;
uint8_t counter_b = 0;
uint8_t counter_all = 0;

uint8_t tx_counter = 0;

void usb_process(void)
{
	if (usb.event == USB_EVENT_RECEIVE_SETUP_DATA)
	{
		switch (usb.rx_buffer[2])
		{
			case (USB_REQUEST_TYPE_FROM_DEVICE):
			{
				switch (usb.rx_buffer[3])
				{
					case (USB_REQUEST_GET_DESCRIPTOR):
					{
						if (usb.rx_buffer[5] == 1)
							usb_send_data(&usb_device_descriptor[0], ARRAY_LENGHT(usb_device_descriptor));
						else if (usb.rx_buffer[5] == 2)
							usb_send_data(&usb_configuration_descriptor[0], ARRAY_LENGHT(usb_configuration_descriptor));

						//if (usb.rx_buffer[5] == 0)
						//		usb_send_data(&usb_device_descriptor[0], ARRAY_LENGHT(usb_device_descriptor));

						//						else if (usb.rx_buffer[5] == 2)
						//							usb_send_data(&usb_interface_descriptor[0], ARRAY_LENGHT(usb_interface_descriptor));

						break;
					}
				}

				break;
			}
			case (USB_REQUEST_TYPE_TO_DEVICE):
			{
				switch (usb.rx_buffer[3])
				{
					case (USB_REQUEST_SET_ADDRESS):
					{
						usb.device_address = usb.rx_buffer[4];

						usb.tx_lenght = 4;
						usb.tx_buffer[0] = 0x80;
						usb.tx_buffer[1] = USB_PID_DATA1;
						usb.tx_buffer[2] = 0;
						usb.tx_buffer[3] = 0;

						usb.event = USB_EVENT_READY_DATA_IN;

						break;
					}
				}

				break;
			}
		}
	}
}
