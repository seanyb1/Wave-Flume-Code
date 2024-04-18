const int padTrigPin = 5;
const int padEchoPin = 4;
const int wave1TrigPin = 6;
const int wave1EchoPin = 7;
const int wave2TrigPin = 9;
const int wave2EchoPin = 10;
const int directionPin = 12;
const int pwmPin = 3;
const int pumpPwmPin = 11;
const float stroke = 10;
//const int overTopPin = A5;
//const int wavePressurePin = A4;
const float g = 98;
const float h = 7;
float xMax = 3;
float xMin = 12.7;
float m = stroke/(xMin-xMax); // m and k are conversion metrics for paddle distance with stroke 10cm
float k = -m*xMax;
bool direct = HIGH;
float kp = 2;
float kd = 0;
float startTime;
float frequency;
float prevError = 0;
float initialHeight;
float prevPos = 0;

// UNITS ARE CM and S

void setup() {
  // put your setup code here, to run once:
  pinMode(padTrigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(padEchoPin, INPUT); // Sets the echoPin as an Input
  pinMode(wave1TrigPin, OUTPUT);
  pinMode(wave1EchoPin, INPUT);
  pinMode(wave2TrigPin, OUTPUT);
  pinMode(wave2EchoPin, INPUT);
 // pinMode(wavePressurePin,INPUT);
 // pinMode(overTopPin,INPUT);
  digitalWrite(directionPin,direct);
  Serial.begin(9600); // Starts the serial communication
  //initialHeight = measureWaveHeight(wave1TrigPin,wave1EchoPin);
  float wavelength = 100;
  float k = (2*PI)/wavelength;
  frequency = sqrt(g*k*tanh(k*h));
  startTime = millis();
}

void loop() { 
  // put your main code here, to run repeatedly:
  
  while((millis() -startTime) < 5*1.24*1000){ 
    analogWrite(pwmPin,160); // 175 FOR 1.24 
    readResults();
  }
  while(measurePadDistance(padTrigPin,padEchoPin) > 0.1){
    analogWrite(pwmPin,50);
    readResults();
  }
  analogWrite(pwmPin,0);
  readResults();
}
void readResults(){
  float distance = measurePadDistance(padTrigPin,padEchoPin);
  float waveDistance1 = measureWaveHeight(wave1TrigPin,wave1EchoPin);
  float waveDistance2 = measureWaveHeight(wave2TrigPin,wave2EchoPin);
  //int pwm = propPWM(distance);
  Serial.print(millis());Serial.print(",");
  //Serial.print(""); Serial.print(waveMakerIdealPos()); Serial.print(",");
  Serial.print(""); Serial.print(distance); Serial.print(",");  
  //Serial.print(""); Serial.print(pwm); Serial.print(",");
  Serial.print("");Serial.print(waveDistance1); Serial.print(",");
  //Serial.print("Pressure:");Serial.print(pressureHeight(wavePressurePin));Serial.print("  ");
  Serial.print(""); Serial.print(waveDistance2);Serial.print("");
  Serial.println("");
  //delay(10);

}
float waveMakerIdealPos(){
  return -(stroke/2) * cos(frequency*(millis()-startTime)/1000) + (stroke/2);
}

float measurePadDistance(int trigPin,int echoPin){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return m*duration*0.034/2 + k;
}
float measureWaveHeight(int trigPin,int echoPin){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return duration*0.034/2;
}

int propPWM(float padDistance){
  int constPWM;
  float idealPos = waveMakerIdealPos();
  if (idealPos > prevPos){
    constPWM = 100;
  }
  else{
    constPWM = -100;
  }
  float error = idealPos - padDistance;
  int pwm = constPWM+round(2.55*kp*error + 2.55*kd*(error-prevError));
  prevError = error;
  if (pwm > 0){
    digitalWrite(directionPin, HIGH);
  }
  else{
    digitalWrite(directionPin, LOW);
  }
  pwm = abs(pwm);
  if (pwm > 255){
    pwm = 255;
    }
  analogWrite(pwmPin,pwm);
  return(pwm);
  }


