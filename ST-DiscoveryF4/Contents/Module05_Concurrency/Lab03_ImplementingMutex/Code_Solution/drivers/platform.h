#ifndef PLATFORM_H
#define PLATFORM_H

#include <STM32F4xx.h>

typedef enum {
	PA0  = (0 << 16) |  0,//User Switch
	PA1  = (0 << 16) |  1,//ADC1
	PD12 = (3 << 16) | 12,//Green LED
	PD13 = (3 << 16) | 13,//Orange LED
	PD14 = (3 << 16) | 14,//Red LED
	PD15 = (3 << 16) | 15,//Blue LED
} Pin;

#define LED_NUM 4

#define LED_ON 1
#define LED_OFF 0

static Pin LED[LED_NUM]={PD12,PD13,PD14,PD15};
static Pin SWITCH = PA0;

#define RED 2
#define GREEN 0
#define BLUE 3
#define ORANGE 1

#define CLK_FREQ 168000000UL

#define GET_PORT_INDEX(pin) ((pin) >> 16)
#define GET_PIN_INDEX(pin) ((pin) & 0xFF)

#define GET_PORT(pin) ((GPIO_TypeDef*)(AHB1PERIPH_BASE + 0x0400 * GET_PORT_INDEX(pin)))

#endif

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
