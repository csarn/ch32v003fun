// Could be defined here, or in the processor defines.
#define SYSTEM_CORE_CLOCK 48000000
#define APB_CLOCK SYSTEM_CORE_CLOCK

#include "ch32v00x.h"
#include <stdio.h>
#include <string.h>

#define WS2812DMA_IMPLEMENTATION

#include "ws2812b_dma_spi_led_driver.h"

#include "color_utilities.h"

#define NR_LEDS 191

uint16_t phases[NR_LEDS];
int frameno;
volatile int tween = 0;

// Callbacks that you must implement.
uint32_t WS2812BLEDCallback( int ledno )
{
	uint8_t index = (phases[ledno])>>8;
	uint8_t rsbase = sintable[index];
	uint8_t rs = rsbase>>3;
	uint32_t fire = huetable[(rs+190&0xff)] | (huetable[(rs+30&0xff)]<<8) | (huetable[(rs+0)]<<16);
	uint32_t ice  = 0xff | ((rsbase)<<8) | ((rsbase)<<16);

	// Because this chip doesn't natively support multiplies, this can be very slow.
	return TweenHexColors( fire, ice, tween ); // Where "tween" is a value from 0 ... 255
}

int main()
{
	int k;
	SystemInit48HSI();

	// Enable GPIOD (for debugging)
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
	GPIOD->CFGLR &= ~(0xf<<(4*0));
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*0);

	GPIOD->BSHR = 1;	 // Turn on GPIOD0
	WS2812BDMAInit( );

	frameno = 0;

	for( k = 0; k < NR_LEDS; k++ ) phases[k] = k<<8;


	int tweendir = 0;

	while(1)
	{
	
		GPIOD->BSHR = 1;	 // Turn on GPIOD0
		// Wait for LEDs to totally finish.
		Delay_Ms( 12 );
		GPIOD->BSHR = 1<<16; // Turn it off

		while( WS2812BLEDInUse );

		frameno++;

		if( frameno == 1024 )
		{
			tweendir = 4;
		}
		if( frameno == 2048 )
		{
			tweendir = -4;
			frameno = 0;
		}

		if( tweendir )
		{
			int t = tween + tweendir;
			if( t > 255 ) t = 255;
			if( t < 0 ) t = 0;
			tween = t;
		}

		for( k = 0; k < NR_LEDS; k++ )
		{
			phases[k] += ((((rands[k&0xff])+0xf)<<2) + (((rands[k&0xff])+0xf)<<1))>>1;
		}

		WS2812BDMAStart( NR_LEDS );
	}
}

