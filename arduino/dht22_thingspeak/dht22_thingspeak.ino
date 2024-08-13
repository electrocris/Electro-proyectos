#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <WiFi.h>
#include "ThingSpeak.h"
#include "data_wifi.h"
// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuración del sensor DHT
#define DHTPIN 13 // Pin donde está conectado el DHT22
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

float h, t;

void setup() {
  Serial.begin(9600);

  // Inicializar el sensor DHT
  dht.begin();

  // Inicializar la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // No continuar, bucle infinito
  }

  // Borrar el buffer
  display.clearDisplay();
  display.display();
  delay(2000); // Pausa de 2 segundos

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");

  // Inicializar ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Failed to read from DHT sensor!"));
    display.display();
    return;
  }

  // Borrar el buffer de la pantalla
  display.clearDisplay();

  // Mostrar temperatura
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Temperatura: "));
  display.print(t);
  display.println(F("C"));

  // Mostrar humedad
  display.setCursor(0, 16);
  display.print(F("Humedad: "));
  display.print(h);
  display.println(F("%"));

  // Mostrar el buffer de la pantalla
  display.display();

  // Enviar datos a ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Actualización de canal exitosa.");
  } else {
    Serial.println("Problema al actualizar el canal. Código de error HTTP: " + String(x));
  }

  delay(30000); // Esperar 10 segundos antes de actualizar el canal nuevamente
}
