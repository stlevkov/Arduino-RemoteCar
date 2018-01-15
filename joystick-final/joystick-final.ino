// Include Transmitter 433MHz library

#include "DataCoder.h"
#include "VirtualWire.h"

//Asign pin numbers for the Joystick
const int SW_pin = 7; // digital pin connected to switch output 
const int X_pin = 0;  // analog pin connected to X output
const int Y_pin = 1;  // analog pin connected to Y output

void setup() {
  // ***** Transmitter settings *****
  vw_set_tx_pin(12);
  vw_set_ptt_inverted(true); 
  vw_setup(1600);
  pinMode(2, OUTPUT);
  
  // put your setup code here, to run once:
 // For the Joystick
 // Reading from the Joystick is as follows:
 // Reading X - Axis Front to back, the positions are 1023 - 500 - 0 while 500 is the middle (idle) stage
 // Reading Y - Axis Left to Right the pistions are 0 - 520 - 1023 while the 520 is the middle (idle) stage
 pinMode(SW_pin, INPUT);
 digitalWrite(SW_pin, HIGH);
 // Serial.begin(9600);
}

void loop() {
  

  
    Serial.print("X_pin: ");
    Serial.print(analogRead(X_pin));
    Serial.print(" ");
    Serial.print(" Y_pin: ");
    Serial.print(analogRead(Y_pin));
    Serial.print(" ");
    Serial.print("SW_pin: ");
    Serial.print(digitalRead(SW_pin));
    Serial.println(" ");
  // -----------------------------------------
    float outArray[RF_DATA_ARRAY_SIZE];
    outArray[0] = analogRead(X_pin);
    outArray[1] = analogRead(Y_pin);
    outArray[2] = digitalRead(SW_pin);

    union RFData outDataSeq;

    EncodeRFData(outArray, outDataSeq);
  //  TransmitRFData(outDataSeq);
    
  // ****************************************
   if (analogRead(X_pin) > 510 ){
    digitalWrite(2, HIGH);
   //   const char *msg = "X DIRECT GO FORWARD";
      Serial.print("Sending Data: ");
      Serial.print(outArray[0]);
      Serial.print(", ");
      Serial.print(outArray[1]);
      Serial.println("");
      TransmitRFData(outDataSeq);
    //  vw_send((uint8_t *)msg, strlen(msg));
     // vw_wait_tx();
      digitalWrite(2, LOW);
   } else  if(analogRead(X_pin) < 495){
   //  const char *msgb = "X DIRECT GO BACKWARD.";
     digitalWrite(2, HIGH);
  //   delay(30);
     TransmitRFData(outDataSeq);
  //   vw_send((uint8_t *)msgb, strlen(msgb));
  //   vw_wait_tx();
     digitalWrite(2, LOW);
   } else if (analogRead(Y_pin) > 525){
        digitalWrite(2, HIGH);
     // delay(30);
      Serial.print("Sending Data: ");
      Serial.print(outArray[0]);
      Serial.print(", ");
      Serial.print(outArray[1]);
      Serial.println("");
      TransmitRFData(outDataSeq);
    //  vw_send((uint8_t *)msg, strlen(msg));
     // vw_wait_tx();
      digitalWrite(2, LOW);
   } else if (analogRead(Y_pin) < 510){
        digitalWrite(2, HIGH);
     // delay(30);
      Serial.print("Sending Data: ");
      Serial.print(outArray[0]);
      Serial.print(", ");
      Serial.print(outArray[1]);
      Serial.println("");
      TransmitRFData(outDataSeq);
    //  vw_send((uint8_t *)msg, strlen(msg));
     // vw_wait_tx();
      digitalWrite(2, LOW);
   } else if (digitalRead(SW_pin) == LOW){
        digitalWrite(2, HIGH);
     // delay(30);
      Serial.print("Sending Data: ");
      Serial.print(outArray[0]);
      Serial.print(", ");
      Serial.print(outArray[1]);
      Serial.println("");
      TransmitRFData(outDataSeq);
    //  vw_send((uint8_t *)msg, strlen(msg));
     // vw_wait_tx();
      digitalWrite(2, LOW);
   }
   digitalWrite(2, LOW);
}
