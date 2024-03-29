/* This program is a parsing routine of TF02 product standard output protocol on Arduino.
The format of data package is 0x59 0x59 Dist_L Dist_H Strength_L Strength_H Sequence_L Sequence_H
CheckSum_L
Refer to the product specification for detailed description.
For Arduino board with one serial port, use software to virtualize serial port’s functions: such as UNO board.
*/
#include<SoftwareSerial.h>// soft serial port header file
SoftwareSerial Serial1(2,3); // define the soft serial port as Serial, pin2 as RX, and pin3 as TX
/*For Arduino board with multiple serial ports such as DUE board, comment out the above two codes, and directly use
Serial port*/
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

bool stop=true;

int cutoff=7000;
int average_over=40;
int s[10]={10};

int average;

void loop()
{
   if (Serial1.available())//check whether the serial port has data input
   {
    //      Serial.println("Test4");
     if(Serial1.read()==HEADER)// determine data package frame header 0x59
     {
      //Serial.println("");
       uart[0]=HEADER;
       if(Serial1.read()==HEADER)//determine data package frame header 0x59
       {
        Serial.println("Test2");
         uart[1]=HEADER;
         for(i=2;i<9;i++)// store data to array
         {
          uart[i]=Serial1.read();
         }
         //Serial.println("test");
         check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
         if(uart[8]==(check&0xff))// check the received data as per protocols
         {
           //Serial.println("Test");
           dist=uart[2]+uart[3]*256;// calculate distance value
           strength=uart[4]+uart[5]*256;// calculate signal strength value
           //Serial.print("dist = ");
           //Serial.print(dist);
           //Serial.print(" "); // a space ' ' or  tab '\t' character is printed between the two values.
           average=0;
           for (int j=0; j<average_over-1; j++){
                s[j]=s[j+1];
                average+=s[j]/average_over;
           }
           average+=strength/average_over;
           s[average_over-1]=strength;
           if (average<cutoff && stop==true){
            stop=false;
            //Serial.println("Go");
           }
           if (average>=cutoff && !stop){
            stop=true;
            //Serial.println("Stop");
           }
//           for (int j=0; j<average_over;j++){
//            Serial.print(s[j]);
//            Serial.print("|");
//           }
           //Serial.print("=====================");
//           Serial.print("strength:"); Serial.print(strength); Serial.print(", ");
           Serial.print("average:"); Serial.print(average); Serial.print(", ");
           Serial.println(); // the last value is followed by a carriage return and a newline characters.
           //delay(4000);
           
         }
       }
     }
   }
}
