/*
 * Settings.h
 *
 * Created: 2024-09-02 21:46:06
 *  Author: Saulius
 */ 

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define F_CPU 20000000
#define TWI0_BAUD(F_SCL, T_RISE) ((((((float)F_CPU / (float)F_SCL)) - 10 - ((float)F_CPU * T_RISE / 1000000))) / 2) //I2C baud calculation
#define TIMEBASE_VALUE ((uint8_t) ceil(F_CPU*0.000001)) //ADC time base calculation
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (8 *(float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include "Sensor32.h"
#include "Cosmos.h"
#include "Filter.h"
#include "RTC.h"

void GPIO_init(); //Attiny1614 CPU speed and Pins settings

void ADC0_init();
uint16_t ADC0_Read(uint8_t ch);
void ReadSensorsValues(); // function for 25 sensors voltage reading

void USART0_init();
void USART0_sendChar(char c);
int USART0_printChar(char c, FILE *stream);
void USART0_sendString(char *str);
char USART0_readChar();

void azimuth(uint8_t Row); // function calculate azimuth using first or second row of sensors (sinus function). Also this function interpolate values betwean each row sensors
//uint16_t average_angle(uint16_t Row0, uint16_t Row1, uint8_t scale); //this function calculate average azimuth according to both rows of sensors and each row weight by sensors count 
void azimuth_average_angle(uint8_t scale);
//uint16_t elevation(); //function whitch one calculate elevation according 3 maximum poins from each sensors rows (square function)
//void elevation2();
//void elevation3();
//void elevation4();
//void elevation5();
void azel(uint8_t Row);
//uint16_t moving_average(uint16_t Data, uint8_t *i, uint8_t ch); //moving average filter which support up to 5 data channels with 80 different values (total 400 values of int32)
void moving_average2();
uint16_t level(); // return averale light level value calculated from all 25 sensors

float GetCoeficients(uint8_t sensor);

void calculate_solar_position(const SolarPositionParameters *params, double *elevation, double *azimuth);

void RTC_init();

#endif /* SETTINGS_H_ */