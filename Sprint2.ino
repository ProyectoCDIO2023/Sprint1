#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; // Crear una instancia del objeto ADS1115

#define channelValue 0     // Canal de lectura del ADC
#define Offset 0.17        // Offset para el cálculo del pH (ajuste específico según el sensor)
#define samplingInterval 20 // Intervalo de tiempo entre muestras de pH (en milisegundos)
#define printInterval 800  // Intervalo de tiempo para imprimir los resultados (en milisegundos)

int pHArray[ArrayLength]; // almacena las muestras 
int pHArrayIndex; // Índice actual en el array de muestras de pH

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de pH");

  // Inicializar el objeto ADS1115
  ads.begin();

  // Configurar la ganancia del ADS1115
  ads.setGain(GAIN_TWOTHIRDS); // Puedes ajustar la ganancia según sea necesario
}

void loop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;

  // Realizar muestreo a intervalos regulares
  if (millis() - samplingTime > samplingInterval) {
    // Realizar una lectura del canal del ADS1115
    int16_t adc0;
    adc0 = ads.readADC_SingleEnded(channelValue);

    // Convertir la lectura a voltaje (suponiendo que el rango de voltaje es de 0 a 3.3V)
    voltage = adc0 * 0.0001875; // La resolución es 0.1875 mV por bit para GAIN_TWOTHIRDS

    // Calcular el valor de pH (¡Esta es una ecuación de ejemplo, necesitas la ecuación adecuada para tu sensor!)
    pHValue = 3.5 * voltage + Offset;

    // Almacenar la lectura en el array
    pHArray[pHArrayIndex++] = pHValue;
    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;

    // Reiniciar el tiempo de muestreo
    samplingTime = millis();
  }

  // Imprimir los resultados a intervalos regulares
  if (millis() - printTime > printInterval) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    printTime = millis();
  }
}
