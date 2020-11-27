/***************************************
 * wdt.c
 *
 * SER486 project 3
 * Fall 2020
 * Written By: Edward Miller
 *
 * this file contains the watchdog timer
 *
 */

 #include "log.h"
 #include "config.h"
 #include "wdt.h"
 #include "led.h"

 #define WDTCSR (*((volatile unsigned char*) 0x60))
 #define SREG (*((volatile unsigned char*) 0x5F ))

 /* timer macros */
 #define WDIE (6)
 #define WDCE (4)
 #define WDE (3)
 #define WDP2 (2)
 #define WDP1 (1)
 #define WDP0 (0)

 static void wdt_config();

 /**********************************
 * wdt_init()
 *
 * this function initializes the watchdog timer. At timeout the timer will generate an
 * interrupt and a system reset. wdt_reset will be called
 * once every cycle to avoid a timeout
 *
 * returns:
 *  nothing
 *
 * changes:
 *  enables watchdog timer
 *
 * NOTE:
 * 2s timeout is default
 *
 */
 void wdt_init()
 {
     /* disable interrupts */
     unsigned char sreg_state = SREG;
     __builtin_avr_cli();

     /* reset wdt */
     wdt_reset();

     /* configure WDTCSR */
     wdt_config();

     /* re-enable interrupts */
     SREG = sreg_state | (1<<7);
 }

 /**********************************
 * wdt_config()
 *
 * this function
 *
 * returns:
 *  nothing
 *
 * changes:
 *  enables watchdog timer via WDTCSR
 *
 * NOTE:
 *  2s timeout is default
 *
 */

 #pragma GCC push_options
 #pragma GCC optimize ("Os")
 static void wdt_config()
 {
     /* in same operation write 1 to WDCE and WDE */
     WDTCSR = (1 << WDCE) | (1 << WDE);

     /* within four cycles, write WDE, and WDP as desired */
     WDTCSR = 1 << WDIE | 1 << WDE | 1 << WDP2 | 1 << WDP1 | 1 << WDP0;
 }
 #pragma GCC pop_options

 /**********************************
 * wdt_reset()
 *
 * this function resets the watchdog timer to prevent a timeout
 *
 * returns:
 *  nothing
 *
 * changes:
 *  resets timer
 *
 */
 void wdt_reset()
 {
    __builtin_avr_wdr();
 }

 /**********************************
 * wdt_force_restart()
 *
 * this function disables the watchdog interrupt and waits for the watchdog to generate a
 * system reset
 *
 * returns:
 *  nothing
 *
 * changes:
 *  forces a system restart
 *
 */
 void wdt_force_restart()
 {
     /* disable watchdog interrupt */
     WDTCSR &= (0 << WDIE);

     /* block until WDT times out */
     while(1);
 }

 /**********************************
 * __vector_6(void)
 *
 * the ISR attempts to a graceful shutdown after a watchdog timeout.
 *
 * returns:
 *  nothing
 *
 * changes:
 *  led, EEPROM, log data, and begins system restart
 *
 */
 void __vector_6(void) __attribute__ ((signal,used, externally_visible));
 void __vector_6()
 {
     /* turn on LED */
     led_on();

     /* add EVENT_WDT to system event log */
     log_add_record(EVENT_WDT);

     /* write back log and config without isr */
     for(int i = 0; i < 16; i++)
    {
        log_update_noisr();
    }
     config_update_noisr();
 }
