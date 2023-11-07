#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include <ESP32Servo.h>

#define servoPIN 26

Servo meuServo;

int serialOrder = 0;
int previousOpen = 0;
char serialCommand;

const int trigPin = 23;
const int echoPin = 22;

long duration;
int distance;

const int trigPin2 = 7;
const int echoPin2 = 8;

long duration2;
int distance2; 

int aberturas = 0;
int porta = 0; // Initialize porta variable

char ssid[] = "Vini"; 
char pass[] = "12345678";
char serverAddress[] = "https://api.tago.io/data";  // TagoIO address
char contentHeader[] = "application/json";
char tokenHeader[] = "d2e67330-bc54-4e6a-902d-a7bf4e67846f"; // TagoIO Token

HTTPClient client;  // Declare an instance of the HTTPClient class

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  init_wifi();

  meuServo.attach(servoPIN);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void init_wifi() {
  Serial.println("Conectando WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Conectado");
  Serial.print("Meu IP eh: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  lerSerial();

  if (serialOrder == 1) {
    moverServo(90);  // Move the servo to 90 degrees
    if (previousOpen == 0) {
      previousOpen = 1;
      aberturas++;
      enviarPortaAberta();
    }
    Serial.println("A porta esta aberta");
  } else {
    lerSensor();

    if (porta == 1) {
      moverServo(90);  // Move the servo to 90 degrees
      if (previousOpen == 0) {
        previousOpen = 1;
        aberturas++;
        enviarPortaAberta();
      }
      Serial.println("A porta esta aberta");
    } else {
      moverServo(0);  // Keep the servo at 0 degrees
      previousOpen = 0;
    }
  }
}

void lerSerial() {
  if (Serial.available() > 0) {
    serialCommand = Serial.read();
    // Check the command and perform the desired action
    if (serialCommand == 'O') {
      serialOrder = 1; // Open the door
    } else if (serialCommand == 'C') {
      serialOrder = 0; // Close the door
    }
  }
}

void lerSensor() {
  // Read Sensor 1

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // Read Sensor 2

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  Serial.print("Distance2: ");
  Serial.println(distance2);


  if (distance <= 10 || distance2 <= 10) {
    porta = 1; // Set porta to 1 if either distance is less than or equal to 10
  } else {
    porta = 0; // Otherwise, set porta to 0
  }
}

void moverServo(int angulo) {
  meuServo.write(angulo);  // Move the servo to the specified position
}

void enviarPortaAberta() {
  char anyData[30];
  char postData[300];
  char anyData1[30];
  char bAny[30];
  int statusCode = 0;
  
  strcpy(postData, "{\n\t\"variable\": \"Aberturas\",\n\t\"value\": ");
  dtostrf(aberturas, 6, 2, anyData);
  strncat(postData, anyData, 100);
  strcpy(anyData1, ",\n\t\"unit\": \"C\"\n\t}\n");
  strncat (postData, anyData1, 100);
  Serial.println(postData);
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(postData);
  delay (500);
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");
  Serial.println();
}
