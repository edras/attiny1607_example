/*
 * File:   user.c
 * Author: M71906
 *
 * Created on 12. Oktober 2022, 14:37
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "user.h"
#include "blink.h"
#include "rtcounter.h"

uint16_t adc_timer = 0;
#define ADC0_time 1000;

void startAdc0(void)
{
    ADC0.COMMAND = 1;
    adc_timer = getMilliseconds() + ADC0_time;
}

void handleAdc0(void)
{
    if(ADC0.COMMAND == 0)
    {
        uint16_t currentMillis = getMilliseconds();
        if(adc_timer == currentMillis)
        {
            adc_timer = currentMillis + ADC0_time;

            int8_t sigrow_offset = SIGROW.TEMPSENSE1;  /* Read signed offset from signature row */
            uint8_t sigrow_gain = SIGROW.TEMPSENSE0;
            uint16_t adc_reading = ADC0.RES;
            uint32_t temp = adc_reading - sigrow_offset;
            temp *= sigrow_gain; /* Result might overflow 16-bit variable (10-bit + 8-bit) */
            temp += 0x80; /* Add 256/2 to get correct integer rounding on division below */
            temp >>= 8; /* Divide result by 256 to get processed temperature in Kelvin */
            int16_t temp_in_degC = temp - 273;
            
            USART0_Write(0x5F);
            USART0_Write(temp_in_degC & 0xFF);
            USART0_Write((temp_in_degC >> 8) & 0xFF);        
            USART0_Write(0xA0);        
            
            ADC0.COMMAND = 1;
        }
    }
}

/**
 * handleRX handle the commands received by serial interface
 */
void handleRx(void)
{
    if(USART0_IsRxReady())
    {
        setBlinkEnable(false);
        switch(USART0_Read())
        {
            case 'T': IO_PB7_Toggle();      break;
            case 'B': setBlinkEnable(true); break;
            case '1': IO_PB7_SetLow();      break;
            case '0': IO_PB7_SetHigh();     break;                
        }
    }
}

void stepPWM(void)
{
    uint16_t period = TCA0.SINGLE.PER;
    uint16_t pwm_step = period / 10;
    uint16_t pwm_temp = TCA0.SINGLE.CMP1 + pwm_step;
    if(pwm_temp > period) pwm_temp = pwm_step;
    TCA0.SINGLE.CMP1BUF = pwm_temp;
}
