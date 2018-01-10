/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensor1 = analogRead(A0);
  int sensor2 = analogRead(A1);
  // print out the value you read:
  Serial.print("this is 1: ");
  Serial.println(sensor1);
  Serial.print("this is 2: ");
  Serial.println(sensor2);
  delay(100);        // delay in between reads for stability
}
