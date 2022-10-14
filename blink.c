/*
 * File:   blink.c
 * Author: M71906
 *
 * Created on October 12, 2022, 4:47 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "rtcounter.h"
#include "blink.h"

bool blinkEnable = false;
bool blinkState = false;
uint16_t blinkTime = 0;

#define BLINK_ON  100
#define BLINK_OFF 1000

bool isBlinkOn(void)
{
    return blinkState;
}

void setBlinkState(bool newState)
{
    blinkState = newState;
    if(blinkState)
        IO_PB7_SetLow();
    else
        IO_PB7_SetHigh();
}

void setBlinkEnable(bool enable)
{
    blinkEnable = enable;
    blinkTime = getMilliseconds() + BLINK_OFF;       
}

void handleBlink(void)
{
    if(!blinkEnable) return;
    uint16_t currentMillis = getMilliseconds();
    if(currentMillis == blinkTime)
    {
        if(isBlinkOn())
        {
            blinkTime = currentMillis + BLINK_OFF;            
            setBlinkState(false);
        }
        else
        {
            blinkTime = currentMillis + BLINK_ON;
            setBlinkState(true);
        }    
    }
}