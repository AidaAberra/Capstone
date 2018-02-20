/*
 * CONNECT TO OUTGOING PORT HC-05 'DEV-B' TO VIEW DATA ON SERIAL MONITOR
 * USE THIS SKETCH ONLY FOR VIEWING SENSOR DATA ON SERIAL MONITOR.....NOT FOR FILE WRITING
 */
int temp; //variable to hold temperature sensor value
long tm,t,d; //variables to record time in seconds

void setup()
{
  Serial.begin(9600);
  pinMode(0,INPUT);//temperature sensor connected to analog 0
  analogReference(DEFAULT);
}

void loop()
{
  temp = analogRead(0); //analog reading temperature sensor values

  //required for converting time to seconds
  tm = millis();
  t = tm/1000;
  d = tm%1000;

  Serial.flush();

  // put your main code here, to run repeatedly:
  for (int i = 0; i < ROW_COUNT; i ++)
  {
    setRow(i);
                             //with SR clks tied, latched outputs are one clock behind
    for (int j = 0; j < COLUMN_COUNT; j++)
    {
      setColumn(j);
      int raw_reading = analogRead(zOutput);
      byte send_reading = (byte) (lowByte(raw_reading >> 2));
      printFixed(send_reading);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
  delay(200);
 
}
void setRow(int row_number)
{
  digitalWrite(PIN_MUX_INHIBIT_0, LOW); // turning the mux on, or HIGH
  digitalWrite(PIN_MUX_INHIBIT_1, LOW);
  for (int i = 0; i < CHANNEL_PINS_PER_MUX; i ++)
  {
    if (bitRead(row_number, i))
    {
      digitalWrite(PIN_MUX_CHANNEL_0 + i, HIGH);
    }
    else
    {
      digitalWrite(PIN_MUX_CHANNEL_0 + i, LOW);
    }
  }
}

void setColumn(int column_number)
{
  for (int i = 0; i < CHANNEL_PINS_PER_MUX; i ++)
  {
    if (bitRead(column_number, i))
    {
      digitalWrite(PIN_MUX_CHANNEL_3 + i, HIGH);
    }
    else
    {
      digitalWrite(PIN_MUX_CHANNEL_3 + i, LOW);
    }
  }
}
void printFixed(byte value)
{
  if (value < 10)
  {
    Serial.print("  ");
  }
  else if (value < 100)
  {
    Serial.print(" ");
  }
  Serial.print(value);
}
