#include <Wire.h>
#include <AccelStepper.h>

// Driver: FULL STEP, Motor 1.8' => 200 bước = 1 vòng 360'
#define BT   A0
#define LM   A1 
int Stepx = 2; int Dirx = 3; int enax = 4;// motor bàn trượt 
int Stepy = 5; int Diry = 6; int enay = 7;// motor cuộn
char readData;
AccelStepper mystepperX(1, Stepx, Dirx, enax); //1 là chế độ dùng Driver
AccelStepper mystepperY(1, Stepy, Diry, enay);

long SET_STEP_SERVO = 16000;
int SPEED_SERVO_X = 200;
int SPEED_SERVO_Y = 250;
int SPEED_SERVO_GO_HOME = 5000;

void setup() 
{
  Serial.begin(9600);
  mystepperX.setMaxSpeed(10000); 
  mystepperY.setMaxSpeed(1000);
  
  pinMode (BT,INPUT_PULLUP);
  pinMode (LM,INPUT_PULLUP);

  Serial.begin (9600);
}

void loop()
 {
  if(Serial.available())
  {
    readData = Serial.read();
  }

  if ((readData == '0')||(digitalRead(BT) == 0))
     { 
       goHome();
     }
  
 

  if( readData == '1')
  {
    delay(300);
    dkbantruot();
  }
  
}

void dkbantruot() {
  controlServo(SET_STEP_SERVO, SPEED_SERVO_X, SPEED_SERVO_Y);
  controlServo(-SET_STEP_SERVO, -SPEED_SERVO_X, SPEED_SERVO_Y);
}

/*
  Function to get home 
*/
void goHome(){
  mystepperX.setCurrentPosition(0); //Set vị trí hiện tại của động cơ là 0
  mystepperX.setSpeed(-SPEED_SERVO_GO_HOME); //Cho motor chạy NGƯỢC với tốc độ 700 bước/s
  mystepperX.runSpeed();

  if(digitalRead(LM) == 0) 
  {
    delay(200);
    mystepperX.setSpeed(0); 
    mystepperX.runSpeed();
    mystepperY.setSpeed(0);
    mystepperY.runSpeed();
  }  
}

void controlServo(long step, int speedMotorX, int speedMotorY) {
  mystepperY.setCurrentPosition(0);
  mystepperX.setCurrentPosition(0); 
  while(mystepperX.currentPosition() != step) {
    
    if((step < 0 && digitalRead(LM) == 0)||(digitalRead(BT) == 0)||(readData == '0'))
    {
      //mystepperX.setCurrentPosition(0); 
      break;      
    }
    mystepperX.setSpeed(speedMotorX); //Cho motor bàn trượt chạy với tốc độ 700 bước/s
    mystepperX.runSpeed();
    mystepperY.setSpeed(speedMotorY); //Cho motor chạy với tốc độ...
    mystepperY.runSpeed();
  }
}
