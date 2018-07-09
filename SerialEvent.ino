/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


long duration;
int  distance;

int object = 0;

const int trigPinL = 6;
const int echoPinL = 7;

const int trigPinR = 8;
const int echoPinR = 9;


const int trigPinF = 10;
const int echoPinF = 11;

const int range = 70;


int checkLeft(){
    digitalWrite(trigPinL, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPinL, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinL, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPinL, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    //Serial.print("L Distance: ");
    //Serial.println(distance);
    if(range > distance)
     return 1;
    else
     return 0;
    //return 1;
}


int checkRight(){
    digitalWrite(trigPinR, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPinR, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinR, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPinR, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    //Serial.print("R Distance: ");
    //Serial.println(distance);
    if(range > distance)
     return 1;
    else
     return 0;
}


int checkFront(){
    digitalWrite(trigPinF, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPinF, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinF, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPinF, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    //Serial.print("F Distance: ");
    //Serial.println(distance);
    if(range > distance)
     return 1;
    else
     return 0;
}


void setup() {
  // initialize serial:
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT); // Sets the echoPin as an Input

  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT); // Sets the echoPin as an Input

  pinMode(trigPinF, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinF, INPUT); // Sets the echoPin as an Input

  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
  
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
}


void trunRight(){
  digitalWrite(A0,HIGH);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,HIGH);
}
void trunLeft(){
   digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,LOW);
}
void stopVehicle(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
}
void forward(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,LOW);
  digitalWrite(A3,HIGH);
 }


void debugUltrasonic(){

  if(checkFront()){
    Serial.println("Front");
  }
  if(checkLeft()){
    Serial.println("Left");
  }
  if(checkRight()){
    Serial.println("Right");
  }
}
 int l,r,f;
 int btl = 0;
 int btr = 0;
 int btf = 0;
 int bts = 1;

void loop() {
  // print the string when a newline arrives:
  
//debugUltrasonic();

  f  = checkFront();
  object=0;
  
  if(f == 1 ){
     object = 1;
      Serial.println("UL Front");
    l = checkLeft();
    if(l == 1){
       r = checkRight();
       Serial.println("UL Left");
       if(r == 1){
        stopVehicle();
        Serial.println("UL Stop");
       }
       else{
        if(bts!=1)
        trunRight();
       }
    }
    else{
      if(bts!=1)
      trunLeft();
    }
  }
  else{
    object = 0;
  }
  

    if(btl == 1 && object == 0 ){
      trunLeft();
    }
    if(btr == 1 && object == 0){
      trunRight();
    }
    if(btf == 1 && object == 0){
      forward();
    }
    if(bts == 1 && object == 0){
      stopVehicle();
    }
  
  if (stringComplete) {
    //Serial.println(inputString);
    if(inputString == "F\n" ){
      btl =0;
      btr = 0;
      btf = 1;
      bts = 0;
       Serial.println("btf");
      //forward();
    }
    else if(inputString == "R\n" ){
      btl = 0;
      btr = 1;
      btf = 0;
      bts = 0;
       Serial.println("btR");
      //trunRight();
    }
    else if(inputString == "L\n" ){
      btl = 1;
      btr = 0;
      btf = 0;
      bts = 0;
      //trunLeft();
       Serial.println("btL");
    }
    else if(inputString == "S\n"){
      btl = 0;
      btr = 0;
      btf = 0;
      bts = 1;
       Serial.println("btS");
       //stopVehicle();
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


