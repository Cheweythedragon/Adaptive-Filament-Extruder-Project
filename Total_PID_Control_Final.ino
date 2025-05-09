#include <AccelStepper.h>
#include <Stepper.h>
#include <HX711.h>
#include <PID_v2.h>
#include "Wire.h" // This library allows you to communicate with I2C devices.

//Stepper driver definitions//
#define dirPin 4
#define stepPin 5
#define motorInterfaceType 1

//Sensor definitions//
#define endStop 3
#define linPot01 A2
#define linPot02 A3
#define hallSens A0

//Load cell definitions//
#define DOUT  7
#define CLK  6

const int sampleSize = 100; // number of data points taken then averaged to send to stepper controller

int stepsPerRevolution = 6400;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

//Begin scale module
HX711 scale;

//Define Variables for Position loop
double PSet, PInput, POutput;

//Define Variables for Force loop
double FSet, FInput, FOutput;

//Specify the tuning parameters for Position loop
double Kpp=100, Kip=1, Kdp=0;
PID PosPID(&PInput, &POutput, &PSet, Kpp, Kip, Kdp, DIRECT);

//Specify the tuning parameters for Force loop
double Kpf=5000, Kif=10, Kdf=0;
PID ForcePID(&FInput, &FOutput, &FSet, Kpf, Kif, Kdf, DIRECT);


void setup() 
{
  Serial.begin(9600);

  pinMode(linPot01, INPUT);
  pinMode(linPot02, INPUT);
  pinMode(hallSens, INPUT);
  pinMode(endStop, INPUT_PULLUP);

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(100000);
  stepper.setAcceleration(100000);

  while (digitalRead(endStop)) 
  {
    digitalWrite(dirPin, LOW);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(20);
  }

  stepper.setCurrentPosition(0);

  scale.begin(DOUT, CLK);
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  //initialize the setpoints
  PSet = 0;
  FSet = 10;

  //turn the PID on
  PosPID.SetMode(AUTOMATIC);
  ForcePID.SetMode(AUTOMATIC);

  stepper.runToNewPosition(91000);

  Serial.println(Force());
  Serial.println("Setup Complete");
}

void loop() 
{

  stepper.runToNewPosition(stepper.currentPosition() + random(1, 20));

  PInput = PosDif();
  PosPID.Compute();
  //Serial.println("pos pid done");

  FInput = Force();
  ForcePID.Compute();
  //Serial.println("force pid done");

  //int speedMap = map(POutput, 0, 255, 20000, 10000);

  Serial.print("Diameter:");
  Serial.print(Diameter());
  Serial.print(",");
  Serial.print("Force:");
  Serial.print(Force());
  Serial.print(",");
  Serial.print("Position:");
  Serial.print(stepper.currentPosition());
  Serial.print(",");
  Serial.print("Force PID:");
  Serial.println(FOutput);

  stepper.runToNewPosition(stepper.currentPosition() - random(1, 20));


  //Movement loops based on force difference 
  // while (FOutput == 0) //When PID low, step backwards myVariable > lowerBound && myVariable < upperBound
  // {
  //   stepper.runToNewPosition(stepper.currentPosition() - 5); //04-15 Changed to -5 steps

  //   PInput = PosDif();
  //   PosPID.Compute();

  //   FInput = Force();
  //   ForcePID.Compute();
    

  //   int speedMap = map(POutput, 0, 255, 20000, 10000);

  //   Serial.print("Diameter:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Force());
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Force PID:");
  //   Serial.println(FOutput);

  // }

  // while (FOutput > 0 && FOutput < 63) 
  // {

  //   stepper.runToNewPosition(stepper.currentPosition() - 2);



  //   PInput = PosDif();
  //   PosPID.Compute();

  //   FInput = Force();
  //   ForcePID.Compute();
    

  //   int speedMap = map(POutput, 0, 255, 20000, 10000);

  //   Serial.print("Diameter:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Force());
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Force PID:");
  //   Serial.println(FOutput);

  // }

  // while (FOutput > 63 && FOutput < 127) 
  // {

  //   stepper.runToNewPosition(stepper.currentPosition() + 1);



  //   PInput = PosDif();
  //   PosPID.Compute();

  //   FInput = Force();
  //   ForcePID.Compute();
    

  //   int speedMap = map(POutput, 0, 255, 20000, 10000);

  //   Serial.print("Diameter:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Force());
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Force PID:");
  //   Serial.println(FOutput);

  // }

  // while (FOutput > 127 && FOutput < 191) 
  // {

  //   stepper.runToNewPosition(stepper.currentPosition() + 2);



  //   PInput = PosDif();
  //   PosPID.Compute();

  //   FInput = Force();
  //   ForcePID.Compute();
    

  //   int speedMap = map(POutput, 0, 255, 20000, 10000);

  //   Serial.print("Diameter:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Force());
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Force PID:");
  //   Serial.println(FOutput);

  // }

  // while (FOutput > 191 && FOutput < 255) 
  // {

  //   stepper.runToNewPosition(stepper.currentPosition() + 3);//04-15 Changed to +3 steps



  //   PInput = PosDif();
  //   PosPID.Compute();

  //   FInput = Force();
  //   ForcePID.Compute();
    

  //   int speedMap = map(POutput, 0, 255, 20000, 10000);

  //   Serial.print("Diameter:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Force());
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Force PID:");
  //   Serial.println(FOutput);

  // }

  // while (FOutput == 255) //When PID HIGH, step forward
  // {
  //   stepper.runToNewPosition(stepper.currentPosition() + 5);//04-15 Changed to +5 steps

  //   PInput = PosDif();
  //   PosPID.Compute();

  //   FInput = Force();
  //   ForcePID.Compute();
    

  //   int speedMap = map(POutput, 0, 255, 20000, 10000);

  //   Serial.print("Diameter:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Force());
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Force PID:");
  //   Serial.println(FOutput);

  // }

}




double Force()
{
  
  double forceSig = scale.get_value(2); //every call averages 2 ADC signals
  //double force = (7.4852 * log(-forceSig)) - 76.179; //force for strain gaugues
  double force = (.000009 * forceSig) - .002; //force for load cell
  return force; //In Newtons
}

float Diameter()
{
  float averageVal = 0;
  for (int i = 0; i < sampleSize; i++)
  {
  float hallSig = analogRead(hallSens); //collects hall effect signal
  averageVal += hallSig/sampleSize;
  }
  float hallPos = (.0134 * averageVal) - 4.3017;
  return hallPos;
}

float linVal()
{

  float averagePot01 = 0;
  float averagePot02 = 0;
  for (int i = 0; i < sampleSize; i++)
  {
  float potSig01 = analogRead(linPot01); //collects linear pot signal
  float potSig02 = analogRead(linPot02); //collects linear pot signal
  averagePot01 += potSig01/sampleSize;
  averagePot02 += potSig02/sampleSize;
  }

  float averagePotVal = (averagePot01 + averagePot02)/2;
  //int potTrunc = (averagePotVal * 1000);
  return averagePotVal;
}

float absPos()
{
  float potPos = ((-635.76*(linVal())) + 98491);
  return potPos;
}

float relPos()
{
  float absHallPos = ((-6540) * Diameter()) + 93600;
  return absHallPos;
}

float PosDif()
{
  float posDif = relPos() - absPos();
  return posDif;
}

