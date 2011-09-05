#include "bugOne.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>
#include <stdlib.h>

#include "avr_compat.h"
#include "rfm12.h"
#include "uart.h"
#include "config.h"

#include "ping.h"


int main ( void )
{
  uint8_t *bufcontents;
  uint16_t ticker = 0;
  char buf[4];

  drive(LED1);
  drive(LED2);

  set_output(LED1);
  set_output(LED2);
	
  uart_init();

  uart_putstr_P(PSTR("Firmware version "));
  uart_putstr_P(PSTR(FWVERSION));
  uart_putstr_P(PSTR("\r\n"));

  config_init();

  uart_putstr_P(PSTR("Node address : "));
  itoa(config.address,buf,10);
  uart_putstr(buf);
  uart_putstr_P(PSTR("\r\n"));

  _delay_ms(250);
  rfm12_init();
  _delay_ms(250);
  //rfm12_set_wakeup_timer(100);	

  sei();

  clr_output(LED1);
  clr_output(LED2);

  uart_putstr_P(PSTR("AVR init complete\r\n"));

  while (1) {
      ticker++;

      rfm12_tick();

      if (rfm12_rx_status() == STATUS_COMPLETE) {
	bufcontents=rfm12_rx_buffer();
	recv(bufcontents);
      }

      if (ticker > 50000) {
	ticker=0;
	send_ping(1);
        toggle_output(LED2);
      }
  }
}
