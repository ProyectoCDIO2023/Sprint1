#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; // Crear una instancia del objeto ADS1115

#define power_pin 5 // Pin para alimentar el sensor de salinidad

#define channelValue 0
#define Offset 0.17
#define pH_samplingInterval 20
#define pH_printInterval 800
#define salinity_samplingInterval 20
#define salinity_printInterval 1000
#define ArrayLength 40 // numero de muestras
int pHArray[ArrayLength]; // almacena las muestras 
int pHArrayIndex = 0;

void setup() {
  pinMode(power_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de pH");

  // Inicializar el objeto ADS1115
  ads.begin();

  // Configurar la ganancia del ADS1115
  ads.setGain(GAIN_TWOTHIRDS); // Puedes ajustar la ganancia según sea necesario

  // Otras configuraciones del ADS1115 si son necesarias, como el modo de operación, tasa de datos, etc.
}

// Función que realiza la interpolación de Lagrange
float interpolacionLagrange(float x, float x0, float x1, float x2, float x3, float x4, float x5, float y0, float y1, float y2, float y3, float y4, float y5) {
  float L0 = ((x - x1) * (x - x2) * (x - x3) * (x - x4) * (x - x5)) / ((x0 - x1) * (x0 - x2) * (x0 - x3) * (x0 - x4) * (x0 - x5));
  float L1 = ((x - x0) * (x - x2) * (x - x3) * (x - x4) * (x - x5)) / ((x1 - x0) * (x1 - x2) * (x1 - x3) * (x1 - x4) * (x1 - x5));
  float L2 = ((x - x0) * (x - x1) * (x - x3) * (x - x4) * (x - x5)) / ((x2 - x0) * (x2 - x1) * (x2 - x3) * (x2 - x4) * (x2 - x5));
  float L3 = ((x - x0) * (x - x1) * (x - x2) * (x - x4) * (x - x5)) / ((x3 - x0) * (x3 - x1) * (x3 - x2) * (x3 - x4) * (x3 - x5));
  float L4 = ((x - x0) * (x - x1) * (x - x2) * (x - x3) * (x - x5)) / ((x4 - x0) * (x4 - x1) * (x4 - x2) * (x4 - x3) * (x4 - x5));
  float L5 = ((x - x0) * (x - x1) * (x - x2) * (x - x3) * (x - x4)) / ((x5 - x0) * (x5 - x1) * (x5 - x2) * (x5 - x3) * (x5 - x4));

  float y = (y0 * L0) + (y1 * L1) + (y2 * L2) + (y3 * L3) + (y4 * L4) + (y5 * L5);
  return y;
}

void readPHValue() {
  static unsigned long pH_samplingTime = millis();
  static unsigned long pH_printTime = millis();
  static float pHValue, voltage;

  if (millis() - pH_samplingTime > pH_samplingInterval) {
    int16_t adc0;
    digitalWrite(power_pin, HIGH);
    delay(100);
    adc0 = ads.readADC_SingleEnded(channelValue);
    digitalWrite(power_pin, LOW);
    delay(1000);
    voltage = adc0 * 0.0001875;
    pHValue = 3.5 * voltage + Offset;
    pHArray[pHArrayIndex++] = pHValue;
    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    pH_samplingTime = millis();
  }

  if (millis() - pH_printTime > pH_printInterval) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    pH_printTime = millis();
  }
}

void calculateSalinity() {
  static unsigned long salinity_samplingTime = millis();
  static unsigned long salinity_printTime = millis();

  if (millis() - salinity_samplingTime > salinity_samplingInterval) {
    // Aquí realiza la lectura analógica y calcula la salinidad con la interpolación de Lagrange
    int16_t adc0;
    digitalWrite(power_pin, HIGH);
    delay(100);
    adc0 = ads.readADC_SingleEnded(channelValue);
    digitalWrite(power_pin, LOW);
    delay(1000);

    // Puntos conocidos (0 gramos - 480 ADC0, 10 gramos - 597 ADC0, 15 gramos - 625 ADC0, 20 gramos - 639 ADC0, 25 gramos - 650 ADC0, 30 gramos - 673 ADC0)
    float x0 = 480;
    float x1 = 597;
    float x2 = 625;
    float x3 = 639;
    float x4 = 650;
    float x5 = 673;
    float y0 = 0;
    float y1 = 10;
    float y2 = 15;
    float y3 = 20;
    float y4 = 25;
    float y5 = 30;
    float salinidadEnGramos = interpolacionLagrange(adc0, x0, x1, x2, x3, x4, x5, y0, y1, y2, y3, y4, y5);

    Serial.print("Lectura digital = ");
    Serial.println(adc0, DEC);
    Serial.print("Salinidad (gramos) = ");
    Serial.println(salinidadEnGramos, 2);

    salinity_samplingTime = millis();
  }
}

void loop() {
  readPHValue();
  calculateSalinity();
}
