/*
 * ADCexample.c
 *
 * Created: 15/05/17
 * Author: NS
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "STDIO_UART.h"

//double dutyCycle = 0;
void setupADC();
void startConversion();
int ExtraTime = 0;

int main(void)
{
	ioinit();
	
	TCCR0A = (1 << WGM01);
	OCR0A = 195;
	TIMSK0 = (1 << OCIE0A);
			
	sei();
	
	setupADC();
	
	TCCR0B = (1 << CS00) | (1 << CS02); //start at prescaler 1024
	
	while(1)
	{
	}

}

void setupADC()
{
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2); //use 5V reference voltage, input from 
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //enable ADC, enable ADC interrupt, ADC prescaler=128 (8000000/128=62.5KHz)
	DIDR0 = (1 << ADC5D); //turn off digital input buffer on pin 28
	
	startConversion();
}

void startConversion()
{
	ADCSRA |= (1 << ADSC); //starts a conversion, cleared when ADC interrupt is fired
}

ISR(TIMER0_COMPA_vect)
{
	ExtraTime++;
	if (ExtraTime > 1) //increase compare value to decrease sample rate
	{
		startConversion();
		ExtraTime = 0;
	}
}

ISR(ADC_vect) //fires when a conversion is done
{
	printf("conversion result is %d \n", ADC);
}