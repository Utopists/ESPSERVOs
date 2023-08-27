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
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver board3 = Adafruit_PWMServoDriver(0x42);

int maximumServo = 1; // how many servos are connected

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Watch video V1 to understand the two lines below: http://youtu.be/y8X9X10Tn1k
// #define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
// #define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN 125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 575 // this is the 'maximum' pulse length count (out of 4096)

int servoAngle = 90;
int servoStep = 8;

int stepDelay = 50; // 50 milliseconds
int servoAngleMin = 70;
int servoAngleMax = 110;
int servoRestPosition = 90;

// minimum angle of each servo
int allServoMin[] = {
    70, 70, 70, 70, 70, 70, 70, 70,  // 1 to 8
    70, 70, 70, 70, 70, 70, 70, 70,  // 9 to 16
    70, 70, 70, 70, 70, 70, 70, 70,  // 17 to 24
    70, 70, 70, 70, 70, 70, 70, 70,  // 25 to 32
    70, 70, 70, 70, 70, 70, 70, 70}; // 26 to 40

// maximum value of each servo
int allServoMax[] = {
    110, 110, 110, 110, 110, 110, 110, 110,  // 1 to 8
    110, 110, 110, 110, 110, 110, 110, 110,  // 9 to 16
    110, 110, 110, 110, 110, 110, 110, 110,  // 17 to 24
    110, 110, 110, 110, 110, 110, 110, 110,  // 25 to 32
    110, 110, 110, 110, 110, 110, 110, 110}; // 26 to 40

// initial position of servos
int allServoPosition[] = {
    90, 90, 90, 90, 90, 90, 90, 90,  // 1 to 8
    90, 90, 90, 90, 90, 90, 90, 90,  // 9 to 16
    90, 90, 90, 90, 90, 90, 90, 90,  // 17 to 24
    90, 90, 90, 90, 90, 90, 90, 90,  // 25 to 32
    90, 90, 90, 90, 90, 90, 90, 90}; // 26 to 40

int servoNumber = 0; // servo to move
int buttonPushed = 0;
int allServo = 0;

void handleServo();        // this is prototype of function defined at the end of this code
int angleToPulse(int ang); // this is prototype of function defined at the end of this code
////////////////////////PCA9685 ended

#include "PCA9684_32Servo_ESP32.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "BT-THAJX6";
const char *pass = "JdakXJ3AQHFLeP";

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
  for (int i = 0; i < maximumServo; i++)
  {
    HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorBtn\"  href=\"/servo?move=");
    HTML_page.concat(i);
    HTML_page.concat("\">SRV ");
    HTML_page.concat(i + 1);
    HTML_page.concat("</a></div>");
  }
  HTML_page.concat("<br/>");
  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=wave\">Wave</a></div>");

  HTML_page.concat("<div class=\"btn\"><a class=\"angleButton colorAll\"  href=\"/servo?do=rest\">Rest</a></div>");

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
  board1.begin();
  board2.begin();
  board3.begin();
  board1.setPWMFreq(60); // Analog servos run at ~60 Hz updates
  board2.setPWMFreq(60);
  board3.setPWMFreq(60);
  // initial position of all servos
  for (int i = 0; i < maximumServo; i++)
  {
    if (i < 16)
    {
      board1.setPWM(i, 0, angleToPulse(servoRestPosition));
      Serial.print("Servo i");
      Serial.print(i);
      Serial.println(allServoPosition[i]);
      //      board1.setPWM(i, 0, angleToPulse(allServoPosition[i]) );
    }
    if (15 < i < 32)
    {
      board2.setPWM(i - 15, 0, angleToPulse(allServoPosition[i]));
    }
    else
    {
      board3.setPWM(i - 31, 0, angleToPulse(allServoPosition[i]));
    }

  } // for initial pos end

  Serial.begin(115200);
  Serial.println("32 channel Servo test!");

  // Servo control using ESP32 from Robojax.com

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
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
} // void setup end

void loop()
{
  server.handleClient();
  // CODE for ALLSERVO button push
  if (allServo)
  {
    for (int angle = servoAngleMin; angle <= servoAngleMax; angle += servoStep)
    {
      for (int i = 0; i < 40; i++)
      {
        board3.setPWM(i, 0, angleToPulse(angle));
        board2.setPWM(i, 0, angleToPulse(angle));
        board1.setPWM(i, 0, angleToPulse(angle));
      }
      delay(stepDelay);
    }

    // robojax PCA9865 32 channel Servo control
    delay(100);
    // CODE for debugging
  } // if pushed
  if (false)
  {
    Serial.print("AllServo# ");
    Serial.print(servoNumber);
    Serial.print(" Angle ");
    Serial.println(allServoPosition[servoNumber]);
  }

  // CODE for individual SERV button push
  if (buttonPushed && (servoNumber >= 0 && servoNumber < maximumServo))
  {
    if (servoNumber < 16)
    {
      board1.setPWM(servoNumber, 0, angleToPulse(allServoPosition[servoNumber]));
      Serial.print(buttonPushed);
      Serial.println("....SERV....");

      Serial.print(servoNumber);
      Serial.println(allServoPosition[servoNumber]);
      delay(500);
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

  buttonPushed = 0;
} // void loop end

/*
 * handleServo()
 * update the buttonPushed variable
 * returns nothing
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

  Serial.print("server.arg(\"move\"): ");
  Serial.println(argMove);

  Serial.print("server.arg(\"do\"): ");
  Serial.println(argDo);

  Serial.print("servoNumber: "); // print without newline
  Serial.println(servoNumber);   // print with newline

  if (argDo == "all")
  {
    Serial.println("....ALL....");
    allServo = 1;
  }
  else
  {
    allServo = 0;
  }

  // argMove
  if (servoNumber >= 0 && servoNumber < maximumServo)
  {
    buttonPushed = 1;

    // restrict the servoPosition to be in the range between [min, max]
    if (allServoPosition[servoNumber] < allServoMin[servoNumber])
    {
      allServoPosition[servoNumber] = allServoMin[servoNumber];
    }
    if (allServoPosition[servoNumber] > allServoMax[servoNumber])
    {
      allServoPosition[servoNumber] = allServoMax[servoNumber];
    }
  }

  if (argDo == "rest"){
    // initial position of all servos
    for (int i = 0; i < maximumServo; i++)
    {
      Serial.println("....REST....");
      Serial.print("servo: ");
      Serial.print(i);
      Serial.print(" allServoPosition[i]: ");
      Serial.print(allServoPosition[i]);
      Serial.println();
      if (i < 16)
      {
        board1.setPWM(i, 0, angleToPulse(allServoPosition[i]));
      }
      if (i > 15 && i < 32)
      {

        board2.setPWM(i - 16, 0, angleToPulse(allServoPosition[i]));
      }
      if (i > 31 && i < 40)
      {
        board3.setPWM(i - 32, 0, angleToPulse(allServoPosition[i]));
      }
      buttonPushed = 0;
    }
  }

  handleRoot();

} // handleServo() end

/*
 * angleToPulse(int ang)
 * gets angle in degree and returns the pulse width
 * also prints the value on seial monitor
 */
int angleToPulse(int ang)
{
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max
  Serial.print("Angle: ");
  Serial.print(ang);
  Serial.print(" pulse: ");
  Serial.println(pulse);
  return pulse;

} // for end
