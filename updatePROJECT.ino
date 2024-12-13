#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define BUZZER 3 //buzzer pin
const int limitSwitchPin = 7; // Pin connected to the NO terminal of the switch
int switchState = 0;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name
Servo myServo2;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(2); //servo pin
  myServo.write(0); //servo start position
  myServo2.attach(6);
  myServo2.write(0);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  pinMode(limitSwitchPin, INPUT); // Set pin as input
  Serial.begin(9600); // Start serial communication
  Serial.println("Put your card to the reader...");
  Serial.println();

}
void loop() 
{
  switchState = digitalRead(limitSwitchPin); // Read the state of the switch
  myServo.write(180);
  myServo2.write(0);   
   
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    tone(BUZZER, 300);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    return;
  }
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x19 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x19 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();

  Serial.print("Message : ");
  content.toUpperCase();


  if (content.substring(1) == "C3 52 29 010") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access"); 
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(0);
    delay(2000);
    myServo2.write(90);
    delay(2000);
    myServo2.write(30);
    delay(500);
    myServo2.write(0);
    delay(2000);
    myServo.write(180);
    delay(2000);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 100);
    delay(1000);
    tone(BUZZER, 150);
    delay(1000);
    tone(BUZZER, 250);
    delay(1000);
    tone(BUZZER, 350);
    delay(1000);
    tone(BUZZER, 250);
    delay(1000);
    tone(BUZZER, 150);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
  
} 

