/*****************************
ver 0.3.0

*/
#ifndef PR_Blink_h
#define PR_Blink_h

#include <arduino.h>
#include <stdint.h>

#ifndef PR_BLINK_LEDS_MAX
#define PR_BLINK_LEDS_MAX 8
#endif

#define PR_NON_INVERSE       0
#define PR_INVERSE           1

enum BlinkMode { BL_MODE0 = 0, BL_CONT=0, BL_MODE1, BL_MODE2, BL_MODE3 };

class BlinkClass {
	public:
		static void mode1configT(uint16_t onTime, uint16_t offTime);
		static void mode2configT(uint16_t onTime, uint16_t offTime);
		static void mode3configT(uint16_t onTime, uint16_t offTime);
        static void mode1configF(float freq, uint8_t duty);
        static void mode2configF(float freq, uint8_t duty);
        static void mode3configF(float freq, uint8_t duty);
		static void update();
		bool		get(BlinkMode m);
		bool 		getM1() 				{ return _modeState.m1; }
		bool 		getM2() 				{ return _modeState.m2; }		
		bool 		getM3() 				{ return _modeState.m3; }		
	protected:
		static 	uint8_t     _timeOn[3];		    //on period for mode[i] in ms
		static 	uint8_t     _timeOff[3];

        struct BlinkData {
                uint8_t m1  : 1;    //b1: state of mode 1
                uint8_t m2  : 1;    //b2: state of mode 2
                uint8_t m3  : 1;    //b2: state of mode 3
         };   
		static BlinkData	_modeState;

}; //class Blink

class SimpleBlinker : public BlinkClass {	//	
	public:
		SimpleBlinker (uint8_t pin, bool onValue);
		static void 	update();	
		void			set(bool val);
        void    		on();
		void    		off();
        void    		togle(); 
		void    		setmode(BlinkMode mode, bool inverse = false);

	protected:
		static uint8_t	_numLeds;
		uint8_t			_led;
		
		union ledData {
			struct {
				uint8_t	pin;
				uint8_t mode 		: 2;
                uint8_t modeInverse : 1;	
				uint8_t onValue 	: 1;
				uint8_t state 		: 1;			
			} ; //bits;
			uint16_t word;
		} ; 
		static union ledData	_data[PR_BLINK_LEDS_MAX];

}; //class
/*
����������� ������������ ��� ����������������� ���� ������������� ��������� ������, ���� 
������� ��������� � #pragma pack (push, sizeof(int)) #pragma pop
��� ������������ ����� ������� ����, ��� �������� ���������� ������� ����.		
		
}
*/

    extern BlinkClass  Blink;

#endif
