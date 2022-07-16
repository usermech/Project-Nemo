#include <Servo.h>
unsigned long millisStart, millisAve = 0;

//Motion Parameters
boolean isbreaked = false;
boolean timing = false;
//Mouse Parameters
String x;
int targetX1 = 0;
int targetX2 = 0;
int currX1 = 0;
int currX2 = 0;
long MouseX1 = 0; //red mouse
long MouseY1 = 0;
long MouseX2 = 0; //white mouse
long MouseY2 = 0;
char stat1,dx1,dy1,stat2,dx2,dy2;
int counter =0;
int inc =0;
byte PS2ReadByte = 0;
#define MS1CLOCK  6
#define MS1DATA   5
#define MS2CLOCK  10
#define MS2DATA   9

//Servo parameters 
Servo servo_1; // servo controller (multiple can exist)
Servo servo_2;

int servo_pin1 = 11; //On the red mouse side
int servo_pin2 =3; //On the white mouse side
int pos1 = 90;// servo starting position
int pos2 = 85;

void PS2GoHi(int pin){
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void PS2GoLo(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2Write(int mouseData, int mouseClk, unsigned char data){
  unsigned char parity=1;

  PS2GoHi(mouseData);
  PS2GoHi(mouseClk);
  delayMicroseconds(300);
  PS2GoLo(mouseClk);
  delayMicroseconds(300);
  PS2GoLo(mouseData);
  delayMicroseconds(10);
    PS2GoHi(mouseClk);

  while(digitalRead(mouseClk)==HIGH);

  for(int i=0; i<8; i++){
    if(data&0x01) PS2GoHi(mouseData);
    else PS2GoLo(mouseData);
    while(digitalRead(mouseClk)==LOW);
    while(digitalRead(mouseClk)==HIGH);
    parity^=(data&0x01);
    data=data>>1;
  }

  if(parity) PS2GoHi(mouseData);
  else PS2GoLo(mouseData);

  while(digitalRead(mouseClk)==LOW);
  while(digitalRead(mouseClk)==HIGH);

  PS2GoHi(mouseData);
  delayMicroseconds(50);

  while(digitalRead(mouseClk)==HIGH);
  while((digitalRead(mouseClk)==LOW)||(digitalRead(mouseData)==LOW));

  PS2GoLo(mouseClk);
}

unsigned char PS2Read(int mouseData, int mouseClk){
  unsigned char data=0, bit=1;

  PS2GoHi(mouseClk);
  PS2GoHi(mouseData);
  delayMicroseconds(50);
  while(digitalRead(mouseClk)==HIGH);

  delayMicroseconds(5);
  while(digitalRead(mouseClk)==LOW);

  for(int i=0; i<8; i++){
    while(digitalRead(mouseClk)==HIGH);
    if(digitalRead(mouseData)==HIGH) data|=bit;
    while(digitalRead(mouseClk)==LOW);
    bit=bit<<1;
  }

  while(digitalRead(mouseClk)==HIGH);
  while(digitalRead(mouseClk)==LOW);
  while(digitalRead(mouseClk)==HIGH);
  while(digitalRead(mouseClk)==LOW);

  PS2GoLo(mouseClk);

  return data;
}

void PS2MouseInit(int mouseData, int mouseClk){
  PS2Write(mouseData,mouseClk,0xFF);
  for(int i=0; i<3; i++) PS2Read(mouseData,mouseClk);
  PS2Write(mouseData,mouseClk,0xF0);
  PS2Read(mouseData,mouseClk);
  delayMicroseconds(100);
}

void PS2MousePos(int mouseData, int mouseClk,char &stat, char &x, char &y){
  PS2Write(mouseData,mouseClk,0xEB);
  PS2Read(mouseData,mouseClk);
  stat=PS2Read(mouseData,mouseClk);
  x=PS2Read(mouseData,mouseClk);
  y=PS2Read(mouseData,mouseClk);
}


void setup(){
  servo_1.attach(servo_pin1); // start servo control
  servo_1.write(pos1); // move servo to 0 degrees
  servo_2.attach(servo_pin2); // start servo control
  servo_2.write(pos2); // move servo to 0 
  PS2GoHi(MS1CLOCK);
  PS2GoHi(MS1DATA);
  PS2GoHi(MS2CLOCK);
  PS2GoHi(MS2DATA);

  Serial.begin(115200);
  while(!Serial); 
  Serial.println("Setup");
  PS2MouseInit(MS1DATA, MS1CLOCK);
  PS2MouseInit(MS2DATA, MS2CLOCK);
  Serial.println("Mouse Ready");
  millisStart=millis();
  MouseX1 = 0;
  MouseY1 = 0;
  MouseX2 = 0;
  MouseY2 = 0;
  Serial.setTimeout(1);
}

 
void loop(){

while (Serial.available()){
  x = Serial.readString();  
   if(x == "w"){
      targetX1 = 5;
      targetX2 = 5; 
      inc =1;
    }
    if (x == "s"){
      targetX1 = 0;
      targetX2 = 0;
      inc = 1;
    }
    if(x == "a"){
      targetX1 = 3;
      targetX2 = -3; 
      inc = 3;
    }
    if (x == "d"){
      targetX1 = -3;
      targetX2 = 3;
      inc = 3;
    }
    
}

if(millis() < millisStart){
    millisStart = millis();
}
 
  PS2MousePos(MS1DATA, MS1CLOCK,stat1,dx1,dy1);  
  PS2MousePos(MS2DATA, MS2CLOCK,stat2,dx2,dy2);
  MouseX1 += dx1;
  MouseY1 += dy1;
  MouseX2 += dx2;
  MouseY2 += dy2;
  counter += 1;
    
   if(counter == 10){
    currX1 = -MouseX1/5 ;
    currX2 = MouseX2/5 ;
    //Serial.print("\tCurrent X1=");
    //Serial.print(currX1, DEC);
    
    //Serial.print("\tCurrent X2=");
    //Serial.println(currX2, DEC);
    //Serial.print("\tTarget Y1=");
    //Serial.print(targetX1, DEC);
    
    //Serial.print("\tTarget X2=");
    //Serial.println(targetX2, DEC);
    
    
    if (currX1 < targetX1){
      pos1 -=inc;
      servo_1.write(pos1);
    }
    if (currX1 > targetX1){
      pos1 +=inc;
      servo_1.write(pos1);
    }
    
    if (currX2 < targetX2){
      pos2 +=inc;
      servo_2.write(pos2);
    }
    if (currX2 > targetX2){
      pos2 -=inc;
      servo_2.write(pos2);
    }
/*
    if (pos1<60 || pos2>115)
    {
      servo_1.write(90);
      servo_2.write(85);
       
   } 
*/


    
   MouseX1 = 0;
   MouseX2 = 0;
   MouseY1 = 0;
   MouseY2 = 0;
   counter = 0;
   //Serial.print("\tServo 1=");
   //Serial.print(pos1, DEC);
    
   //Serial.print("\tServo 2=");
   //Serial.println(pos2, DEC); 
  }
  
  
 
  
}
