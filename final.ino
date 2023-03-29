#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
const int PulseWire = 0;      
int Threshold = 550; 
PulseSensorPlayground pulseSensor;

int fsrpin = 0;     
int fsrReading;     
int fsrVoltage;     
unsigned long fsrResistance;  
unsigned long fsrConductance; 
long fsrForce;       
double pressure; 
double pres[50];
int bpm1[50];
int arrayindex=0;
int n=50;

void setup(void) {
  Serial.begin(9600); 
  pulseSensor.analogInput(PulseWire);      
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

}
double pressure1(){
  fsrReading=analogRead(fsrpin);
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  fsrResistance = 5000 - fsrVoltage; 
    fsrResistance *= 10000;             
    fsrResistance /= fsrVoltage;
    fsrConductance = 1000000;           
    fsrConductance /= fsrResistance;
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      pressure = fsrForce*0.00750062/0.0001266769;
      Serial.print("PRESSURE : ");
      Serial.println(pressure);    
      return pressure;
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      pressure = fsrForce*0.00750062/0.0001266769;
      Serial.print("PRESSURE : "); 
      Serial.println(pressure);
      return pressure;         
    }
    delay(20);
}
int bpm(){
  if (pulseSensor.sawStartOfBeat()) {         
    int myBPM = pulseSensor.getBeatsPerMinute();  
    Serial.print("BPM: ");                        
    Serial.println(myBPM); 
    return myBPM;                       
    }
 delay(20);
}
void loop(void)
{
  while(arrayindex<=50)
  { 
   double p=pressure1();
   if(p>0)
   {
   pres[arrayindex]=p;
   bpm1[arrayindex]=bpm();
   arrayindex++;
   }
  }
  int len=arrayindex;
	float Xr[len];
	float Xi[len];
	float amp[len];
	int k, n, N = 50;
	for (k = 0; k < N; k++) {
		Xr[k] = 0;
		Xi[k] = 0;
		for (n = 0; n < len; n++) {
			Xr[k] = (Xr[k] + bpm1[n] * cos(2 * 3.141592 * k * n / N));
			Xi[k] = (Xi[k] - bpm1[n] * sin(2 * 3.141592 * k * n / N));
		}
		amp[k] = sqrt(Xr[k]*Xr[k]+Xi[k]*Xi[k]);
	}
  n=50;
  double points[n][2];
  for(int i=0;i<n;i++)
  {
    points[i][0]=pres[i];
    points[i][1]=amp[i];
  }
  for(int i=0;i<n;i++){
    Serial.println(points[i][0]);
  }
    int flag=0,q=0;
    int lowtresh[5];
    int hightresh[5];
    int mi=0;
    int ma=0;
    int maf;
    int mab=0;
    int mini[n];
    int maxi[n];
    float A1[n];
    float A2[n];
    double B1[n];
    float B2[n];
    float B3[n];
    float m2;
    float m3;
    float s2;
    float s3;
    float p;
    int l;
    float y;
    float x;
    int inc=0;
    int threshold = 5;
    for(int i=0;i<n;i++){
        flag = 0;
        while(flag==0 && i<n){
            if(i<threshold){
                for(int j=0;j<threshold;j++){
                    hightresh[j]= points[j+i][1];
                }
                for(int j=0;j<i+1;j++){
                    lowtresh[j]= points[i-j][1];
                }
                flag=1;
                for(int j=0;j<threshold;j++){
                    if(hightresh[j]<points[i][1])
                        flag=0;
                }
                for(int j=0;j<i+1;j++){
                    if(lowtresh[j]<points[i][1])
                        flag=0;
                }
            }
            else if(i>n-threshold){
                for(int j=0;j<threshold;j++){
                    lowtresh[j]= points[i-j][1];
                }
                for(int j=0;j<n-i;j++){
                    hightresh[j]= points[i+j][1];
                }
                flag=1;
                for(int j=0;j<threshold;j++){
                    if(lowtresh[j]<points[i][1])
                        flag=0;
                }
                for(int j=0;j<n-i;j++){
                    if(hightresh[j]<points[i][1])
                        flag=0;
                }
            }
            else{
                for(int j=0;j<threshold;j++){
                    lowtresh[j]= points[i-j][1];
                    hightresh[j]= points[j+i][1];
                }
                flag=1;
                for(int j=0;j<threshold;j++){
                    if(lowtresh[j]<points[i][1]||hightresh[j]<points[i][1])
                        flag=0;
                }
            }
            if(flag==0)
                i++;
        }
        if(flag==1){
            mini[mi]=i;
            mi++;
        }
    }
    for(int i=0;i<n;i++){
        flag = 0;
        while(flag==0 && i<n){
            if(i<threshold){
                for(int j=0;j<threshold;j++){
                    hightresh[j]= points[j+i][1];
                }
                for(int j=0;j<i+1;j++){
                    lowtresh[j]= points[i-j][1];
                }
                flag=1;
                for(int j=0;j<threshold;j++){
                    if(hightresh[j]>points[i][1])
                        flag=0;
                }
                for(int j=0;j<i+1;j++){
                    if(lowtresh[j]>points[i][1])
                        flag=0;
                }
            }
            else if(i>n-threshold){
                for(int j=0;j<threshold;j++){
                    lowtresh[j]= points[i-j][1];
                }
                for(int j=0;j<n-i;j++){
                    hightresh[j]= points[i+j][1];
                }
                flag=1;
                for(int j=0;j<threshold;j++){
                    if(lowtresh[j]>points[i][1])
                        flag=0;
                }
                for(int j=0;j<n-i;j++){
                    if(hightresh[j]>points[i][1])
                        flag=0;
                }
            }
            else{
                for(int j=0;j<threshold;j++){
                    lowtresh[j]= points[i-j][1];
                    hightresh[j]= points[j+i][1];
                }
                flag=1;
                for(int j=0;j<threshold;j++){
                    if(lowtresh[j]>points[i][1]||hightresh[j]>points[i][1])
                        flag=0;
                }
            }
            if(flag==0)
                i++;
        }
        if(flag==1){
            maxi[ma]=i;
            ma++;
        }
    }
    maf=ma;
    if(maxi[0]<mini[0]){
        mab=1;
    }
    if(maxi[ma-1]>mini[mi-1]){
        maf=ma-1;
    }
    int vma, vmi1, vmi2;
    for(int i=0;i<maf-mab;i++){
        vma = maxi[i+mab];
        B1[i] = points[vma][0];
        A1[i] = points[vma][1];
        vmi1 = mini[i];
        vmi2 = mini[i+1];
        s2 = points[vmi1][1];
        s3 = points[vmi2][1];
        m2 = (A1[i]+s2)/2;
        m3 = (A1[i]+s3)/2;
        l=0;
        for(int j=vmi1;j<vma;j++){
            p = points[j][1];
            if(abs(m2-p)<abs(m2-points[l][1]))
                l=j;
        }
        B2[i] = points[l][0];
        l=0;
        for(int j=vma;j<vmi2;j++){
            p = points[j][1];
            if(abs(m3-p)<abs(m3-points[l][1]))
                l=j;
        }
        B3[i] = points[l][0];
    }
    double DBP = (0.65*B1)-(1.54*(A2/A1)*B2)+26.2;
    double MBP = (0.68*B1)-(1.53*(A2/A1)*B2)+38.8;
    double SBP = (2.5*MBP)-(1.5*DBP);
    Serial.print("Systolic pressure: ");
    Serial.println(SBP);
    Serial.print("Diastolic pressure: ");
    Serial.println(DBP);
    Serial.print("Mean pressure: ");
    Serial.println(MBP);
}

