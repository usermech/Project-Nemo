//Arduino Ultrasonic Sensor HC-SR04
//Re-writed by Umut Kurt

#define trigPin1 2
#define echoPin1 3
#define trigPin2 4
#define echoPin2 5


//Variables 
long duration;
float distance, RightSensor, LeftSensor ;
unsigned long timeout = 20000;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
 // pinMode(trigPin3, OUTPUT);
 // pinMode(echoPin3, INPUT);
}

void loop() {
  SonarSensor(trigPin1, echoPin1);
  RightSensor = distance;
  SonarSensor(trigPin2, echoPin2);
  LeftSensor = distance;
  //SonarSensor(trigPin3, echoPin3);
  //FrontSensor = distance;
  Serial.print(LeftSensor);
  Serial.print(" – ");
  //Serial.print(FrontSensor);
  //Serial.print(” – “);
  Serial.println(RightSensor);
}

void SonarSensor(int trigPin,int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH,timeout);
  distance = duration * 0.0343 / 2;
  }
  
