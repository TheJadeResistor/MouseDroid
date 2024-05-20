#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MAXSPEED 255
#define midSpeed 80
#define speedOffset 0
#define arraySize 5
#define speedOffset 15

//create an RF24 object
RF24 radio(3, 2);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

struct MyStruct {
  int left;
  int right;
};


TMRpcm tmrpcm;
unsigned long time_now = 0;
int motorL = 0;
int motorR = 0;
int valuesL[arraySize];
int valuesR[arraySize];
int dummy[arraySize] = {0, 0, 0, 0, 0};;
int counter = 0;
int leftSpeed = 0;
int rightSpeed = 0;
int idk=0;

// Motor A connections
int enA = 5;
int in1 = 10;
int in2 = A0;
// Motor B connections
int enB = 6;
int in3 = 8;
int in4 = 7;

void setup(){
  digitalWrite(9, LOW);
  //tmrpcm.speakerPin=9; //needs to be 9
  Serial.begin(115200);
  //if(!SD.begin(SD_ChipSelectPin))
  //{
  //  Serial.println("SD fail");
  //  return;
  //}
  //tmrpcm.setVolume(4);
  //tmrpcm.play("test2.wav"); 
  //test
  //MouseDroidSounds
  //test2
  //tmrpcm.play("MouseDroidMain.wav");
  //tmrpcm.play("test2.wav"); 
  Serial.println("Code start");
  //time_now = millis();


	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);


  radio.begin();
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
  motorL = 0;
  motorR = 0;
  counter = 0;
}

void loop() {
  
  // if(millis() > time_now + 10000){
  //   tmrpcm.play("MouseDroidMain.wav"); //2 sec
  //   time_now = millis();
  // }

  if (radio.available()){
    MyStruct myStruct;
    radio.read(&myStruct, sizeof(myStruct));
    motorL = map(myStruct.left, 0, 1024, 0, MAXSPEED);
    motorR = map(myStruct.right, 0, 1024, 0, MAXSPEED);
    //Serial.print("rightSpeed: "); Serial.println(motorR);
    //Serial.print("leftSpeed: "); Serial.println(motorL);
    //if(counter == (arraySize+1))counter=0;
    //valuesL[counter] = motorL;
    //valuesR[counter] = motorR;
    //counter++;
    //Serial.println(counter);
    leftSpeed = motorR;
    rightSpeed = motorL;
  }

  //if(counter == arraySize){
    // // for(int i=0; i<sizeof(valuesL); i++){
    // //  Serial.print("leftSpeed: "); Serial.println(valuesL[i]);Serial.println();
    // //  Serial.print("rightSpeed: "); Serial.println(valuesR[i]);Serial.println();
    // // }
    // leftSpeed = findMode(valuesL, counter);//sizeof(valuesL));
    // Serial.print("leftSpeed: "); Serial.println(leftSpeed);
    // leftSpeed = findMode(dummy, counter); //clear findMode??
    // delay(500);
    // rightSpeed = findMode(valuesR, counter);//sizeof(valuesR)); //makes L show R vals
    // //Serial.print("leftSpeed: "); Serial.println(leftSpeed);
    // Serial.print("rightSpeed: "); Serial.println(rightSpeed);
    // Serial.println();
    // rightSpeed = findMode(dummy, counter); //clear findMode??
  //}


//leftSpeed is the new absolute value to use
//left
  if(leftSpeed >= 75 && leftSpeed <= 115){ //stopped
    analogWrite(enA, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    Serial.println("stopL");
  }
  else if(leftSpeed < 75){ //rev left
    analogWrite(enA, map(leftSpeed, 75, 0, speedOffset, (MAXSPEED*.75)));
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    Serial.print("revL: "); Serial.println(map(leftSpeed, 75, 0, speedOffset, (MAXSPEED*.75)));
  }
  else if(leftSpeed > 115){ //fwd left
    analogWrite(enA, map(leftSpeed, 115, MAXSPEED, speedOffset, MAXSPEED));
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    Serial.print("fwdL: "); Serial.println(map(leftSpeed, 115, MAXSPEED, speedOffset, MAXSPEED));
  }

//right
  if(rightSpeed >= 75 && rightSpeed <= 115){ //stopped
    analogWrite(enB, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    Serial.println("stopR");
  }
  else if(rightSpeed < 75){ //rev left
    analogWrite(enB, map(rightSpeed, 75, 0, speedOffset, (MAXSPEED*.75)));
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    Serial.print("revR: "); Serial.println(map(rightSpeed, 75, 0, speedOffset, (MAXSPEED*.75)));
  }
  else if(rightSpeed > 115){ //fwd left
    analogWrite(enB, map(rightSpeed, 115, MAXSPEED, speedOffset, MAXSPEED));
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    Serial.print("fwdR: "); Serial.println(map(rightSpeed, 115, MAXSPEED, speedOffset, MAXSPEED));
  }

}

int findMode(int arr[], int size) {
  int mode = 0;
  int maxCount = 0;

  for (int i = 0; i < size; i++) {
    int count = 0;

    for (int j = 0; j < size; j++) {
      if (arr[j] == arr[i]) {
        count++;
      }
    }

    if (count > maxCount) {
      maxCount = count;
      mode = arr[i];
    }
  }

  return mode;
}