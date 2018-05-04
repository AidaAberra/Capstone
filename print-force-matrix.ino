#define BAUD_RATE                 9600
#define ROW_COUNT                 2
#define COLUMN_COUNT              2

#define zOutput                  A0

// for deMUX
#define PIN_MUX_CHANNEL_0         2
//#define PIN_MUX_CHANNEL_1         3
//#define PIN_MUX_CHANNEL_2         4
#define PIN_MUX_INHIBIT_1         9 //enable pin for deMUX

//for MUX
#define PIN_MUX_CHANNEL_3         5
//#define PIN_MUX_CHANNEL_4         6
//#define PIN_MUX_CHANNEL_5         7
#define PIN_MUX_INHIBIT_0         8  //inhibit = active low enable. All mux IC enables must be wired to consecutive Arduino pins
//#define PIN_MUX_INHIBIT_1         8

#define ROWS_PER_MUX              8
#define MUX_COUNT                 2
#define CHANNEL_PINS_PER_MUX      1 

const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

void setup() {
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

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < ROW_COUNT; i ++)
  {
    setRow(i);
    //with SR clks tied, latched outputs are one clock behind
    for (int j = 0; j < COLUMN_COUNT; j++)
    {
      setColumn(j);
      //int raw_reading = analogRead(zOutput);
      //byte send_reading = (byte) (lowByte(raw_reading >> 2));
      int fsrADC = analogRead(zOutput);
      // If the FSR has no pressure, the resistance will be
      // near infinite. So the voltage should be near 0.
      float force;

      // Use ADC reading to calculate voltage:
      float fsrV = fsrADC * VCC / 1023.0;
      // Use voltage and static resistor value to
      // calculate FSR resistance:
      float fsrR = R_DIV * (VCC / fsrV - 1.0);
      //Serial.println("Resistance: " + String(fsrR) + " ohms");
      // Guesstimate force based on slopes in figure 3 of
      // FSR datasheet:

      float fsrG = 1.0 / fsrR; // Calculate conductance
      // Break parabolic curve down into two linear slopes:
      if (fsrR <= 600)
        force = (fsrG - 0.00075) / 0.00000032639;
      else
        force =  fsrG / 0.000000642857;

//prints the force of each resistor in grams
      printFixed(force);
      Serial.print(" ");

    }
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
void printFixed(float value)
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
