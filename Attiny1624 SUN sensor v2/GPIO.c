/*
 * GPIO.c
 *
 * Created: 2024-10-02 21:45:19
 *  Author: Saulius
 */ 
 #include "Settings.h"

 void GPIO_init(){
	 CPU_CCP = CCP_IOREG_gc;
	 CLKCTRL.MCLKCTRLB = 0 << CLKCTRL_PEN_bp;
	 while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm);

	 PORTA.DIRSET = PIN4_bm | PIN5_bm | PIN6_bm| PIN7_bm;
	 PORTB.DIRSET = PIN0_bm | PIN2_bm; //Out RS485: XDIR, TX
	 PORTB.DIRCLR = PIN3_bm; //In RS485: RX
	 PORTB.PIN0CTRL = PORT_PULLUPEN_bm; //pullup XDIR
	 PORTB.PIN2CTRL = PORT_PULLUPEN_bm; //pullup TX
	 PORTB.PIN3CTRL = PORT_PULLUPEN_bm; //pullup RX

	 PORTA.DIRCLR = PIN1_bm | PIN2_bm; //AIN1, AIN2 as ADC channels
	 PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc /*| PORT_PULLUPEN_bm*/; //digital input buffer off
	 PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc /*| PORT_PULLUPEN_bm*/; //digital input buffer off
 }

