#include "PR_Blinker.h"

        Blinker::Blinker () {
            _led = _numLeds;
            _data[_led]=0;
            if (_numLeds++ > PR_BLINKER_LEDS_MAX ) _numLeds = PR_BLINKER_LEDS_MAX; //предотвращаем переполнение массива
        }
 
		void Blinker::mode1config(uint16_t onTime, uint16_t offTime) {   
			_modeOn[0] = constrain ( (onTime >> 5), 1, 255);      //reduce prescise to 1000/32= 0.03 s
			_modeOff[0] =constrain ( (offTime >> 5), 1, 255); 
		}
		void Blinker::mode2config(uint16_t onTime, uint16_t offTime) {
			_modeOn[1] = constrain ( (onTime >> 5), 1, 255);
			_modeOff[1] =constrain ( (offTime >> 5), 1, 255); 			
		}
		void Blinker::mode3config(uint16_t onTime, uint16_t offTime) {
			_modeOn[2] = constrain ( (onTime >> 5), 1, 255);
			_modeOff[2] =constrain ( (offTime >> 5), 1, 255); 				
			//_modeOn[2] = onTime >> 5;     if (!_modeOn[2]) _modeOn[2]=1;
			//_modeOff[2] = offTime >> 5;   if (!_modeOff[2]) _modeOff[2]=1;
		}
		
		void	Blinker::set(bool val)       { bitWrite(_data[_led], 7, val); }
        void    Blinker::on()                { bitWrite(_data[_led], 7, 1); }
		void    Blinker::off()               { bitWrite(_data[_led], 7, 0); }
        void    Blinker::togle()             { bitWrite(_data[_led], 7, !bitRead(_data[_led], 7)); }
                
        void    Blinker::setmode(uint8_t mode, bool inverse=false) { 
            _data[_led] = (_data[_led] & 0b11111100 ) + (mode & 0b00000011);
            bitWrite(_data[_led], 5, inverse); 
        }

		void Blinker::update() {
            static  uint8_t        timeMode1, timeMode2 , timeMode3;
                    uint8_t        time1, time2 , time3;
            
            uint8_t currtime = (uint8_t)(millis() >> 5);
			time1 = currtime - timeMode1;
            time2 = currtime - timeMode2;
            time3 = currtime - timeMode3;            
			//обновляем состояние мигания - mode1         
			if (bitRead(_modeStateFlags, 1)) {	//текущее состояние = On
				if (time1 > _modeOn[0] ) { bitWrite(_modeStateFlags, 1, 0); timeMode1 = currtime; }
			}
			else { // //текущее состояние = Off
				if (time1 > _modeOff[0] ) {  bitWrite(_modeStateFlags, 1, 1); timeMode1 = currtime;  }   
			}
            //обновляем состояние мигания - mode2
            if (bitRead(_modeStateFlags, 2)) {
                if (time2 > _modeOn[1] ) { bitWrite(_modeStateFlags, 2, 0); timeMode2 = currtime; }
            }
            else { 
                if (time2 > _modeOff[1] ) {  bitWrite(_modeStateFlags, 2, 1); timeMode2 = currtime;  }   
            }
            //обновляем состояние мигания - mode3
            if (bitRead(_modeStateFlags, 3)) { 
                if (time3 > _modeOn[2] ) { bitWrite(_modeStateFlags, 3, 0); timeMode3 = currtime; }
            }
            else { 
                if (time3 > _modeOff[2] ) {  bitWrite(_modeStateFlags, 3, 1); timeMode3 = currtime;  }   
            }               
		} //update()

		simpleLED::simpleLED (uint8_t pin, bool onValue) {  //onValue = pin state to "on" the led
			_pin[_led] = pin;
    		bitWrite(_data[_led], 6, !onValue); 
            pinMode(_pin[_led], OUTPUT);
		}
       
		void simpleLED::update() {
			bool s;
			Blinker::update(); //update current state of modes1..3

			for (uint8_t i=0; i < _numLeds; i++) {
				switch (_data[i] & 0b00000011) { //маска битов режима
					case 0: //continous mode0
                        digitalWrite(_pin[i], bitRead(_data[i], 7) ^ bitRead(_data[i], 6) );
						break;
					case 1:
						s =  ( ((bitRead(_modeStateFlags, 1)^bitRead(_data[i], 5) ) & bitRead(_data[i], 7) ) ^ bitRead(_data[i], 6) );
						digitalWrite(_pin[i], s);                  
						break;
					case 2:
						s =  ( ((bitRead(_modeStateFlags, 2)^bitRead(_data[i], 5) ) & bitRead(_data[i], 7) ) ^ bitRead(_data[i], 6) );
						digitalWrite(_pin[i], s); 
						break;
                    case 3:
                        s =  ( ((bitRead(_modeStateFlags, 3)^bitRead(_data[i], 5) ) & bitRead(_data[i], 7) ) ^ bitRead(_data[i], 6) );
                        digitalWrite(_pin[i], s);
                        break;                      
				} //switch
			} //for 
		} //update()

          uint8_t      Blinker::_numLeds=0; 
          uint8_t      Blinker::_data[PR_BLINKER_LEDS_MAX];     //b0-1: mode[0..3] b5: inverse blink mode   b6: onValue  b7: current on/off state       
          uint8_t      Blinker::_modeOn[3];                     //on period for mode[i] in 1000/32 
          uint8_t      Blinker::_modeOff[3];
          uint8_t      Blinker::_modeStateFlags=0;              //b1: state of mode1, b2: stater of mode 2, b3: state of mode 3 
          long         Blinker::_timeLast;
          uint8_t      simpleLED::_pin[PR_BLINKER_LEDS_MAX];


