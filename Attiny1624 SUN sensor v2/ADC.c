/*
 * ADC.c
 *
 * Created: 2024-10-02 21:45:41
 *  Author: Saulius
 */ 
 #include "Settings.h"

 void ADC0_init(){
	 ADC0.CTRLA = ADC_ENABLE_bm | ADC_LOWLAT_bm; //adc enable
	 ADC0.CTRLB = ADC_PRESC_DIV4_gc; // 20M / 4 = 5MHz ADC clock (MAX adc clock is 6M)
	 ADC0.CTRLC = (TIMEBASE_VALUE << ADC_TIMEBASE_gp)
	 | ADC_REFSEL_2048MV_gc; //4.096V reference
	 ADC0.CTRLE = 0; //How long to take a one sample (ADC clock cycles) so it will take 0+ 0.5 = half cycle //time for discharge external capacitor
	 ADC0.CTRLF = ADC_SAMPNUM_NONE_gc ; //128 samples:
	 ADC0.COMMAND = ADC_MODE_SINGLE_12BIT_gc | ADC_DIFF_bm; // 64 measurements at one time // use muxneg and muxpos
	 ADC0.PGACTRL = ADC_PGAEN_bm | ADC_GAIN_4X_gc | ADC_PGABIASSEL_1X_gc | ADC_ADCPGASAMPDUR_20CLK_gc;
	 VREF.CTRLA = VREF_AC0REFSEL_AVDD_gc;

	 ADC0.MUXPOS = ADC_MUXPOS_DACREF0_gc /*| ADC_VIA_PGA_gc*/;
	 while (ADC0.STATUS & ADC_ADCBUSY_bm); // wait untill all settings will be set up

 }

 uint16_t ADC0_Read(uint8_t ch){
	 ADC0.MUXNEG = ch | ADC_VIA_PGA_gc;
	 ADC0.COMMAND |= ADC_START_IMMEDIATE_gc; //start conversion
	 while (ADC0.STATUS & ADC_ADCBUSY_bm); // wait untill all settings will be set up
	 while (!(ADC0.INTFLAGS & ADC_SAMPRDY_bm));
	 ADC0.INTFLAGS = ADC_SAMPRDY_bm; //clear result ready flag
	 return ADC0.SAMPLE; //return average ADC value of 64 measurements
 }