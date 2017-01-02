int analogPin5 = 5;
int raw = 0;
int Vin = 5;
double Vout = 0;
double R1 = 1000;
double R2 = 0;  //Unknown resistance
double buffer = 0;
double Iout = 0;

//470 μF to 18 pF Capacitors
//Pretty accurate with smaller capacitors
const int OUT_PIN = A4;
const int IN_PIN = A3;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;  
const int MAX_ADC_VALUE = 1023;

int ledPin = 13;

void setup()
{ 
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  raw = analogRead(analogPin5);
  if (raw)
  {
    //digitalWrite(ledPin, HIGH);
    //delay(1000);
    //digitalWrite(ledPin, LOW);
    //delay(1000);
    buffer = raw * Vin;
    Vout = (buffer) / 1024.0;
    buffer = (Vin / Vout) - 1;
    R2 = R1 * buffer;
    Iout = Vout / R2;
    Serial.print("Vout: ");
    Serial.print(Vout);
    Serial.println(" V");

    Serial.print("Iout: ");
    Serial.print(Iout);
    Serial.println(" A");
    
    Serial.print("R2: ");
    Serial.print(R2);
    Serial.println(" Ohms");
    //delay(2000);
  }
  pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);

    if (val < 1000)
    {
      pinMode(IN_PIN, OUTPUT);

      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

      Serial.print(F("Capacitance Value = "));
      Serial.print(capacitance, 3);
      Serial.print(F(" pF ("));
      Serial.print(val);
      Serial.println(F(") "));
    }
    else
    {
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;

      do
      {
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } while ((digVal < 1) && (t < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);

      float capacitance = -(float)t / R_PULLUP
                              / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

      Serial.print(F("Capacitance Value = "));
      if (capacitance > 1000.0)
      {
        Serial.print(capacitance / 1000.0, 2);
        Serial.print(F(" uF"));
      }
      else
      {
        Serial.print(capacitance, 2);
        Serial.print(F(" nF"));
      }

      Serial.print(F(" ("));
      Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
      Serial.print(F(", t= "));
      Serial.print(t);
      Serial.print(F(" us, ADC= "));
      Serial.print(val);
      Serial.println(F(")"));
    }
    while (millis() % 1000 != 0)
      ;  

}



