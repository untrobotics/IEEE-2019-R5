/*
* @file Motor driver DRI0042_Test.ino
* @brief DRI0042_Test.ino  Motor control program
*
* control motor positive inversion
* 
* @author bernie.chen@dfrobot.com
* @version  V1.0
* @date  2016-8-10
*/

// Motors 1 and 2
// Motor Driver 1
// 1: Front Left
// 2: Back Left
const int IN1_D1=22;
const int IN2_D1=23;
const int ENA_D1=2;

const int IN3_D1=24;
const int IN4_D1=25;
const int ENB_D1=3;

// Motors 3 and 4
// Motor Driver 2
// 3: Front Right
// 4: Back Right
const int IN1_D2=26;
const int IN2_D2=27;
const int ENA_D2=4;

const int IN3_D2=28;
const int IN4_D2=29;
const int ENB_D2=5;

void setup() {
     Serial.begin(9600);
     pinMode(IN1_D1, OUTPUT);
     pinMode(IN2_D1, OUTPUT);
     pinMode(ENA_D1, OUTPUT);
     
     pinMode(IN4_D1, OUTPUT);
     pinMode(IN3_D1, OUTPUT);
     pinMode(ENB_D1, OUTPUT);

     pinMode(IN1_D2, OUTPUT);
     pinMode(IN2_D2, OUTPUT);
     pinMode(ENA_D2, OUTPUT);
     
     pinMode(IN4_D2, OUTPUT);
     pinMode(IN3_D2, OUTPUT);
     pinMode(ENB_D2, OUTPUT);
}

void loop() {
 brake();
 delay(100);
 
 forward(50);
 delay(5000);
 
 brake();
 delay(100);
 
 backward(50);
 delay(5000); 
}

void brake() {
  Motor1_Brake();
  Motor2_Brake();
  Motor3_Brake();
  Motor4_Brake();
}

void forward(int Speed) {
  Motor1_Forward(Speed);
  Motor2_Forward(Speed);
  Motor3_Forward(Speed);
  Motor4_Forward(Speed);
}

void backward(int Speed) {
  Motor1_Backward(Speed);
  Motor2_Backward(Speed);
  Motor3_Backward(Speed);
  Motor4_Backward(Speed);
}

// forward
void Motor1_Forward(int Speed) {
     Serial.println("1 Forward");
     digitalWrite(IN1_D1,HIGH); 
     digitalWrite(IN2_D1,LOW);  
     analogWrite(ENA_D1,Speed);
}

void Motor2_Forward(int Speed) {
     digitalWrite(IN3_D1,HIGH); 
     digitalWrite(IN4_D1,LOW);  
     analogWrite(ENB_D1,Speed);
}

void Motor3_Forward(int Speed) {
     Serial.println("1 Forward");
     digitalWrite(IN1_D2,HIGH); 
     digitalWrite(IN2_D2,LOW);  
     analogWrite(ENA_D2,Speed);
}

void Motor4_Forward(int Speed) {
     digitalWrite(IN3_D2,HIGH); 
     digitalWrite(IN4_D2,LOW);  
     analogWrite(ENB_D2,Speed);
}


// brakes
void Motor1_Brake() {
     digitalWrite(IN1_D1,LOW); 
     digitalWrite(IN2_D1,LOW); 
}

void Motor2_Brake() {
     digitalWrite(IN3_D1,LOW); 
     digitalWrite(IN4_D1,LOW); 
}

void Motor3_Brake() {
     digitalWrite(IN1_D2,LOW); 
     digitalWrite(IN2_D2,LOW); 
}

void Motor4_Brake() {
     digitalWrite(IN3_D2,LOW); 
     digitalWrite(IN4_D2,LOW); 
}

// backward
void Motor1_Backward(int Speed) {    
     Serial.println("1 Back");
     digitalWrite(IN1_D1,LOW); 
     digitalWrite(IN2_D1,HIGH);  
     analogWrite(ENA_D1,Speed);
}

void Motor2_Backward(int Speed) {    
     digitalWrite(IN3_D1,LOW); 
     digitalWrite(IN4_D1,HIGH);  
     analogWrite(ENB_D1,Speed);
}

void Motor3_Backward(int Speed) {    
     Serial.println("1 Back");
     digitalWrite(IN1_D2,LOW); 
     digitalWrite(IN2_D2,HIGH);  
     analogWrite(ENA_D2,Speed);
}

void Motor4_Backward(int Speed) {    
     digitalWrite(IN3_D2,LOW); 
     digitalWrite(IN4_D2,HIGH);  
     analogWrite(ENB_D2,Speed);
}
