#include <Servo.h>
#include <Stepper.h>

const int photo1Pin = A0;   // Black
const int photo2Pin = A1;   // Red
const int ButtonPin = 2;    // Blue


/*
const int sevenA = 7;
const int sevenB = 8;
const int sevenC = 9;
const int sevenD = 10;
const int sevenE = 11;
const int sevenF = 12;
const int sevenG = 13;
*/

const int LED3Pin = 10;   // LED for number 0
const int LED4Pin = 11;   // LED for number 1
const int LED5Pin = 12;   // LED for number 2
const int LED6Pin = 13;   // LED for number 3  
const int LED7Pin = 9;   // LED for number 4  

const int stepperIn1 = 8;
const int stepperIn2 = 7;
const int stepperIn3 = 5;
const int stepperIn4 = 4;

const int stepsPerRevolution = 2048;



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

// DRAW NUMBERS :
/*
void draw7Seg(){
  if (count == 0){
    // 0 = A B C E F G
    digitalWrite(sevenA, HIGH);
    digitalWrite(sevenB, HIGH);
    digitalWrite(sevenC, HIGH);
    digitalWrite(sevenD, LOW);
    digitalWrite(sevenE, HIGH);
    digitalWrite(sevenF, HIGH);
    digitalWrite(sevenG, HIGH);
    
  }else if(count == 1){
    // 1 = F C
    digitalWrite(sevenA, LOW);
    digitalWrite(sevenB, LOW);
    digitalWrite(sevenC, HIGH);
    digitalWrite(sevenD, LOW);
    digitalWrite(sevenE, LOW);
    digitalWrite(sevenF, HIGH);
    digitalWrite(sevenG, LOW);
    
  }else if(count == 2){
    // 2 = A B D F G
    digitalWrite(sevenA, HIGH);
    digitalWrite(sevenB, HIGH);
    digitalWrite(sevenC, LOW);
    digitalWrite(sevenD, HIGH);
    digitalWrite(sevenE, LOW);
    digitalWrite(sevenF, HIGH);
    digitalWrite(sevenG, HIGH);
    
  }else if(count == 3){
    // 3 = A C D F G
    digitalWrite(sevenA, HIGH);
    digitalWrite(sevenB, LOW);
    digitalWrite(sevenC, HIGH);
    digitalWrite(sevenD, HIGH);
    digitalWrite(sevenE, LOW);
    digitalWrite(sevenF, HIGH);
    digitalWrite(sevenG, HIGH);
    
  }else if(count == 4){
    // 4 = C D E F
    digitalWrite(sevenA, LOW);
    digitalWrite(sevenB, LOW);
    digitalWrite(sevenC, HIGH);
    digitalWrite(sevenD, HIGH);
    digitalWrite(sevenE, HIGH);
    digitalWrite(sevenF, HIGH);
    digitalWrite(sevenG, LOW);
  }else if(count == -1){
    // - = D
    digitalWrite(sevenA, LOW);
    digitalWrite(sevenB, LOW);
    digitalWrite(sevenC, LOW);
    digitalWrite(sevenD, HIGH);
    digitalWrite(sevenE, LOW);
    digitalWrite(sevenF, LOW);
    digitalWrite(sevenG, LOW);
  }
}
*/

void drawOnLed(){

  if(count == 0){
    digitalWrite(LED3Pin, HIGH);
    digitalWrite(LED4Pin, LOW);
    digitalWrite(LED5Pin, LOW);
    digitalWrite(LED6Pin, LOW);
    digitalWrite(LED7Pin, LOW);
  }else if(count == 1){
    digitalWrite(LED3Pin, LOW);
    digitalWrite(LED4Pin, HIGH);
    digitalWrite(LED5Pin, LOW);
    digitalWrite(LED6Pin, LOW);  
    digitalWrite(LED7Pin, LOW);
  }else if(count == 2){
    digitalWrite(LED3Pin, LOW);
    digitalWrite(LED4Pin, LOW);
    digitalWrite(LED5Pin, HIGH);
    digitalWrite(LED6Pin, LOW);  
    digitalWrite(LED7Pin, LOW);
  }else if(count == 3){
    digitalWrite(LED3Pin, LOW);
    digitalWrite(LED4Pin, LOW);
    digitalWrite(LED5Pin, LOW);
    digitalWrite(LED6Pin, HIGH);  
    digitalWrite(LED7Pin, LOW);
  }else if(count == 4){
    digitalWrite(LED3Pin, LOW);
    digitalWrite(LED4Pin, LOW);
    digitalWrite(LED5Pin, LOW);
    digitalWrite(LED6Pin, LOW);  
    digitalWrite(LED7Pin, HIGH);
  }else{
    digitalWrite(LED3Pin, HIGH);
    digitalWrite(LED4Pin, LOW);
    digitalWrite(LED5Pin, LOW);
    digitalWrite(LED6Pin, LOW);
    digitalWrite(LED7Pin, LOW);
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  myStepper.setSpeed(5);
  
  // initialize the LED pin as an output:
  /*
  pinMode(LED3Pin, OUTPUT);
  pinMode(LED4Pin, OUTPUT);
  pinMode(LED5Pin, OUTPUT);
  pinMode(LED6Pin, OUTPUT);
  pinMode(LED7Pin, OUTPUT);

  
  pinMode(sevenA, OUTPUT);
  pinMode(sevenB, OUTPUT);
  pinMode(sevenC, OUTPUT);
  pinMode(sevenD, OUTPUT);
  pinMode(sevenE, OUTPUT);
  pinMode(sevenF, OUTPUT);
  pinMode(sevenG, OUTPUT);
  */
  
  //pinMode(6, OUTPUT);
  //digitalWrite(6, HIGH);

  myservo.attach(servoPin);
  
  // initialize the pushbutton pin as an input:
  pinMode(ButtonPin, INPUT);
  // Attach an interrupt to the ISR vector
  
  Serial.begin(9600);
  
  attachInterrupt(digitalPinToInterrupt(ButtonPin) , ISR_Button, RISING);
  
 }

// the loop function runs over and over again forever
void loop() {


  if(analogRead(DayNight) > dayNightSwitch && count == 4){
    //myservo.write(-90);
    night();
  }

  if (ON){

    
    Serial.print("DayNight: ");
    Serial.print(analogRead(DayNight));

    int photo1 = analogRead(photo1Pin);
    int photo2 = analogRead(photo2Pin);
    
    Serial.print(" | photo1: ");
    Serial.print(photo1);
    Serial.print(" | photo2: ");
    Serial.print(photo2);
    
    Serial.print(" | Count: ");
    Serial.print(count);
    
    counter();
    //drawOnLed();

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

  //myStepper.step(stepsPerRevolution);
  /*myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  */
  myservo.write(90);
  delay(1000);
  
  while(analogRead(DayNight) > dayNightSwitch){
    
    Serial.println(analogRead(A5));
    delay(1000);
  }
  Serial.print("Open servo");

  myservo.write(0);
  
  //myStepper.step(-1 * stepsPerRevolution);
  /*myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  */
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
