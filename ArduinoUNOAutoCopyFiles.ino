#include <SPI.h>
#include <SD.h>

const int originalCardChipSelect = 5; // CS pin pre pôvodnú SD kartu
const int targetCardChipSelect = 10;  // CS pin pre cieľovú SD kartu
const int externalLedPin = 3; // Externý diódový pin pre úspešné kopírovanie
const int emptyCardLedPin = 4; // Oranžová diódová pin pre prázdnu SD kartu
const int initErrorLedPin = 2; // Červená diódová pin pre zlyhanú inicializáciu SD karty

void setup() {
  pinMode(externalLedPin, OUTPUT); // Nastavte externý diódový pin ako výstup
  pinMode(emptyCardLedPin, OUTPUT); // Nastavte diódový pin pre prázdnu kartu ako výstup
  pinMode(initErrorLedPin, OUTPUT); // Nastavte diódový pin pre zlyhanú inicializáciu ako výstup
  
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(targetCardChipSelect, OUTPUT);
  if (!SD.begin(originalCardChipSelect)) {
    Serial.println("Nepodarilo sa inicializovať pôvodnú SD kartu.");
    return;
  }

  Serial.println("Pôvodná SD karta inicializovaná.");

  File targetRoot = SD.open("/");
  if (targetRoot) {
    bool targetSDEmpty = true;
    while (true) {
      File entry = targetRoot.openNextFile();
      if (!entry) {
        break;
      }
      if (!entry.isDirectory()) {
        copyFile(entry.name());
        targetSDEmpty = false;
      }
      entry.close();
    }
    
    if (targetSDEmpty) {
      Serial.println("TargetSD-EMPTY!");
      digitalWrite(emptyCardLedPin, HIGH); // Rozsvieti oranžovú LED diódu pre prázdnu kartu
    }
    
    targetRoot.close();
  } else {
    Serial.println("Nepodarilo sa otvoriť priečinok na cieľovej SD karte.");
    digitalWrite(initErrorLedPin, HIGH); // Rozsvieti červenú LED diódu pre zlyhanú inicializáciu SD karty
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
  File targetFile = SD.open(filename);
  if (!targetFile) {
    Serial.println("Nepodarilo sa otvoriť cieľový súbor.");
    return;
  }

  File originalFile = SD.open(filename, FILE_WRITE);
  if (!originalFile) {
    Serial.println("Nepodarilo sa otvoriť pôvodný súbor.");
    targetFile.close();
    return;
  }

  while (targetFile.available()) {
    originalFile.write(targetFile.read());
  }

  targetFile.close();
  originalFile.close();

  Serial.println("Súbor bol úspešne skopírovaný.");
}
