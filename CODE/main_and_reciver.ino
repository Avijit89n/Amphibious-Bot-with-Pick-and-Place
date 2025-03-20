#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int LN1 = 2;
int LN2 = 4;


int LNen = 3;

#define default_bck_frd_joy1 511
#define default_lft_rig_joy1 523
#define default_bck_frd_joy2 517
#define default_lft_rig_joy2 507

#define bts1_l_rpm 5
#define bts1_r_rpm 6
#define bts2_l_rpm 9
#define bts2_r_rpm 10

int max_val = 255;

RF24 radio(7, 8);
const byte address[6] = "IETE2";

struct values{
  int joy1_forward_backward_val = 0;
  int joy1_left_right_val = 0;
  int joy2_forward_backward_val = 0;
  int joy2_left_right_val = 0;
  int joy1_sw = 1;
  int joy2_sw = 1;
  int step_sw = 0;
};

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

void forward(int fd){
  analogWrite(bts1_l_rpm, fd); 
  analogWrite(bts1_r_rpm, 0);

  analogWrite(bts2_l_rpm, fd); 
  analogWrite(bts2_r_rpm, 0);
}

void backward(int bk){
  analogWrite(bts1_l_rpm, 0); 
  analogWrite(bts1_r_rpm, bk);

  analogWrite(bts2_l_rpm, 0); 
  analogWrite(bts2_r_rpm, bk);
}

void left(int lf){
  analogWrite(bts1_l_rpm, 0); 
  analogWrite(bts1_r_rpm, lf);

  analogWrite(bts2_l_rpm, lf); 
  analogWrite(bts2_r_rpm, 0);
}

void right(int rg){
  analogWrite(bts1_l_rpm, rg); 
  analogWrite(bts1_r_rpm, 0);

  analogWrite(bts2_l_rpm, 0); 
  analogWrite(bts2_r_rpm, rg);
}

void forward_left(int fd, int lf, int max_val){
  int normal_val = map(fd+lf, 0, 2*max_val, 0, max_val);
  analogWrite(bts1_l_rpm, 0); 
  analogWrite(bts1_r_rpm, 0);

  analogWrite(bts2_l_rpm, normal_val); 
  analogWrite(bts2_r_rpm, 0);
}

void backward_left(int bk, int lf, int max_val){
  int normal_val = map(bk+lf, 0, 2*max_val, 0, max_val);
  analogWrite(bts1_l_rpm, 0); 
  analogWrite(bts1_r_rpm, 0);

  analogWrite(bts2_l_rpm, 0); 
  analogWrite(bts2_r_rpm, normal_val);
}

void forward_right(int fd, int rg, int max_val){
  int normal_val = map(fd+rg, 0, 2*max_val, 0, max_val);
  analogWrite(bts1_l_rpm, normal_val); 
  analogWrite(bts1_r_rpm, 0);

  analogWrite(bts2_l_rpm, 0); 
  analogWrite(bts2_r_rpm, 0);
}

void backward_right(int bk, int rg, int max_val){
  int normal_val = map(bk+rg, 0, 2*max_val, 0, max_val);
  analogWrite(bts1_l_rpm, 0); 
  analogWrite(bts1_r_rpm, normal_val);

  analogWrite(bts2_l_rpm, 0); 
  analogWrite(bts2_r_rpm, 0);
}

void stop(){
  analogWrite(bts1_l_rpm, 0); 
  analogWrite(bts1_r_rpm, 0);

  analogWrite(bts2_l_rpm, 0); 
  analogWrite(bts2_r_rpm, 0);
}

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  pinMode(bts1_l_rpm, OUTPUT);
  pinMode(bts1_r_rpm, OUTPUT);
  pinMode(bts2_l_rpm, OUTPUT);
  pinMode(bts2_r_rpm, OUTPUT);
}

void hands_up(int speed){
  digitalWrite(LN1, HIGH);
  digitalWrite(LN2, LOW);

  analogWrite(LNen, speed);
}

void hands_down(int speed){
  digitalWrite(LN1, LOW);
  digitalWrite(LN2, HIGH);

  analogWrite(LNen, speed);
}

void hands_stop(){
  digitalWrite(LN1, LOW);
  digitalWrite(LN2, LOW);
}

struct values controller_data;
struct reciverdata reciver_data;
unsigned long lastSignalTime = 0;
unsigned long signalTimeout = 200;


void loop() {
  if(radio.available()){
    lastSignalTime = millis();
    radio.read(&controller_data, sizeof(controller_data));
    
    if(controller_data.step_sw == 0) max_val = 180;
    else max_val = 255;

    reciver_data.forward_data = map(controller_data.joy1_forward_backward_val, default_bck_frd_joy1+1, 1023, 0, max_val);
    reciver_data.backward_data = map(controller_data.joy1_forward_backward_val, default_bck_frd_joy1-1, 0, 0, max_val);
    reciver_data.left_data = map(controller_data.joy1_left_right_val, default_lft_rig_joy1-1, 0, 0, max_val);
    reciver_data.right_data = map(controller_data.joy1_left_right_val, default_lft_rig_joy1+1, 1023, 0, max_val);

    reciver_data.hands_up = map(controller_data.joy2_forward_backward_val, default_bck_frd_joy2+1, 1023, 0, max_val);
    reciver_data.hands_down = map(controller_data.joy2_forward_backward_val, default_bck_frd_joy2-1, 0, 0, max_val);
    reciver_data.claws_open = map(controller_data.joy2_left_right_val, default_lft_rig_joy2-1, 0, 0, 120);
    reciver_data.claws_close = map(controller_data.joy2_left_right_val, default_lft_rig_joy2+1, 1023, 0, 120);


    if(reciver_data.forward_data < 0) reciver_data.forward_data = 0;
    if(reciver_data.backward_data < 0) reciver_data.backward_data = 0;
    if(reciver_data.left_data < 0) reciver_data.left_data = 0;
    if(reciver_data.right_data < 0) reciver_data.right_data = 0;

    if(reciver_data.hands_up < 0) reciver_data.hands_up = 0;
    if(reciver_data.hands_down < 0) reciver_data.hands_down = 0;
    if(reciver_data.claws_open < 0) reciver_data.claws_open = 0;
    if(reciver_data.claws_close < 0) reciver_data.claws_close = 0;

    if(reciver_data.hands_up > 0){
      hands_up(reciver_data.hands_up);
    }
    else if(reciver_data.hands_down > 0){
      hands_down(reciver_data.hands_down);
    }
    else{
      hands_stop();
    }

    if(reciver_data.forward_data > 0 && reciver_data.backward_data == 0 && reciver_data.left_data == 0 && reciver_data.right_data == 0) forward(reciver_data.forward_data);
    else if(reciver_data.forward_data == 0 && reciver_data.backward_data > 0 && reciver_data.left_data == 0 && reciver_data.right_data == 0) backward(reciver_data.backward_data);
    else if(reciver_data.forward_data == 0 && reciver_data.backward_data == 0 && reciver_data.left_data > 0 && reciver_data.right_data == 0) left(reciver_data.left_data);
    else if(reciver_data.forward_data == 0 && reciver_data.backward_data == 0 && reciver_data.left_data == 0 && reciver_data.right_data > 0) right(reciver_data.right_data);

    else if(reciver_data.forward_data > 0 && reciver_data.backward_data == 0 && reciver_data.left_data > 0 && reciver_data.right_data == 0) forward_left(reciver_data.forward_data, reciver_data.left_data, max_val);
    else if(reciver_data.forward_data == 0 && reciver_data.backward_data > 0 && reciver_data.left_data > 0 && reciver_data.right_data == 0) backward_left(reciver_data.backward_data, reciver_data.left_data, max_val);
    else if(reciver_data.forward_data > 0 && reciver_data.backward_data == 0 && reciver_data.left_data == 0 && reciver_data.right_data > 0) forward_right(reciver_data.forward_data, reciver_data.right_data, max_val);
    else if(reciver_data.forward_data == 0 && reciver_data.backward_data > 0 && reciver_data.left_data == 0 && reciver_data.right_data > 0) backward_right(reciver_data.backward_data, reciver_data.right_data, max_val);
    else if(reciver_data.forward_data == 0 && reciver_data.backward_data == 0 && reciver_data.left_data == 0 && reciver_data.right_data == 0) stop();

    Serial.print("frd data: ");
    Serial.print(reciver_data.forward_data);
    Serial.print("  bkd data: ");
    Serial.print(reciver_data.backward_data);
    Serial.print("  lft data: ");
    Serial.print(reciver_data.left_data);
    Serial.print("  rgt data: ");
    Serial.print(reciver_data.right_data);
    Serial.print("  hands up: ");
    Serial.print(reciver_data.hands_up);
    Serial.print("  hands down: ");
    Serial.print(reciver_data.hands_down);
    Serial.print("  claws open: ");
    Serial.print(reciver_data.claws_open);
    Serial.print("  claws close: ");
    Serial.print(reciver_data.claws_close);

    reciver_data.sw1 = controller_data.joy1_sw;
    reciver_data.sw2 = controller_data.joy2_sw;

    Serial.write('<');
    Serial.write((uint8_t *)&reciver_data, sizeof(reciver_data)); 
    Serial.write('>');

    Serial.println();
  }else if (millis() - lastSignalTime > signalTimeout) {
    stop();
  }
} 
