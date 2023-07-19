 //************************************************************************
// Copyright Valetude Primus Healthcare Pvt. Ltd. - All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
//************************************************************************
/*
   This file contains functions to read user input and run motors
   in the specified axis for the specified number of steps.
*/

// x-axis pinout
const int xmotorPin1 = 13;
const int xmotorPin2 = 12;
const int xmotorPin3 = 11;
const int xmotorPin4 = 10;

// y-axis pinout
const int ymotorPin1 = 9;
const int ymotorPin2 = 8;
const int ymotorPin3 = 7;
const int ymotorPin4 = 6;

// z-fine pinout
//const int zmotorPin1 = 5;
//const int zmotorPin2 = 4;
//const int zmotorPin3 = 3;
//const int zmotorPin4 = 2;

// z-fine pinout
const int dirPin1 = 5;//4;
const int stepPin1 = 4;// 5;
//

const int stepsPerRevolution = 400;

// End_Stops
int endstop_x = A4;
int endstop_y = 3;
int endstop_z = A5;

// z-coarse pinout
const int dirPin = A3;
const int stepPin = A2;

//const int stepsPerRevolution = 400;

// variables
const String finalOutput = "Done";  // for python to catch
String content = "";                // command given by user
int moveCount = 0;                  // step count given by user
int stepCount = 8;                  // half-step count
const String final1 = "#";

int forwardSequence[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

int backwardSequence[8][4] = {
  {1, 0, 0, 1},
  {0, 0, 0, 1},
  {0, 0, 1, 1},
  {0, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 0},
  {1, 1, 0, 0},
  {1, 0, 0, 0}
};

// initialize pinout
void setup()
{
  Serial.begin(9600);

  Serial.println("XYZ testing code");
  pinMode(endstop_x, INPUT);
  pinMode(endstop_y, INPUT);
  pinMode(endstop_z, INPUT);

  pinMode(xmotorPin1, OUTPUT);
  pinMode(xmotorPin2, OUTPUT);
  pinMode(xmotorPin3, OUTPUT);
  pinMode(xmotorPin4, OUTPUT);
  pinMode(ymotorPin1, OUTPUT);
  pinMode(ymotorPin2, OUTPUT);
  pinMode(ymotorPin3, OUTPUT);
  pinMode(ymotorPin4, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  //digitalWrite(dirPin, HIGH);
  //digitalWrite(dirPin1, HIGH);

}

void loop()
{
  // read user input
  if (Serial.available())
  {
    check();
  }

  // run corresponding motor
  if (content != "Done")
  {
    if (content == "xclk")
    {
      for (int i = 0; i < moveCount; i++)
      {
        xreverse(); //xforward
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "xcclk")
    {
      for (int i = 0; i < moveCount; i++)
      {
        int esx = digitalRead(endstop_x);
        Serial.println(esx);
        if (esx == 1)
        {
          break;
        }
        xforward(); //xreverse
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "fxclk")
    {
      for (int i = 0; i < moveCount; i++)
      {
        fxforward();
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "fxcclk")
    {
      for (int i = 0; i < moveCount; i++)
      {
        fxreverse();
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "yclk")
    {
      for (int j = 0; j < moveCount; j++)
      {
        int esy = digitalRead(endstop_y);
        if (esy == 1)
        {
          break;
        }
        yreverse();// yforward
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "ycclk")
    {
      for (int j = 0; j < moveCount; j++)
      {
        yforward(); // yreverse
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "fyclk")
    {
      for (int j = 0; j < moveCount; j++)
      {
        fyforward();
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "fycclk")
    {
      for (int j = 0; j < moveCount; j++)
      {
        fyreverse();
      }
      Serial.println(finalOutput);
      check();
    }
    else if (content == "zclk")
    {
      zforward();
      Serial.println(finalOutput);
      check();
    }
    else if (content == "zcclk")
    {
      zreverse();
      Serial.println(finalOutput);
      check();
    }
    else if (content == "szclk")
    {
      zforward1();
      Serial.println(finalOutput);
      check();
    }
    else if (content == "szcclk")
    {
      zreverse1();
      Serial.println(finalOutput);
      check();
    }
    else if (content == "nclk")
    {
      nforward();
      Serial.println(finalOutput);
      check();
      pinMode(stepPin, INPUT);
      pinMode(dirPin, INPUT);
    }
    else if (content == "ncclk")
    {
      nreverse();
      Serial.println(finalOutput);
      check();
      pinMode(stepPin, INPUT);
      pinMode(dirPin, INPUT);
    }
    else if (content.equalsIgnoreCase("esy"))
    {
      int esy = digitalRead(endstop_y);
      Serial.println(esy);
      Serial.print("#");
      check();
    }
    else if (content.equalsIgnoreCase("esx"))
    {
      int esx = digitalRead(endstop_x);
      Serial.println(esx);
      Serial.print("#");
      check();
    }
    else if (content.equalsIgnoreCase("esz"))
    {
      int esz = digitalRead(endstop_z);
      Serial.println(esz);
      Serial.print("#");
      check();
    }
    else if (content == "init")
    {

      for (int n = 0; n < 450; n++)
      {
        int esz = digitalRead(endstop_z);
        if (esz == 1)
        {
          Serial.println(esz);
          break;
        }
        pinMode(stepPin1, OUTPUT);
        pinMode(dirPin1, OUTPUT);

        digitalWrite(dirPin1, LOW);
        digitalWrite(stepPin1, HIGH);
        delayMicroseconds(900);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(900);
        delay(10);
      }

      for (int j = 0; j < 200; j++)
      {
        int esy = digitalRead(endstop_y);
        if (esy == 1)
        {
          break;
        }
        yforward();
      }
      for (int i = 0; i < 350; i++)
      {
        int esx = digitalRead(endstop_x);
        Serial.println(esx);
        if (esx == 1)
        {
          break;
        }
        xreverse();
      }


      Serial.println(finalOutput);
      check();
    }
    else if (content == "zfi")
    {
      Serial.println(finalOutput);
      for (int n = 0; n < 10; n++)
      {
        for (int q = 0; q < 50; q++)
        {
          digitalWrite(dirPin1, LOW);
          digitalWrite(stepPin1, HIGH);
          delayMicroseconds(900);
          digitalWrite(stepPin1, LOW);
          delayMicroseconds(900);
          delay(1);
          //          Serial.println(q);
        }
        delay(250);
        Serial.println(final1);
      }
      Serial.println(finalOutput);
      check();
    }

  }
}

// interpret user input
void check() {
  String data = Serial.readString();
  int found = 0;
  int index = 1;
  char separator = ',';
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  data.toLowerCase();

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  if (found > index) {
    content = data.substring(0, strIndex[0] - 1);
    moveCount = data.substring(strIndex[0], strIndex[1]).toInt();
  }
  else {
    content = data;
    moveCount = 0;
  }
};

// send signals to corresponding motors
void xforward()
{
  //Serial.println("xforward");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(xmotorPin1, forwardSequence[i][0]);
//    delay(10);
    digitalWrite(xmotorPin2, forwardSequence[i][1]);
//    delay(10);
    digitalWrite(xmotorPin3, forwardSequence[i][2]);
//    delay(10);
    digitalWrite(xmotorPin4, forwardSequence[i][3]);
    delay(1);//4
  }
  digitalWrite(xmotorPin1, 0);
  digitalWrite(xmotorPin2, 0);
  digitalWrite(xmotorPin3, 0);
  digitalWrite(xmotorPin4, 0);
};

void xreverse()
{
  //Serial.println("xreverse");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(xmotorPin1, backwardSequence[i][0]);
//    delay(10);
    digitalWrite(xmotorPin2, backwardSequence[i][1]);
//    delay(10);
    digitalWrite(xmotorPin3, backwardSequence[i][2]);
//    delay(10);
    digitalWrite(xmotorPin4, backwardSequence[i][3]);
    delay(1);//4
  }
  digitalWrite(xmotorPin1, 0);
  digitalWrite(xmotorPin2, 0);
  digitalWrite(xmotorPin3, 0);
  digitalWrite(xmotorPin4, 0);
};

void yforward()
{
  //Serial.println("yforward");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(ymotorPin1, forwardSequence[i][0]);
    digitalWrite(ymotorPin2, forwardSequence[i][1]);
    digitalWrite(ymotorPin3, forwardSequence[i][2]);
    digitalWrite(ymotorPin4, forwardSequence[i][3]);
    delay(4);
  }
  digitalWrite(ymotorPin1, 0);
  digitalWrite(ymotorPin2, 0);
  digitalWrite(ymotorPin3, 0);
  digitalWrite(ymotorPin4, 0);
};

void yreverse()
{
  //Serial.println("yreverse");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(ymotorPin1, backwardSequence[i][0]);
    digitalWrite(ymotorPin2, backwardSequence[i][1]);
    digitalWrite(ymotorPin3, backwardSequence[i][2]);
    digitalWrite(ymotorPin4, backwardSequence[i][3]);
    delay(4);
  }
  digitalWrite(ymotorPin1, 0);
  digitalWrite(ymotorPin2, 0);
  digitalWrite(ymotorPin3, 0);
  digitalWrite(ymotorPin4, 0);
};

void fxforward()
{
  //Serial.println("xforward");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(xmotorPin1, forwardSequence[i][0]);
    digitalWrite(xmotorPin2, forwardSequence[i][1]);
    digitalWrite(xmotorPin3, forwardSequence[i][2]);
    digitalWrite(xmotorPin4, forwardSequence[i][3]);
    delay(1);
  }
  digitalWrite(xmotorPin1, 0);
  digitalWrite(xmotorPin2, 0);
  digitalWrite(xmotorPin3, 0);
  digitalWrite(xmotorPin4, 0);
};

void fxreverse()
{
  //Serial.println("xreverse");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(xmotorPin1, backwardSequence[i][0]);
    digitalWrite(xmotorPin2, backwardSequence[i][1]);
    digitalWrite(xmotorPin3, backwardSequence[i][2]);
    digitalWrite(xmotorPin4, backwardSequence[i][3]);
    delay(1);
  }
  digitalWrite(xmotorPin1, 0);
  digitalWrite(xmotorPin2, 0);
  digitalWrite(xmotorPin3, 0);
  digitalWrite(xmotorPin4, 0);
};

void fyforward()
{
  //Serial.println("yforward");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(ymotorPin1, forwardSequence[i][0]);
    digitalWrite(ymotorPin2, forwardSequence[i][1]);
    digitalWrite(ymotorPin3, forwardSequence[i][2]);
    digitalWrite(ymotorPin4, forwardSequence[i][3]);
    delay(1);
  }
  digitalWrite(ymotorPin1, 0);
  digitalWrite(ymotorPin2, 0);
  digitalWrite(ymotorPin3, 0);
  digitalWrite(ymotorPin4, 0);
};

void fyreverse()
{
  //Serial.println("yreverse");
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(ymotorPin1, backwardSequence[i][0]);
    digitalWrite(ymotorPin2, backwardSequence[i][1]);
    digitalWrite(ymotorPin3, backwardSequence[i][2]);
    digitalWrite(ymotorPin4, backwardSequence[i][3]);
    delay(1);
  }
  digitalWrite(ymotorPin1, 0);
  digitalWrite(ymotorPin2, 0);
  digitalWrite(ymotorPin3, 0);
  digitalWrite(ymotorPin4, 0);
};

//void zforward()
//{
//  //Serial.println("zforward");
//  for (int i = 0; i < stepCount; i++) {
//    digitalWrite(zmotorPin1, forwardSequence[i][0]);
//    digitalWrite(zmotorPin2, forwardSequence[i][1]);
//    digitalWrite(zmotorPin3, forwardSequence[i][2]);
//    digitalWrite(zmotorPin4, forwardSequence[i][3]);
//    delay(1);
//  }
//  digitalWrite(zmotorPin1, 0);
//  digitalWrite(zmotorPin2, 0);
//  digitalWrite(zmotorPin3, 0);
//  digitalWrite(zmotorPin4, 0);
//};
//
//void zreverse()
//{
//  //Serial.println("zreverse");
//  for (int i = 0; i < stepCount; i++) {
//    digitalWrite(zmotorPin1, backwardSequence[i][0]);
//    digitalWrite(zmotorPin2, backwardSequence[i][1]);
//    digitalWrite(zmotorPin3, backwardSequence[i][2]);
//    digitalWrite(zmotorPin4, backwardSequence[i][3]);
//    delay(1);
//  }
//  digitalWrite(zmotorPin1, 0);
//  digitalWrite(zmotorPin2, 0);
//  digitalWrite(zmotorPin3, 0);
//  digitalWrite(zmotorPin4, 0);
//};

void nforward()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //digitalWrite(dirPin, HIGH);
  digitalWrite(dirPin, LOW);
  // Spin motor slowly
  for (int x = 0; x < moveCount; x++)
  {
    int esz = digitalRead(endstop_z);
    if (esz == 1)
    {
      break;
    }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(900);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(900);
    delay(10);
  }

  //delay(100); // Wait a second
}

void nreverse()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //digitalWrite(dirPin, LOW);
  digitalWrite(dirPin, HIGH);
  // Spin motor slowly
  for (int x = 0; x < moveCount; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(900);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(900);
    delay(10);
  }
  //delay(100); // Wait a second
}
void zforward()
{
//

  digitalWrite(dirPin1, HIGH);

  // Spin motor slowly
  for (int x = 0; x < moveCount; x++)
  {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(10);//900
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(10);//100
    delay(1);
  }

  // Wait a second
}
void zforward1()
{
//

  digitalWrite(dirPin1, HIGH);

  // Spin motor slowly
  for (int x = 0; x < moveCount; x++)
  {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(500);//900
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(300);//100
    delay(1);
  }

  // Wait a second
}

void zreverse()
{
//  pinMode(stepPin1, OUTPUT);
//  pinMode(dirPin1, OUTPUT);
  digitalWrite(dirPin1, LOW);

  // Spin motor slowly
  for (int x = 0; x < moveCount; x++)
  {

    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(500);//900
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(300);//100
    delay(1);
  }
  //  delay(300); // Wait a second
}
void zreverse1()
{
//  pinMode(stepPin1, OUTPUT);
//  pinMode(dirPin1, OUTPUT);
  digitalWrite(dirPin1, LOW);

  // Spin motor slowly
  for (int x = 0; x < moveCount; x++)
  {

    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(10);//900
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(10);//100
    delay(4);
  }
  //  delay(300); // Wait a second
}
