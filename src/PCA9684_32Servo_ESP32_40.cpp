/*
* Original PCA9685 Module library sourse: https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
 *
 * This is the Arduino code PCA6985 32 channel servo controller
 * to control 32 Servo Motors over WiFi using ESP32 MCU
 *
 * Watch video for this code:
 *
 * Related Videos
V5 video of PCA9685 32 Servo with ESP32 with WiFi https://youtu.be/bvqfv-FrrLM
V4 video of PCA9685 32 Servo with ESP32 (no WiFi): https://youtu.be/JFdXB8Za5Os
V3 video of PCA9685 how to control 32 Servo motors https://youtu.be/6P21wG7N6t4
V2 Video of PCA9685 3 different ways to control Servo motors: https://youtu.be/bal2STaoQ1M
V1 Video introduction to PCA9685 to control 16 Servo  https://youtu.be/y8X9X10Tn1k
 *
 * Written by Ahmad Shamshiri for Robojax Video channel www.Robojax.com
 * Date: Jan 25, 2021, in Ajax, Ontario, Canada

 * this code has been downloaded from http://robojax.com/learn/arduino/

 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

****************************
Get early access to my videos via Patreon and have  your name mentioned at end of very
videos I publish on YouTube here: http://robojax.com/L/?id=63 (watch until end of this video to list of my Patrons)
****************************

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.*
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
////////////////////// PCA9685 settings started
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver board3 = Adafruit_PWMServoDriver(0x42);

int maximumServo = 40; // how many servos are connected
//#define I2C_SDA 21
//#define I2C_SCL 22
// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Watch video V1 to understand the two lines below: http://youtu.be/y8X9X10Tn1k
// #define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
// #define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN 125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 575 // this is the 'maximum' pulse length count (out of 4096)
//Alternatives from github post to research
//#define SERVOMIN 102   // ( 500 / 20000 ) * 4096
//#define SERVOMID 307   // ( 1500 / 20000 ) * 4096
//#define SERVOMAX 491   // ( 2500 / 20000 ) * 4096
//#define USMIN  500
//#define USMAX 2500

int servoAngle = 100;
int servoStep = 10;

int stepDelay = 50; // 50 milliseconds
int servoAngleMin = 30; // Was 50
int servoAngleMax = 150;  // 110
int servoRestPosition = 120;  //  70

// minimum angle of each servo
int allServoMin[] = {
    30, 30, 30, 30, 30, 30, 30, 30,  // 1 to 8
    30, 30, 30, 30, 30, 30, 30, 30,  // 9 to 16
    30, 30, 30, 30, 30, 30, 30, 30,  // 17 to 24
    30, 30, 30, 30, 30, 30, 30, 30,  // 25 to 32
    30, 30, 30, 30, 30, 30, 30, 30}; // 26 to 40

// maximum value of each servo
int allServoMax[] = {
    150, 150, 150, 150, 150, 150, 150, 150,  // 1 to 8
    150, 150, 150, 150, 150, 150, 150, 150,  // 9 to 16
    150, 150, 150, 150, 150, 150, 150, 150,  // 17 to 24
    150, 150, 150, 150, 150, 150, 150, 150,  // 25 to 32
    150, 150, 150, 150, 150, 150, 150, 150}; // 26 to 40

// initial position of servos
int allServoPosition[] = {
    120, 120, 120, 120, 120, 120, 120, 120,  // 1 to 8
    120, 120, 120, 120, 120, 120, 120, 120,  // 9 to 16
    120, 120, 120, 120, 120, 120, 120, 120,  // 17 to 24
    120, 120, 120, 120, 120, 120, 120, 120,  // 25 to 32
    120, 120, 120, 120, 120, 120, 120, 120}; // 26 to 40

int servoNumber = 100; // servo to move
int buttonPushed = 0;
int allServo = 0;

void handleServo();        // this is prototype of function defined at the end of this code
int angleToPulse(int ang); // this is prototype of function defined at the end of this code
////////////////////////PCA9685 ended

#include <PCA9684_32Servo_ESP32.h>
//#include <Adafruit_PWMServoDriver.h>//an alternative suggestion

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#define CONNECTION_TIMEOUT 10

const char *ssid = "BT-THAJX6";
const char *pass = "";

WebServer server(80);

const int led = 13;

/////////////////////////////////////
void handleRoot()
{
  // Watch video explanation for this https://youtu.be/bvqfv-FrrLM
  String HTML_page = pageHeader_p1;

  if (allServo)
  {
    HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=stop\">Stop Servo</a></div>");
  }
  else
  {
    HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=all\">All Servos</a></div>");
  }
  HTML_page.concat("<br/>");

  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=rest\">Rest</a></div>");

  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=max\">Max</a></div>");

  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=min\">Min</a></div>");

  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=col\">Col</a></div>");

  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=row\">Row</a></div>");


  for (int i = 0; i < 5 ; i++)
// Change 5 back to maximumServo to provide buttons for every servo

  {
    HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorBtn\"  href=\"/servo?move=");
    HTML_page.concat(i);
    HTML_page.concat("\">SRV ");
    HTML_page.concat(i + 1);
    HTML_page.concat("</a></div>");
  }
  HTML_page.concat("<br/>");
  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=wave\">Wave</a></div>");


  HTML_page.concat("</body>\n</html>");
  server.send(200, "text/html", HTML_page);
}

void handleNotFound()
{
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
void setup()
{
  // Watch video explanation for this https://youtu.be/bvqfv-FrrLM
  // Set all servos to initial 'neutral' position

Serial.begin(115200);
Wire.begin();
//Wire.begin (I2C_SDA, I2C_SCL); 


  board1.begin();
  board2.begin();
  board3.begin();
  board1.setPWMFreq(60); // Analog servos run at ~60 Hz updates
  board2.setPWMFreq(60);
  board3.setPWMFreq(60);
// initial position of all servos
  //Serial.println("Initial position -start");

 for (int i = 0; i < maximumServo; i++)
 {
    if (i < 16)
    {
      board1.setPWM(i , 0, angleToPulse(allServoPosition[i]));
    }
    if (15 < i < 32)
    {
      board2.setPWM(i-15 , 0, angleToPulse(allServoPosition[i]));
    }
    if (31 < i < 40)
    {
      board3.setPWM(i-31 , 0, angleToPulse(allServoPosition[i]));
    }
    delay(stepDelay);
   }
   // for initial pos end
   // Serial.println("Initial position -end");

  Serial.println("40 channel Servo test!");

  // Servo control using ESP32 from Robojax.com

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  int timeout_counter = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    int status = WL_IDLE_STATUS;
    Serial.print(".");
    timeout_counter++;
    if(timeout_counter >= CONNECTION_TIMEOUT*5){
    ESP.restart();
    }    
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("robojaxESP32"))
  {
    Serial.print("MDNS responder started at http://");
    Serial.println("robojaxESP32");
  }

  server.on("/", handleRoot);
  server.on("/servo", HTTP_GET, handleServo);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
 
 }// void setup end

void loop()
 {
  Serial.println("void loop started)");
  server.handleClient();
  // CODE for ALLSERVO button push
   if (allServo){
  Serial.println("All Servos 281");
     {
    for (int angle = servoAngleMin; angle <= servoAngleMax; angle += servoStep)
    {
      Serial.println("ALL SERVOS");
      for (int i = 0; i < maximumServo; i++)
      {
       board3.setPWM(i, 0, angleToPulse(angle));

       board2.setPWM(i, 0, angleToPulse(angle));

       board1.setPWM(i, 0, angleToPulse(angle));
      }
      delay(stepDelay);
    }

//      for (int r = 0; r= 5; r++)
//     {
//        for (int angle = servoAngleMin; angle <= servoAngleMax; angle += servoStep)
//    {
//      Serial.println("SERVO LINE");
//      for (int i = 0; i < 8; i++)
//      {
//       board1.setPWM(i, 0, angleToPulse(angle));
//
//       board2.setPWM(i, 0, angleToPulse(angle));

//       board3.setPWM(i, 0, angleToPulse(angle));
//      }
//      delay(stepDelay);
//    }
//     } //end of single line rpt
    // robojax PCA9865 32 channel Servo control
      delay(100);
    // CODE for debugging
   }//if pushed
  
  if (false)
  {
    Serial.print("AllServo# ");
    Serial.print(servoNumber);
    Serial.print(" Angle ");
    Serial.println(allServoPosition[servoNumber]);
  }
    // CODE for individual SERV button push
  if (buttonPushed && (servoNumber >= 0 && servoNumber < maximumServo)) {
    if (servoNumber < 16)
     {
      board1.setPWM(servoNumber, 0, angleToPulse(allServoPosition[servoNumber]));
     }
    if (15 < servoNumber < 32)
     {
      board2.setPWM(servoNumber - 15, 0, angleToPulse(allServoPosition[servoNumber]));
     }
    else
     {
      board3.setPWM(servoNumber - 31, 0, angleToPulse(allServoPosition[servoNumber]));
     }
  }
   }
    Serial.println("void loop end");
  buttonPushed = 0;
} // void loop end

/*
 * handleServo()
 * update the buttonPushed variable
 * re turns nothing
 * Written by Ahmad Shamshiri on Dec 29, 2019
 * www.Robojax.com
 * http://youTube.com/robojaxTV
 */
void handleServo()
{
  // Watch video explanation for this https://youtu.be/bvqfv-FrrLM
  Serial.println("handleServo()");
  int servoNumber = server.arg("move").toInt();
  String argDo = server.arg("do");
  String argMove = server.arg("move");

  Serial.println("handleServo starts");

  Serial.print("server.arg(\"move\"): ");
  Serial.println(argMove);

  Serial.print("server.arg(\"do\"): ");
  Serial.println(argDo);

  Serial.print("servoNumber: "); // print without newline
  Serial.println(servoNumber);   // print with newline

  if (argDo == "all")
  {
    Serial.println(".........ALL.........");
    allServo = 1;
  }
  else
  {
    allServo = 0;
  }

  // argMove
 
  int servoNumberRequested= server.arg("move").toInt();
  if(servoNumberRequested >=0 && servoNumberRequested < maximumServo)
  {
  if (servoNumber >= 0 && servoNumber < maximumServo)
  {
    buttonPushed = 1;
    // restrict the servoPosition to be in the range between [min, max]
    if (allServoPosition[servoNumber] < allServoMin[servoNumber])
    {
      allServoPosition[servoNumber] = allServoMin[servoNumber];// <below> make = min
    }
    if (allServoPosition[servoNumber] > allServoMax[servoNumber])
    {
      allServoPosition[servoNumber] = allServoMax[servoNumber];// <above> make =
    }
    if (servoNumberRequested < 16)
      {
        board1.setPWM(servoNumberRequested, 0, angleToPulse(servoRestPosition));
      }
    if (servoNumberRequested > 15 && servoNumberRequested < 32)
      {
        board2.setPWM(servoNumberRequested, 0, angleToPulse(servoRestPosition));
      }
    if (servoNumberRequested  > 31 && servoNumberRequested  < 40)
      {
        board3.setPWM(servoNumberRequested, 0, angleToPulse(servoRestPosition));
      }
      buttonPushed = 0;
  }

  if (argDo == "rest"){
    // initial position of all servos
    {
    for (int i = 0; i < maximumServo; i++)
    {
      if (i < 16)
      {
        board1.setPWM(i, 0, angleToPulse(allServoPosition[i]));
        Serial.print(i);
      }
      if (i > 15 && i < 32)
      {
        board2.setPWM(i-15, 0, angleToPulse(allServoPosition[i]));
        Serial.print(i);
      }
      if (i > 31 && i < 40)
      {
        board3.setPWM(i-31, 0, angleToPulse(allServoPosition[i]));
        Serial.print(i);
      }

    } // end of servo i
    }
          buttonPushed = 0;  

   }// end of rest

    if (argDo == "max"){
    // initial position of all servos
    for (int i = 0; i < maximumServo; i++)
    {
      if (i < 16)
      {
        board1.setPWM(i, 0, angleToPulse(allServoMax[i]));
      }
      if (i > 15 && i < 32)
      {
        board2.setPWM(i-15, 0, angleToPulse(allServoMax[i]));
      }
      if (i > 31 && i < 40)
      {
        board3.setPWM(i-31, 0, angleToPulse(allServoMax[i]));
      }

    }
      buttonPushed = 0;

  }// end of max

    if (argDo == "min"){
    // initial position of all servos
    for (int i = 0; i < maximumServo; i++)
    {
      if (i < 16)
      {
        board1.setPWM(i, 0, angleToPulse(allServoMin[i]));
      }
      if (i > 15 && i < 32)
      {
        board2.setPWM(i-15, 0, angleToPulse(allServoMin[i]));
      }
      if (i > 31 && i < 40)
      {
        board3.setPWM(i-31, 0, angleToPulse(allServoMin[i]));
      }

    }
      buttonPushed = 0;

  }// end of min

    if (argDo == "col"){
    // Columns
    Serial.println("Columns");
    {
    const int NUM_COLUMNS = 8;
    const int NUM_ROWS = 5;
for (int column=0; column < NUM_COLUMNS; column++){
    Serial.println(column);

    for (int row=0; row < NUM_ROWS; row++){
        //board = [1, 2, 3]
            int board = row / 2 +1 ;
            //int index = column + (row % 2) * NUM_COLUMNS;
           int index = column + row * NUM_COLUMNS;
     for (int angle = servoAngleMin; angle <= servoAngleMax; angle += servoStep)
      {
      if (board = 1) {
         board1.setPWM(index, 0, angleToPulse(angle));
         }
      if (board = 2) {
         board2.setPWM(index, 0, angleToPulse(angle));
         }
      if (board = 3) {
         board3.setPWM(index, 0, angleToPulse(angle));
         }
      }
    
         Serial.print(index);
     }//row
        }//column
    }
      buttonPushed = 0;
    }//end of row
////////////////////////////////////////////////
        if (argDo == "row"){
    // Horizontal
    Serial.println("Rows");
    {
    const int NUM_COLUMNS = 8;
    const int NUM_ROWS = 5;

for (int row=0; row < NUM_ROWS; row++){
    Serial.println(row);

    for (int column=0; column < NUM_COLUMNS; column++){
        //board = [1, 2, 3]
            int board = row / 2 +1 ;
            //int index = column + (row % 2) * NUM_COLUMNS;
           int index = column + row * NUM_COLUMNS;

      if (board = 1) {
         board1.setPWM(index, 0, angleToPulse(allServoMin[index]+5));
         delay(50);
         board1.setPWM(index, 0, angleToPulse(allServoMax[index]));
         }
      if (board = 2) {
         board2.setPWM(index, 0, angleToPulse(allServoMin[index]+5));
         delay(50);
         board2.setPWM(index, 0, angleToPulse(allServoMax[index]));
         }
      if (board = 3) {
         board3.setPWM(index, 0, angleToPulse(allServoMin[index]+5));
         delay(50);
         board3.setPWM(index, 0, angleToPulse(allServoMax[index]));
         }
         Serial.print(index);
 //        delay(100);
    
     }//column
        }//row
    }
    }//end of hor
        buttonPushed = 0;
  }
   handleRoot();

  Serial.println("handleServo ends");

} // handleServo() end

/*
 * angleToPulse(int ang)
 * gets angle in degree and returns the pulse width
 * also prints the value on seial monitor
 */
int angleToPulse(int ang)
{
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max
//  Serial.print("Angle: ");
//  Serial.print(ang);
//  Serial.print(" pulse: ");
//  Serial.println(pulse);
  return pulse;

} // for end
