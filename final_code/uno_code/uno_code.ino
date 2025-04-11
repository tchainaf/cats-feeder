#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_ON_PIN 5
#define LED_OPN_PIN 6

#define MTR1_PIN 2
#define VEL_PIN 3
#define MTR2_PIN 4

#define BTN_OPN_PIN 7
#define BTN_CLSD_PIN 8

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of RFID reader class
MFRC522::MIFARE_Key key;

String gCard = "1645211228";
String gTag = "1341922126";
//1341922126 - yellow tag
//5924741210 - white tag
//425313227 - c card

boolean isOpen = false;
int tryCount = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);

  pinMode(LED_ON_PIN, OUTPUT);
  pinMode(LED_OPN_PIN, OUTPUT);
  pinMode(VEL_PIN, OUTPUT);
  pinMode(MTR1_PIN, OUTPUT);
  pinMode(MTR2_PIN, OUTPUT);

  digitalWrite(LED_ON_PIN, HIGH);
  analogWrite(VEL_PIN, 50);  //Motor velocity
  checkLidStatus();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    readRFID();
    tryCount = 0;
  } else {
    if (tryCount == 10) {
      Serial.println("...");
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
  Serial.print(uidString);

  if (uidString == gTag || uidString == gCard) {  // Check if is a match to Gaia TAGs
    Serial.println(" - TAG is a match!");
    openLid();
    delay(1000);
  } else {
    Serial.println(" - TAG not allowed");
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
      digitalWrite(LED_ON_PIN, LOW);
      delay(500);
      digitalWrite(LED_ON_PIN, HIGH);
      delay(500);
      digitalWrite(LED_ON_PIN, LOW);
      delay(500);
      digitalWrite(LED_ON_PIN, HIGH);
      delay(500);
      digitalWrite(LED_ON_PIN, LOW);
      break;
    default:
      digitalWrite(LED_ON_PIN, HIGH);
      delay(500);
      digitalWrite(LED_ON_PIN, LOW);
      delay(500);
      digitalWrite(LED_ON_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_ON_PIN, LOW);
      break;
  }
}

void openLid() {
  if (isOpen)
    return;
  analogWrite(VEL_PIN, 50);  //Motor velocity

  Serial.println("...Opening...");
  digitalWrite(MTR1_PIN, LOW);
  digitalWrite(MTR2_PIN, HIGH);

  while (!isOpen) {
    checkLidStatus();
    delay(500);
  }
}

void closeLid() {
  if (!isOpen)
    return;
  analogWrite(VEL_PIN, 80);  //Motor velocity

  Serial.println("...Closing...");
  digitalWrite(MTR1_PIN, HIGH);
  digitalWrite(MTR2_PIN, LOW);

  while (isOpen) {
    checkLidStatus();
    delay(500);
  }
}

void stopMotor() {
  digitalWrite(MTR1_PIN, LOW);
  digitalWrite(MTR2_PIN, LOW);
}

void checkLidStatus() {
  if (digitalRead(BTN_OPN_PIN) == HIGH) {
    stopMotor();
    if (!isOpen) {
      isOpen = true;
      digitalWrite(LED_ON_PIN, LOW);
      digitalWrite(LED_OPN_PIN, HIGH);
      Serial.println("Lid is OPEN");
    }
  } else if (digitalRead(BTN_CLSD_PIN) == HIGH) {
    stopMotor();
    if (isOpen) {
      isOpen = false;
      digitalWrite(LED_ON_PIN, HIGH);
      digitalWrite(LED_OPN_PIN, LOW);
      Serial.println("Lid is CLOSED");
    }
  }
}