/*
 * Cosmos.c
 *
 * Created: 2024-11-21 12:38:58
 *  Author: Saulius
 */ 
#include "Settings.h"
#include "CosmosVar.h"

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

	////////////////////////NETEISINGAI apskaièiuojamas azimutas kai saulë yra nuo 359 iki 180 laipsniu pataisyta versija yra attiny212 clock programoje


	if (hour_angle > 0) {
		*azimuth = 360.0 - azimuth_temp;
		} else {
		*azimuth = azimuth_temp;
	}
}