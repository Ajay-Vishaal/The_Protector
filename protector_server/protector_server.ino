/*
  Protector Server

  This sketch is the source code of the project "Protector". Protector is a safety and automation which enables
  user to interact with appliances without any physical contact as well as scans the temperature of the entering inside into the place.

  More details are made available in https://www.hackster.io/Ajay_Vishaal/the-protector-bce054

  Circuit:
     Circuit schematics are available in https://www.hackster.io/Ajay_Vishaal/the-protector-bce054

  created on 26/08/2020
  by Ajay Vishaal T
*/

// Include the necessary libraries
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <WiFiNINA.h>

#include <SPI.h>
#include <MFRC522.h>

// Assign the Reset and SDA pins for RC522 Reader
#define SS_PIN A6
#define RST_PIN 7

#include "arduino_secrets.h"
// Enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

// Variable to hold incoming data from the client data
String currentLine = "";

int status = WL_IDLE_STATUS;

WiFiServer server(80);

// variables for the switch counter and person counter
int counter = 0;
int count = 1;
int counter1 = 0;
int count1 = 1;
int counter2 = 0;
int count2 = 1;
int c1 = 0;
int c2 = 0;
int c3 = 0;

int person = 0;
int limit = 10;

int al = 0;
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Defining the I/O to be accessed
#define latch 6
#define sock 0
#define light 1
#define fan 2
#define buzz 3

#define trig1 A4
#define trig2 A3
#define echo1 4
#define echo2 5

#define ir A0
#define ir2 A1
#define ir3 A2

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

/***************************************************************************************
  Setup function will be called only once. Use this function to set-up your device.
 **************************************************************************************/
void setup() {
  //  initializing MLX90614 and RFID reader
  mlx.begin();
  SPI.begin();// Init SPI bus
  rfid.PCD_Init();

  //set pinmode of I/O pins of Arduino
  pinMode(latch, OUTPUT);
  pinMode(sock, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);

  //set all the pins low at the begining
  digitalWrite(latch, LOW);
  digitalWrite(sock, LOW);
  digitalWrite(light, LOW);
  digitalWrite(fan, LOW);
  digitalWrite(buzz, LOW);
  digitalWrite(trig1, LOW);
  digitalWrite(trig2, LOW);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
}

// function for reading distance using ultrasonic sensor in the outer unit
int US01() {
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  int  duration1 = pulseIn(echo1, HIGH);
  int us1 = (duration1 / 2) / 29.1;
  return us1;
}

// function for reading distance using ultrasonic sensor in the inner unit
int US02() {
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  int duration2 = pulseIn(echo2, HIGH);
  int us2 = (duration2 / 2) / 29.1;
  return us2;
}

/***************************************************************************************
  This function will be called again and again and it will never exit.
 **************************************************************************************/
void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();

  //variable to hold analog readings of IR sensor
  int sen = analogRead(ir);
  int sen2 = analogRead(ir2);
  int sen3 = analogRead(ir3);
  delay(500);

  //check the IR sensor value and run the counter accordingly
  if (sen < 1000 ) {
    counter = counter + 1;
    count = counter % 2;
  }

  if (sen2 < 1000 ) {
    counter1 = counter1 + 1;
    count1 = counter1 % 2;
  }

  if (sen3 < 1000 ) {
    counter2 = counter2 + 1;
    count2 = counter2 % 2;
  }
  //checks the counter data for performing actions
  if (count == 0 && c1 == 0) {
    c1 = 1;
    digitalWrite(sock, HIGH);
  }

  else if (count != 0 && c1 == 1) {
    c1 = 0;
    digitalWrite(sock, LOW);
  }

  if (count1 == 0 && c2 == 0) {
    c2 = 1;
    digitalWrite(light, HIGH);
  }

  else if (count1 != 0 && c2 == 1) {
    c2 = 0;
    digitalWrite(light, LOW);
  }

  if (count2 == 0 && c3 == 0) {
    c3 = 1;
    digitalWrite(fan, HIGH);
  }

  else if (count2 != 0 && c3 == 1) {
    c3 = 0;
    digitalWrite(fan, LOW);
  }

  if (client) {// if you get a client,

    while (client.connected()) {                        // loop while the client's connected
      if (client.available()) {                         // if there's bytes to read from the client,
        char c = client.read();                         // read a byte
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            //            sends the data to the client
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<t1>");
            client.print (person);
            client.println("</t1>");
            client.println("<t2>");
            client.println(limit);
            client.println("</t2>");
            client.println("<c1>");
            client.println(c1);
            client.println("</c1>");
            client.println("<c2>");
            client.println(c2);
            client.println("</c2>");
            client.println("<c3>");
            client.println(c3);
            client.println("</c3>");
            client.println("<alt>");
            client.println(al);
            client.println("</alt>");
            client.println("</html>");
            // break out of the while loop:
            break;
          }
          else {   // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') { // if you got a newline, then clear currentLine:
          currentLine += c;     // add it to the end of the currentLine
        }

        // Check to see the client request and perform actions accordingly
        if (currentLine.endsWith("GET /H") && c1 == 0) {
          count = 0;
          digitalWrite(sock, HIGH);
        }
        if (currentLine.endsWith("GET /L") && c1 == 1) {
          count = 1;
          digitalWrite(sock, LOW);
        }
        if (currentLine.endsWith("GET /O") && c2 == 0) {
          count1 = 0;
          digitalWrite(light, HIGH);
        }
        if (currentLine.endsWith("GET /C") && c2 == 1) {
          count1 = 1;
          digitalWrite(light, LOW);
        }
        if (currentLine.endsWith("GET /F") && c3 == 0) {
          count2 = 0;
          digitalWrite(fan, HIGH);
        }
        if (currentLine.endsWith("GET /B") && c3 == 1) {
          count2 = 1;
          digitalWrite(fan, LOW);
        }
        if (currentLine.endsWith("GET /U")) {
          limit = limit + 1;
          Serial.print(limit);
        }
        if (currentLine.endsWith("GET /D")) {
          limit = limit - 1;
          Serial.print(limit);
        }
      }
    }
    // close the connection:
    client.stop();
  }

  //checks the sensor value and limit to read human temperature
  if (US01() < 10 && person < limit) {
    Serial.println(mlx.readObjectTempC());
    //    reads the human temperature and check with the reference value
    if (mlx.readObjectTempC() < 37.7) {
      digitalWrite(latch, HIGH);
      digitalWrite(buzz, HIGH);
      delay(1000);
      digitalWrite(buzz, LOW);
      delay(4000);
      digitalWrite(latch, LOW);
      //      allows the person and adds 1 in counter
      person = person + 1;
      Serial.println(person);
    }
  }

  //checks the sensor value and limit
  if (US01() < 10 && person == limit) {
    digitalWrite(latch, LOW);
    digitalWrite(buzz, HIGH);
    delay(500);
    digitalWrite(buzz, LOW);
    delay(500);
    digitalWrite(buzz, HIGH);
    delay(500);
    digitalWrite(buzz, LOW);
  }
  //checks the sensor value and temperature
  if (US01() < 10 && mlx.readObjectTempC() > 37.7) {
    digitalWrite(latch, LOW);
    al = al + 1;
    digitalWrite(buzz, HIGH);
    delay(800);
    digitalWrite(buzz, LOW);
    delay(200);
    digitalWrite(buzz, HIGH);
    delay(800);
    digitalWrite(buzz, LOW);
  }

  //checks the sensor value and unlocks the door
  if (US02() < 10) {
    digitalWrite(latch, HIGH);
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
    delay(4000);
    digitalWrite(latch, LOW);
    //    deduce 1 from counter when the person leaves
    person = person - 1;
    Serial.println(person);
  }

  else {
    digitalWrite(latch, LOW);
  }
  // RFID Section (Handles RFID key and checks it with the reference key inorder to allow acess)
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // check the type of RFID tag
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }

  //variable to hold Tag id
  String strID = "";
  //loop to read tag id
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  //convert data to uppercase characters
  strID.toUpperCase();
  //  Serial.print("Tap card key: ");
  //  Serial.println(strID);
  delay(1000);

  if (strID.indexOf(" ") >= 0) { //put your own tap card key;
    digitalWrite(latch, HIGH);
    digitalWrite(buzz, HIGH);
    delay(500);
    digitalWrite(buzz, LOW);
    delay (5000);
    digitalWrite(latch, LOW);
    return;
  }
  else {
    digitalWrite(latch, LOW);
    return;
  }
}
