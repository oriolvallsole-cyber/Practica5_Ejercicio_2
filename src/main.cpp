#include <SPI.h>
#include <MFRC522.h>

// Pines (ajústalos si tu placa usa otros)
#define SS_PIN 10   // SDA / CS
#define RST_PIN 9   // Reset

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  delay(2000);

  SPI.begin(12, 13, 11, SS_PIN);  // SCK, MISO, MOSI, SS

  mfrc522.PCD_Init();
  Serial.println("Lectura del UID");
}

void loop() {
  // Detectar tarjeta
  if (mfrc522.PICC_IsNewCardPresent()) {

    // Leer tarjeta
    if (mfrc522.PICC_ReadCardSerial()) {

      Serial.print("Card UID:");

      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }

      Serial.println();

      // Finalizar lectura
      mfrc522.PICC_HaltA();
    }
  }
}