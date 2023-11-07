#include <Servo.h>


// TODO: we need to remove all Serial stuff so we dont send stuff into nowhere while its not needed

/*
HERE WE DEFINE ON WHAT PINS WHAT IS...
*/

const int photo1Pin = A0;   // Black
const int photo2Pin = A1;   // Red
const int ButtonPin = 2;    // Blue


// Motor-RELAYS:
const int mRelay1 = 8;         // Positive
const int mRelay2 = 9;         // Negative 1     for: opening    motor +
const int mRelay3 = 10;        // Negative 2     for: closing    motor -

// Laser-REALS:                                                   (so wee dont waste too much power)
const int lRelay1 = 7;         // Positive
const int lRelay2 = 8;         // Negative

const int openCloseDelay = 1000; //TODO: NEEDS TO BE CONFIURED SO IT FULLY OPENS/CLOSES DOOR


const int DayNight = A5;

volatile int startingLvL1 = 300;  // Here we define when we don't see the laser
volatile int startingLvL2 = 300;
volatile bool ON = false;

const int dayNightSwitch = 500;
int OFFcounter = 0;
int count = -1;

bool photo1Off = false;
bool photo2Off = false;

Servo myservo;
Stepper myStepper = Stepper(stepsPerRevolution, stepperIn1, stepperIn2, stepperIn3, stepperIn4);

#define servoPin 6

int InOut = 0;    // 0 -> nothing; 1 -> in; 2 -> out;

int timeDelay = 0;


// the setup function runs once when you press reset or power the board
void setup() {

  // initialize Relay-Pins:
  pinMode(mRelay1, OUTPUT);
  pinMode(mRelay2, OUTPUT);
  pinMode(mRelay3, OUTPUT);
  pinMode(lRelay1, OUTPUT);
  pinMode(lRelay2, OUTPUT);


  pinMode(3, OUTPUT);
  
  // initialize the pushbutton pin as an input:
  pinMode(ButtonPin, INPUT);

  // Attach an interrupt to the ISR vector
  
  Serial.begin(9600);
  
  attachInterrupt(digitalPinToInterrupt(ButtonPin) , ISR_Button, RISING);
  
 }

// the loop function runs over and over again forever
void loop() {


  if(analogRead(DayNight) > dayNightSwitch && count == 4){
    night();
  }

  if (ON){
    
    Serial.print("DayNight: ");
    Serial.print(analogRead(DayNight));

    digitalWrite(lRelay1, HIGH);              // we turn the Lasers On and off so we can preserve Energy.
    digitalWrite(lRelay2, HIGH);

    int photo1 = analogRead(photo1Pin);
    int photo2 = analogRead(photo2Pin);

    digitalWrite(lRelay1, LOW);
    digitalWrite(lRelay2, LOW);
    
    Serial.print(" | photo1: ");
    Serial.print(photo1);
    Serial.print(" | photo2: ");
    Serial.print(photo2);
    
    Serial.print(" | Count: ");
    Serial.print(count);
    
    counter();

    if (photo1 >= startingLvL1){
      photo1Off = true;
        
    }else{
      photo1Off = false;
    }

    if(photo2 >= startingLvL2){
      photo2Off = true;      
        
    }else{
      photo2Off = false;    
        
    }  
     
  }else{
    OFFcounter++;
    if(OFFcounter == 100){
      
      Serial.println("OFF");
      count = -1;
      OFFcounter = 0;
    }
    delay(50);
    }

  
}

void night(){

  /*
    here we have the 'night-loop' here we dont need to count the chickens so we only need to check the outside photo-resistor
    from time to time. Ande when its bright enough we open the servo and than open the motor.
  */
  
  open();

  while(analogRead(DayNight) > dayNightSwitch){
    
    Serial.println(analogRead(A5));
    delay(1000);
  }
  
  close();

  // TODO: here we need to open and close the second Relay1 which is reversed from the other 
}

void open(){

  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  
  delay(openCloseDelay);
  
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);

  myservo.write(90);
  delay(10);  

  return;
}

void close(){

  digitalWrite(Relay1, LOW);
  digitalWrite(Relay3, HIGH);
  
  delay(openCloseDelay);
  
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay3, LOW);

  myservo.write(0);
  delay(10);  

  return;

}


void counter(){
  
  Serial.print(" | IN/OUT: ");
  
  if(photo1Off && photo2Off){
    if(InOut == 2){
      Serial.println("IN");  
      count++;
      if(count > 4){
        count = 4;
        }
      
    }else if(InOut == 1){
      Serial.println("OUT");
      count--;    
      if(count < 0){
        count = 0;
      }else if(count == 3){
      }
    }else{
      Serial.println("NAN");  
    }
    InOut = 0;
    
  }else if(photo1Off && !photo2Off){
    Serial.print("RUN");
    if(InOut == 3 || InOut == 1){
      Serial.println(" OUT");
      InOut = 1;
    }
    else{
      Serial.println("");
    }
  }else if(!photo1Off && photo2Off){
    Serial.print("RUN");
    if(InOut == 3 || InOut == 2){
      Serial.println(" IN");
      InOut = 2;
    }
    else{
      Serial.println("");
    }
    
  }else if(!photo1Off && !photo2Off){
    InOut = 3;
    Serial.println("RUN");  
  }
}

  
void ISR_Button() {
  ON = !ON;
  if(ON){
    count = 0;
    startingLvL1 = analogRead(photo1Pin) - 50;
    startingLvL2 = analogRead(photo2Pin) - 50;
  }else{
    count = -1;
  }
  
}
