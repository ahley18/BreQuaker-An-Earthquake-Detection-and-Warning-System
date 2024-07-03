#include <SoftwareSerial.h>
#include <MQ2.h>
#define MQ2pin 0

#define rxPin 2
#define txPin 3

SoftwareSerial sim800(rxPin, txPin);
int motor = 9;
int buttonState = 0;
int RESET = 7;

int RELAY = 8;
int BUZZ = 6;
int LED = 5;
int vs = 4; // vibration sensor
int i=0;

float smo;

void setup(){
  pinMode(RESET, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(vs, INPUT); 
  Serial.begin(9600);  // Initialize serial monitor
  sim800.begin(9600);  // Initialize SIM800L communication
  digitalWrite(motor, LOW);
  digitalWrite(RELAY, HIGH);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW); 
  initiate();
}

void initiate(){
  digitalWrite(BUZZ, 1);
  delay(1000);
  digitalWrite(BUZZ, 0);

  delay(10000);

  digitalWrite(BUZZ, 1);
  delay(250);
  digitalWrite(BUZZ, 0);
  delay(250);
  digitalWrite(BUZZ, 1);
  delay(250);
  digitalWrite(BUZZ, 0);
  delay(250);
}

void earthquake_message(){
    sim800.println("AT");
    delay(250);
    Serial.println("SIM800L is ready");
    sim800.println("AT+CMGF=1");
    delay(250);
    sim800.println("AT+CMGS=\"+639063465990\"");
    delay(250);
    sim800.print("Earthquake Detected in Household Area");
    sim800.write(0x1A);
    delay(1000);
    Serial.println("Message sent");
}

void reset_message(){
    sim800.println("AT");
    delay(250);
    Serial.println("SIM800L is ready");
    sim800.println("AT+CMGF=1");
    delay(250);
    sim800.println("AT+CMGS=\"+639063465990\"");
    delay(250);
    sim800.print("System Reset Initiated");
    sim800.write(0x1A);
    delay(1000);
    Serial.println("Message sent");
}

void smoke_message(){
    sim800.println("AT");
    delay(250);
    Serial.println("SIM800L is ready");
    sim800.println("AT+CMGF=1");
    delay(250);
    sim800.println("AT+CMGS=\"+639063465990\"");
    delay(250);
    sim800.print("Smoke Detected! \nSystem has turned off LPG supply.");
    sim800.write(0x1A);
    delay(1000);
    Serial.println("Message sent");
}

void lpg_message(){
    sim800.println("AT");
    delay(250);
    Serial.println("SIM800L is ready");
    sim800.println("AT+CMGF=1");
    delay(250);
    sim800.println("AT+CMGS=\"+639063465990\"");
    delay(250);
    sim800.print("LPG Leak Detected! \nSystem has turned off LPG supply.");
    sim800.write(0x1A);
    delay(1000);
    Serial.println("Message sent");
}

void loop(){
buttonState = digitalRead(RESET);
int vib = digitalRead(vs);
smo = analogRead(MQ2pin); 

  if (smo >= 300){
    digitalWrite(motor, HIGH);
    Serial.println("Smoke Detected!");
    delay(250);
    smoke_message();
    for(i=0;i<10;i++)
    {
    digitalWrite(BUZZ, HIGH);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(BUZZ, LOW);
    digitalWrite(LED, LOW); 
    delay(100);
    }
  }

  if (buttonState == LOW){
    Serial.println("1");
    Serial.println("System Reset Initiated");
    delay(300);
    reset_message();
    delay(1000);
    digitalWrite(motor, LOW);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZ, LOW); 
    
  }

  if(vib == HIGH)
  {
    digitalWrite(motor, HIGH);
    digitalWrite(RELAY, LOW);
    Serial.println("Earthquake detected");
    earthquake_message();
    for(i=0;i<10;i++)
    {
    digitalWrite(BUZZ, HIGH);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(BUZZ, LOW);
    digitalWrite(LED, LOW); 
    delay(100);
  }
  }
  else{
    Serial.println("0");
    Serial.println("No earthquake");
    Serial.print("Smoke Value: ");
	  Serial.println(smo);
    digitalWrite(LED, LOW); 
  }

delay(250);
}