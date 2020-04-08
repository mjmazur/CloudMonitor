/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet2.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

//////////////////////////////////
// Setup light sensor variables //
//////////////////////////////////
const bool LIGHTSENSOR  = true;
int lightPin = 0;
int lightReading;
const int numReadings = 10;
int total = 0;
int average = 0;
float lux;

///////////////////////////////
// Setup IR sensor variables //
///////////////////////////////
const byte IRAddress1 = 0x5A;
const byte IRAddress2 = 0x5B;

float temp1 = 1;
float temp2 = 2;

IRTherm therm1;
IRTherm therm2;

// Enter a MAC address and IP address for your controller below.
byte mac[] = { 0xBA, 0xD1, 0xDE, 0xA5, 0x00, 0x00 };
IPAddress ip(192, 168, 1, 10);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // Initialize the IR sensors and set units to Celsius
  therm1.begin(IRAddress1);
  therm2.begin(IRAddress2);
  therm1.setUnit(TEMP_C);
  therm2.setUnit(TEMP_C);

  // Send a reset to the W5500 to make sure it works
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delayMicroseconds(500);
  digitalWrite(9, HIGH);
  delayMicroseconds(1000);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        lux = getLux();
        temp1 = therm1.object();
        temp2 = therm2.object();
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.print("Temp 1: ");
          if (therm1.read()) { client.print(therm1.object());}
          client.println("<br />");
          client.print("Temp 2: ");
          if (therm2.read()) { client.print(therm2.object());}
          client.println("<br />");
          client.println("------- -----");
          client.println("<br />");
          client.print("T1-T2:  ");
          client.println(therm1.object()-therm2.object());
          client.println("<br />");
          client.print("Lux: ");
          client.println(lux);
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

////////////////////////////////////////////////////
// Function to get the ambient light level in lux //
////////////////////////////////////////////////////
float getLux() {
    // Average light sensor readings over 'numReadings'
  for (int i = 0; i < numReadings; i++){
    lightReading = analogRead(lightPin);
    total = total + lightReading;
    //Serial.println(total);
    delay(10);
  }
  average = total / numReadings;
  total = 0;

  // Convert analog light reading to lux
  lux = pow(10,6.6162) * pow(average,-1.9191);

  // Print readings to serial monitor
  Serial.print("Reading: "); Serial.print(average);
  Serial.print(" ==> "); Serial.print(lux); Serial.println(" lux");
  return lux;
}
