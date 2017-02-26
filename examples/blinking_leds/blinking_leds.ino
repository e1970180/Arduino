
#define PR_BLINKER_LEDS_MAX 4	//number of leds in project (8 by default if not defined here)
								//this line must be before #include "PR_Blinker.h"
#include "PR_Blinker.h"

   

simpleLED    Led1(7, HIGH); //pin number for led, pin state for "on" state is HIGH (led connected to GND)
simpleLED    d8(8, HIGH);
simpleLED    d9(9, LOW); 	//pin number for, pin state for "on" state is LOW (led connected to Vcc)
simpleLED    d10(10, HIGH);
//simpleLED    d11(11, 1);
//simpleLED    d12(12, 1);


            
            

void setup() {


Blinker::mode1config(400,200);			//blinking 400ms "on" and 200ms "off"
Blinker::mode2config(1000,1000);
Blinker::mode3config(100,2000);
Led1.setmode(PR_MODE0, PR_NON_INVERSE);   //continouse mode
d8.setmode(PR_MODE1, PR_NON_INVERSE);		//set blinking mode 1 for led d8 
d9.setmode(PR_MODE2, PR_NON_INVERSE);		//set blinking mode 2 for led d9 
d10.setmode(PR_MODE3, PR_INVERSE);			//set inverted blinking mode 2 for led d10 meaning d10 is "on" than d9 is "off"

  
}


void loop() {
		
	Led1.off();
	d8.on();
	d9.on();
	d10.on(); 
	d9.off();
	
	simpleLED::update();
    delay(1); 	// do not use long delay () in main loop!!! 
				// simpleLED::update() should be called often, but must be at least 3 times per minimum value at mode1..3 
}
