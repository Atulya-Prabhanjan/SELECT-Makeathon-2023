#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
int fsrPin = 0;     
int fsrReading;     
int fsrVoltage; 
const int PulseWire = 1;
const int LED13 = 13; 
int Threshold = 550;  
unsigned long fsrResistance;  
unsigned long fsrConductance; 
long fsrForce;       
double a;
float t1,t;
PulseSensorPlayground pulseSensor;
float x[7];
float y[7];
void setup(void) {
  Serial.begin(9600);
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.  
}
 
void loop(void) {
  
  fsrReading = analogRead(fsrPin);  
 
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
    fsrResistance = 5000 - fsrVoltage; 
    fsrResistance *= 10000;             
    fsrResistance /= fsrVoltage;
 
    fsrConductance = 1000000;           
    fsrConductance /= fsrResistance;

      fsrForce = fsrConductance / 80;
      a = fsrForce*0.00750062/0.0001266769;
      Serial.print("PRESSURE : ");
      Serial.println(a);    
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
    int b = pulseSensor.getBeatsPerMinute();
    for(int i=0;i<8;i++){
        y[i] = b;
        }
     for(int i=0;i<8;i++){
        for(int j=i+1;j<8;j++){
            if(y[i]>y[j]){
                t=y[i];
                x[i]=y[j];
                y[j]=t;
            }
        }
    } 

    float a1 = y[7];
    float a2 = (y[1]+y[2])/2;
    float b1=(x[3]+x[4])/2;
    float b2 =(x[1]+x[2])/2; 
    float b3 = b2; 
    float DBP = 0.65*b1-1.54*a2/a1*b2+26.2;
    float MBP = 0.68*b1-1.53*a2/a1*b2+38.8;
    float SBP = 2.5*MBP-1.5*DBP;
    Serial.print("Systolic pressure: ");
    Serial.println(SBP);
    Serial.print("Diastolic pressure: ");
    Serial.println(DBP);
    Serial.print("Mean pressure: ");
    Serial.println(MBP);
}
 