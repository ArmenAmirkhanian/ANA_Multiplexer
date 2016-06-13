//****************************************//
// Program: ANA_Multiplexer               //
//                                        //
// Date of latest revision: 6/10/2016     //
// Author: Dr. Armen Amirkhanian          //
// Copyright Armen Amirkhanian 2016       //
//                                        //
// Description: This code facilitates the //
// communication of an Arduino board with //
// the ADG732 multiplexer.                //
//                                        //
// This program is free software: you can //
// redistribute it and/or modify it under //
// the terms of the GNU General Public    //
// License as published by the Free       //
// Software Foundation, either version 3  //
// of the License, or (at your option)    //
// any later version.                     //
//                                        //
// This program is distributed in the     //
// hope that it will be useful, but       //
// WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or //
// FITNESS FOR A PARTICULAR PURPOSE.  See //
// the GNU General Public License for     //
// more details.                          //
//                                        //
// You should have received a copy of the //
// GNU General Public License along with  //
// this program.  If not, see             //
// <http://www.gnu.org/licenses/>.        //
//****************************************//

// Command syntax:
// xX\r
//
// x - can be anything except CR. Will be used in future updates.
// X - channel to open or enquiry
// \r - all commands MUST be terminated with a CR
//
// Upon startup, all channels are closed
// Switching channels is done by sending the ASCII code
// starting at 65 (A). For example, activating channel
// 12 would be done by sending 'L' which is 76.

//Change pinouts based on setup/board  
  const int EN = 10;
  const int WR = 6;
  const int CS = 23;
  const int REDLED = 13;
  const int GRELED = 8;

//Define read buffer and confirmation variable
  char rBuffer[3];
  int rwSuccess;
  char ack = 6;

//Define error codes
  char errHeader = 21; //using NAK because it's old school
  char outsideCHRange = 99;

//Channnel switch truth table
  const int CH[32][5] = {
    {0,0,0,0,0},//CH1
    {1,0,0,0,0},//CH2
    {0,1,0,0,0},//CH3
    {1,1,0,0,0},//CH4
    {0,0,1,0,0},//CH5
    {1,0,1,0,0},//CH6
    {0,1,1,0,0},//CH7
    {1,1,1,0,0},//CH8
    {0,0,0,1,0},//CH9
    {1,0,0,1,0},//CH10
    {0,1,0,1,0},//CH11
    {1,1,0,1,0},//CH12
    {0,0,1,1,0},//CH13
    {1,0,1,1,0},//CH14
    {0,1,1,1,0},//CH15
    {1,1,1,1,0},//CH16
    {0,0,0,0,1},//CH17
    {1,0,0,0,1},//CH18
    {0,1,0,0,1},//CH19
    {1,1,0,0,1},//CH20
    {0,0,1,0,1},//CH21
    {1,0,1,0,1},//CH22
    {0,1,1,0,1},//CH23
    {1,1,1,0,1},//CH24
    {0,0,0,1,1},//CH25
    {1,0,0,1,1},//CH26
    {0,1,0,1,1},//CH27
    {1,1,0,1,1},//CH28
    {0,0,1,1,1},//CH29
    {1,0,1,1,1},//CH30
    {0,1,1,1,1},//CH31
    {1,1,1,1,1},//CH32
  };

int SwitchChannel(int toCH){
  digitalWrite(CS,LOW);
  delay(10);
  digitalWrite(WR,LOW);
  delay(10);
  digitalWrite(A0,CH[toCH][0]);
  digitalWrite(A1,CH[toCH][1]);
  digitalWrite(A2,CH[toCH][2]);
  digitalWrite(A3,CH[toCH][3]);
  digitalWrite(A4,CH[toCH][4]);
  delay(10);
  digitalWrite(WR,HIGH);
  digitalWrite(CS,HIGH);

  return toCH+1;
}

void ack(){
  Serial.write(ack);
  Serial.write('\r');
}

void setup() {
  // Enable all I/O pins in use
  pinMode(EN,OUTPUT);
  pinMode(WR,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(REDLED,OUTPUT);
  pinMode(GRELED,OUTPUT);
  
  // Turn on LED and set EN pin low to enable the ADG732 chip
  // The ADG732 needs EN low constantly for the chip to be enabled when powered
  // Future updates will allow user to turn off chip
  digitalWrite(REDLED,HIGH);
  digitalWrite(GRELED,HIGH);
  digitalWrite(EN,LOW);
  delay(10);

  Serial.begin(9600);
  Serial.setTimeout(1000);
}

void loop() {
  digitalWrite(GRELED,LOW);

  if(Serial.available() > 0){
    digitalWrite(GRELED,HIGH);
    Serial.readBytesUntil('\r',rBuffer,3);
    if(rBuffer[1]<65 || rBuffer[1]>96){
      if(rBuffer[1] = 5){
        ack();
      }
      else{
      Serial.write(errHeader);
      Serial.write(outsideCHRange);
      Serial.write('\r');
    }
    }
    else{
    rwSuccess = SwitchChannel(rBuffer[1]-65);
    Serial.print(rwSuccess);
    Serial.print('\r');
    }
  }
  delay(4);//At a 9600 baud rate, a character is written about every ms
}
