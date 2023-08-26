#include <SPI.h>
#include <SD.h>

const int originalCardChipSelect = 5; // CS pin pre ORIGINAL SD kartu
const int targetCardChipSelect = 10;  // CS pin pre TARGET SD kartu
const int externalLedPin = 3; // Externý diódový pin

void setup() {
  pinMode(externalLedPin, OUTPUT); // Nastavte externý diódový pin ako výstup
  
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(targetCardChipSelect, OUTPUT);
  if (!SD.begin(originalCardChipSelect)) {
    Serial.println("Nepodarilo sa inicializovať pôvodnú SD kartu.");
    return;
  }

  Serial.println("Pôvodná SD karta inicializovaná.");

  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    if (!entry.isDirectory()) {
      copyFile(entry.name());
    }
    entry.close();
  }
  
  // Blikanie externej LED diódy 10 krát
  for (int i = 0; i < 10; i++) {
    digitalWrite(externalLedPin, HIGH);
    delay(200);
    digitalWrite(externalLedPin, LOW);
    delay(200);
  }
  
  Serial.println("Všetky súbory boli skopírované.");
}

void loop() {
  
}

void copyFile(const char* filename) {
  File originalFile = SD.open(filename);
  if (!originalFile) {
    Serial.println("Nepodarilo sa otvoriť pôvodný súbor.");
    return;
  }

  File targetFile = SD.open(filename, FILE_WRITE);
  if (!targetFile) {
    Serial.println("Nepodarilo sa otvoriť cieľový súbor.");
    originalFile.close();
    return;
  }

  while (originalFile.available()) {
    targetFile.write(originalFile.read());
  }

  originalFile.close();
  targetFile.close();

  Serial.println("Súbor bol úspešne skopírovaný.");
}
