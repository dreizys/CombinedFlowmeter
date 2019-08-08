/*
Arduino Water flow meter
YF- S201 Hall Effect Water Flow Sensor
Water Flow Sensor output processed to read in litres/min
*/
#define FIRST_SEGMENT_PIN   3
#define SEGMENT_COUNT       8
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_min; // Calculated litres/min
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
byte numbers[12] ={ 
 0b00111111,
 0b00000110,
 0b01011011,
 0b01001111,
 0b01100110,
 0b01101101,
 0b01111101,
 0b00000111,
 0b01111111,
 0b01101111,
 0b10110000,
 0b01110001,

};

void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
 //setup the 7-digit indicator
   for (int i = 0; i < SEGMENT_COUNT; ++i) 
    pinMode(i + FIRST_SEGMENT_PIN, OUTPUT);
    
}
void loop ()
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      //l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      l_min = (flow_frequency / 7.5); // (Pulse frequency ) / 7.5Q = flowrate in L/min
      flow_frequency = 0; // Reset Counter
      //Serial.print(l_min, DEC); // Print litres/hour
      //Serial.println(" L/min");
      int number  = l_min;
         if(number > 10){ 
            number = 11;             
         }
      int mask = numbers[number];
      for (int i = 0; i < SEGMENT_COUNT; ++i) {
         boolean enableSegment = bitRead(mask, i);
         // включаем/выключаем сегмент на основе полученного значения
          digitalWrite(i + FIRST_SEGMENT_PIN, enableSegment);
       }
   }
}
