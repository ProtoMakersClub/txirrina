// This #include statement was automatically added by the Particle IDE.
//#include <Keypad_Particle.h>


/*********************************************************
Serial Graphic LCD Library Demo
Joel Bartlett
SparkFun Electronics
October 14, 2013

Updated for Partilce on 7-2-15

License:
This code is beerware: feel free to use it, with or without attribution,
in your own projects. If you find it helpful, buy me a beer next time you
see me at the local pub.

This is an example sketch to accompany the Serial Graphic LCD Library.
This is intended for use with SparkFun's Serial Graphic LCDs: the 128x64 pixel,
the 160x128 pixel, and the Graphic LCD Serial Backpack. There is no guarantee
that this code will work on any other LCDs or backpacks.

***********************************************************/

#define RELAY_PIN 6
#define RELAY_LED_PIN 7
#define ENABLE_RELAY_LED
#define RELAY_DELAY 2000
#define DEBOUNCE_DELAY 2

#define DOOR_ENDSTOP_PIN 5
int doorValue;
int lastDoorValue;

#define KEYPRESS_DELAY 500

#include "SparkFun_Serial_Graphic_LCD.h"//inculde the Serial Graphic LCD library

//This demo code was created for both the 128x64 and the 160x128 pixel LCD.
//Thus, these maximum limits are set to allow this code to work on both sizes of LCD.
//The default size is the 128x64, but you can delete those values and uncomment
//the other values if you have a 160x128 pixel LCD. If you are writing code for just
//one size LCD, you can omit this information entirely and just use numerical values
//in place of these variable names.

#define maxX 159
#define maxY 127
//Each maximum value is one less than the stated value to account for position 0,0
//Thus, position 127 is actually the 128th pixel.

//Create an instance of the LCD class named LCD. We will use this instance to call all the
//subsequent LCD functions
LCD LCD;
const unsigned long period = 200;
unsigned long prevMillis = 0;


int iRow = 0, iCol = 0;
const byte countRows = 4;
const byte countColumns = 4;

const byte rowsPins[countRows] = { A0, A1, A2, A3 };
const byte columnsPins[countColumns] = { A4, A5, A6, A7 };

String keys[countRows][countColumns] = {
   { "1","2","3", "A" },
   { "4","5","6", "B" },
   { "7","8","9", "C" },
   { "*","0","#", "D" }
};

String keyBuffer;

bool readKeypad()
{
   // Barrer todas las filas
   for (int r = 0; r < countRows; r++)
   {
    // pinMode(rowsPins[r], OUTPUT);
      digitalWrite(rowsPins[r], LOW);      // Poner en LOW fila

      // Comprobar la fila
      for (int c = 0; c < countColumns; c++)
      {

         if (digitalRead(columnsPins[c]) == LOW)   // Pulsacion detectada
         {
            #ifdef DEBOUNCE_DELAY
            delay(DEBOUNCE_DELAY);
            byte delayRead = digitalRead(columnsPins[c]);
            if(delayRead == LOW){
             #endif
            iRow = r;
            iCol = c;
            return true;
            #ifdef DEBOUNCE_DELAY
            }
            #endif
         }
      }

    //  pinMode(rowsPins[r], HIGH);         // Ponen en HIGH fila
      digitalWrite(rowsPins[r], HIGH);   // Poner en alta impedancia
      delay(10);
   }
   return false;
}

void setup()
{
pinMode(RELAY_PIN,OUTPUT);
#ifdef ENABLE_RELAY_LED
    pinMode(RELAY_LED_PIN,OUTPUT);
#endif
Particle.function("ireki",handleIreki);

pinMode(DOOR_ENDSTOP_PIN,INPUT_PULLUP);
Particle.variable("doorOpen", &doorValue, INT);

//we're just going to run through a bunch of demos to show the functionality of the LCD.

delay(1200);///wait for the one second spalsh screen before anything is sent to the LCD.

LCD.setHome();//set the cursor back to 0,0.
LCD.clearScreen();//clear anything that may have been previously printed ot the screen.
delay(10);

LCD.println("Txirina! //Ongi etorri gure zerbitzura//");
delay(1500);

//This function should only be used if you accidentally changed the baud rate or if you forgot to what rate it was changed.
//LCD.restoreDefaultBaud();// needs more work for the Photon


Serial.begin(9600);

   for (byte c = 0; c < countColumns; c++)
   {
      pinMode(columnsPins[c], INPUT_PULLUP);
   }
   for (byte r = 0; r < countRows; r++)
   {
      pinMode(rowsPins[r], OUTPUT);
      digitalWrite(rowsPins[r], HIGH);
   }

}
//-------------------------------------------------------------------------------------------
void loop()
{
    if (millis() - prevMillis > period)   // Espera no bloqueante
   {
      prevMillis = millis();

      doorValue = !digitalRead(DOOR_ENDSTOP_PIN);
      if(lastDoorValue != doorValue){
          if(doorValue == HIGH){
              Particle.publish("doorStatus","true");
          } else {
              Particle.publish("doorStatus","false");
          }

      }
      lastDoorValue = doorValue;
      if (readKeypad())   // Detección de tecla pulsada
      {
          //Serial.println(iRow);

          String currentKey=keys[iRow][iCol];
          //Particle.publish("key",currentKey);
          //LCD.print(String(currentKey) + "");
          if(currentKey == "#"){
            Particle.publish("deia",keyBuffer);
            keyBuffer = "";
            //LCD.print("Deia!");
            LCD.clearScreen();
            LCD.print("");
          } else if (currentKey == "*") {
            keyBuffer = "";
            LCD.clearScreen();
          } else {
            keyBuffer = keyBuffer + "" + currentKey;
            char output[keyBuffer.length()];
            currentKey.toCharArray(output, keyBuffer.length());
            LCD.clearScreen();
            //LCD.setHome();
            Serial1.print(keyBuffer);
            delay(KEYPRESS_DELAY);
          }
          if(keyBuffer == ""){
              //delay(1000);
              //LCD.clearScreen();
          }

//LCD.print(*keyBuffer);   // Mostrar tecla
         //delay(100);
      }
   }
}

int handleIreki(String command) {
    digitalWrite(RELAY_PIN,HIGH);
    #ifdef ENABLE_RELAY_LED
        digitalWrite(RELAY_LED_PIN,HIGH);
    #endif
    delay(RELAY_DELAY);
    digitalWrite(RELAY_PIN,LOW);
    #ifdef ENABLE_RELAY_LED
        digitalWrite(RELAY_LED_PIN,LOW);
    #endif
    return 1;
}
