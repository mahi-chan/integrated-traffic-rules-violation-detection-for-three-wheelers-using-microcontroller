#include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include <TimerOne.h>
#include <SoftwareSerial.h>
float dist = 3.14 * 6.5;
LiquidCrystal lcd(12, 10, 11, 4, 5, 6); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 
const int trigPin = 8;
const int echoPin = 9;
long duration;
int distanceCm;

const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1

// Integers for pulse counters
unsigned int counter1 = 0;
unsigned int counter2 = 0;

// Float for number of slots in encoder disk
float diskslots = 20.00;

//integers for buzzer
int buzzer = 7;

// Interrupt Service Routines

// Motor 1 pulse count ISR
void ISR_count1()  
{
  counter1++;  // increment Motor 1 counter value
} 

// Motor 2 pulse count ISR
void ISR_count2()  
{
  counter2++;  // increment Motor 2 counter value
} 

// TimerOne ISR
float ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
  //Serial.print("Motor Speed 1: "); 
  float rotation1 = (counter1 / diskslots) * 60.00;  // calculate RPM for Motor 1
  //Serial.print(rotation1);  
  //Serial.print(" RPM - "); 
  counter1 = 0;  //  reset counter to zero
  //Serial.print("Motor Speed 2: "); 
  float rotation2 = (counter2 / diskslots) * 60.00;  // calculate RPM for Motor 2
  //Serial.print(rotation2);  
 // Serial.println(" RPM"); 
  counter2 = 0;  //  reset counter to zero
  Timer1.attachInterrupt( ISR_timerone );  // Enable the timer
  return rotation2;
}

void setup() {
setup1();
setup2();
}

void setup1()
{
lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(buzzer,OUTPUT);
}

void setup2()
{
Serial.begin(9600);
  
Timer1.initialize(1000000); // set timer for 1sec
attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, RISING);  // Increase counter 1 when speed sensor pin goes High
attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, RISING);  // Increase counter 2 when speed sensor pin goes High
Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
}

void loop() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.print("Distance:"); // Prints string "Distance" on the LCD
lcd.print(distanceCm); // Prints the distance value from the sensor
lcd.print("cm");
delay(200);

if(distanceCm <= 8){
  //tone(buzzer, 1000, 1000);
  Serial.print(distanceCm);
  lcd.print(" ");
  delay(100);
  lcd.clear();
}

Serial.print(",");

lcd.setCursor(0,1);
lcd.print("speed: ");
float rotation = ISR_timerone();
int speed = rotation * dist;
lcd.print(speed);
if(speed > 300 ){
  //tone(buzzer, 1000, 1000);
  Serial.print(speed);
  lcd.print(" ");
  delay(100);
  lcd.clear();
}

//lcd.print(" ");
delay(2000);
}