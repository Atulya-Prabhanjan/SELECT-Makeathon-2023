#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     10000
// Create a PulseOximeter object
PulseOximeter pox;
// Time at which the last beat occurred
uint32_t tsLastReport = 0;
int fsrPin = 0;     
int fsrReading;     
int fsrVoltage;     
unsigned long fsrResistance;  
unsigned long fsrConductance; 
long fsrForce;       
double a;
float t1,t;
float x[7];
float y[7];

void onBeatDetected() {
    //Serial.println("Beat!");
}

void setup() {
    Serial.begin(9600);

    //Serial.print("Initializing pulse oximeter..");

    // Initialize sensor
    if (!pox.begin()) {
        //Serial.println("FAILED");
        for(;;);
    } else {
        //Serial.println("SUCCESS");
    }

    // Configure sensor to use 7.6mA for LED drive
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    // Read from the FSR sensor
    fsrReading = analogRead(fsrPin);  
    fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
    fsrResistance = 5000 - fsrVoltage; 
    fsrResistance *= 10000;             
    fsrResistance /= fsrVoltage;
    fsrConductance = 1000000;           
    fsrConductance /= fsrResistance;

    // Read from the pulse oximeter sensor
    pox.update();

    // Display both sensor data simultaneously
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
       int b = pox.getHeartRate();
        Serial.print("Heart rate: ");
        Serial.print(b);
        Serial.print(" bpm");
    for(int i=0;i<8;i++)
    {
        y[i] = b;
    }
     for(int i=0;i<8;i++)
     {
        for(int j=i+1;j<8;j++)
        {
            if(y[i]>y[j])
            {
                t=y[i];
                x[i]=y[j];
                y[j]=t;
            }
        }
     } 

        if (fsrConductance <= 1000)
         {
            fsrForce = fsrConductance / 80;
            a = fsrForce*0.00750062/0.0001266769;
            Serial.print("Pressure: ");
            Serial.print(a);
            Serial.println(" kPa");
          } else 
          {
            fsrForce = fsrConductance - 1000;
            fsrForce /= 30;
            a = fsrForce*0.00750062/0.0001266769;
            Serial.print("Pressure: ");
            Serial.print(a);
            Serial.println(" kPa");
          }
          for(int i=0;i<8;i++){
        x[i] = a;
        }
     for(int i=0;i<8;i++){
        for(int j=i+1;j<8;j++){
            if(x[i]>x[j]){
                t=x[i];
                x[i]=x[j];
                x[j]=t;
            }
        }
    }
    tsLastReport=millis();
    }
    float a1 = y[7];
    float a2 = (y[1]+y[2])/2;
    float b1=(x[3]+x[4])/2;
    float b2 =(x[1]+x[2])/2; 
    float b3 = b2; 
    float DBP = 0.65*b1-1.54*a2/a1*b2+26.2;
    float MBP = 0.68*b1-1.53*a2/a1*b2+38.8;
    float SBP = 2.5*MBP-1.5*DBP;
    Serial.print("Systolic blood pressure: ");
    Serial.println(SBP);
    Serial.print("Diastolic blood pressure: ");
    Serial.println(DBP);
    Serial.print("Mean blood pressure: ");
    Serial.println(MBP);
}