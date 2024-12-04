#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define MTR_PIN 2
#define LED_PIN 4

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of RFID reader class
MFRC522::MIFARE_Key key;

Servo motor;  // Instance of servo motor class

String gCard = "1645211228";
String gTag ="1341922126";

boolean isOpen = false;
int tryCount = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  motor.attach(MTR_PIN);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    readRFID();
    tryCount = 0;
  } else {
    if (tryCount == 10) {
      Serial.println("No TAGs around, closing");
      closeLid();
      tryCount = 0;
    } else {
      tryCount += 1;
      delay(100);
    }
  }
  delay(100);
}

void readRFID() {
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Check is the PICC of Classic MIFARE type
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("\nYour tag is not of type MIFARE Classic."));
    blinkLed(0);
    return;
  }

  String uidString = String(rfid.uid.uidByte[0]) + String(rfid.uid.uidByte[1]) + String(rfid.uid.uidByte[2]) + String(rfid.uid.uidByte[3]);
  Serial.println(uidString);

  if (uidString == gTag || uidString == gCard) { // Check if is a match to Gaia TAGs
    Serial.println("TAG is a match!");
    openLid();
    delay(1000);
  } else {
    Serial.println("TAG not allowed, closing");
    closeLid();
  }

  // Halt PICC & Stop encryption on PCD
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1000);
  rfid.PCD_Init();
}

void blinkLed(int code) {
  switch (code) {
    case 0:  // Error to read TAG
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      break;
    default:
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
      break;
  }
}

void openLid() {
  if (isOpen)
    return;

  digitalWrite(LED_PIN, LOW);
  isOpen = true;

  motor.write(89);
  delay(5000);                    // Wait motor to open lid
  motor.writeMicroseconds(1500);  // Stops motor
}

void closeLid() {
  if (!isOpen)
    return;

  digitalWrite(LED_PIN, HIGH);
  isOpen = false;

  motor.write(99);
  delay(3650);                    // Wait motor to close lid
  motor.writeMicroseconds(1500);  // Stops motor
}
