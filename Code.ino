#include <Servo.h>
#include <NewPing.h>

#define LINE_FOLLOWER_WITH_OBSTACLE_AVOIDING_MODE 1
#define OBSTACLE_AVOIDING_MODE                    2
#define ROBOT_CONTROL_MODE LINE_FOLLOWER_WITH_OBSTACLE_AVOIDING_MODE

#define SERVO_PIN 3
#define BINSERVO_PIN 11
#define ULTRASONIC_SENSOR_TRIG 13
#define ULTRASONIC_SENSOR_ECHO 12
#define BIN_SENSOR_TRIG 2
#define BIN_SENSOR_ECHO 4

#define BOTTOM_IR_SENSOR_RIGHT A0
#define BOTTOM_IR_SENSOR_LEFT A1
/*#define TOP_IR_SENSOR_RIGHT A3
#define TOP_IR_SENSOR_LEFT A2*/


//Right motor
int enableRightMotor=5;
int rightMotorPin1=7;
int rightMotorPin2=8;

//Left motor
int enableLeftMotor=6;
int leftMotorPin1=9;
int leftMotorPin2=10;

NewPing mySensor[2] = {NewPing(ULTRASONIC_SENSOR_TRIG, ULTRASONIC_SENSOR_ECHO, 400), NewPing(BIN_SENSOR_TRIG, BIN_SENSOR_ECHO, 400)};

Servo myServo;

Servo myBinServo;

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);
  myBinServo.attach(BINSERVO_PIN);
  myBinServo.write(120);

  pinMode(BOTTOM_IR_SENSOR_RIGHT, INPUT);
  pinMode(BOTTOM_IR_SENSOR_LEFT, INPUT);

/*  pinMode(TOP_IR_SENSOR_RIGHT, INPUT);
  pinMode(TOP_IR_SENSOR_LEFT, INPUT);*/
    
  rotateMotor(0,0);
}
/*void loop(){
  
  int distance = mySensor[0].ping_cm();
  int rightIRSensorValue = analogRead(BOTTOM_IR_SENSOR_RIGHT);
  int leftIRSensorValue = analogRead(BOTTOM_IR_SENSOR_LEFT);

  Serial.print(leftIRSensorValue);
  Serial.print("\t");
  Serial.print(rightIRSensorValue);
  Serial.print("\n");
  delay(5000);
  // High IR analog value means less reflected light, i.e, no object or black surface
  // Low IR analog value means high reflected light, i.e, object or reflective white surface

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue > 900){
    myServo.write(180);
    delay(500);
    myServo.write(0);
    delay(500);
    myServo.write(180);
    delay(500);
  }

  if (distance > 0 && distance < 30) {
    myServo.write(180);
    delay(500);
    myServo.write(0);
    delay(500);
    myServo.write(180);
    delay(500);
  }
}*/

/*void obstacleAvoidingControl()
{
  static int MAX_REGULAR_MOTOR_SPEED  = 150;
  static int MAX_MOTOR_TURN_SPEED  = 200;
  static int DISTANCE_TO_CHECK  = 30;
  
  int distance = mySensor[0].ping_cm();

  //If distance is within 30 cm then adjust motor direction as below
  if (distance > 0 && distance < DISTANCE_TO_CHECK)
  {
    //Stop motors
    rotateMotor(0, 0);
    delay(500);  
       
    //Reverse motors
    rotateMotor(-MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);        
    delay(200);
    
    //Stop motors
    rotateMotor(0, 0);
    delay(500);
    
    //Rotate servo to left    
    myServo.write(180);
    delay(500);

    //Read left side distance using ultrasonic sensor
    int distanceLeft = mySensor[0].ping_cm();    

    //Rotate servo to right
    myServo.write(0);    
    delay(500);    

    //Read right side distance using ultrasonic sensor   
    int distanceRight = mySensor[0].ping_cm();

    //Bring servo to center
    myServo.write(90); 
    delay(500);        
    
    if (distanceLeft == 0 )
    {
      rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
      delay(500);
    }
    else if (distanceRight == 0 )
    {
      rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);
      delay(500);
    }
    else if (distanceLeft >= distanceRight)
    {
      rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
      delay(500);
    }
    else
    {
      rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);
      delay(500);      
    }
    rotateMotor(0, 0);    
    delay(200);
  }
  else
  {
    rotateMotor(MAX_REGULAR_MOTOR_SPEED, MAX_REGULAR_MOTOR_SPEED);
  }  
}*/

void lineFollowerWithObstacleAvoidingControl()
{
  static int MOTOR_SPEED = 255;
  static int TURNING_MOTOR_SPEED = 255;
  static int DISTANCE_TO_CHECK  = 20;
    
  int rightIRSensorValue = digitalRead(BOTTOM_IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(BOTTOM_IR_SENSOR_LEFT);  Serial.print("Left sensor: ");
  Serial.print(leftIRSensorValue?"Black detected":"White");
  Serial.print("\t Right Sensor: ");
  Serial.print(rightIRSensorValue?"Black detected":"White");
  Serial.print("\t UV sensor: ");

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    int distance = mySensor[0].ping_cm();
  Serial.print(distance);
  Serial.print("cm");
    //If object detected that is if distance is within 30 cm then adjust motor direction as below
    if (distance > 0 && distance < DISTANCE_TO_CHECK)
    {
      adjustRobotToAvoidObjectWhileFollowingLine();
    }
    else
    {
      rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
    }
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
      rotateMotor(-TURNING_MOTOR_SPEED, TURNING_MOTOR_SPEED); 
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  {
      rotateMotor(TURNING_MOTOR_SPEED, -TURNING_MOTOR_SPEED); 
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    rotateMotor(0, 0);
  }  
  Serial.print("\n");
}


void adjustRobotToAvoidObjectWhileFollowingLine()
{
  static int MAX_MOTOR_TURN_SPEED  = 255;

  //Stop motors
  rotateMotor(0, 0);
  delay(500);
    
  //Rotate servo to left    
  myServo.write(150);
  delay(500);

  //Read left side distance using ultrasonic sensor
  int distanceLeft = mySensor[0].ping_cm();    

  //Rotate servo to right
  myServo.write(10);    
  delay(500);    

  //Read right side distance using ultrasonic sensor   
  int distanceRight = mySensor[0].ping_cm();

  //Bring servo to center
  myServo.write(80);
  delay(500);        
  Serial.print("\n");
  Serial.print(distanceLeft);
  Serial.print(" (left dist)\t");
  Serial.print(distanceRight);
  Serial.print(" (right dist)\n");
//  delay(1000);

  bool robotMovedToLeftDirection = false;
  if (distanceLeft == 0 )
  {
    robotMovedToLeftDirection = true;
    rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
    delay(500);
  }
  else if (distanceRight == 0 )
  {
    rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);
    delay(500);
  }
  else if (distanceLeft >= distanceRight)
  {
    robotMovedToLeftDirection = true;
    rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
    delay(500);
  }
  else
  {
    rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);
    delay(500);      
  }
  
  rotateMotor(MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);    
  delay(500); 

  if (robotMovedToLeftDirection)
  {
    rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);
    delay(800);  
  }
  else
  {
    rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
    delay(800);  
  }
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  Serial.print("\n Left motor: ");
  Serial.print(leftMotorSpeed);
  Serial.print("\t Right Motor: ");
  Serial.print(rightMotorSpeed);
  if ((rightMotorSpeed<0)&&(leftMotorSpeed>0)){
    Serial.print("\t\t right turn");
  } else if ((rightMotorSpeed>0)&&(leftMotorSpeed<0)){
    Serial.print("\t\t left turn");
  } else if ((rightMotorSpeed>0)&&(leftMotorSpeed>0)){
    Serial.print("\t\t straight");
  } else if ((rightMotorSpeed==0)&&(leftMotorSpeed==0)){
    Serial.print("\t\t stop");
  }
  Serial.print("\n");
  rightMotorSpeed=-rightMotorSpeed;
  leftMotorSpeed=-leftMotorSpeed;
  delay(500);
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed >= 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed >= 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}

void bin() {
  int dist = mySensor[1].ping_cm();
    Serial.print("\n UV Sensor: ");
    Serial.print(dist);
  //If distance is within 30 cm then adjust motor direction as below
  if (dist > 0 && dist < 20)
  {
    myBinServo.write(10);
    Serial.print("\t open \n");

  }
  else {
    myBinServo.write(120);
    Serial.print("\t close \n");
  }
}

void loop()
{
  switch (ROBOT_CONTROL_MODE)
  {
    /*case OBSTACLE_AVOIDING_MODE:
      obstacleAvoidingControl();
      break;
*/
    case LINE_FOLLOWER_WITH_OBSTACLE_AVOIDING_MODE:
      lineFollowerWithObstacleAvoidingControl();
      break;

    default:
      break;  
  }
  bin();
//  delay(100);
}
