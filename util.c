/***************************************
 * util.c
 *
 * SER486 project 3
 * Fall 2020
 * Written By: Edward Miller
 *
 * this file includes functionality for updating the warning and alarm ranges of
 * the temperature sensor
 *
 */

 #include "config.h"
 #include "util.h"


 config_struct config;

 /**********************************
 * update_tcrit_hi(int value)
 *
 * this function sets the threshold value of the high temperature alarms for
 * the ATMEGA 328P
 *
 * returns:
 * 0 if temperature is successfully set, 1 if not
 *
 * changes
 *  the threshold value of the high temperature alarm
 *
 */
 int update_tcrit_hi(int value)
{

    int ret = 0;

    /* ensure new value is above hi_warn */
    if (value > config.hi_warn)
    {
        config.hi_alarm = value;
        config_set_modified();
    }
    else
    {
        ret = 1;
    }

    return ret;
 }

  /**********************************
 * update_twarn_hi(int value)
 *
 * this function sets the threshold value of the high temperature warning for
 * the ATMEGA 328P
 *
 * returns:
 *  0 or 1
 *
 * changes
 *  the threshold value of the high temperature warning
 *
 */
 int update_twarn_hi(int value)
 {
    int ret = 0;

    /* ensure new value is below hi_alarm and above lo_warn */
    if (value < config.hi_alarm && value > config.lo_warn)
    {
        config.hi_warn = value;
        config_set_modified();
    }
    else
    {
        ret = 1;
    }
    return ret;
 }

 /**********************************
 * update_tcrit_lo(int value)
 *
 * this function sets the threshold
 *
 * returns:
 *  0 or 1
 *
 * changes
 *  the threshold value of the low temperature warning
 *
 */
 int update_tcrit_lo(int value)
 {
    int ret = 0;

    /* ensure new value is below lo_alarm */
    if (value < config.lo_alarm)
    {
        config.lo_alarm = value;
        config_set_modified();
    }
    else
    {
        ret = 1;
    }
    return ret;
 }

  /**********************************
 * update_twarn_lo(int value)
 *
 * this function
 *
 * returns:
 *  0 or 1
 *
 * changes
 *  the threshold value of the low temperature alarm
 *
 */
 int update_twarn_lo(int value)
 {
    int ret = 0;

    /* ensure new value is below hi_warn and above lo_alarm */
    if (value < config.hi_warn && value > config.lo_alarm) {
        config.lo_warn = value;
        config_set_modified();
    }
    else
    {
        ret = 1;
    }
    return ret;
 }
