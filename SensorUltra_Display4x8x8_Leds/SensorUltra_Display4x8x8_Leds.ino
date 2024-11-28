#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Configuração do Bluetooth (opcional)
SoftwareSerial bt(3, 2); // Rx = 3, Tx = 2

// Definir os pinos para o sensor HC-SR04
#define TRIGGER_PIN 8
#define ECHO_PIN 9

// Configuração dos LEDs
#define GREEN_LED1 2
#define GREEN_LED2 3
#define YELLOW_LED1 4
#define YELLOW_LED2 5
#define RED_LED1 6
#define RED_LED2 7

// Configuração do display
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   12
#define DATA_PIN  10
#define CS_PIN    11

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Buffer para a mensagem
#define BUF_SIZE 75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Distancia:  " };
bool newMessageAvailable = true;

void setup() {
  Serial.begin(57600);
  bt.begin(9600);

  // Configuração dos pinos do sensor HC-SR04
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configuração dos LEDs
  pinMode(GREEN_LED1, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(YELLOW_LED1, OUTPUT);
  pinMode(YELLOW_LED2, OUTPUT);
  pinMode(RED_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);

  // Configuração do display
  P.begin();
  P.setIntensity(15);  // Define a intensidade do display
  P.displayText(curMessage, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT); // Texto estático
}

void loop() {
  // Ler a distância do sensor HC-SR04
  long duration, distance;

  // Enviar o sinal de Trigger
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Ler o sinal de Echo
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calcular a distância (em centímetros)
  distance = duration * 0.0344 / 2;  // Dividir por 2 pois o som vai e volta

  // Converter a distância para string para exibir no display
  snprintf(newMessage, BUF_SIZE, " %ld cm", distance);
  newMessageAvailable = true;

  // Exibir no display
  if (P.displayAnimate()) {
    if (newMessageAvailable) {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }

  // Lógica para acender os LEDs
  if (distance > 20) {
    // Acende os LEDs verdes
    digitalWrite(GREEN_LED1, HIGH);
    digitalWrite(GREEN_LED2, HIGH);
    digitalWrite(YELLOW_LED1, LOW);
    digitalWrite(YELLOW_LED2, LOW);
    digitalWrite(RED_LED1, LOW);
    digitalWrite(RED_LED2, LOW);
  } 
  else if (distance > 10 && distance <= 20) {
    // Acende os LEDs amarelos
    digitalWrite(GREEN_LED1, LOW);
    digitalWrite(GREEN_LED2, LOW);
    digitalWrite(YELLOW_LED1, HIGH);
    digitalWrite(YELLOW_LED2, HIGH);
    digitalWrite(RED_LED1, LOW);
    digitalWrite(RED_LED2, LOW);
  } 
  else {
    // Acende os LEDs vermelhos
    digitalWrite(GREEN_LED1, LOW);
    digitalWrite(GREEN_LED2, LOW);
    digitalWrite(YELLOW_LED1, LOW);
    digitalWrite(YELLOW_LED2, LOW);
    digitalWrite(RED_LED1, HIGH);
    digitalWrite(RED_LED2, HIGH);
  }

  // Adicionar um pequeno delay para estabilidade
  delay(500);
}
