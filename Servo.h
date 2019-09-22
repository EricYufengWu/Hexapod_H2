 /*************************************************** 
Servo code for 12-channel hexapod.
Modified from Adafruit tutorial on 12-channel servo driver
install "Adafruit_PWMServoDriver" library in ArduinoIDE
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define MIN  280
#define MAX  380
#define BASEMIN  300 // this is the 'minimum' pulse length count (out of 4096)
#define BASEMAX  360 // this is the 'maximum' pulse length count (out of 4096)
#define LEGMIN  260
#define LEGMAX  300
#define MID_ALL  330
#define MID_LEG  300
#define MOVEVAL  20
int MID[] = {330,330,330,330,330,330,330,330,330,330,330,350};
/*            0   1   2   3   4   5   6   7   8   9  10  11        */
/*           larger value: turns towards shorter side of servo   /down for legs            */

// our servo # counter
uint8_t servonum = 0;
uint8_t center_pos = 200;

void reset_pos(){
    
    //set 3 legs first
    pwm.setPWM(6, 0, MID[6] - 40);
    pwm.setPWM(8, 0, MID[8] - 40);
    pwm.setPWM(10, 0, MID[10] - 40);
    delay(100);
    pwm.setPWM(0, 0, MID[0]);
    pwm.setPWM(2, 0, MID[2]);
    pwm.setPWM(4, 0, MID[4]);
    pwm.setPWM(6, 0, MID[6]);
    pwm.setPWM(8, 0, MID[8]);
    pwm.setPWM(10, 0, MID[10]);
    delay(100);
    pwm.setPWM(7, 0, MID[7] - 40);
    pwm.setPWM(9, 0, MID[9] - 40);
    pwm.setPWM(11, 0, MID[11] - 40);
    delay(100);
    pwm.setPWM(1, 0, MID[1]);
    pwm.setPWM(3, 0, MID[3]);
    pwm.setPWM(5, 0, MID[5]);
    pwm.setPWM(7, 0, MID[7]);
    pwm.setPWM(9, 0, MID[9]);
    pwm.setPWM(11, 0, MID[11]);
    /*
    for (int i = 0; i < 12; i++) {
        pwm.setPWM(i, 0, MID[i]);
    }*/
}



//drives hex forward
void forward(int dur){
    pwm.setPWM(6, 0, MID[6] - 20);
    pwm.setPWM(8, 0, MID[8] - 20);
    pwm.setPWM(10, 0, MID[10] - 20);

    for (int i = 0; i < dur; i++){
        int legmove = -10;       //amount that the leg moves up and down
        for (uint16_t pulselen = BASEMIN; pulselen < MID_ALL; pulselen++) {
            //chassis servo
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]) - 80);
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]) + 80);
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]) - 80);
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]) + 80);
            pwm.setPWM(1, 0, BASEMAX - (pulselen + (MID_ALL - MID[1]) - BASEMIN));
            pwm.setPWM(4, 0, BASEMAX - (pulselen + (MID_ALL - MID[4]) - BASEMIN));
            //leg servo
            pwm.setPWM(6, 0, MID[6] + legmove*3);
            pwm.setPWM(8, 0, MID[8] + legmove*3);
            pwm.setPWM(10, 0, MID[10] + legmove*3);  
            legmove++;            
        }

        for (uint16_t pulselen = MID_ALL; pulselen < BASEMAX; pulselen++) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]) - 80);
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]) + 80);
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]) - 80);
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]) + 80);
            pwm.setPWM(1, 0, BASEMAX - (pulselen + (MID_ALL - MID[1]) - BASEMIN));
            pwm.setPWM(4, 0, BASEMAX - (pulselen + (MID_ALL - MID[4]) - BASEMIN));

            pwm.setPWM(6, 0, MID[6] + legmove*3);
            pwm.setPWM(8, 0, MID[8] + legmove*3);
            pwm.setPWM(10, 0, MID[10] + legmove*3);
            legmove--;    
        }

        delay(100);
        for (uint16_t pulselen = BASEMAX; pulselen > MID_ALL; pulselen--) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]) - 80);
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]) + 80);
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]) - 80);
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]) + 80);
            pwm.setPWM(1, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[1]))));
            pwm.setPWM(4, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[4]))));
 
            pwm.setPWM(7, 0, MID[7] + legmove*3);
            pwm.setPWM(9, 0, MID[9] + legmove*3);
            pwm.setPWM(11, 0, MID[11] + legmove*3);
            legmove++;  
        }
        for (uint16_t pulselen = MID_ALL; pulselen > BASEMIN; pulselen--) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]) - 80);
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]) + 80);
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]) - 80);
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]) + 80);
            pwm.setPWM(1, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[1]))));
            pwm.setPWM(4, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[4]))));
               
            pwm.setPWM(7, 0, MID[7] + legmove*3);
            pwm.setPWM(9, 0, MID[9] + legmove*3);
            pwm.setPWM(11, 0, MID[11] + legmove*3);
            legmove--;      
        }
        delay(100);
    }
}
  
//3 and 5 switched sign b4 60
//legs - 60

//drives hex backward
void backward(int dur){

    for (int i = 0; i < dur; i++){
        int legmove = -10;
        for (uint16_t pulselen = BASEMIN; pulselen < MID_ALL; pulselen++) {
            pwm.setPWM(0, 0, BASEMAX - (pulselen + (MID_ALL - MID[0]) - BASEMIN) - 80);
            pwm.setPWM(2, 0, BASEMAX - (pulselen + (MID_ALL - MID[2]) - BASEMIN) + 80);
            pwm.setPWM(3, 0, BASEMAX - (pulselen + (MID_ALL - MID[3]) - BASEMIN) - 80);
            pwm.setPWM(5, 0, BASEMAX - (pulselen + (MID_ALL - MID[5]) - BASEMIN) + 80);
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));
            
            pwm.setPWM(6, 0, MID[6] - legmove*3);
            pwm.setPWM(8, 0, MID[8] - legmove*3);
            pwm.setPWM(10, 0, MID[10] - legmove*3);  
            legmove++;          
        }

        for (uint16_t pulselen = MID_ALL; pulselen < BASEMAX; pulselen++) {
            pwm.setPWM(0, 0, BASEMAX - (pulselen + (MID_ALL - MID[0]) - BASEMIN) - 80);
            pwm.setPWM(2, 0, BASEMAX - (pulselen + (MID_ALL - MID[2]) - BASEMIN) + 80);
            pwm.setPWM(3, 0, BASEMAX - (pulselen + (MID_ALL - MID[3]) - BASEMIN) - 80);
            pwm.setPWM(5, 0, BASEMAX - (pulselen + (MID_ALL - MID[5]) - BASEMIN) + 80);
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));

            pwm.setPWM(6, 0, MID[6] - legmove*3);
            pwm.setPWM(8, 0, MID[8] - legmove*3);
            pwm.setPWM(10, 0, MID[10] - legmove*3);  
            legmove--;       
        }

        delay(100);
        for (uint16_t pulselen = BASEMAX; pulselen > MID_ALL; pulselen--) {
            pwm.setPWM(0, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[0]))) - 80);
            pwm.setPWM(2, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[2]))) + 80);
            pwm.setPWM(3, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[3]))) - 80);
            pwm.setPWM(5, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[5]))) + 80);
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));
 
            pwm.setPWM(7, 0, MID[7] - legmove*3);
            pwm.setPWM(9, 0, MID[9] - legmove*3);
            pwm.setPWM(11, 0, MID[11] - legmove*3);
            legmove++;
        }
        for (uint16_t pulselen = MID_ALL; pulselen > BASEMIN; pulselen--) {
            pwm.setPWM(0, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[0]))) - 80);
            pwm.setPWM(2, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[2]))) + 80);
            pwm.setPWM(3, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[3]))) - 80);
            pwm.setPWM(5, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[5]))) + 80);
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));

            pwm.setPWM(7, 0, MID[7] - legmove*3);
            pwm.setPWM(9, 0, MID[9] - legmove*3);
            pwm.setPWM(11, 0, MID[11] - legmove*3);
            legmove--;            
        }
        delay(100);
    }
}



void right(int dur){

    for (int i = 0; i < dur; i++){
        int legmove = -10;
        for (uint16_t pulselen = BASEMIN; pulselen < MID_ALL; pulselen++) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]));
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));
            pwm.setPWM(1, 0, BASEMAX - (pulselen + (MID_ALL - MID[1]) - BASEMIN));
            pwm.setPWM(3, 0, BASEMAX - (pulselen + (MID_ALL - MID[3]) - BASEMIN));
            pwm.setPWM(5, 0, BASEMAX - (pulselen + (MID_ALL - MID[5]) - BASEMIN));
            
            pwm.setPWM(6, 0, MID[6] - legmove*3);
            pwm.setPWM(8, 0, MID[8] - legmove*3);
            pwm.setPWM(10, 0, MID[10] - legmove*3);  
            legmove++;        
        }
        for (uint16_t pulselen = MID_ALL; pulselen < BASEMAX; pulselen++) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]));
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));
            pwm.setPWM(1, 0, BASEMAX - (pulselen + (MID_ALL - MID[1]) - BASEMIN));
            pwm.setPWM(3, 0, BASEMAX - (pulselen + (MID_ALL - MID[3]) - BASEMIN));
            pwm.setPWM(5, 0, BASEMAX - (pulselen + (MID_ALL - MID[5]) - BASEMIN));
             
            pwm.setPWM(6, 0, MID[6] - legmove*3);
            pwm.setPWM(8, 0, MID[8] - legmove*3);
            pwm.setPWM(10, 0, MID[10] - legmove*3);
            legmove--;       
        }

        delay(100);
        for (uint16_t pulselen = BASEMAX; pulselen > MID_ALL; pulselen--) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]));
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));
            pwm.setPWM(1, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[1]))));
            pwm.setPWM(3, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[3]))));
            pwm.setPWM(5, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[5]))));

            pwm.setPWM(7, 0, MID[7] - legmove*3);
            pwm.setPWM(9, 0, MID[9] - legmove*3);
            pwm.setPWM(11, 0, MID[11] - legmove*3);
            legmove++; 
        }
        for (uint16_t pulselen = MID_ALL; pulselen > BASEMIN; pulselen--) {
            pwm.setPWM(0, 0, pulselen - (MID_ALL - MID[0]));
            pwm.setPWM(2, 0, pulselen - (MID_ALL - MID[2]));
            pwm.setPWM(4, 0, pulselen - (MID_ALL - MID[4]));
            pwm.setPWM(1, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[1]))));
            pwm.setPWM(3, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[3]))));
            pwm.setPWM(5, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[5]))));
             
            pwm.setPWM(7, 0, MID[7] - legmove*3);
            pwm.setPWM(9, 0, MID[9] - legmove*3);
            pwm.setPWM(11, 0, MID[11] - legmove*3);
            legmove--;           
        }
        delay(100);
    }
}



void left(int dur){

    for (int i = 0; i < dur; i++){
        int legmove = -10;
        for (uint16_t pulselen = BASEMIN; pulselen < MID_ALL; pulselen++) {
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]));
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]));
            pwm.setPWM(0, 0, BASEMAX - (pulselen + (MID_ALL - MID[0]) - BASEMIN));
            pwm.setPWM(2, 0, BASEMAX - (pulselen + (MID_ALL - MID[2]) - BASEMIN));
            pwm.setPWM(4, 0, BASEMAX - (pulselen + (MID_ALL - MID[4]) - BASEMIN));
            
            pwm.setPWM(6, 0, MID[6] - legmove*3);
            pwm.setPWM(8, 0, MID[8] - legmove*3);
            pwm.setPWM(10, 0, MID[10] - legmove*3);  
            legmove++;         
        }

        for (uint16_t pulselen = MID_ALL; pulselen < BASEMAX; pulselen++) {
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]));
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]));
            pwm.setPWM(0, 0, BASEMAX - (pulselen + (MID_ALL - MID[0]) - BASEMIN));
            pwm.setPWM(2, 0, BASEMAX - (pulselen + (MID_ALL - MID[2]) - BASEMIN));
            pwm.setPWM(4, 0, BASEMAX - (pulselen + (MID_ALL - MID[4]) - BASEMIN));

             
            pwm.setPWM(6, 0, MID[6] - legmove*3);
            pwm.setPWM(8, 0, MID[8] - legmove*3);
            pwm.setPWM(10, 0, MID[10] - legmove*3);  
            legmove--;     
        }

        delay(100);
        for (uint16_t pulselen = BASEMAX; pulselen > MID_ALL; pulselen--) {
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]));
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]));
            pwm.setPWM(0, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[0]))));
            pwm.setPWM(2, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[2]))));
            pwm.setPWM(4, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[4]))));
 
            pwm.setPWM(7, 0, MID[7] - legmove*3);
            pwm.setPWM(9, 0, MID[9] - legmove*3);
            pwm.setPWM(11, 0, MID[11] - legmove*3);
            legmove++;    
        }
        for (uint16_t pulselen = MID_ALL; pulselen > BASEMIN; pulselen--) {
            pwm.setPWM(1, 0, pulselen - (MID_ALL - MID[1]));
            pwm.setPWM(3, 0, pulselen - (MID_ALL - MID[3]));
            pwm.setPWM(5, 0, pulselen - (MID_ALL - MID[5]));
            pwm.setPWM(0, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[0]))));
            pwm.setPWM(2, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[2]))));
            pwm.setPWM(4, 0, BASEMIN + (BASEMAX - (pulselen + (MID_ALL - MID[4]))));

            pwm.setPWM(7, 0, MID[7] - legmove*3);
            pwm.setPWM(9, 0, MID[9] - legmove*3);
            pwm.setPWM(11, 0, MID[11] - legmove*3);
            legmove--;        
        }
        delay(100);
    }
}
