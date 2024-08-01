#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int encendido = 2; // Ajusta esto según tu configuración
const int apagado = 4; // Ajusta esto según tu configuración
const int modos = 5; // Nuevo LED para los modos
const char *pin = "1234"; // PIN para emparejamiento

void setup() {
  pinMode(encendido, OUTPUT);
  pinMode(apagado, OUTPUT);
  pinMode(modos, OUTPUT);
  Serial.begin(115200);

  // Inicia Bluetooth con el nombre del dispositivo
  if (!SerialBT.begin("Estimulador-Derecho")) {
    Serial.println("Error al iniciar Bluetooth");
  } else {
    Serial.println("El dispositivo está listo para emparejarse.");
  }

  // Establecer el PIN para el emparejamiento
  SerialBT.setPin(pin);
}

void Apagar() {
  for (int i = 0; i < 19; i++) {
    digitalWrite(apagado, HIGH);
    delay(50);
    digitalWrite(apagado, LOW);
    delay(50);
  }
  Serial.println("LED 2 pulsado 19 veces por 50ms");
}

void handleMode(const String& modeStr) {
  int mode = modeStr.toInt();
  Serial.printf("Modo recibido: %d\n", mode);
  for (int i = 0; i < mode-1; i++) {
    digitalWrite(modos, HIGH);
    delay(50);
    digitalWrite(modos, LOW);
    delay(50);
  }
}

void handleIntensity(const String& intensityStr) {
  int intensity = intensityStr.toInt();
  Serial.printf("Intensidad recibida: %d\n", intensity);
  //Apagar();
  for (int i = 0; i < intensity; i++) {
    digitalWrite(encendido, HIGH);
    delay(50);
    digitalWrite(encendido, LOW);
    delay(50);
  }
}

void handleTurnOn() {
  digitalWrite(encendido, HIGH);
  delay(50);
  digitalWrite(encendido, LOW);
  Serial.println("Salida de Encendido por 50ms");
}

void handleTurnOff() {
  Serial.println("Recibida señal para apagado");
  Apagar();
}

void handleIncomingString(const String& str) {
  if (str.length() == 0) {
    Serial.println("Comando vacío recibido");
    return;
  }
  if (str.startsWith("M")) {
    handleMode(str.substring(1));
  } else if (str.startsWith("I")) {
    handleIntensity(str.substring(1));
  } else if (str == "1") {
    handleTurnOn();
  } else if (str == "0") {
    handleTurnOff();
  } else {
    Serial.printf("Comando no reconocido: %s\n", str.c_str());
  }
}

void loop() {
  if (SerialBT.available()) {
    String incomingString = SerialBT.readStringUntil('\n');
    handleIncomingString(incomingString);
  }
}

