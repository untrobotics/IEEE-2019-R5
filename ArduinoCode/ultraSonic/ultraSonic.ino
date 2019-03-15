 //code comes from https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
const int trigOutput = 10;    // Trig pin
const int echoInput = 11;    // Echo pin
double dist, dura = 0; //distance is going to be in mm

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigOutput, OUTPUT);
  pinMode(echoInput, INPUT);
}

void loop() {
  //starts with low push to clear area for use of high then goes back to low
  digitalWrite(trigOutput, LOW);
  delayMicroseconds(10);
  digitalWrite(trigOutput, HIGH);
  delayMicroseconds(20);
  //pulseIn() uses microseconds to time the speed of sound, so program needs delayMicroseconds or pulseIn will time out
  dura = pulseIn(echoInput, HIGH);// reads in time it took to pulse to object
   // Convert the time into a distance aka mm/s
  dist = (dura/2) * .343;     // speed of sound is 343 meters/second
  Serial.print(dist);
  Serial.print("mm/s");
  Serial.println();
  //dealy in milliseconds to next fire of 
  delay(250);
} 

