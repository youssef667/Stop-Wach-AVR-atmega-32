/*
 * stopwatchproject.c
 *
 *  Created on: Sep 17, 2021
 *      Author: youssef
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char tick=0;
unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;
void timer_Init()
{
    TCCR1A =(1<<FOC0);
    TCCR1B = (1<<WGM01)|(1<<CS11)|(1<<CS10);
    TCNT1 =0;
    OCR1A = 15625;
    TIMSK|=(1<<OCIE1A);
    SREG=(1<<7);

	}
 ISR (TIMER1_COMPA_vect)

 {
	 second++;
	 if(second ==60)
	 {
		 minute++;
		 second =0;
	 }
	 if(minute==60)
	 {
		 hour++;
		 second =0;
		 minute =0;
	 }

	 if (hour==24)
	 {
		 second =0;
	 minute =0;
		 hour=0;
	 }
 }
 void NT0RESET_Init(void)
 {
 	DDRD &=~(1<<PD2);
 	PORTD|=(1<<PD2);
 	MCUCR|=(1<<ISC01);
 	GICR |=(1<<INT0);
 	SREG |=(1<<7);
 }
 ISR(INT0_vect)
 {
 	second = 0;
 	minute = 0;
 	hour = 0 ;
 }
 void INT1Pause_Init(void)
 {
 	DDRD &= ~(1<<PD3);
 	MCUCR|=(1<<ISC10)|(1<<ISC11);
 	GICR |=(1<<INT1);
 	SREG|=(1<<7);
 }
 ISR(INT1_vect)
 {

 	TCCR1B &= ~(1<<CS10)&~(1<<CS11)&~(1<<CS12);
 }
 void INT2Resume_Init(void)
 {
 	DDRB &=~(1<<PB2);
 	PORTB |= (1<<PB2);
 	MCUCR&=~(1<<ISC2);
 	GICR |=(1<<INT2);
 	SREG|=(1<<7);
 }
 ISR(INT2_vect)
 {
 	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);
 }
int main(void)

{
	timer_Init();
	NT0RESET_Init();
	INT1Pause_Init();
	INT2Resume_Init();
DDRA =0xff;
PORTA = 0xff ;
DDRC= 0x0f;
PORTC = 0x00;

while(1)
{

	PORTA = (1<<5);
			PORTC = second % 10;
			_delay_ms(5);
	PORTA = (1<<4);
			PORTC = second / 10;
			_delay_ms(5);
	PORTA = (1<<3);
			PORTC = minute % 10;
			_delay_ms(5);
	PORTA = (1<<2);
			PORTC = minute / 10;
			_delay_ms(5);
	PORTA = (1<<1);
			PORTC = hour % 10;
			_delay_ms(5);
	PORTA = (1<<0);
			PORTC = hour / 10;
			_delay_ms(5);

	}
}
