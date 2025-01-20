/*
 * RTC.h
 *
 * Created: 2024-11-21 13:42:32
 *  Author: Saulius
 */ 


#ifndef RTC_H_
#define RTC_H_

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t month;
	uint16_t year;
} Time;

extern volatile Time currentTime;


#endif /* RTC_H_ */