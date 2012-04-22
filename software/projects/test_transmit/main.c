#include "bugOne.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


#include "avr_compat.h"
#include "uart.h"
#include "config.h"

#include "rfm12.h"

uint8_t size = 0, count = 0;
ISR(TIMER0_OVF_vect) {
	size = 0;
	count =0;
}

int main ( void )
{
_delay_ms(10000);
	uint8_t teststring[] = "HelloWorld\r\n";
	uint8_t packettype = 0xEE;
	uint8_t *rx_buffer;

	rfm12_init();
  	uart_init();
	
	drive(LED1);
	drive(LED2);

        set_output(LED1);
	set_output(LED2);
        _delay_ms(250);
	clr_output(LED1);
	clr_output(LED2);

        uart_putstr_P(PSTR("AVR init complete\r\n"));



	while (1) {	

		uart_putstr_P(PSTR("RFM12 RX Status :\r\n"));
		switch (rfm12_rx_status())
			{
				case STATUS_FREE:
					uart_putstr_P(PSTR("STATUS_FREE\r\n"));
					break;
				case STATUS_OCCUPIED:
					uart_putstr_P(PSTR("STATUS_OCCUPIED\r\n"));
					break;
				case STATUS_COMPLETE:
					uart_putstr_P(PSTR("STATUS_COMPLETE\r\n"));
					break;
				default:
					uart_putstr_P(PSTR("?????????????????\r\n"));
			}

		uart_putstr_P(PSTR("RFM12 TX Status :\r\n"));
		switch (rfm12_tx(sizeof(teststring), packettype, teststring))
			{
				case RFM12_TX_ERROR:
					uart_putstr_P(PSTR("RFM12_TX_ERROR\r\n"));
					break;
				case RFM12_TX_OCCUPIED:
					uart_putstr_P(PSTR("RFM12_TX_OCCUPIED\r\n"));
					break;
				case RFM12_TX_ENQUEUED:
					uart_putstr_P(PSTR("RFM12_TX_ENQUEUED\r\n"));
					break;
				default:
					uart_putstr_P(PSTR("?????????????????\r\n"));
			}

		rfm12_tick();
			set_output(LED1);
			set_output(LED2);
        		_delay_ms(250);
			clr_output(LED2);
			clr_output(LED1);
			_delay_ms(250);

	}

}
