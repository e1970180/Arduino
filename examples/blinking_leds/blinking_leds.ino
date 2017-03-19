
#define PR_Blink_LEDS_MAX 4	//number of leds in project (8 by default if not defined here)
								//this line must be before #include "PR_Blink.h"
#include "PR_Blink.h"		//ver 0.3.0


SimpleBlinker    d2(2, HIGH);   //pin number for led, pin state for "on" state is HIGH (led connected to GND)
SimpleBlinker    d3(3, HIGH);
SimpleBlinker    d4(4, LOW); 	//pin number for led, pin state for "on" state is LOW (led connected to Vcc)
SimpleBlinker    d5(5, HIGH);
SimpleBlinker    d6(6, 1);
          
            

void setup() {

    Serial.begin(9600);

    Blink.mode1configT(100,900);			//blinking period 400ms for "on" state and 200ms for "off" state.  
                                            //Period truncated to [16 ... 4080] ms

    Blink.mode2configF(15, 10);             //blinking freq is 15Hz with duty 10%. 
                                            //freqduty truncated to [16 ... 4080] Hz , duty range truncated [1% ... 100%]

    Blink.mode3configT(400,400);
      
    d2.setmode(BL_MODE0, PR_NON_INVERSE);       //continouse mode
    d3.setmode(BL_CONT);		                //continouse mode. NON_INVERSE by default 
    
    d4.setmode(BL_MODE1, PR_NON_INVERSE);		//set blinking mode 1 
    
    d5.setmode(BL_MODE3, PR_NON_INVERSE);		//set mode 2 
    d6.setmode(BL_MODE3, PR_INVERSE);           //set inverted blinking mode 2, meaning d6 is "on" than d5 is "off"

    d2.on();
    d3.on();
    d4.on();
    d5.set(true);
    d6.off();
    d6.togle();

}


void loop() {
		
	SimpleBlinker::update();
  
    delay(1); 	// do not use long delay () in main loop!!! 
				// SimpleBlinker::update() should be called often, but must be at least 3 times per minimum value at mode1..3 

    Serial.print("Current state of mode1  ");Serial.println(Blink.getM1());
    Serial.print("Current state of mode2  ");Serial.println(Blink.getM2());
    Serial.print("Current state of mode3  ");Serial.println(Blink.getM3()); 
    Serial.print("Current state of mode3 too ");Serial.println(Blink.get(3));    	
}
