/*******************************************\
|* Übersetzer von PS2 zu KC85/2-4 Keyboard *|
\*******************************************/

//PS2Keyboard library wurde dafür etwas angepasst

//MEMO an mich: DL123 GND und PIN gegenüber
//sind die richtigen.
//Ein Optokoppler, npn-Transi, LED und ein 
//Widerstand reichen dann für den Spaß.
//Erst per Serial testen, dann meckern :P

#include "PS2KeyboardKC.h"

const int REMO = 12;
const int DataPin = 8;
const int IRQpin =  3;
PS2Keyboard keyboard;

byte incomingByte;
byte lastByte;
bool lower;

void setup() 
{       
  delay(1000);
  keyboard.begin(DataPin, IRQpin, PS2Keymap_German);
  pinMode(REMO, OUTPUT);
  Serial.begin(9600);
  lastByte = 0;
  lower = false;
}

void loop()
{
  ReaderPS2();
  ReaderSerial();
}

void ReaderPS2()
{
  if (keyboard.available()) 
  {
    incomingByte = keyboard.read();
    //Serial.print(incomingByte, DEC);
    if (incomingByte == PS2_CAPS)
    {
       lower = !lower;
       return;
    }
    if (!lower)
    {
        if (incomingByte > 64 && incomingByte < 90)
          incomingByte += 32;
        else if (incomingByte > 96 && incomingByte < 123)
          incomingByte -= 32;
    }
    ConvertASCII(incomingByte);
  }
  
}

void ReaderSerial()
{
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();
    ConvertASCII(incomingByte);
  }
}

void ConvertASCII(unsigned int Data)
{
    if (lastByte == Data)
      delayMicroseconds(14336); //Haben wir 2x die gleiche Taste dann DW
    lastByte = Data;
    switch (Data)
    {
      case 7: SendByte(B10111100); break; //END = BREAK

      case 8: SendByte(B11100000); break; //PL
      case 10: SendByte(B11101110); break; //PD
      case 11: SendByte(B10011110); break; //PU
      case 13: SendByte(B01111110); break; //CR
      case 21: SendByte(B11011110); break; //PR

      case 27: SendByte(B10110010); break; //ESC = STOP

      case 32: SendByte(B11100010); break; //SP
      case 33: SendByte(B00101110); break; //!
      case 34: SendByte(B00100000); break; //"
      case 35: SendByte(B00101000); break; //#
      case 36: SendByte(B00100110); break; //$
      case 37: SendByte(B00100100); break; //%
      case 38: SendByte(B00101010); break; //&
      case 39: SendByte(B00101100); break; //'
      case 40: SendByte(B00100010); break; //(
      case 41: SendByte(B01011100); break; //)
      case 42: SendByte(B01011000); break; //*
      case 43: SendByte(B10010110); break; //+
      case 44: SendByte(B11010010); break; //,
      case 45: SendByte(B11010000); break; //-
      case 46: SendByte(B11011010); break; //.
      case 47: SendByte(B11010110); break; ///
      case 48: SendByte(B11010100); break; //0
      case 49: SendByte(B10101110); break; //1
      case 50: SendByte(B10100000); break; //2
      case 51: SendByte(B10101000); break; //3
      case 52: SendByte(B10100110); break; //4
      case 53: SendByte(B10100100); break; //5
      case 54: SendByte(B10101010); break; //6
      case 55: SendByte(B10101100); break; //7
      case 56: SendByte(B10100010); break; //8
      case 57: SendByte(B11011100); break; //9
      case 58: SendByte(B11011000); break; //:
      case 59: SendByte(B00010110); break; //;
      case 60: SendByte(B01010010); break; //<
      case 61: SendByte(B01010000); break; //=
      case 62: SendByte(B01011010); break; //>
      case 63: SendByte(B01010110); break; //?
      case 64: SendByte(B01010100); break; //@
      case 65: SendByte(B11000000); break; //A
      case 66: SendByte(B11111010); break; //B
      case 67: SendByte(B11110110); break; //C
      case 68: SendByte(B11000110); break; //D
      case 69: SendByte(B10001000); break; //E
      case 70: SendByte(B11000100); break; //F
      case 71: SendByte(B11001010); break; //G
      case 72: SendByte(B11001100); break; //H
      case 73: SendByte(B10000010); break; //I
      case 74: SendByte(B11000010); break; //J
      case 75: SendByte(B10010010); break; //K
      case 76: SendByte(B10011010); break; //L
      case 77: SendByte(B11110010); break; //M
      case 78: SendByte(B11111100); break; //N
      case 79: SendByte(B11101100); break; //O
      case 80: SendByte(B11100100); break; //P
      case 81: SendByte(B10001110); break; //Q
      case 82: SendByte(B10000110); break; //R
      case 83: SendByte(B11001000); break; //S
      case 84: SendByte(B10000100); break; //T
      case 85: SendByte(B10001100); break; //U
      case 86: SendByte(B11110100); break; //V
      case 87: SendByte(B10000000); break; //W
      case 88: SendByte(B11111000); break; //X
      case 89: SendByte(B11110000); break; //Y
      case 90: SendByte(B10001010); break; //Z
      //91-93 Nicht da
      case 94: SendByte(B11101000); break; //^
      case 95: SendByte(B11100110); break; //_
      //96 ist auch weg
      case 97: SendByte(B01000000); break; //a
      case 98: SendByte(B01111010); break; //b
      case 99: SendByte(B01110110); break; //c
      case 100: SendByte(B01000110); break; //d
      case 101: SendByte(B00001000); break; //e
      case 102: SendByte(B01000100); break; //f
      case 103: SendByte(B01001010); break; //g
      case 104: SendByte(B01001100); break; //h
      case 105: SendByte(B00000010); break; //i
      case 106: SendByte(B01000010); break; //j
      case 107: SendByte(B00010010); break; //k
      case 108: SendByte(B00011010); break; //l
      case 109: SendByte(B01110010); break; //m
      case 110: SendByte(B01111100); break; //n
      case 111: SendByte(B01101100); break; //o
      case 112: SendByte(B01100100); break; //p
      case 113: SendByte(B00001110); break; //q
      case 114: SendByte(B00000110); break; //r
      case 115: SendByte(B01001000); break; //s
      case 116: SendByte(B00000100); break; //t
      case 117: SendByte(B00001100); break; //u
      case 118: SendByte(B01110100); break; //v
      case 119: SendByte(B00000000); break; //w
      case 120: SendByte(B01111000); break; //x
      case 121: SendByte(B01110000); break; //y
      case 122: SendByte(B00001010); break; //z
      case 127: SendByte(B11100000); SendByte(B10010100); break; //BSP
      case 159: SendByte(B01100110); break; //|
      case 161: SendByte(B10111110); break; //F1
      case 162: SendByte(B10110000); break; //F2
      case 163: SendByte(B10111000); break; //F3
      case 164: SendByte(B10110110); break; //F4
      case 165: SendByte(B10110100); break; //F5
      case 166: SendByte(B10111010); break; //F6
      case 167: SendByte(B00111110); break; //F7
      case 168: SendByte(B00110000); break; //F8
      case 169: SendByte(B00111000); break; //F9
      case 170: SendByte(B00110110); break; //F10
      case 171: SendByte(B00110100); break; //F11
      case 172: SendByte(B00111010); break; //F12
      case 174: SendByte(B10011100); break; //INS
      case 175: SendByte(B10010000); break; //HOME
      case 176: SendByte(B00011100); break; //°
      case 177: SendByte(B00010000); break; //CLS
      case 178: SendByte(B10010100); break; //DEL
    }
}

void SendByte(unsigned int DatenBit)
{
  for (int i = 0; i < 2; i++)
  {
    BURST(); //7
    Delay(DatenBit & B10000000);
    BURST(); //6
    Delay(DatenBit & B01000000);
    BURST(); //5
    Delay(DatenBit & B00100000);
    BURST(); //4
    Delay(DatenBit & B00010000);
    BURST(); //3
    Delay(DatenBit & B00001000);
    BURST(); //2
    Delay(DatenBit & B00000100);
    BURST(); //1
    Delay(DatenBit & B00000010);
    BURST();                  //Ich weiß nicht warum, aber der will das so
    delayMicroseconds(14336); //Der geht nur wenn nicht 2x die gleiche Taste hintereinander kommt, sonst bitte dd
  }
}

void Delay(bool in)
{
  if (in)
    delayMicroseconds(6988);
  else
    delayMicroseconds(4968);
  return;  
}

void BURST()
{
  //Wir nutzen mal 5 bursts weil die Mädels das so schreiben
  //16 us will er haben und die bekommt er auch
  for (unsigned int i = 0; i < 5; i++)
  {
    digitalWrite(REMO, HIGH);
    delayMicroseconds(16);
    digitalWrite(REMO, LOW);
    delayMicroseconds(16);
  }
}
