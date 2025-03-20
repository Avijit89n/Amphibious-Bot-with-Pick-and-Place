#include<Servo.h>

Servo servo1;
Servo servo2;

struct reciverdata {
  int forward_data = 0;
  int backward_data = 0;
  int left_data = 0;
  int right_data = 0;

  int hands_up = 0;
  int hands_down = 0;
  int claws_open = 0;
  int claws_close = 0;

  int sw1 = 1;
  int sw2 = 1;
};

int servopin1 = 5;
int servopin2 = 6;

int r1 = 9; 
int r2 = 8;

void setup() {
  Serial.begin(115200);
  pinMode(r1, OUTPUT);
  pinMode(r2,OUTPUT);
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo1.write(10);
  servo2.write(120);
}

unsigned long lastPacketTime = 0; 
const unsigned long packetTimeout = 200;
bool chk1 = false;
bool chk2 = false;

int pre_max_open_val = 0;
int pre_max_close_val = 0;


void loop() {
  static bool isReceiving = false;
  static uint8_t index = 0;
  static uint8_t buffer[sizeof(reciverdata)];
  struct reciverdata value_recived;

  while (Serial.available() > 0) {
    char incomingByte = Serial.read();

    if (incomingByte == '<') {
      isReceiving = true;
      index = 0;
    } else if (incomingByte == '>') {
      if (isReceiving && index == sizeof(reciverdata)) {
        memcpy(&value_recived, buffer, sizeof(reciverdata));

        Serial.print("frd data: ");
        Serial.print(value_recived.forward_data);
        Serial.print("  bkd data: ");
        Serial.print(value_recived.backward_data);
        Serial.print("  lft data: ");
        Serial.print(value_recived.left_data);
        Serial.print("  rgt data: ");
        Serial.print(value_recived.right_data);
        Serial.print("  hands up: ");
        Serial.print(value_recived.hands_up);
        Serial.print("  hands down: ");
        Serial.print(value_recived.hands_down);
        Serial.print("  claws open: ");
        Serial.print(value_recived.claws_open);
        Serial.print("  claws close: ");
        Serial.print(value_recived.claws_close);
        Serial.print(" r1: ");
        Serial.print(value_recived.sw1);
        Serial.print("  r2: ");
        Serial.print(value_recived.sw2);
        Serial.println();

        if(value_recived.claws_open > 0) chk1 = false;
        else chk1 = true;
        if(value_recived.claws_close > 0) chk2 = false;
        else chk2 = true;
        
        int val1 = value_recived.claws_open;
        int val2 = 45 - value_recived.claws_open;
        int val3 = 45 - value_recived.claws_close;
        int val4 = value_recived.claws_close;
        
        if(value_recived.claws_open > pre_max_open_val && chk1 == false){
          servo1.write(val1);
          servo2.write(val2);
          chk1 = true;
          pre_max_open_val = value_recived.claws_open;
          pre_max_close_val = pre_max_close_val - pre_max_open_val;
        }
        if(value_recived.claws_close > pre_max_close_val && chk2 == false){
          servo1.write(val3);
          servo2.write(val4);
          chk2 = true;
          pre_max_close_val = value_recived.claws_close;
          pre_max_open_val = pre_max_open_val - pre_max_close_val;
        }
        

        if(value_recived.sw1 == 0){
          digitalWrite(r1,HIGH);
        } else {
          digitalWrite(r1,LOW);
        }

        if(value_recived.sw2 == 0){
          digitalWrite(r2,HIGH);
        } else {
          digitalWrite(r2,LOW);
        }

        lastPacketTime = millis(); 
      }
      isReceiving = false;
    } else if (isReceiving) {
      if (index < sizeof(reciverdata)) {
        buffer[index++] = incomingByte;
      }
    }
  }

  if (millis() - lastPacketTime > packetTimeout) {
  }
}
