#include <AccelStepper.h>
#include <Stepper.h>
#include <HX711.h>
#include <PID_v2.h>

//Stepper driver definitions//
#define dirPin 5
#define stepPin 4
#define motorInterfaceType 1

//Sensor definitions//
#define endStop 3
#define linPot A2
#define hallSens A0

//Load cell definitions//
#define DOUT  7
#define CLK  6

const int sampleSize = 1000; // number of data points taken then averaged to send to stepper controller

int stepsPerRevolution = 6400;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

HX711 scale;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=5000, Ki=10, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);



void setup() {

  Serial.begin(9600);

  pinMode(linPot, INPUT);
  pinMode(hallSens, INPUT);
  pinMode(endStop, INPUT_PULLUP);

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(20000);
  stepper.setAcceleration(10000);

  while (digitalRead(endStop)) 
  {
    digitalWrite(dirPin, LOW);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(50);
  }

  stepper.setCurrentPosition(0);


  //initialize the variables we're linked to
  Setpoint = 10;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  

  scale.begin(DOUT, CLK);
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  stepper.runToNewPosition(19550);
}

void loop() 
{


  Input = Force();
  myPID.Compute();

  Serial.print("Diam:");
  Serial.print(Diameter());
  Serial.print(",");
  Serial.print("Force:");
  Serial.print(Input);
  Serial.print(",");
  Serial.print("Position:");
  Serial.print(stepper.currentPosition());
  Serial.print(",");
  Serial.print("Output:");
  Serial.println(Output);



  // while (Output >= 223) //When PID is at max Ramp 10 steps
  // {

  //   Input = Force();

  //   stepper.move(10);
  //   stepper.run();

  //   myPID.Compute();
    
  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }

  // while (Output < 223 && Output > 191) //When PID is at 88%. Ramp 5 steps
  // {

  //   Input = Force();

  //   stepper.move(5);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }
  
  // while (Output < 191 && Output > 159) //When PID is at 75%. Ramp 4 steps
  // {

  //   Input = Force();

  //   stepper.move(4);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }

  // while (Output < 159 && Output > 127) //When PID is at 63%. Ramp 3 steps
  // {

  //   Input = Force();

  //   stepper.move(3);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }

  // while (Output < 127 && Output > 95) //When PID is at 50%. 2
  // {

  //   Input = Force();

  //   stepper.move(2);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }

  // while (Output < 95 && Output > 80) //When PID is at 38%. Ramp 1 steps
  // {

  //   Input = Force();

  //   stepper.move(1);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }
  
  // while (Output < 80 && Output > 31) //When PID is at 25%. Ramp -1 steps
  // {

  //   Input = Force();

  //   stepper.move(-1);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }

  // while (Output <= 31) //When PID is at 0-13%. Ramp -2
  // {

  //   Input = Force();

  //   stepper.move(-3);
  //   stepper.run();

  //   myPID.Compute();

  //   Serial.print("Diam:");
  //   Serial.print(Diameter());
  //   Serial.print(",");
  //   Serial.print("Force:");
  //   Serial.print(Input);
  //   Serial.print(",");
  //   Serial.print("Position:");
  //   Serial.print(stepper.currentPosition());
  //   Serial.print(",");
  //   Serial.print("Output:");
  //   Serial.println(Output);


  // }


}


double Force()
{
  double forceSig = scale.get_value(2); //every call averages 2 ADC signals
  double force = (.000009 * forceSig) - .002; //force in N 

  return force;
}


float Diameter()
{
  float averageVal = 0;
  for (int i = 0; i < 1000; i++)
  {

  float hallSig = analogRead(hallSens); //collects hall effect signal
  

  averageVal += hallSig/1000;
  
  
  }

  float hallPos = (.011*averageVal) - 2.7473;
  //absHallPos = ((-6238) * avrgHallPos) + 33453;

  return hallPos;

}

