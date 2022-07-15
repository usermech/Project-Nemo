  
unsigned long millisStart, millisAve = 0;
long mouseX1 = 0;
long mouseY1 = 0;
long mouseX2 = 0;
long mouseY2 = 0;
char stat1,dx1,dy1,stat2,dx2,dy2;
int counter =0;
int currX1 = 0;
int currY1 = 0;
int currX2 = 0;
int currY2 = 0;

byte PS2ReadByte = 0;

#define MS1CLOCK  6
#define MS1DATA   5

#define MS2CLOCK  10
#define MS2DATA   9


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
  mouseX1 = 0;
  mouseY1 = 0;
  mouseX2 = 0;
  mouseY2 = 0;
}

void loop(){

if(millis() < millisStart){
    millisStart = millis();
  }
  //millisStart = millis();  
  PS2MousePos(MS1DATA, MS1CLOCK,stat1,dx1,dy1);  
  PS2MousePos(MS2DATA, MS2CLOCK,stat2,dx2,dy2);
  //millisAve += millis()- millisStart;
    counter += 1;
    mouseX1 += dx1; 
    mouseY1 += dy1;
    mouseX2 += dx2;
    mouseY2 += dy2;
    
   if(counter == 15){
    currX1 = mouseX1 /15 ;
    currY1 = mouseY1 /15 ;
    currX2 = mouseX2 /15 ;
    currY2 = mouseY2 /15;
    Serial.print("\tCurrent X1=");
    Serial.print(currX1, DEC);
    Serial.print("\tCurrent Y1=");
    Serial.print(currY1, DEC);
    Serial.print("\tCurrent X2=");
    Serial.print(currX2, DEC);
    Serial.print("\tCurrent Y2=");
    Serial.println(currY2, DEC);
    
    counter = 0;
    mouseX1 = 0; 
    mouseY1 = 0;
    mouseX2 = 0;
    mouseY2 = 0;
    
  }
  
 
  if(millis() - millisStart > 100){
    
    millisStart = millis();
  }

  //delay(1000);
}
