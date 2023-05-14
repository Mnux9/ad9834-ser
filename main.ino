#include "ad9834.h"

#define FSYNC   8
#define CLK     9
#define DATA    10
#define FXTAL   50000000                 // F_MCLK is 75 MHz

AD9834 ad9834(FSYNC, CLK, DATA);         // Arduino pins to FSYNC, CLK, DATA

String fset = "25000000";
uint32_t out = 14000000;

//Serial stuff
static String buffer;

#define wf  TRIANGLE
//---------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  ad9834.init(FXTAL);
  ad9834.set_waveform(SINE);
  ad9834.internal_clock_control(0);

  ad9834.set_freq(out);

  Serial.begin(9600);
  Serial.println("---------INIT----------");
  Serial.println("INITIALIZED AD983 WITH:");
  Serial.print("LO frequency: ");
  Serial.println(FXTAL);
  Serial.print("OUT frequency: ");
  Serial.println(out);
  Serial.print("Waveform: ");
  Serial.println("SINE");
  Serial.println("-----------------------");
  Serial.println("");
  Serial.println("Enter a frequency in Hz or a waveform (TRIANGLE, SINE):");
  Serial.println("");

  fset = 1000000;
}


void loop() {
    while (Serial.available() > 0) {
        char rx = Serial.read();
        buffer += rx;
        if (rx != '\n') { // THIS IS IMPORTANT!!! -M
            continue;
        }
        buffer.trim();
        
        //setting the waveform to triangle
        if (buffer.equals("TRIANGLE")) {
            Serial.println("Output wavefor set to: TRIANGLE");
            ad9834.set_waveform(TRIANGLE);
        }

        //setting the waveform to sine
        else if (buffer.equals("SINE")) {
            Serial.println("Output wavefor set to: SINE");
            ad9834.set_waveform(SINE);
        }

        else if (buffer.startsWith("F")) {
            fset = buffer.substring(1,20);
            Serial.print("Output frequency set to: ");
            Serial.println(fset);
            //out = atoi(fset.c_str());
            out = strtoul(fset.c_str(), NULL, 10);
            ad9834.set_freq(out);
        }
        
      buffer = "";
    }
  

}
