/*
 * Azimuth.c
 *
 * Created: 2024-10-11 23:24:15
 *  Author: Saulius
 */ 
#include "Settings.h"
 #include "Sensor32.h"



void azimuth(uint8_t Row) {

	//uint16_t tarpiniai[360]={0};

	float sum_a0 = 0, sum_an = 0, sum_bn = 0;
	ROW * rows[4] = {&first, &second, &third, &fourth}; //creating pointer array of row objects (to reach variables start, total and angles array)
	//uint16_t sum = 0;
	//float koef[15]={0.0};
	if(Row != 3){
		uint16_t angle = 0, voltage = 0;  // Maximum voltage angle and value
		
		for (uint8_t i = 0; i < rows[Row]->total; i++) {//compute interpolation coefficients according to each row sensors total number
			float angle_rad = rows[Row]->angles[i] * M_PI / 180.0; // Convert angle to radians
			//sum += sensor_data[ i + rows[Row]->start ];
			sum_a0 += sensor_data[ i + rows[Row]->start ];
			sum_an += sensor_data[ i + rows[Row]->start ] * cos(angle_rad);
			sum_bn += sensor_data[ i + rows[Row]->start ] * sin(angle_rad);
		}

		for (uint16_t i = 0; i < 360; i++) {// Calculate interpolated voltages and find maximum value
			float angle_rad = i * M_PI / 180.0; // Convert degrees to radians

			
			uint16_t nowmV = (sum_a0 / rows[Row]->total) +
			(sum_an / (rows[Row]->total / 2)) * cos(angle_rad) +
			(sum_bn / (rows[Row]->total / 2)) * sin(angle_rad);
			//tarpiniai[i] = nowmV;
			if (nowmV > voltage) {
				voltage = nowmV;
				angle = i;
			}
		}
	sensor_top_of_rows[Row] = (int16_t)voltage;// Store maximum voltage value based on row
	rows[Row]->top_angle = angle;
	}		
	else
		sensor_top_of_rows[Row] = (int16_t)sensor_data[rows[Row]->start]; // Store the last sensor value for elevation calculation

		/*for (uint8_t i = 0; i < rows[Row]->total; i++){
			koef[i] = (float)((double)sum/rows[Row]->total)/sensor_data[ i + rows[Row]->start ];
		}
		printf("%3d", sum );
		printf("%3.2f", koef[2] );*/
}

/*
//skaièiavimas pagal laikà idealiai
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)
#define BASE_TIMEZONE_OFFSET 2 // Bazinë laiko juosta: GMT+2 (þiemà)

bool is_daylight_saving_time(int year, int month, int day) {
	int march_last_sunday = 31 - ((5 * year / 4 + 4) % 7);
	int october_last_sunday = 31 - ((5 * year / 4 + 1) % 7);

	if (month > 3 && month < 10) {
		return true;
	}
	if (month == 3 && day >= march_last_sunday) {
		return true;
	}
	if (month == 10 && day < october_last_sunday) {
		return true;
	}
	return false;
}

double calculate_julian_day(int year, int month, int day, int hour, int minute, int second) {
	if (month <= 2) {
		year -= 1;
		month += 12;
	}
	int A = year / 100;
	int B = 2 - A + A / 4;

	double JD = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) +
	day + B - 1524.5 + (hour + minute / 60.0 + second / 3600.0) / 24.0;
	return JD;
}

double calculate_solar_mean_anomaly(double julian_century) {
	return fmod(357.52911 + julian_century * (35999.05029 - 0.0001537 * julian_century), 360.0);
}

double calculate_true_anomaly(double mean_anomaly, double julian_century) {
	double C = (1.914602 - julian_century * (0.004817 + 0.000014 * julian_century)) * sin(DEG_TO_RAD * mean_anomaly) +
	(0.019993 - 0.000101 * julian_century) * sin(2 * DEG_TO_RAD * mean_anomaly) +
	0.000289 * sin(3 * DEG_TO_RAD * mean_anomaly);
	return mean_anomaly + C;
}

double calculate_solar_apparent_longitude(double true_longitude, double julian_century) {
	double omega = 125.04 - 1934.136 * julian_century;
	return true_longitude - 0.00569 - 0.00478 * sin(DEG_TO_RAD * omega);
}

double calculate_obliquity_of_ecliptic(double julian_century) {
	double mean_obliquity = 23.439292 - julian_century * (0.013004167 + 0.0000001639 * julian_century - 0.0000005036 * julian_century * julian_century);
	return mean_obliquity;
}

double calculate_solar_declination(double apparent_longitude, double obliquity) {
	return asin(sin(DEG_TO_RAD * obliquity) * sin(DEG_TO_RAD * apparent_longitude)) * RAD_TO_DEG;
}

double calculate_atmospheric_refraction(double elevation) {
	if (elevation > -1.0) {
		double tan_elevation = tan(DEG_TO_RAD * (elevation + 10.3 / (elevation + 5.11)));
		return 0.0167 / tan_elevation;
	}
	return 0.0;
}

void calculate_solar_position(double latitude, double longitude, int year, int month, int day,
int hour, int minute, int second, double altitude,
double *elevation, double *azimuth) {
	// Nustatyti galutiná laiko juostos poslinká (su vasaros laiku, jei taikoma)
	int timezone_offset = BASE_TIMEZONE_OFFSET + (is_daylight_saving_time(year, month, day) ? 1 : 0);

	double JD = calculate_julian_day(year, month, day, hour - timezone_offset, minute, second);
	double JC = (JD - 2451545.0) / 36525.0;

	double M = calculate_solar_mean_anomaly(JC);
	double true_anomaly = calculate_true_anomaly(M, JC);
	double true_longitude = fmod(280.46646 + JC * (36000.76983 + JC * 0.0003032), 360.0) + (true_anomaly - M);
	double apparent_longitude = calculate_solar_apparent_longitude(true_longitude, JC);
	double obliquity = calculate_obliquity_of_ecliptic(JC);
	double declination = calculate_solar_declination(apparent_longitude, obliquity);

	double eq_time = 4.0 * (280.46646 + 36000.76983 * JC - apparent_longitude +
	(2.466 * sin(2 * DEG_TO_RAD * (280.46646 + 36000.76983 * JC))) -
	(0.053 * sin(4 * DEG_TO_RAD * (280.46646 + 36000.76983 * JC))));
	double solar_time = (hour * 60 + minute + second / 60.0 + eq_time + 4.0 * longitude - 60.0 * timezone_offset) / 60.0;

	double hour_angle = (solar_time - 12.0) * 15.0;

	double latitude_rad = latitude * DEG_TO_RAD;
	double declination_rad = declination * DEG_TO_RAD;
	double hour_angle_rad = hour_angle * DEG_TO_RAD;

	double sin_elevation = sin(latitude_rad) * sin(declination_rad) +
	cos(latitude_rad) * cos(declination_rad) * cos(hour_angle_rad);

	*elevation = asin(sin_elevation) * RAD_TO_DEG;
	*elevation += calculate_atmospheric_refraction(*elevation);

	double cos_azimuth = (sin(declination_rad) - sin(latitude_rad) * sin(DEG_TO_RAD * (*elevation))) /
	(cos(latitude_rad) * cos(DEG_TO_RAD * (*elevation)));
	double azimuth_temp = acos(cos_azimuth) * RAD_TO_DEG;

	if (hour_angle > 0) {
		*azimuth = 360.0 - azimuth_temp;
		} else {
		*azimuth = azimuth_temp;
	}
}

int main() {
	double latitude = 0;
	double longitude = ;
	double altitude = 84.0;
	int year = 2024, month = 10, day = 5;
	int hour = 16, minute = 0, second = 0;

	double elevation, azimuth;

	calculate_solar_position(latitude, longitude, year, month, day, hour, minute, second,
	altitude, &elevation, &azimuth);

	printf("Saulës aukðtis: %.6f°\n", elevation);
	printf("Saulës azimutas: %.6f°\n", azimuth);

	return 0;
}

//////////////////////////////////
/////////////////////////////////
///////////////////////////////////
/////////////////////////////////
su struktûra
//////////////////////////////
//////////////////////////////////
/////////////////////////////////
///////////////////////////////////
/////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)
#define BASE_TIMEZONE_OFFSET 2 // Bazinë laiko juosta: GMT+2 (þiemà)

// Struktûra duomenims apie saulës pozicijos parametrus
typedef struct {
	double latitude;
	double longitude;
	double altitude;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} SolarPositionParameters;

// Patikrina, ar taikomas vasaros laikas
bool is_daylight_saving_time(int year, int month, int day) {
	int march_last_sunday = 31 - ((5 * year / 4 + 4) % 7);
	int october_last_sunday = 31 - ((5 * year / 4 + 1) % 7);

	if (month > 3 && month < 10) {
		return true;
	}
	if (month == 3 && day >= march_last_sunday) {
		return true;
	}
	if (month == 10 && day < october_last_sunday) {
		return true;
	}
	return false;
}

// Skaièiuoja Julian Day
double calculate_julian_day(int year, int month, int day, int hour, int minute, int second) {
	if (month <= 2) {
		year -= 1;
		month += 12;
	}
	int A = year / 100;
	int B = 2 - A + A / 4;

	double JD = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) +
	day + B - 1524.5 + (hour + minute / 60.0 + second / 3600.0) / 24.0;
	return JD;
}

// Skaièiuoja saulës vidutinæ anomalijà
double calculate_solar_mean_anomaly(double julian_century) {
	return fmod(357.52911 + julian_century * (35999.05029 - 0.0001537 * julian_century), 360.0);
}

// Skaièiuoja tikràjà anomalijà
double calculate_true_anomaly(double mean_anomaly, double julian_century) {
	double C = (1.914602 - julian_century * (0.004817 + 0.000014 * julian_century)) * sin(DEG_TO_RAD * mean_anomaly) +
	(0.019993 - 0.000101 * julian_century) * sin(2 * DEG_TO_RAD * mean_anomaly) +
	0.000289 * sin(3 * DEG_TO_RAD * mean_anomaly);
	return mean_anomaly + C;
}

// Skaièiuoja saulës akivaizdø ilgumà
double calculate_solar_apparent_longitude(double true_longitude, double julian_century) {
	double omega = 125.04 - 1934.136 * julian_century;
	return true_longitude - 0.00569 - 0.00478 * sin(DEG_TO_RAD * omega);
}

// Skaièiuoja ekliptikos posvyrá
double calculate_obliquity_of_ecliptic(double julian_century) {
	return 23.439292 - julian_century * (0.013004167 + 0.0000001639 * julian_century - 0.0000005036 * julian_century * julian_century);
}

// Skaièiuoja saulës deklinacijà
double calculate_solar_declination(double apparent_longitude, double obliquity) {
	return asin(sin(DEG_TO_RAD * obliquity) * sin(DEG_TO_RAD * apparent_longitude)) * RAD_TO_DEG;
}

// Atmosferos refrakcijos korekcija
double calculate_atmospheric_refraction(double elevation) {
	if (elevation > -1.0) {
		double tan_elevation = tan(DEG_TO_RAD * (elevation + 10.3 / (elevation + 5.11)));
		return 0.0167 / tan_elevation;
	}
	return 0.0;
}

// Saulës pozicijos skaièiavimas pagal struktûrà
void calculate_solar_position(const SolarPositionParameters *params, double *elevation, double *azimuth) {
	int timezone_offset = BASE_TIMEZONE_OFFSET + (is_daylight_saving_time(params->year, params->month, params->day) ? 1 : 0);

	double JD = calculate_julian_day(params->year, params->month, params->day, params->hour - timezone_offset, params->minute, params->second);
	double JC = (JD - 2451545.0) / 36525.0;

	double M = calculate_solar_mean_anomaly(JC);
	double true_anomaly = calculate_true_anomaly(M, JC);
	double true_longitude = fmod(280.46646 + JC * (36000.76983 + JC * 0.0003032), 360.0) + (true_anomaly - M);
	double apparent_longitude = calculate_solar_apparent_longitude(true_longitude, JC);
	double obliquity = calculate_obliquity_of_ecliptic(JC);
	double declination = calculate_solar_declination(apparent_longitude, obliquity);

	double eq_time = 4.0 * (280.46646 + 36000.76983 * JC - apparent_longitude +
	(2.466 * sin(2 * DEG_TO_RAD * (280.46646 + 36000.76983 * JC))) -
	(0.053 * sin(4 * DEG_TO_RAD * (280.46646 + 36000.76983 * JC))));
	double solar_time = (params->hour * 60 + params->minute + params->second / 60.0 + eq_time + 4.0 * params->longitude - 60.0 * timezone_offset) / 60.0;

	double hour_angle = (solar_time - 12.0) * 15.0;

	double latitude_rad = params->latitude * DEG_TO_RAD;
	double declination_rad = declination * DEG_TO_RAD;
	double hour_angle_rad = hour_angle * DEG_TO_RAD;

	double sin_elevation = sin(latitude_rad) * sin(declination_rad) +
	cos(latitude_rad) * cos(declination_rad) * cos(hour_angle_rad);

	*elevation = asin(sin_elevation) * RAD_TO_DEG;
	*elevation += calculate_atmospheric_refraction(*elevation);

	double cos_azimuth = (sin(declination_rad) - sin(latitude_rad) * sin(DEG_TO_RAD * (*elevation))) /
	(cos(latitude_rad) * cos(DEG_TO_RAD * (*elevation)));
	double azimuth_temp = acos(cos_azimuth) * RAD_TO_DEG;

	if (hour_angle > 0) {
		*azimuth = 360.0 - azimuth_temp;
		} else {
		*azimuth = azimuth_temp;
	}
}

// Pagrindinë funkcija
int main() {
	SolarPositionParameters solar_params = {
		.latitude = 0,
		.longitude = 0,
		.altitude = 84.0,
		.year = 2024,
		.month = 6,
		.day = 20,
		.hour = 16,
		.minute = 0,
		.second = 0
	};

	double elevation, azimuth;

	calculate_solar_position(&solar_params, &elevation, &azimuth);

	printf("Saulës aukðtis: %.6f°\n", elevation);
	printf("Saulës azimutas: %.6f°\n", azimuth);

	return 0;
}


//////////////////////////////////
/////////////////////////////////
///////////////////////////////////
/////////////////////////////////
funkcija koreguojanti elevacija ir azimuta: slegiu, temperatura, dregme
//////////////////////////////
//////////////////////////////////
/////////////////////////////////
///////////////////////////////////
/////////////////////////////////

#include <math.h>
#include <stdio.h>

// Refrakcijos korekcijos funkcija
// Atmosferos slëgis hPa, temperatûra °C, drëgmë 0-1, elevacija laipsniais
void adjust_solar_position(double pressure_hpa, double temperature_c, double humidity,
double *elevation_deg, double *azimuth_deg) {
	// Originali elevacija laipsniais
	double original_elevation = *elevation_deg;

	// Atmosferos refrakcijos skaièiavimas (Hohenwart'o formulë supaprastinta versija)
	double refr_coeff = 0.0167 / tan((original_elevation + 10.3 / (original_elevation + 5.11)) * M_PI / 180.0);

	// Koreguojamas refrakcijos koeficientas pagal slëgá ir temperatûrà
	double pressure_correction = pressure_hpa / 1010.0; // 1010 hPa - standartinis slëgis
	double temperature_correction = 283.0 / (273.0 + temperature_c); // Kelvino temperatûra

	refr_coeff *= pressure_correction * temperature_correction;

	// Refrakcijos korekcijos taikymas
	double corrected_elevation = original_elevation + refr_coeff;

	// Azimuto koregavimas (minimalus, bet galima pritaikyti drëgmës átakà, jei reikia)
	double azimuth_correction = 0.0; // Daugeliu atveju azimutas nekinta dël refrakcijos
	
	// Papildoma korekcija, jei norite átraukti drëgmës átakà (daþniausiai labai maþa):
	// azimuth_correction = humidity * 0.01; // Pvz., 1% korekcija uþ drëgmæ

	// Nustatyti naujas vertes
	*elevation_deg = corrected_elevation;
	*azimuth_deg += azimuth_correction;
}

int main() {
	double elevation = 30.0; // Pvz., 30 laipsniø elevacija
	double azimuth = 150.0;  // Pvz., 150 laipsniø azimutas

	double pressure = 1013.25; // Standartinis atmosferos slëgis (hPa)
	double temperature = 15.0; // Temperatûra °C
	double humidity = 0.5;     // 50% santykinë drëgmë

	printf("Pradinës reikðmës: Elevacija: %.2f°, Azimutas: %.2f°\n", elevation, azimuth);

	adjust_solar_position(pressure, temperature, humidity, &elevation, &azimuth);

	printf("Pakoreguotos reikðmës: Elevacija: %.2f°, Azimutas: %.2f°\n", elevation, azimuth);

	return 0;
}


*/