#define power_pin 5 // Pin para alimentar el sensor de salinidad

void setup() {
  pinMode(power_pin, OUTPUT);
  Serial.begin(9600);
}

// Función que realiza la interpolación de Lagrange
float interpolacionLagrange(float x, float x0, float x1, float x2, float y0, float y1, float y2) {
  float L0 = ((x - x1) * (x - x2)) / ((x0 - x1) * (x0 - x2));
  float L1 = ((x - x0) * (x - x2)) / ((x1 - x0) * (x1 - x2));
  float L2 = ((x - x0) * (x - x1)) / ((x2 - x0) * (x2 - x1));

  float y = (y0 * L0) + (y1 * L1) + (y2 * L2);
  return y;
}

void loop() {
  int16_t adc0; // Alimentamos la sonda con un tren de pulsos
  digitalWrite(power_pin, HIGH); 
  delay(100);
  // Leemos cuando hay un nivel alto
  adc0 = analogRead(A0);
  digitalWrite(power_pin, LOW);
  delay(1000);

  // Puntos conocidos (0 gramos - 480 ADC0, 10 gramos - 597 ADC0, 25 gramos - 650 ADC0)
  float x0 = 480;
  float x1 = 597;
  float x2 = 650;
  float y0 = 0;
  float y1 = 10;
  float y2 = 25;

  // Calculamos la salinidad en gramos
  float salinidadEnGramos = interpolacionLagrange(adc0, x0, x1, x2, y0, y1, y2);

  // Presentamos la lectura
  Serial.print("Lectura digital = ");
  Serial.println(adc0, DEC);
  Serial.print("Salinidad (gramos) = ");
  Serial.println(salinidadEnGramos, 2); // Muestra la salinidad en gramos con 2 decimales
}
