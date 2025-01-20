/*
 * Attiny1624 SUN sensor v2.c
 *
 * Created: 2024-10-11 21:00:42
 * Author : Saulius
 */ 
#include "Settings.h"
int main(void){
	GPIO_init();
	ADC0_init();
	USART0_init();
	RTC_init();
//float debuganswer = 0;
	sei();
	while (1){

		ReadSensorsValues();
		for (uint8_t i = 0; i<5; i++){
			azel(i);
			//azimuth(i);
		}
		//azel(4);

		azimuth_average_angle(1);

		//moving_average2();		
	/*	debuganswer = GetCoeficients(17);
	printf("koeficientas: %.5f\r\n", debuganswer);*/
	printf(" %4d-%02d-%02d %02d:%02d:%02d: ", currentTime.year,  currentTime.month, currentTime.day, currentTime.hours, currentTime.minutes, currentTime.seconds);
	printf("Az.: % 5.2f | El.: % 5.2f | Lv.: % 5d |", AZIMUTH.total_average_angle, fifth.top_angle, level());

	solar_params.second = currentTime.seconds;
	solar_params.minute = currentTime.minutes;
	solar_params.hour = currentTime.hours;
	solar_params.day = currentTime.day;
	solar_params.month = currentTime.month;
	solar_params.year = currentTime.year;

	calculate_solar_position(&solar_params, &ELEVATION.cosmos_angle, &AZIMUTH.cosmos_angle);

	printf("AzC.: % 5.2f | ElC.: % 5.2f\r\n", AZIMUTH.cosmos_angle, ELEVATION.cosmos_angle);
	}
	
}