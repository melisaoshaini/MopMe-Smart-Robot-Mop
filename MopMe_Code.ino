#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <Wire.h>

const int echoLeft = 2;
const int trigLeft = 3;
const int echoMiddle = 4;
const int trigMiddle = 5;
const int echoRight = 7;
const int trigRight = 8;
const int leftMotor1 = 6;
const int leftMotor2 = 9;
const int rightMotor1 = 10;
const int rightMotor2 = 11;
const int buttonPin = 12;
const int pumpPin = 13;
int motorSpeed = 250;
int maxDistance = 200;
int distanceLeft = 0;
int distanceMiddle = 0;
int distanceRight = 0;
char incomingByte;

NewPing sonarLeft(trigLeft, echoLeft, maxDistance);
NewPing sonarMiddle(trigMiddle, echoMiddle, maxDistance);
NewPing sonarRight(trigRight, echoRight, maxDistance);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{  
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
  digitalWrite(pumpPin, LOW);
  lcd.init();
  lcd.backlight();  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  Serial.begin(9600);
  delay(2000);
}

void loop()
{
  if (digitalRead(buttonPin) == LOW)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Manual Mode");
    while (true)
    {
      manualMode();
      if (digitalRead(buttonPin) == HIGH)
      {
        moveStop();
        break;
      }
    }
    delay(100);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Automatic Mode");
    while (true)
    {
      automaticMode();
      if (digitalRead(buttonPin) == LOW)
      {
        moveStop();
        break;
      }
    }
    delay(100);
  }
}

void manualMode()
{
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
  }

  switch (incomingByte)
  {
    case 'F':
      moveForward();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Forward");
      incomingByte = '*';
      break;
    case 'B':
      moveBackward();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Backward");
      incomingByte = '*';
      break;
    case 'L':
      moveLeft();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Left");
      incomingByte = '*';
      break;
    case 'R':
      moveRight();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Right");
      incomingByte = '*';
      break;
    case 'S':
      moveStop();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stop");
      incomingByte = '*';
      break;
    // ... (other cases)
  }
}

void automaticMode()
{
  distanceLeft = readSensorLeft();
  distanceMiddle = readSensorMiddle();
  distanceRight = readSensorRight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("L=");
  lcd.print(distanceLeft);
  lcd.print("cm ");
  lcd.print("M=");
  lcd.print(distanceMiddle);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("R=");
  lcd.print(distanceRight);
  lcd.print("cm");

  // ... (rest of the automaticMode function)
}

int readSensorLeft()
{
  delay(70);
  int cmLeft = sonarLeft.ping_cm();
  if (cmLeft == 0)
  {
    cmLeft = 250;
  }
  return cmLeft;
}

int readSensorMiddle()
{
  delay(70);
  int cmMiddle = sonarMiddle.ping_cm();
  if (cmMiddle == 0)
  {
    cmMiddle = 250;
  }
  return cmMiddle;
}

int readSensorRight()
{
  delay(70);
  int cmRight = sonarRight.ping_cm();
  if (cmRight == 0)
  {
    cmRight = 250;
  }
  return cmRight;
}

void moveForward()
{
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
}

void moveBackward()
{
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
}

void moveLeft()
{
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
}

void moveRight()
{
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
}

void moveStop()
{
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
}
