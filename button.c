/*
 * File:   keys.c
 * Author: M71906
 *
 * Created on October 14, 2022, 12:49 PM
 */
#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/delay.h"
#include "rtcounter.h"
#include "button.h"

#define DEBOUNCE 100

void initKeys(void) {
    // Using 'melody' these lines are not needed 
    IO_PC4_SetDigitalInput();
    IO_PC4_SetPullUp();
}

bool getKey(void)
{
    if(PORTC_get_pin_level(4) == 0)
    {
        DELAY_milliseconds(50);
        while(PORTC_get_pin_level(4) == 0);
        DELAY_milliseconds(50);
        return true;
    }
    return false;
}
