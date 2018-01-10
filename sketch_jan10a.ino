#define BAUD_RATE                 9600
#define ROW_COUNT                 2
#define COLUMN_COUNT              1

#define PIN_ADC_INPUT             A0
#define PIN_SHIFT_REGISTER_DATA   2
#define PIN_SHIFT_REGISTER_CLOCK  3
#define PIN_MUX_CHANNEL_0         4
#define PIN_MUX_INHIBIT_0         7  //inhibit = active low enable. All mux IC enables must be wired to consecutive Arduino pins
//#define PIN_MUX_INHIBIT_1         8

#define ROWS_PER_MUX              8
#define MUX_COUNT                 1
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(PIN_ADC_INPUT, INPUT);
  pinMode(PIN_SHIFT_REGISTER_DATA, OUTPUT);
  pinMode(PIN_SHIFT_REGISTER_CLOCK, OUTPUT);
  pinMode(PIN_MUX_CHANNEL_0, OUTPUT);
  pinMode(PIN_MUX_INHIBIT_0, OUTPUT);
  //pinMode(PIN_MUX_INHIBIT_1, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < ROW_COUNT; i ++)
  {
    setRow(i);
    //    shiftColumn(true);
    //    shiftColumn(false);                         //with SR clks tied, latched outputs are one clock behind

    int raw_reading = analogRead(PIN_ADC_INPUT);
    byte send_reading = (byte) (lowByte(raw_reading >> 2));
    // shiftColumn(false);
    printFixed(send_reading);
    Serial.print(" ");

    Serial.println();
  }
  Serial.println();
  delay(200);
}

/**********************************************************************************************************
  setRow() - Enable single mux IC and channel to read specified matrix row.
**********************************************************************************************************/
void setRow(int row_number)
{
  digitalWrite(PIN_MUX_INHIBIT_0, HIGH); // turning the mux on
  if (bitRead(row_number, 0))
  {
    digitalWrite(PIN_MUX_CHANNEL_0, HIGH);
  }
  else
  {
    digitalWrite(PIN_MUX_CHANNEL_0, LOW);
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

