#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define joy1_frd_bck A0
#define joy1_lft_rgt A1
#define joy2_frd_bck A2
#define joy2_lft_rgt A3
int joy1_btn = 2;
int joy2_btn = 3;
int step_btn = 4;

struct values{
  int joy1_forward_backward_val = 0;
  int joy1_left_right_val = 0;
  int joy2_forward_backward_val = 0;
  int joy2_left_right_val = 0;
  int joy1_sw = 1;
  int joy2_sw = 1;
  int step_sw = 0;
};

RF24 radio(7, 8);
const byte address[6] = "IETE2";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();

  pinMode(joy1_frd_bck, INPUT);
  pinMode(joy1_lft_rgt, INPUT);
  pinMode(joy2_frd_bck, INPUT);
  pinMode(joy2_lft_rgt, INPUT);
  pinMode(joy1_btn, INPUT);
  pinMode(joy2_btn, INPUT);

  digitalWrite(joy1_btn, HIGH);
  digitalWrite(joy2_btn, HIGH);
}

struct values controller_data;

void loop() {
  controller_data.joy1_forward_backward_val = analogRead(joy1_frd_bck);
  controller_data.joy1_left_right_val = analogRead(joy1_lft_rgt);
  controller_data.joy2_forward_backward_val = analogRead(joy2_frd_bck);
  controller_data.joy2_left_right_val = analogRead(joy2_lft_rgt);
  controller_data.joy1_sw = digitalRead(joy1_btn);
  controller_data.joy2_sw = digitalRead(joy2_btn);
  controller_data.step_sw = digitalRead(step_btn);
  
  Serial.print("j1_frd_bkd: ");
  Serial.print(controller_data.joy1_forward_backward_val);
  Serial.print("  j1_lft_rgt: ");
  Serial.print(controller_data.joy1_left_right_val);
  Serial.print("  j2_frd_bkd: ");
  Serial.print(controller_data.joy2_forward_backward_val);
  Serial.print("  j2_lft_rgt: ");
  Serial.print(controller_data.joy2_left_right_val);
  Serial.print("  j1_sw: ");
  Serial.print(controller_data.joy1_sw);
  Serial.print("  j2_sw: ");
  Serial.print(controller_data.joy2_sw);
  Serial.print("  jumper_sw: ");
  Serial.print(controller_data.step_sw);

  
  Serial.println();
  radio.write(&controller_data, sizeof(controller_data));
}
