#include <Wire.h>
#include <AccelStepper.h>

// Driver: FULL STEP, Motor 1.8' => 200 bước = 1 vòng 360'
#define BT   A0
#define LM   A1 
int Stepx = 2; int Dirx = 3; int enax = 4;// motor bàn trượt 
int Stepy = 5; int Diry = 6; int enay = 7;// motor cuộn
int dem=0;
int chedo = 0,read_BT,nd=0,gh,v;
byte data[3];
AccelStepper mystepperX(1, Stepx, Dirx, enax); //1 là chế độ dùng Driver
AccelStepper mystepperY(1, Stepy, Diry, enay);

long SET_STEP_SERVO = 44800;
int SPEED_SERVO_X = 5000;
int SPEED_SERVO_Y = 300;
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
  goHome();

  if(digitalRead(BT)==0)
  {
    delay(300);
    dkbantruot();
  }
  
}
void dkbantruot()
{
  controlServo(SET_STEP_SERVO, SPEED_SERVO_X, SPEED_SERVO_Y);

  dem = dem + ((mystepperY.currentPosition())/3200 );
  Serial.print("Dem: "); Serial.println(dem);
  //delay(500); //dừng chờ 1s
  //Dao chieu

  controlServo(-SET_STEP_SERVO, -SPEED_SERVO_X, SPEED_SERVO_Y);

  dem = dem + ((mystepperY.currentPosition())/3200 );
  Serial.print("Dem: "); Serial.println(dem);

 // delay(500);   
}
void nhan()
{
  if(Serial.available()>2)
  {
    data[0] = Serial.read();
    if(data[0]=='A')
    {
      data[1] = Serial.read()-48;
      data[2] = Serial.read()-48;
      gh = (data[1]*10)+data[2];
      Serial.println(gh);
    }
     if(data[0]=='B')
    {
      data[1] = Serial.read()-48;
      data[2] = Serial.read()-48;
      v = (data[1]*10)+data[2];
      Serial.println(v);
    }
  }
}


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
  while(mystepperX.currentPosition() != step) 
  {
    if(digitalRead(BT) == 1) break;
    if(step < 0 && digitalRead(LM) == 0){
      mystepperX.setCurrentPosition(0); 
      break;      
    }
    mystepperX.setSpeed(speedMotorX); //Cho motor bàn trượt chạy với tốc độ 700 bước/s
    mystepperX.runSpeed();
    mystepperY.setSpeed(speedMotorY); //Cho motor chạy với tốc độ...
    mystepperY.runSpeed();
  }
}
