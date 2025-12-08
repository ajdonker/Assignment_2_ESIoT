#ifndef __CONFIG__
#define __CONFIG__

#define BT_PIN     2
#define RED_PIN    13
#define GREEN1_PIN 4
#define GREEN2_PIN 12
#define MOTOR_PIN  5
#define TIMER_TWO_PIN1 3
#define TIMER_TWO_PIN2 11
#define TIMER_ONE_PIN1 9
#define TIMER_ONE_PIN2 10 // for clarity, these CANNNOT BE USED 
#define PIR_PIN 7 // check if it needs to be specific pin for any of these 
#define SONAR_TRIG_PIN 6  // currently replaced with Fakesensor dont forget! 
#define SONAR_ECHO_PIN 8
#define TEMP_PIN A3
#define LCD1_PIN A4
#define LCD2_PIN A5
#define SONAR_TIMEOUT_TIME 30000
#endif
