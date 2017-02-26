/*****************************
ver 0.1.0

*/
#ifndef PR_Blinker_h
#define PR_Blinker_h

#include <arduino.h>
#include <stdint.h>

#ifndef PR_BLINKER_LEDS_MAX
#define PR_BLINKER_LEDS_MAX 8
#endif


#define PR_MODE0       0
#define PR_MODE1       1
#define PR_MODE2       2
#define PR_MODE3       3

#define PR_NON_INVERSE       0
#define PR_INVERSE           1

class Blinker {
	public:
        Blinker();
		static void mode1config(uint16_t onTime, uint16_t offTime);
		static void mode2config(uint16_t onTime, uint16_t offTime);
		static void mode3config(uint16_t onTime, uint16_t offTime);
		
		void	set(bool val);
        void    on();
		void    off();
        void    togle(); 
        void    setmode(uint8_t mode, bool inverse);
		static void update();
	protected:
		static	uint8_t     _numLeds; 
		static	uint8_t     _data[PR_BLINKER_LEDS_MAX];    //b0-1: mode[0..3] b5: inverse blink mode   b6: onValue  b7: current on/off state		
		static 	uint8_t     _modeOn[3];		    //on period for mode[i] in ms
		static 	uint8_t     _modeOff[3];
		static	uint8_t	    _modeStateFlags;	//b1: state of mode1, b2: stater of mode 2, b3: state of mode 3 
		static 	long		_timeLast;
		uint8_t			    _led;
}; //class Blinker

class simpleLED : public Blinker {	//	
	public:
		simpleLED (uint8_t pin, bool onValue);
		static	void update();	
	protected:
		static	uint8_t _pin[PR_BLINKER_LEDS_MAX];
}; //class


#endif
