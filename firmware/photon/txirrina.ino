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

#define RELAY_PIN 2
#define RELAY_LED_PIN 3
#define ENABLE_RELAY_LED
#define RELAY_DELAY 2000

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
const unsigned long period = 50;
unsigned long prevMillis = 0;
 
byte iRow = 0, iCol = 0;
const byte countRows = 4;
const byte countColumns = 4;
 
const byte rowsPins[countRows] = { A1, A2, A3, A4 };
const byte columnsPins[countColumns] = { 7, 6, 5, 4 };
 
String keys[countRows][countColumns] = {
   { "1","2","3", "A" },
   { "4","5","6", "B" },
   { "7","8","9", "C" },
   { "#","0","*", "D" }
};

String keyBuffer;

bool readKeypad()
{
   // Barrer todas las filas
   for (byte r = 0; r < countRows; r++)
   {
    // pinMode(rowsPins[r], OUTPUT);
      digitalWrite(rowsPins[r], LOW);      // Poner en LOW fila
 
      // Comprobar la fila
      for (byte c = 0; c < countColumns; c++)
      {
         if (digitalRead(columnsPins[c]) == LOW)   // Pulsacion detectada
         {
            iRow = r;
            iCol = c;
            return true;
         }
      }
      
    //  pinMode(rowsPins[r], HIGH);         // Ponen en HIGH fila
      digitalWrite(rowsPins[r], INPUT);   // Poner en alta impedancia
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
//we're just going to run through a bunch of demos to show the functionality of the LCD.

delay(1200);///wait for the one second spalsh screen before anything is sent to the LCD.

LCD.setHome();//set the cursor back to 0,0.
LCD.clearScreen();//clear anything that may have been previously printed ot the screen.
delay(10);

LCD.print("Txirina! //Ongi etorri gure zerbitzura//");
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
      if (readKeypad())   // Detección de tecla pulsada
      {
          //Serial.println(iRow);
          
          String currentKey=keys[iRow][iCol];
          if(currentKey == "#"){
            Particle.publish("key",keyBuffer);
            keyBuffer = "";
          } else if (currentKey == "*") {
            keyBuffer = "";
          } else {
            keyBuffer += currentKey;
          }
         
         LCD.print(*keyBuffer);   // Mostrar tecla
         delay(100);
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
}

