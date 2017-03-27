/*
 * Created by: Edgar Solis Vizcarra
 * Date: 26/03/2017
 * Ping code from: http://playground.arduino.cc/Code/NewPing and https://www.instructables.com/id/Ultrasonic-Range-detector-using-Arduino-and-the-SR/
 * With some inspiration and logic from: https://create.arduino.cc/projecthub/SAnwandter1/programming-4-digit-7-segment-led-display-2d33f8 and http://forum.arduino.cc/index.php?topic=20201.0
 * 
 * Code is just a prototype
*/
#include <NewPing.h>

#define TRIGGER_PIN  13   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     14   // Arduino pin tied to echo pin on the ultrasonic sensor.
                          //Used 14(A0) Because I was testing with Serial.print() so I was unable to use pin 0, 1 and my digital pins where full
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Creating the NewPing Object.

int segmentA = 2; // Segments A thru P == pins 0 thru 7
int digit0 = 9; // Digits 3 thru 4 == pins 9 thru 12 (0 is LEFT most digit)
/*
W I R I N G   G U I D E

=== Sonar ===
Arduino  Sonar
 5V        vcc
 GND       GND
 13        Trig
 A0        Echo


=== Common Cathode digits ===
Arduino  Display  Digit
 9         12      1 Leftmost
 10        9       2
 11        8       3
 12        6       4 Rightmost
 
=== Anode Segments ===
Arduino  Display   Segment
  2       11         A
  3       7          B
  4       4          C
  5       2          D
  6       1          E
  7       10         F
  8       5          G

  The dot would be pin 3 on the display but I'm not using it for this project.
  I got the diagram of the pins from: http://arduino.fisch.lu/Uploads/arduino/circuit_TYC-365GWB.png
*/

/*
10 digits:
Each defines which segments should be on/off for that digit: A,B,C,D,E,F,G,P
*/
byte numbers[10] =
{
//If you want to add the dot you would have to add a digit to each of the numbers right after 'B', the result would be PGFEDCBA
//GFEDCBA
 B0111111, // 0
 B0000110, // 1
 B1011011, // 2
 B1001111, // 3
 B1100110, // 4
 B1101101, // 5
 B1111101, // 6
 B0000111, // 7
 B1111111, // 8
 B1101111  // 9
};

void setup()
{
 for (byte segment = 0; segment < 8; segment++) //We assign the pins to each of the segments
   pinMode(segmentA+segment,OUTPUT);
 for (byte digit = 0; digit < 4; digit++) //We assign the pins to each of the digits
   pinMode(digit0+digit,OUTPUT);
}
  int digits[4] = {0,0,0,0}; //We need to have each digit of the int as an individual int so we assign them to an array
  
void refresh()
{
  int val = sonar.ping_cm(); //We get the ping distance in cm, the documentation for the NewPing library can be found at http://playground.arduino.cc/Code/NewPing
  for (int i = 3; i >= 0; i--){ //We separate each of the elements of val and add it to our digits array
    digits[i] = val % 10; // "% 10" gives us the rigthmost digit of our val int and assigns it to the appropiate value of our digits array
    val /= 10; // We get rid of the element we just added
  }
  for (int h = 0; h < 5; h++){
    //In order to write a different number to each digit we need to loop through them and assign them individually.
    byte number = numbers[digits[h]]; //We get the segments pattern from our binary patterns
    digitalWrite((digit0 + h) - 1, HIGH); //We power up the digit
    delay(4); //you can play with this delay to watch how it loops. try 100 instead of 4
   for (byte seg = 0; seg < 8; seg++) //We power up the segments of the digit
   {
    digitalWrite(segmentA+seg, 1-bitRead(number,seg));
   }
   //now we need to power down all the digits so we dont get repeated digits
   for(int d = 0; d < 4; d++){
    digitalWrite(digit0 + d, LOW);
   }
  }
}

void loop()
{
  refresh();
}
