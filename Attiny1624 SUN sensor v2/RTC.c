/*
 * RTC.c
 *
 * Created: 2024-11-21 13:22:35
 *  Author: Saulius
 */ 
#include "Settings.h"


volatile Time currentTime = {20, 03, 15, 21, 11, 2024}; // Pradinë data ir laikas
// Mënesiø dienø kiekis
const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Tikrinimas, ar metai yra keliamieji
uint8_t isLeapYear(uint16_t year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// RTC inicializavimas
void RTC_init() {
	 while (RTC.STATUS > 0); //wait until RTC synchronyze
	 RTC.CLKSEL = RTC_CLKSEL_INT32K_gc; //32768khz
	 RTC.CTRLA = RTC_RTCEN_bm | RTC_PRESCALER_DIV1_gc; // Ájungti RTC
	 RTC.INTCTRL = 0 << RTC_CMP_bp    /* Compare Match Interrupt enable: disabled */
	 | 1 << RTC_OVF_bp;
	 RTC.PER = 32767;
}

// RTC pertraukimo tvarkytuvas
ISR(RTC_CNT_vect) {
	RTC.INTFLAGS = RTC_OVF_bm; // Iðvalyti pertraukimo vëliavëlæ

	// Atlikti laiko skaièiavimus
	currentTime.seconds++;
	if (currentTime.seconds >= 60) {
		currentTime.seconds = 0;
		currentTime.minutes++;
		if (currentTime.minutes >= 60) {
			currentTime.minutes = 0;
			currentTime.hours++;
			if (currentTime.hours >= 24) {
				currentTime.hours = 0;
				currentTime.day++;
				
				uint8_t daysThisMonth = daysInMonth[currentTime.month - 1];
				if (currentTime.month == 2 && isLeapYear(currentTime.year)) {
					daysThisMonth = 29;
				}

				if (currentTime.day > daysThisMonth) {
					currentTime.day = 1;
					currentTime.month++;
					if (currentTime.month > 12) {
						currentTime.month = 1;
						currentTime.year++;
					}
				}
			}
		}
	}
}