// ---------------------------------------------------------------------------
// Ejemplo medidor de distancia con módulos Grove:  - v1.0 - 20/07/2020
//
// AUTOR:
// Creado por Angel Villanueva - @avilmaru
//
// LINKS:
// Blog: http://www.mecatronicalab.es
//
//
// HISTORICO:
// 20/07/2020 v1.0 - Release inicial.
//
// ---------------------------------------------------------------------------

#include "Ultrasonic.h"
#include "TM1637.h"
#include <Grove_LED_Bar.h>

// Ultrasonic sensor
const int ULSTRASONIC_PIN = 4;
Ultrasonic ultrasonic(ULSTRASONIC_PIN);

// 4 Digit diplay
const int PIN_CLK_DIGIT_DISPLAY = 8;
const int PIN_DIO_DIGIT_DIPLAY = 9;
TM1637 tm1637(PIN_CLK_DIGIT_DISPLAY, PIN_DIO_DIGIT_DIPLAY);

// LED Bar
const int PIN_CLK_LED_BAR = 3;
const int PIN_DIO_LED_BAR = 2;
Grove_LED_Bar bar(PIN_CLK_LED_BAR, PIN_DIO_LED_BAR, 0, LED_BAR_10); // Clock pin, Data pin, Orientation

const int TIEMPO_VERIFICACION = 60;
const int PIN_BUTTON = 6;
const int PIN_LED_BUTTON = 5;

const int PIN_BUZZER = 7;

int activaSonido = 1;
int level;

unsigned long startMillis= millis();  // Start of sample window
unsigned int intervalSound;


void setup() {
  
  tm1637.init();
  tm1637.set(BRIGHTEST);  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  
  bar.begin();
  bar.setGreenToRed(true);
  
  pinMode(PIN_DIO_DIGIT_DIPLAY, OUTPUT);
  pinMode(PIN_DIO_LED_BAR, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT); 
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED_BUTTON, OUTPUT); 

  digitalWrite(PIN_LED_BUTTON,HIGH);  

}

void loop() {

  int lecturaPULSADOR = lecturaPin(PIN_BUTTON);
  if (lecturaPULSADOR == LOW)  //  default on LED BUTTON is high
  {
    if (activaSonido == 0)
    {
      // 4 Digit Display
      tm1637.displayStr("ON");
      activaSonido = 1;
    }
    else if (activaSonido == 1)
    {
      // 4 Digit Display
      tm1637.displayStr("OFF");
      activaSonido = 0;
    }

    digitalWrite(PIN_LED_BUTTON,activaSonido);  
    delay(1000);  

  }


  long RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  
  if (RangeInCentimeters >= 0 && RangeInCentimeters < 3)
  {
    level = 10;  
    intervalSound = 60;
  }
   
  else if (RangeInCentimeters >= 3 && RangeInCentimeters < 6)
  {
    level = 9;
    intervalSound = 100;  
  }
  else if (RangeInCentimeters >= 6 && RangeInCentimeters < 9)
  {
    level = 8; 
    intervalSound = 200;
  }
  else if (RangeInCentimeters >= 9 && RangeInCentimeters < 12)
  {
    level = 7;  
    intervalSound = 400;
  }
  else if (RangeInCentimeters >= 12 && RangeInCentimeters < 15)
  {
    level = 6;  
    intervalSound = 600;
  }
  else if (RangeInCentimeters >= 15 && RangeInCentimeters < 18)
  {
    level = 5; 
    intervalSound = 600;
  }
  else if (RangeInCentimeters >= 18 && RangeInCentimeters < 21)
  {
    level = 4; 
    intervalSound = 700;
  }
  else if (RangeInCentimeters >= 21 && RangeInCentimeters < 24)
  {
    level = 3; 
    intervalSound = 700;
  }
  else if (RangeInCentimeters >= 24 && RangeInCentimeters < 27)
  {
    level = 2;  
    intervalSound = 900;
  }
  else if (RangeInCentimeters >= 27 && RangeInCentimeters < 30)
  {
    level = 1;  
    intervalSound = 900;
  }
  else 
    level = 0; 


  if (level > 0)
  {
     // 4 Digit Display
    tm1637.displayNum(RangeInCentimeters);  
  
    // Sound
    if (activaSonido == 1)
      soundProximity();
      
  }else
  {
    // 4 Digit Display
    tm1637.displayStr("----");
  }
 
  // LED Bar display
  bar.setLevel(level);

}



void soundProximity() 
{
    
    if (millis() - startMillis > intervalSound)
    {
     digitalWrite(PIN_BUZZER, HIGH);
     delay(30);
     startMillis= millis();
    }

    digitalWrite(PIN_BUZZER, LOW);
      
}


boolean lecturaPin(int pin)
{
    boolean estado;
    boolean estadoAnterior;
    
    estadoAnterior = digitalRead(pin);  
    for(int i=0; i < TIEMPO_VERIFICACION; i++)
    {
      delay(1);                       // esperar 1 milisegundo
      estado = digitalRead(pin);     
      if( estado != estadoAnterior)  // si el estado cambia -> resetear el contador y guardar el estado
      {
        i = 0; 
        estadoAnterior = estado;
      }
    }
    
    // si hemos llegado aqui el estado se ha mantenido estable (no ha cambiado) por un periodo de tiempo y damos por válida la lectura
    return estado;
}
