#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define RST_PIN 9  // Configurable, see typical pin layout above
#define SS_PIN 10  // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial nodemcu(5, 6);
int led = 4;
int lock = 3;

void setup() {
  pinMode(lock, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);  // Initialize serial communications with the PC
  nodemcu.begin(115200);
  while (!Serial)
    ;  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();
  digitalWrite(lock, LOW);
  digitalWrite(led, HIGH);  // Init SPI bus
  mfrc522.PCD_Init();       // Init MFRC522
  delay(4);
  digitalWrite(led, LOW);             // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

bool readRes() {
  String msg = nodemcu.readString();

  if (msg.length() > 0) {
    Serial.println(msg);

    if (msg.startsWith("1")) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("ACCESS GRANTED.");
      digitalWrite(lock, HIGH);
      delay(2000);
      digitalWrite(lock, LOW);
      digitalWrite(led, LOW);
      msg = "-1";
    }

    if (msg.startsWith("0")) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("ACCESS DENIED.");
      digitalWrite(lock, LOW);
      digitalWrite(led, LOW);
      msg = "-1";
    }

    return true;
  } 
  else {
    Serial.println("Processing..");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Processing..");
    return false;
  }
}

void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("LOCKED");
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  digitalWrite(led, HIGH);

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";

  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.print("\n");

  digitalWrite(led, HIGH);

  nodemcu.println(content);

  for (;;){
    bool resultRecieved = readRes();
    if(resultRecieved){
      break;
    }
  }

  // delay(3000);
  digitalWrite(led, LOW);
}