/* This program is a parsing routine of TF02 product standard output protocol on Arduino.
The format of data package is 0x59 0x59 Dist_L Dist_H Strength_L Strength_H Sequence_L Sequence_H
CheckSum_L
Refer to the product specification for detailed description.
For Arduino board with one serial port, use software to virtualize serial port’s functions: such as UNO board.
*/
#include<SoftwareSerial.h>// soft serial port header file
SoftwareSerial Serial1(2,3); // define the soft serial port as Serial1, pin2 as RX, and pin3 as TX
/*For Arduino board with multiple serial ports such as DUE board, comment out the above two codes, and directly use
Serial1 port*/
int dist;// LiDAR actually measured distance value
int strength;// LiDAR signal strength
int check;// check numerical value storage
int i;
int uart[9];// store data measured by LiDAR
const int HEADER=0x59;// data package frame header
void setup()
{
 Serial.begin(9600);//set the Baud rate of Arduino and computer serial port
 Serial1.begin(115200);//set the Baud rate of LiDAR and Arduino serial port
}


int cutoff=7000;
int average_over=160;
int s[160]={10};

float average;
int n=0;
int max_strength=-32768;
int min_strength=32767;
void loop()
{
   if (Serial1.available())//check whether the serial port has data input
   {
     if(Serial1.read()==HEADER)// determine data package frame header 0x59
     {
       uart[0]=HEADER;
       if(Serial1.read()==HEADER)//determine data package frame header 0x59
       {
         uart[1]=HEADER;
         for(i=2;i<9;i++)// store data to array
         {
          uart[i]=Serial1.read();
         }
         check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
         if(uart[8]==(check&0xff))// check the received data as per protocols
         {
           n++;
           dist=uart[2]+uart[3]*256;// calculate distance value
           strength=uart[4]+uart[5]*256;// calculate signal strength value
  
           average=0;
           //shift all values backward
           for (int i=0; i<average_over-1; i++){s[i]=s[i+1]; average+=s[i]/average_over;}
           s[average_over-1]=strength;
           if (strength>max_strength){
            max_strength=strength;
           }
           if (strength<min_strength){
            min_strength=strength;
           }
           average+=strength/average_over; 
           if (n==average_over){   
//           Serial.print("max_strength = "); Serial.print(max_strength);
//           Serial.print(" min_strength = "); Serial.print(min_strength);
           Serial.print(" average_strength = ");
           Serial.print(average);// output signal strength value
           Serial.print('\n');
           n=0;
           }
         }
       }
     }
   }
}
