#define LED_ON_PIN 5
#define LED_OPN_PIN 6

#define BTN_OPN_PIN 7
#define BTN_CLSD_PIN 8

void setup() {
  Serial.begin(9600);

  pinMode(LED_ON_PIN, OUTPUT);
  pinMode(LED_OPN_PIN, OUTPUT);
  pinMode(BTN_OPN_PIN, INPUT);
  pinMode(BTN_CLSD_PIN, INPUT);

  digitalWrite(LED_ON_PIN, HIGH);
  Serial.println("Initialized");
}

void loop() {
  checkLidStatus();
  delay(2000);
}

void checkLidStatus() {
  if (digitalRead(BTN_OPN_PIN) == HIGH) {
    digitalWrite(LED_OPN_PIN, HIGH);
    Serial.println("OPEN pressed");
  }
  if (digitalRead(BTN_CLSD_PIN) == HIGH) {
    digitalWrite(LED_OPN_PIN, LOW);
    Serial.println("CLOSE pressed");
  }
}