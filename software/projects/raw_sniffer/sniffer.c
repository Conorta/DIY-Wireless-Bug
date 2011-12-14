#include "bugOne.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

#include "avr_compat.h"
#include "rfm12.h"
#include "uart.h"

int main(void) {
	uint8_t *rx_buffer;
	uint8_t  tx_buffer[256]; // Not RFM12_TX_BUFFER_SIZE since we don't know what will be sent on uart
	uint8_t c;
	uint8_t size = 0, count = 0;

	uart_init();
	rfm12_init();

	sei();
	drive(LED1);
	drive(LED2);

	clr_output(LED1);
	clr_output(LED2);

	while (1) {
		if (rfm12_rx_status() == STATUS_COMPLETE) {
			rx_buffer = rfm12_rx_buffer();
			uint8_t length = rfm12_rx_len();
			uint8_t checksum = 0x00;
			uart_putc(length);
			while (length--) {
				uint8_t data = *rx_buffer++;
				uart_putc(data);
				checksum ^= data;
			}
			toggle_output(LED2);
			uart_putc(checksum);
			rfm12_rx_clear();
		}

		#if !(RFM12_TRANSMIT_ONLY)
		if (uart_getc_nb(&c) == 1) {
			if (size == 0) {
				size = c;
				count = 0;
			} else {
				tx_buffer[count] = c;
				if (count == size) {
					rfm12_tx(size, 0 /*type*/, tx_buffer);
					toggle_output(LED1);
					size = count = 0;
				} else {
					count++;
				}
			}
		}
		#endif

		rfm12_tick();
	}

}
