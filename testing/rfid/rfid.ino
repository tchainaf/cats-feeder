#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of RFID reader class
MFRC522::MIFARE_Key key;

String gCard = "1645211228";
String gTag = "1341922126";

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);

  Serial.println("Alcance: " + rfid.PCD_GetAntennaGain());
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    readRFID();
  }
  delay(1000);
}

void readRFID() {
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Check is the PICC of Classic MIFARE type
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("\nYour tag is not of type MIFARE Classic."));
    return;
  }

  String uidString = String(rfid.uid.uidByte[0]) + String(rfid.uid.uidByte[1]) + String(rfid.uid.uidByte[2]) + String(rfid.uid.uidByte[3]);
  Serial.print(uidString);

  if (uidString == gTag || uidString == gCard) {  // Check if is a match to Gaia TAGs
    Serial.println(" - TAG is a match!");
    delay(1000);
  } else {
    Serial.println(" - TAG not allowed");
  }

  // Halt PICC & Stop encryption on PCD
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1000);
  rfid.PCD_Init();
}