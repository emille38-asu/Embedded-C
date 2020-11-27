/***************************************
 * tempfsm.c
 *
 * SER486 project 3
 * Fall 2020
 * Written By: Edward Miller
 *
 * This file implements a fsm for hysteresis of temperature readings
 *
 */

 #include "alarm.h"
 #include "led.h"
 #include "tempfsm.h"
 #include "log.h"


 enum state {
     NORM1, /* initial state */
     NORM2, /* state after return to normal from low */
     NORM3, /* state after return to normal from high */
     HIWARN1, /* high state after normal */
     HIWARN2, /* high state after critical high */
     CRITICALHI, /* critical state after hi warn */
     LOWARN1, /* low state after normal */
     LOWARN2, /* low state after critical */
     CRITICALLO /* critical state after low warn */
     };

 static enum state current_state;

 /* macros for led alert */
 #define LED_CRIT (".")
 #define LED_WARN ("-")
 #define LED_NORM (" ")

 /**********************************
 * tempfsm_init()
 *
 * this function initializes the fsm
 *
 * returns:
 *  nothing
 *
 * changes:
 *  the state of the fsm
 *
 */
 void tempfsm_init()
 {
     current_state = NORM1;
     alarm_init();
 }

 /**********************************
 * tempfsm_reset()
 *
 * this function resets the fsm state to NORM1
 *
 * returns:
 *  nothing
 *
 * changes:
 *  the state of the fsm
 *
 */
 void tempfsm_reset()
 {
     current_state = NORM1;
 }

 /**********************************
 * tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn)
 *
 * this function updates the state of the fsm based on current temperature and
 * temperature limits.  Sends alarms and updates the led blink
 *
 * returns:
 *  nothing
 *
 * changes
 *  uart, led, and eventnlogs
 *
 */
 void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn){

     switch(current_state)
     {
         case NORM1:
             {
            if(current >= hiwarn)
                {
                /* go to hiwarn1 */
                current_state = HIWARN1;
                /* alert temp hi */
                alarm_send(EVENT_HI_WARN);
                /* log warning */
                log_add_record(EVENT_HI_WARN);
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else if(current <= lowarn)
            {
                /* go to lowarn1 */
                current_state = LOWARN1;
                /* alert temp low */
                alarm_send(EVENT_LO_WARN);
                /* log warning */
                log_add_record(EVENT_LO_WARN);
                /* set led warn */
                led_set_blink(LED_WARN);
            }
         }
         break;

         case NORM2:
             {
            if(current >= hiwarn)
                {
                /* go to hiwarn1 */
                current_state = HIWARN1;
                /* alert temp hi */
                alarm_send(EVENT_HI_WARN);
                /* log warning */
                log_add_record(EVENT_HI_WARN);
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else if(current <= lowarn)
            {
                /* go to lowarn1 */
                current_state = LOWARN1;
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else
            {
                /* set led norm */
                led_set_blink(LED_NORM);
            }
         }
         break;

         case NORM3:
             {
            if(current <= lowarn)
                {
                /* go to lowarn1 */
                current_state = LOWARN1;
                /* alert temp lo */
                alarm_send(EVENT_LO_WARN);
                /* log warning */
                log_add_record(EVENT_LO_WARN);
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else if(current >= hiwarn)
            {
                /* go to hiwarn1 */
                current_state = HIWARN1;
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else
            {
                /* set led norm */
                led_set_blink(LED_NORM);
            }
         }
         break;

         case HIWARN1:
             {
            if(current >= hicrit)
                {
                /* go to hicrit */
                current_state = CRITICALHI;
                /* alarm hi */
                alarm_send(EVENT_HI_ALARM);
                /* log alarm */
                log_add_record(EVENT_HI_ALARM);
                /* set led crit */
                led_set_blink(LED_CRIT);
            }
            else if(current < hiwarn)
            {
                /* go to norm3 */
                current_state = NORM3;
                /* set led norm */
                led_set_blink(LED_NORM);
            }
            else
            {
                /* set led norm */
                led_set_blink(LED_NORM);
            }
         }
         break;

         case HIWARN2:
             {
            if(current >= hicrit)
                {
                /* go to hicrit */
                current_state = CRITICALHI;
                /* set led crit */
                led_set_blink(LED_CRIT);
            }
            else if(current < hiwarn)
            {
                /* go to norm3 */
                current_state = NORM3;
                /* set led norm */
                led_set_blink(LED_NORM);
            }
            else
            {
                /* set led norm */
                led_set_blink(LED_NORM);
            }
         }
         break;

         case CRITICALHI:
             {
             if(current < hicrit)
                {
                /* go to hiwarn2 */
                current_state = HIWARN2;
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else
            {
                /* set led crit */
                led_set_blink(LED_CRIT);
            }
         }
         break;

         case LOWARN1:
             {
             if(current <= locrit)
                {
                /* go to locrit */
                current_state = CRITICALLO;
                /* alarm lo */
                alarm_send(EVENT_LO_ALARM);
                /* log warning */
                log_add_record(EVENT_LO_ALARM);
                /* set led crit */
                led_set_blink(LED_CRIT);
            }
            else if(current > lowarn)
                {
                /* go to norm2 */
                current_state = NORM2;
                /* set led norm */
                led_set_blink(LED_NORM);
            }
            else
            {
                /* set led warn */
                led_set_blink(LED_WARN);
            }
         }
         break;

         case LOWARN2:
             {
            if(current < locrit)
                {
                /* go to LOcrit */
                current_state = CRITICALLO;
                /* set led crit */
                led_set_blink(LED_CRIT);
            }
            else if(current > lowarn)
            {
                /* go to norm3 */
                current_state = NORM3;
                /* set led norm */
                led_set_blink(LED_NORM);
            }
            else
            {
                /* set led warn */
                led_set_blink(LED_WARN);
            }
         }
         break;

         case CRITICALLO:
             {
            if(current > locrit)
                {
                /* go to lowarn2 */
                current_state = LOWARN2;
                /* set led warn */
                led_set_blink(LED_WARN);
            }
            else
            {
                /* set led crit */
                led_set_blink(LED_CRIT);
            }
         }
         break;
     }
 }
