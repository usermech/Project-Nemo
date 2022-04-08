//Arduino Ultrasonic Sensor HC-SR04
//Re-writed by Umut Kurt

#define trigPin 11
#define echoPin 12

//Variables 
long duration;
float distance;

void setup() {
  pinMode(trigPin,OUTPUT);    //Define trigger pin as output pin
  pinMode(echoPin, INPUT);    //Define echo pin as input pin
  
  Serial.begin(9600);       //Serial Communication is started
}

void loop()  {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);         // Sends a LOW pulse for 3 microseconds to clear the trigPin before HIGH pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);        //Sends a HIGH pulse for 10 microseconds to trigger the sensor
  digitalWrite(trigPin, LOW);
  
//Read the signal from echo pin for the duration of travelling soundwave 
  duration = pulseIn(echoPin, HIGH);
//Convert duration to distance
  distance = duration * 0.0343 / 2;
//Display from serial monitor
  Serial.print(distance);
  delay(10);
}
  
