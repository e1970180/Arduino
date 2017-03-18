#include "PR_Blink.h"

#include <arduino.h>


		void BlinkClass::mode1configT(uint16_t onTime, uint16_t offTime) {   
			_timeOn[0] = constrain ( (onTime  >> 4), 1, 255);      //reduce prescise to 1000/16= 0.016 s (f ~= 30Hz)
			_timeOff[0] =constrain ( (offTime >> 4), 1, 255);      //so min t = 0.016s   max t = 3s
		}
		void BlinkClass::mode2configT(uint16_t onTime, uint16_t offTime) {
			_timeOn[1] = constrain ( (onTime  >> 4), 1, 255);
			_timeOff[1] =constrain ( (offTime >> 4), 1, 255); 			
		}
		void BlinkClass::mode3configT(uint16_t onTime, uint16_t offTime) {
			_timeOn[2] = constrain ( (onTime  >> 4), 1, 255);
			_timeOff[2] =constrain ( (offTime >> 4), 1, 255); 				
		}
        void BlinkClass::mode1configF(float freq, uint8_t duty) {
            mode1configT( (uint16_t)(constrain(duty, 1, 100)*10/freq), (uint16_t)( (100-constrain(duty, 1, 100))*10/freq) );
        }
        void BlinkClass::mode2configF(float freq, uint8_t duty) {
            mode2configT( (uint16_t)(constrain(duty, 1, 100)*10/freq), (uint16_t)( (100-constrain(duty, 1, 100))*10/freq) );
        }		
        void BlinkClass::mode3configF(float freq, uint8_t duty) {
            mode3configT( (uint16_t)(constrain(duty, 1, 100)*10/freq), (uint16_t)( (100-constrain(duty, 1, 100))*10/freq) );
        }		
		
		void BlinkClass::update() {
            static  uint8_t        timeMode1, timeMode2 , timeMode3;
                    uint8_t        time1, time2 , time3;
            
            uint8_t currtime = (uint8_t)(millis() >> 4);
			time1 = currtime - timeMode1;
            time2 = currtime - timeMode2;
            time3 = currtime - timeMode3;            
			//обновляем состояние мигания - mode1         
			if (_modeState.m1) {	//текущее состояние = On
				if (time1 > _timeOn[0] ) { _modeState.m1 = 0; timeMode1 = currtime; }
			}
			else { // //текущее состояние = Off
				if (time1 > _timeOff[0] ) { _modeState.m1 = 1; timeMode1 = currtime;  }   
			}
            //обновляем состояние мигания - mode2
            if (_modeState.m2) {
                if (time2 > _timeOn[1] ) { _modeState.m2 = 0;  timeMode2 = currtime; }
            }
            else { 
                if (time2 > _timeOff[1] ) { _modeState.m2 = 1; timeMode2 = currtime;  }   
            }
            //обновляем состояние мигания - mode3
            if (_modeState.m3) { 
                if (time3 > _timeOn[2] ) { _modeState.m3 = 0;  timeMode3 = currtime; }
            }
            else { 
                if (time3 > _timeOff[2] ) { _modeState.m3 = 1; timeMode3 = currtime;  }   
            }               
		} //update()

		
		
		SimpleBlinker::SimpleBlinker (uint8_t pin, bool onValue) {  //onValue = pin state to "on" the led
		    _led = _numLeds;
            if (_numLeds < PR_BLINK_LEDS_MAX-1 ) _numLeds++; //предотвращаем переполнение массива
            _data[_led].word = 0;
			_data[_led].pin 	= pin;
    		_data[_led].onValue = onValue; 
			
            pinMode(_data[_led].pin, OUTPUT);
		}
       
		void SimpleBlinker::update() {
			bool s;
			BlinkClass::update(); //update current state of modes1..3

			for (uint8_t i = 0; i < _numLeds; i++) {
				switch (_data[i].mode) { 
					case BL_CONT: 					//continous mode0
                        s = true;
						break;
					case BL_MODE1:
						s = _modeState.m1;                
						break;
					case BL_MODE2:
						s = _modeState.m2; 
						break;
                    case BL_MODE3:
                        s = _modeState.m3; 
                        break;                      
				} //switch
                s = ( s ^ _data[i].modeInverse) & _data[i].state; 
				digitalWrite(_data[i].pin, s );
			} //for 
		} //update()

		void	SimpleBlinker::set(bool val)       { _data[_led].state = val ^ !_data[_led].onValue; 	}
        void    SimpleBlinker::on()                { _data[_led].state =  _data[_led].onValue;			}
		void    SimpleBlinker::off()               { _data[_led].state = !_data[_led].onValue; 			}
        void    SimpleBlinker::togle()             { _data[_led].state = !_data[_led].state; 			}
                
	    void    SimpleBlinker::setmode(BlinkMode mode, bool inverse = false) {			
			_data[_led].mode = mode;
			_data[_led].modeInverse	= inverse;
        }	
		
          BlinkClass   Blink;
          uint8_t      BlinkClass::_timeOn[3];                     //on period for mode[i] in 1000/32 
          uint8_t      BlinkClass::_timeOff[3];
          static union BlinkClass::BlinkData      BlinkClass::_modeState;           

          uint8_t      SimpleBlinker::_numLeds=0; 
          static union SimpleBlinker::ledData      SimpleBlinker::_data[PR_BLINK_LEDS_MAX];


