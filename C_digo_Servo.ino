#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include <Servo.h>

Servo meuServo;

const int pinoPIR = 3; //PINO DIGITAL UTILIZADO PELO SENSOR DE PRESENÇA

char ssid[] = "iPhone Airton";
char pass[] = "2153818aa";
char serverAddress[] = "https://api.tago.io/data";  // TagoIO address
char contentHeader[] = "application/json";
char tokenHeader[]   = "c4123b90-822d-45a0-bf0d-8c77662f09a6"; // TagoIO Token

porta == 0;


void setup() {​
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_wifi();

  meuServo.attach(porta);
  meuServo.write(0); // Inicia motor posição zero
  pinMode(pinoPIR, INPUT); //DEFINE O PINO COMO ENTRADA
  
}​
void init_wifi() {​
  Serial.println("Conectando WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {​
    Serial.print(".");
    delay(1000);
  }​
  Serial.println("Conectado");
  Serial.print("Meu IP eh: ");
  Serial.println(WiFi.localIP());
}​

float temperatura = 0;
void loop() {​

  lerSensor();

  if (porta == 1) {
      moverServo(90);  // Move o servo para 90 graus
      enviarPortaAberta()
    else {
      moverServo(0);  // Mantém o servo na posição 0 graus
    }
  }
  


}​

void lerSensor(){
  if(digitalRead(pinoPIR) == HIGH){
    porta == 1;
  }else{
    porta == 0;
  }
 }

void moverServo(int angulo) {
  meuServo.write(angulo);  // Move o servo para a posição especificada
  delay(500);  // Aguarda um curto período para o servo atingir a posição desejada
}

void enviarPortaAberta() {
  char anyData[30];
  char postData[300];
  char anyData1[30];
  char bAny[30];
  int statusCode = 0;
  strcpy(postData, "{​\n\t\"variable\": \"Status Porta\",\n\t\"value\": \"Aberta\n");
  strncat (postData, anyData1, 100);
  Serial.println(postData);
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(postData);
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");
  Serial.println();
}
