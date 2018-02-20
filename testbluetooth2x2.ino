/*
 * CONNECT TO OUTGOING PORT HC-05 'DEV-B' TO VIEW DATA ON SERIAL MONITOR
 * USE THIS SKETCH ONLY FOR VIEWING SENSOR DATA ON SERIAL MONITOR.....NOT FOR FILE WRITING
 */
int temp; //variable to hold temperature sensor value
long tm,t,d; //variables to record time in seconds
#define BAUD_RATE             9600
#define ROW_COUNT                 2
#define COLUMN_COUNT              2

#define zOutput                  A0

// for deMUX
#define PIN_MUX_CHANNEL_0         2
//#define PIN_MUX_CHANNEL_1       3
//#define PIN_MUX_CHANNEL_2       4
#define PIN_MUX_INHIBIT_1         9 //enable pin for deMUX

//for MUX
#define PIN_MUX_CHANNEL_3         5
//#define PIN_MUX_CHANNEL_4       6
//#define PIN_MUX_CHANNEL_5       7
#define PIN_MUX_INHIBIT_0         8  //inhibit = active low enable. All mux IC enables must be wired to consecutive Arduino pins
//#define PIN_MUX_INHIBIT_1       8

#define ROWS_PER_MUX              8
#define MUX_COUNT                 2
#define CHANNEL_PINS_PER_MUX      1 //changed this

void setup()
{
  analogReference(DEFAULT);

  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(zOutput, INPUT);
  //pinMode(zOutput1, INPUT);

  pinMode(PIN_MUX_CHANNEL_0, OUTPUT);
//  pinMode(PIN_MUX_CHANNEL_1, OUTPUT);
//  pinMode(PIN_MUX_CHANNEL_2, OUTPUT); 
  pinMode(PIN_MUX_INHIBIT_0, OUTPUT);
  pinMode(PIN_MUX_CHANNEL_3, OUTPUT);
//  pinMode(PIN_MUX_CHANNEL_4, OUTPUT);
//  pinMode(PIN_MUX_CHANNEL_5, OUTPUT); //extra?
  pinMode(PIN_MUX_INHIBIT_1, OUTPUT);
}

void loop()
{
  //temp = analogRead(0); //analog reading temperature sensor values

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
