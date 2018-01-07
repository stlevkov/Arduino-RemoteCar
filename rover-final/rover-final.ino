#include "DataCoder.h" // for 433MHz

const int SW_pin = 7; // digital pin connected to switch output 

// This is the library for the Servo motor micro 
#include <ServoTimer2.h>
ServoTimer2 servo; // using the Class Servo
const int servo_pin = 6;

// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 2
#define AIN2 4
#define PWMA 5
#define STBY 9
// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
long mval = 0;
long mval1 = 0;
float inArray[RF_DATA_ARRAY_SIZE];
union RFData inDataSeq;

// This is the library for the Radio Transmitter and Reciever 433MHz
#include "VirtualWire.h"
const int recieve_pin = 12;
const int led_pin = 3;


void setup() {
     Serial.begin(9600);
     Serial.println("******** SKL Electronics *******");
     delay(3000);
     Serial.println("System started. Tests will start in 1 sec.");
     delay(3000);
     Serial.println("------------------------- - -----------------------");
  // Servo settings starts here *****
    servo.attach(servo_pin); // Setting pin 8 to be OUTPUT to the servo motor
    Serial.println("Servo diagnostics starts.");
    Serial.println("Servo go to middle position!");
    pinMode(SW_pin, INPUT);
    digitalWrite(SW_pin, HIGH);
    // Setting up Servo Test Fast Run 
    /*
    servo.write(1500); // Turn Middle 
    delay(2000);
    Serial.println("Servo go to Left position!");
    servo.write(1300);   // Turn Left
    delay(2000);
    Serial.println("Servo return to middle position.");
    servo.write(1500); // Turn Middle 
    delay(2000);
    Serial.println("Servo try turning to RIGHT.");
    servo.write(1700);   // Turn Right
    delay(2000);
    Serial.println("Servo return in middle position.");
    servo.write(1500); // Turn Middle 
    delay(2000); 
    */
  // Servo settings ends here *****


  // Motor settings starts Here -----
  /*
    Serial.println("Motor diagnostics starts.");
    Serial.println("Motor go forward for 1.2 seconds");
    mval = map(600, 500, 1023, 0, 255);
    for (int i = 0; i < 10; i++){
      motor1.drive(mval);
      delay(100);
    }
    Serial.println("Motor stop!");
    motor1.brake();
    mval = 0;

    Serial.println("Motor go backwards for 1.2 seconds");
    mval1 = map(300,500, 0, 0 , -255);
    for (int i = 0; i < 10; i++){  
      motor1.drive(mval1);
      delay(100);
    }
    Serial.println("Motor stop!");
    mval1 = 0;
    motor1.brake();
    delay(2000);
    */
  // Motor settings ends Here -----

  // Reciever 433MHz settings starts Here *****
     Serial.println("Reciever 433MHz start procedure begin.");
     pinMode(led_pin, OUTPUT);
     vw_set_rx_pin(recieve_pin);
     vw_set_ptt_inverted(true);
     vw_setup(1600);
     Serial.println("Starting up the listening vw_rx_start().");
     vw_rx_start();

    // test the LED 
    /*
     Serial.println("Testing the diagnostic LED ON, OFF, between 2 seconds"); 
     digitalWrite(led_pin, HIGH);
     delay(2000);
     digitalWrite(led_pin, LOW);
     delay(2000);
     digitalWrite(led_pin, HIGH);
     delay(2000);
     digitalWrite(led_pin, LOW);
     delay(2000);
     */
  // Reciever 433MHz settings ends Here *****

    Serial.println("End of diagnostics.");
    delay(1000);
    Serial.println("System boot finished.");
    Serial.println("------------------ --- ---------------------");

}


void loop() {
 //    Serial.println("Waiting for new command...");

     uint8_t buf[VW_MAX_MESSAGE_LEN];
     uint8_t buflen = VW_MAX_MESSAGE_LEN;
      
    
     if (vw_get_message(buf, &buflen)) {
      digitalWrite(led_pin, HIGH);
      for (int i = 0; i < buflen; i++){
        inDataSeq.s[i] = buf[i];
      }
      digitalWrite(led_pin, LOW);
      DecodeRFData(inArray, inDataSeq);
      for (int i = 0; i < RF_DATA_ARRAY_SIZE; i++){
        Serial.print((int)inArray[i]);
        Serial.print(", ");
      } 
      Serial.println("");

      if (inArray[0] > 510){
         Serial.println("FRONT DIRECTION ------------->");
         digitalWrite(led_pin, HIGH);
         mval = map(inArray[0], 511, 1023, 0, 255);
         motor1.drive(mval, 300);  
         
         inArray[0] = 500;   
      }else if (inArray[0] < 495){
         Serial.println("<------------ BACK DIRECTION");
         digitalWrite(led_pin, HIGH);
         mval1 = map(inArray[0], 494, 0, 0, -255);
         motor1.drive(mval1, 300);
         inArray[0] = 500; 
      } else {
        motor1.standby();
        inArray[0] = 500; 
      }

      if (inArray[1] > 530){
          Serial.println("RIGHT TURN >>>>>>>>>>>>");
         servo.write(map(inArray[1], 531, 1023, 1500, 2500));
         delay(300);
      } else if (inArray[1] < 495){
         digitalWrite(led_pin, HIGH);
         Serial.println("<<<<<<<<<< LEFT TURN");
         servo.write(map(inArray[1], 494, 0, 1500, 1300));  
         delay(300);   
      } 
      

    } else {
      motor1.standby();
    }
       
}


