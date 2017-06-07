//Based off of the Arduino servo sweep example
//Code is to move servo arm with a possible air blower attached in front of a mouse.
//Code can both repeat a consistent 2 second sweep, or a random sweep from 1-3 secs.
//By Tom Vajtay 05-19-2016

#include <Wire.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos;        //Defines initial position
int step_time = 2;  //Time in ms of each degree of arm movement
int x = 0;
void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(9, OUTPUT);
  
  myservo.write(2);    //Automatically places servo arm at zero*(Servo can not go perfectly to 0 degrees)

  Wire.begin(10); //Begins I2C protocol and declares I2C slave address as 10
  Wire.onReceive(what2do); //On recieve runs function what2do which defines x as the recieved byte.
}

void loop() {

if (x == 4){
  servo_test();
}
else if(x == 6){
  servo_event1();
}
else if(x == 7){
  servo_event2();
}

}

void servo_test() {
  for (pos = 0; pos <= 90; pos += 1) // goes from 0 degrees to 90 degrees
  { // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(step_time);                       // waits 180ms for the servo to reach the position
  }

  delay(500);

  for (pos = 90; pos >= 0; pos -= 1) // goes from 90 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(step_time);                 
  }
  x = 0;
}

void servo_event1(){
  for (pos = 0; pos <= 90; pos += 1) // goes from 0 degrees to 90 degrees
      { // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(step_time);                       // waits 2ms for the servo to reach the position
      }
      x = 0;
}

void servo_event2(){
  for (pos = 90; pos >= 0; pos -= 1) // goes from 90 degrees to 0 degrees
      {
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(step_time);                       // waits 2ms for the servo to reach the position
      }
      x = 0;
}

void what2do(int byteCount){

    int control_char = Wire.read();

    if (control_char == 4) {
      x = 4;
    }

    if (control_char == 6){
      x = 6;
    }

    if (control_char == 7){
      x = 7;
    }

    if (control_char == 8){
      myservo.write(2);
    }


}

