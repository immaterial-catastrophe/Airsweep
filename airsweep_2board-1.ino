//Based off of the Arduino servo sweep example
//Code is to move servo arm with a possible air blower attached in front of a mouse.
//Code can both repeat a consistent 2 second sweep, or a random sweep from 1-3 secs.
//By Tom Vajtay 05-19-2016

#include <Wire.h>

char control_char;  //Declares variable
bool trial_start = false;
bool randomizer = false;
long Time = 2000;
int End;
int count = 0;
int LED = 9;
int CamPin = 5;

void setup()
{

  pinMode(LED, OUTPUT);
  pinMode(CamPin, OUTPUT);
  digitalWrite(CamPin, LOW);
  digitalWrite(LED, LOW);


  //Uses analog signal from empty A3 to serve as random starting point for random number generation
  randomSeed(analogRead(3));

  Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(10);
  Wire.write(4);
  Wire.endTransmission();

  //Serial Monitor Setup
  Serial.println("Behavioral Setup");
  Serial.println("Press 1 to start experiment");
  Serial.println("Press 2 to end session");
  Serial.println("Press 3 to toggle random servo movement");
  Serial.println("Press 4 to test servo");
  Serial.println("Press 5 to define LED paradigm");
  delay(500);

}

void loop() {

  if (Serial.available()) {
    control_char = Serial.read();

    if (control_char == '3') {
      randomizer = !randomizer;
      Serial.print("Randomizer:\t");
      Serial.print("\t");
      Serial.println(randomizer);
    }


    if (control_char == '4') {
      Wire.beginTransmission(10);
      Wire.write(4);
      Wire.endTransmission();
    }

    if (control_char == '1') {      //Starts the trial
      count = 0;
      Serial.println("Trial Begins");
      Serial.println();
      Serial.println("Count#\tDuration");
      trial_start = true;
    }

    if (control_char == '5') {

    }
  }
  while (trial_start) {
    if (Serial.available()) {
      control_char = Serial.read();

      if (control_char == '2') {
        Serial.println("Session end");
        //Stops the trial
        end_session();
      }
    }
    if (randomizer) {
      randTime();
    }
    Serial.print(count);
    Serial.print("\t");
    Serial.println(Time);

    Wire.beginTransmission(10);
    Wire.write(6);
    Wire.endTransmission();

    delay(Time - 1000);

    digitalWrite(CamPin, HIGH);   //Sends TTL pulse to trigger Master 9
    delay(100);
    digitalWrite(CamPin, LOW);
    delay(900);                  //One second delay before air sweep

    Wire.beginTransmission(10);
    Wire.write(7);
    Wire.endTransmission();

    delay(8000);

    count = count + 1;
  }
}





void randTime() {
  Time = random(1, 3);
  Time = Time * 1000;


}

void end_session() {
  Serial.println();
  Wire.beginTransmission(10);
  Wire.write(8);
  Wire.endTransmission();
  delay(500);
  asm volatile ("  jmp 0");   // Makes the Arduino soft reset
}

void led() {
  Serial.println("Please input desired length of light pulse in ms");
  if (Serial.available()) {
    long led_length = Serial.read();
    if (led_length > 5000) {
      Serial.println("Your input exceeded 5 seconds");
      led();
    }
    else {
      Serial.print("Light pulse length is ");
      Serial.println(led_length);
      Serial.println();
      Serial.println("Please input the desired start time relative to the servo return as t = 0");
      if (Serial.available()) {
        long led_start = Serial.read();
        if (led_start > 1000 or led_start < -5000) {
          Serial.println("Start exceeds parameters");
          led();
        }
        else {
          Serial.print("LED start occurs ");
          Serial.print(led_start);
          Serial.println(" ms from the return of the servo arm");
          Serial.println();
        }
      }
    }
  }
}

