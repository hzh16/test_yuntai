#include "LobotServoController.h"
#include <SoftwareSerial.h> 
#include <Process.h>
SoftwareSerial myse1(10,11);
SoftwareSerial LobotServoController::mySerial(12,13); //RX,TX
Process mediaSever;

LobotServoController myse;

boolean first_pt;
byte first_pan;
byte first_tilt;
int yuntai_time;
void setup() {
  Serial.begin(9600);
  LobotServoController::mySerial.begin(9600);
  myse1.begin(115200); 
  Bridge.begin();

  Process wifiCheck;  // initialize a new process
  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");
  while (wifiCheck.available() > 0) {
    char c = wifiCheck.read();
    Serial.print(c);
  }
  Serial.println();
  first_pt=true;
  mediaSever.begin("/root/RPRobot-master/test");
  mediaSever.runAsynchronously();
  while (mediaSever.available() > 0) {
    char c = mediaSever.read();
    Serial.print(c);
  }
  
  //myse.setActionGroupSpeed(0,100);
  //myse.runActionGroup(0,0);
  //delay(1000);
  //myse.stopActiongGroup();
  yuntai_time=-1000;
}

void loop() {
  // put your main code here, to run repeatedly:
  String comdata="";
  //String angle="";  
  byte pan, tilt;
  int pan1, tilt1;
  bool get_pt=false;
  if (Serial1.available()) {
    byte b;
    b=Serial1.read();
    Serial.print(b);
    Serial.print(" ");
    delay(2);
    pan = Serial1.read();
    delay(2);
    tilt = Serial1.read();
    Serial.print(pan);
    Serial.print(" ");
    Serial.print(tilt);
    Serial.print(" ");
    /*if(first_pt)
    {
      first_pt=false;
      first_pan=pan;
      first_tilt=tilt;
    }*/
    //pan1=((int)pan-(int)first_pan);
    //tilt1=((int)tilt-(int)first_tilt);
    pan1=(int)pan;
    tilt1=(int)tilt;
    /*if(pan1<-255)
      pan1+=256;
    if(pan1>255)
      pan1-=256;
    if(tilt1<-255)
      tilt1+=256;
    if(tilt1>255)
      tilt1-=256;*/
    //pan1+=90;
    //if(pan1>=180)
    //  pan1-=180;
    if(pan1>=128)
      pan1-=256;
    if(tilt1>=128)
      tilt1-=256;
    Serial.print(pan1);
    Serial.print(" ");
    Serial.print(tilt1);
    Serial.print(" ");
    int temp_pan=pan1;
    pan1=temp_pan*100/9+1500;
    Serial.print(pan1);
    if(tilt1>128)
      tilt1-=256;
    tilt1=tilt1*100/9+1500;
    Serial.print(" ");
    Serial.println(tilt1);
    int t=millis();
    //if(t-yuntai_time>=100)
    {
      myse.moveServo(19, pan1, 100);
      myse.moveServo(20, tilt1, 100);
      yuntai_time=t;
    }
  }
  while (Serial.available() > 0)  
     {
         comdata += char(Serial.read());
         delay(2);
     }
     if (comdata.length() > 0 )
     {
         //Serial.println(comdata);
         if(comdata.length()>=2 && comdata.charAt(0)=='A')
         {
            /*while(Serial.available()>0){
              angle=char(Serial.read());
              delay(2);
            }*/
            int angle;
            Serial.println(angle=comdata.substring(1).toInt());
            myse.moveServo(19, angle, 1000);
         }
         //vertal 1200
         //   myse.moveServo(19, 500, 1000);
        
         else if(comdata.length()>=2 && comdata.charAt(0)=='B')
         {
            int angle;
            Serial.println(angle=comdata.substring(1).toInt());
            myse.moveServo(20, angle, 1000);
         }
     } 
     //     myse.moveServo(20, 2500, 1000);
     comdata = "";
}

