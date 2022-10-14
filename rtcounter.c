/*
 * File:   time.c
 * Author: M71906
 *
 * Created on October 12, 2022, 4:04 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "rtcounter.h"

uint16_t milliseconds = 0;

void callBack(void)
{
    milliseconds++;
}

void initTime(void)
{
    RTC_SetOVFIsrCallback(callBack);
}

uint16_t getMilliseconds(void)
{
    return milliseconds;
}
