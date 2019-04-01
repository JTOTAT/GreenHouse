

// GreenHouse 2B added string of HX plus time stamp 3/36/19
#include <SoftwareSerial.h>

// Attach the serial enabld LCD's RX line to digital pin 11
SoftwareSerial LCD(2, 8); // Arduino SS_RX = pin 2 (unused), Arduino SS_TX = pin 8
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <X10Firecracker.h>
#define PRINT_USA_DATE
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin
#define DATA_PIN 6
// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 9
// Index of sensors connected to data pin, default: 0
#define SENSOR_INDEX 0

const int buttonPin = 3;     // the number of the pushbutton pin
const int button5 = 5;     // the number of the pushbutton pin
const int button7 = 7;     // the number of the pushbutton pin

int buttonState = 0;         // variable for reading the pushbutton status
int butState5 = 0;        // variable for reading the pushbutton status
int butState7 = 0;        // variable for reading the pushbutton status

int screen = 1; // LCD screen controller
OneWire oneWire(DATA_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;
String stringLA0, stringLA1, stringLA2, stringLA3, stringLA4, stringLAH;
void setup()
{ 
// Use the serial monitor to view time/date output
  Serial.begin(9600);
int dateint = 666;
 sensors.begin();
  sensors.getAddress(sensorDeviceAddress, 0);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
stringLA0 = String("St0 ");
stringLA1 = String("St1 ");
stringLA2 = String("St2 ");
stringLA3 = String("St3 ");
stringLA4 = String(":");  
stringLAH = String("Hr"); 
pinMode(buttonPin, INPUT); 
pinMode(button5, INPUT); 
pinMode(button7, INPUT); 
pinMode(A0, OUTPUT);
pinMode(9, OUTPUT); // SS Relay pin for pump

  // Call rtc.begin([cs]) to initialize the library
  // The chip-select pin should be sent as the only parameter
  rtc.begin(DS13074_CS_PIN); // DS3234rtc.begin(DS3234_CS_PIN); // DS3234
  rtc.set24Hour(); // Use rtc.set12Hour to set to 12/24 hour mode
  

  // Now set the time...
  // You can use the autoTime() function to set the RTC's clock and
  // date to the compiliers predefined time. (It'll be a few seconds
  // behind, but close!)
  //rtc.autoTime();
  // Or you can use the rtc.setTime(s, m, h, day, date, month, year)
  // function to explicitly set the time:
  // e.g. 7:32:16 | Monday October 31, 2016:
  //rtc.setTime(0, 15, 3, 2, 17, 1, 19);  // Uncomment to manually set time

  // Update time/date values, so we can set alarms
  rtc.update();
  // Configure Alarm(s): Not Used!!!
  // (Optional: enable SQW pin as an intterupt)
 // rtc.enableAlarmInterrupt();
  // Set alarm1 to alert when seconds hits 30
  //rtc.setAlarm1(30);
  // Set alarm2 to alert when minute increments by 1
 // rtc.setAlarm2(rtc.minute() + 1);

 X10.init( 2, 4, 1); // set X10 lines
 
  LCD.begin(9600); // set up serial port for 9600 baud
 delay(500); // wait for display to boot up

  // cursor to beginning of first line
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
 delay (30);
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(128);
  LCD.write("GH2b Controller    "); 
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write(" Jon Turnquist     ");
  LCD.write("                   "); 
 delay (3000);
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
 delay (30);
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
 
} 
int casecop = 1;
int whatthing = 0; // last action number
int ss = 9;
int dateCOP = 666;
int cop = 1;  //used for data control
int Heatcop = 1;
int Fancop = 1;
int HT = 0; //High temp
int TE = 55; //24 temp extreme  
int LT = 100; //Low temp
int X10ALT = 0; //on/off var
int var = 1 ; // X10 var = unit number for fan
int Hvar = 5; //X10 var for heater
int XPump = 2; // X10 var = unit number for pump
int Pontime = 15; // Pump ON duration per watering, change this to alter pump ON normal (not hot+ )time
int HotPtime = 10; 
int TooHotAlarm = 0;
int FanONTemp = 80; // default temp to turn fan on - can set as needed in program
int HeatONtemp = 36; // default temp to turn heater on - can set as needed in program
int pstat = 0; 
int sec, hor, Min;
int da, mo, yr;
int m, h;
int whatthingcop = 0;
int Pcop = 0;
//int m = rtc.minute();
//int h = rtc.hour();
//int da = rtc.date();
//int mo = rtc.month();
//int yr = rtc.year();
// da;mo;yr;


char hourstring[10], secstring[10], Minstring[10];// create string arrays

void loop() 
{ 
  
buttonState = digitalRead(buttonPin);
 if (buttonState == HIGH) 
     {
     screen = screen +1 ;
     } 
    
   if (screen >8) // how many case screens
   {
    screen = 1; // reset back to first (main) screen
    whatthingcop = 0; // reset
   } 
      
 sensors.requestTemperatures();
  // Measurement may take up to 750ms

  float temperatureInFahrenheit = sensors.getTempFByIndex(SENSOR_INDEX);
  
static int8_t lastSecond = -1;
  
  // Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
  // etc. return functions.
rtc.update();

  if (rtc.second() != lastSecond) // If the second has changed
  {
    printTime(); // Print the new time
    
    lastSecond = rtc.second(); // Update lastSecond value
  } 
  
  sec = rtc.second(); 
  hor = rtc.hour();
  Min = rtc.minute();
  

if (dateCOP == 666) // run through 1 time only to set reset time/date
{
 m = Min; 
 h = hor; 
  da = rtc.date();
  mo = rtc.month();
  yr = rtc.year();
dateCOP = 1;
}

// start of pump control
 
if (Min == 1 && sec < Pontime)// turn pump on for set time at 1 min after each hour

{
if (pstat == 1) // 1st time through only if pstat (pump status) is 0 (off)
{goto Bail;}

digitalWrite(ss, HIGH);  // turn pump SS relay ON

X10.sendCmd( hcC, (XPump), cmdOn );  //  X10 TX ON
whatthing = 1; // (pump on)
whatthingcop = 0; // reset screen 8 
pstat = 1; // pump status is ON 
goto Bail; 
}




if (Min == 16 && sec < Pontime) // water at 16 after each hour
{

if (pstat == 1) // 1st time through only
{goto Bail;} 

if (hor > 18 || hor < 9) //This is OR as in either one can send it to BAIL: This is in loop of 16Min, to decrese watering at night from 1:00AM (13) to 9:00AM 

{goto Bail;} 

digitalWrite(ss, HIGH);
X10.sendCmd( hcC, (XPump), cmdOn );
whatthing = 1; // (pump on)
whatthingcop = 0; // reset screen 8 
pstat = 1;
goto Bail;
}




if (Min == 31 && sec < Pontime)// 
{
  if (pstat == 1) // 1st time through only if pump status is 0 (off)
{goto Bail;} 
 
digitalWrite(ss, HIGH); 
X10.sendCmd( hcC, (XPump), cmdOn );
whatthing = 1; // (pump on)
whatthingcop = 0; // reset screen 8 
pstat = 1; // pump status is ON
goto Bail;
} 




if (Min == 46 && sec < Pontime)
{
 if (pstat == 1) // 1st time through only if pump is OFF
{goto Bail;} 


if (hor > 19 || hor < 9) // decrese watering at night NO :46 after watering from 7:00PM to 9:00 AM
{goto Bail;} 

digitalWrite(ss, HIGH);
X10.sendCmd( hcC, (XPump), cmdOn );
delay (500); 
whatthing = 1; // (pump on)
whatthingcop = 0; // reset screen 8 
pstat = 1; // pump ON
goto Bail;
}



if (pstat == 1)  // will only get here if nothing is true above
{
 digitalWrite(ss,LOW); // pump = off 
delay (50);
X10.sendCmd( hcC, (XPump), cmdOff );
delay (500);
whatthing = 2; // (pump OFF)
whatthingcop = 0; // reset screen 8 
pstat = 0; //turn off and allow one pass through only
}


Bail: // exit loop here for decreased night watering 

if (hor == 23 && Min == 59 && sec == 59)  // reset at Midnight
{
  
TE = 55; // reset 24 hr temp extreme
screen = 1; // reset screen just in case at midnight
}

  sprintf(hourstring,"%2d",hor); // create strings from the numbers
  sprintf(secstring,"%2d",sec); // right-justify to 4 spaces thw %4d 
  sprintf(Minstring,"%2d",Min); // right-justify to 4 spaces thw %4d 


switch (screen) {
    case 1:

 if (casecop != 1)   //  !=  is does not equal...

 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  }
    
  LCD.write(254); // cursor to 7th position on first line
  LCD.write(128);
  
 
  LCD.write(hourstring); // write out the Hor value 
  LCD.write(254); // cursor to 7th position on second line
  LCD.write(130);
  LCD.write(":"); // write out the TEMP value

  LCD.write(254); // cursor to 7th position on first line
  LCD.write(131);
  LCD.write(Minstring); // write out the MIN value
  
  LCD.write(254); // cursor to 7th position on second line
  LCD.write(133);
  LCD.write(":"); // write out the TEMP value

  LCD.write(254); // cursor to 7th position on second line
  LCD.write(134);
  LCD.write(secstring); // write out the RPM value // sec

  LCD.write(254); // cursor to 7th position on second line
  LCD.write(137);
   LCD.write("T: "); // write out the Fan TEMP ON value
   LCD.print(temperatureInFahrenheit, 0);
 //LCD.print(temperatureInFahrenheit);
  LCD.write(254); 
  LCD.write(192); //start of second Line
  //LCD.write("T: 68"); // write out the TEMP value
  // LCD.print(temperatureInFahrenheit, 0);
LCD.write("HT"); // HT = High Temp
LCD.print(HT); //  write  the High all time temp
LCD.write(" LT"); // LT = low temp
LCD.print(LT); // write  the low all time temp
LCD.write(" TE:"); // write out last 24 HR temp extrimes
LCD.print(TE); // write out the last 24 HR temp extrimes
 casecop = 1; 

     break;
  case 2:  // Next page of LCD Adjust Fan turn ON/Off temp

 butState5 = digitalRead(button5);
  if ( butState5  == HIGH)
 {
FanONTemp = FanONTemp +1;
 }
butState7 = digitalRead(button7);
  if ( butState7  == HIGH)
 {
FanONTemp = FanONTemp -1;
 }
 
 if (casecop != 2) // must equal 2 
 
  {LCD.write(254); 
  LCD.write(128);
  LCD.write("                    "); 
  LCD.write(254); 
  LCD.write(192);
 LCD.write("                    ");
  }

  LCD.write(254); 
  LCD.write(128);
  //  LCD.write("Screen = :"); // write out the Fan TEMP ON value
 // LCD.print(screen); // write out the Pump ON duration time
  
  LCD.write("Fan on Temp:"); // write out the Fan TEMP ON value
  if (FanONTemp < 10)
  {
    LCD.write(254); 
    LCD.write(210);
    LCD.write(" ");// erase 0
  }

    
  LCD.print(FanONTemp); // write out thelast water value
  LCD.write(254); 
  LCD.write(192);
  LCD.write("Buttons up/down"); // write out the Fan TEMP ON value
   casecop = 2;
      break;

case 3:  // Next page of LCD Adjust Pump ON duration

 butState5 = digitalRead(button5);
  if ( butState5  == HIGH)
 {
  
HotPtime = HotPtime +1;
 }
butState7 = digitalRead(button7);
  if ( butState7  == HIGH)
 {
HotPtime = HotPtime -1;
 }
 
 if (casecop != 3) // must equal 2 
 
  {LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  }

  LCD.write(254); 
  LCD.write(128);
  LCD.write("Fan ON + time:"); // write out the Fan TEMP ON value
  //LCD.write("Fan on Temp:"); // write out the Fan TEMP ON value
  LCD.print(HotPtime); // write out the Pump ON duration time
  if (HotPtime < 10)
  {
  LCD.write(254); 
  LCD.write(143);
  LCD.write(" ");
  }
  
  LCD.write(254); 
  LCD.write(192);
  LCD.write("Buttons up/down"); // write out the Fan TEMP ON value
   casecop = 3;
      break; 

case 4:  // Next page of LCD Adjust Pump ON duration

if (casecop != 4) // must equal 4 
 
  {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                "); 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                ");

  LCD.write(254); 
  LCD.write(128);
  LCD.write("UP for C1 ON:  "); // write out the Fan TEMP ON value
  LCD.write(254); 
  LCD.write(192);
  LCD.write("DN for C1 OFF  "); // write out the Fan TEMP ON value

  }

 butState5 = digitalRead(button5);
  if ( butState5  == HIGH)
 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("UP for C1 ON: 1"); // write out the Fan TEMP ON value
  LCD.write(254); 
  LCD.write(192);
  LCD.write("DN for C1 OFF 0"); // write out the Fan TEMP ON value
  //digitalWrite(ss,HIGH); // SS relay pump = ON
  X10.sendCmd( hcC, (var), cmdOn );
  delay (750);
  
 }
butState7 = digitalRead(button7);
  if ( butState7  == HIGH)
 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("UP for C1 ON: 0"); // write out the Fan TEMP ON value
  LCD.write(254); 
  LCD.write(192);
  LCD.write("DN for C1 OFF 1"); // write out the Fan TEMP ON value

 // digitalWrite(ss,LOW); // pump = off
  X10.sendCmd( hcC, (var), cmdOff );
  delay (750);
  
 }
 
   casecop = 4;
      break; 
case 5:  // Next page  Pump ON overide

if (casecop != 5) // must equal 4 
 
  {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                "); 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                ");

  LCD.write(254); 
  LCD.write(128);
  LCD.write("UP= PUMP ON:  "); // 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("DN= PUMP OFF:  "); // 

  }



 butState5 = digitalRead(button5);
  if ( butState5  == HIGH)
 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("UP=PUMP ON: 1"); // 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("DN=PUMP OFF: 0"); // 
  digitalWrite(ss,HIGH); // SS relay pump = ON
  delay (50);
  X10.sendCmd( hcC, (XPump), cmdOn ); 
  delay (700);
  
 }
butState7 = digitalRead(button7);
  if ( butState7  == HIGH)
 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("UP=PUMP ON: 0"); // 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("DN=PUMP OFF: 1"); // 

  digitalWrite(ss,LOW); // pump = off
  delay (50);
  X10.sendCmd( hcC, (XPump), cmdOff );
  delay (700);
  
 }
  
   casecop = 5;
      break; 


// new case start
 case 6:

 if (casecop != 6)   //  !=  is does not equal...to keep from flicker of reprinting LCD

 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  }
  LCD.write(254); // cursor to 7th position on first line
  LCD.write(128);
  LCD.write("Reset: "); // write out last rerset time
  LCD.print(mo); // write out the month
  LCD.write(":");
  LCD.print(da); // the day 
  LCD.write(":");
  LCD.print(yr); // write out the year
  LCD.write(254); 
  LCD.write(192);
  LCD.write(" X10: F1 P2 H5");

 casecop = 6;
      break; 

case 7:


 butState5 = digitalRead(button5);
  if ( butState5  == HIGH)
 {
HeatONtemp = HeatONtemp +1;
 }
butState7 = digitalRead(button7);
  if ( butState7  == HIGH)
 {
HeatONtemp = HeatONtemp -1;
 }
 
 if (casecop != 7) // must equal 2 
 
  {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                    "); 
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                    ");
  }

  LCD.write(254); 
  LCD.write(128);
   
  LCD.write("Heat on Temp:"); // write out the Heat ON value
  LCD.print(HeatONtemp); // write out thelast water value
  LCD.write(254); 
  LCD.write(192);
  LCD.write("Buttons up/down"); // write out the Fan TEMP ON value
   casecop = 7;
      break;

case 8:
if (whatthingcop == 66) 
{goto WThingPass;}  // keep from repeating


if (whatthing == 0)
  {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
  LCD.write(254); 
  LCD.write(128);
  LCD.write("Last Action: "); // write out last command
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write("Waiting...");
  }

if (whatthing == 1)
  {
  
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(128);
  
   
stringLA0 = "P ON ";  
stringLA1 = secstring;
stringLA2 = Minstring;
stringLAH = hourstring;
stringLA3 =  stringLA0;
stringLA3 =  stringLA3 + stringLAH ; // add Hr to "Pump ON"
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA2 ; // add Min to string  
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA1 ; // add Sec to string  
  LCD.print(stringLA3);
  whatthingcop = 66; // keep from repeating
  Pcop = 1;
  }

if (whatthing == 2)
  {
  
  if (Pcop != 1)
  
  {LCD.write(254); 
  LCD.write(128);
  LCD.write("                   "); 
  }
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
 
stringLA0 = "P OFF ";  
stringLA1 = secstring;
stringLA2 = Minstring;
stringLAH = hourstring;
stringLA3 =  stringLA0;
stringLA3 =  stringLA3 + stringLAH ; // add Hr to "P Off"
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA2 ; // add Min to string  
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA1 ; // add Sec to string  
  LCD.print(stringLA3);
  whatthingcop = 66; // keep from repeating
  Pcop = 0;
  }

if (whatthing == 3)
  {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(128);

stringLA0 = "FAN ON ";  
stringLA1 = secstring;
stringLA2 = Minstring;
stringLAH = hourstring;
stringLA3 =  stringLA0; // could just use stringLA0
stringLA3 =  stringLA3 + stringLAH ; // add Hr to "Fan ON"
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA2 ; // add Min to string  
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA1 ; // add Sec to string  
  LCD.print(stringLA3);
  whatthingcop = 66;
  
  }

if (whatthing == 4)
  {

  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(128);
      
stringLA0 = "FAN OFF";  
stringLA1 = secstring;
stringLA2 = Minstring;
stringLAH = hourstring;
stringLA3 =  stringLA0;
stringLA3 =  stringLA3 + stringLAH ; // add Hr to "Pump ON"
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA2 ; // add Min to string  
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA1 ; // add Sec to string  
  LCD.print(stringLA3);
  whatthingcop = 66;
     
  }
  
if (whatthing == 5)
  {
  
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(128);
  

stringLA0 = "He ON";  
stringLA1 = secstring;
stringLA2 = Minstring;
stringLAH = hourstring;
stringLA3 =  stringLA0;
stringLA3 =  stringLA3 + stringLAH ; // add Hr to "Pump ON"
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA2 ; // add Min to string  
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA1 ; // add Sec to string  
  LCD.print(stringLA3);

  whatthingcop = 66;
  }

if (whatthing == 6)
 {
  LCD.write(254); 
  LCD.write(128);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(192);
  LCD.write("                   ");
  LCD.write(254); 
  LCD.write(128);
  

stringLA0 = "He OFF";  
stringLA1 = secstring;
stringLA2 = Minstring;
stringLAH = hourstring;
stringLA3 =  stringLA0;
stringLA3 =  stringLA3 + stringLAH ; // add Hr to "Pump ON"
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA2 ; // add Min to string  
stringLA3 =  stringLA3 + stringLA4 ; // add ":"
stringLA3 =  stringLA3 + stringLA1 ; // add Sec to string  
  LCD.print(stringLA3);
 whatthingcop = 66;
 }
 WThingPass: // keep from repeating but still allow screen 8 to refreash with current data

  break;
//======= end 
    
 } // end of switch case


 delay(750); // short delay // changed from 1000
//24 HR TE High and LOW. Reads the LOW temp TE till temp rises above 60 then reads the HIGH as TE
if (temperatureInFahrenheit > HT) // add alarm if over 100 degrees
{
HT = temperatureInFahrenheit; // set new higher temp
}

if (temperatureInFahrenheit > 60 && temperatureInFahrenheit > TE && hor > 13 )// only report high TE in the afternoon you sleeply head
{
TE = temperatureInFahrenheit; // 24 hr (TE) Temp Extreme high 
casecop = 0;
 }


if (temperatureInFahrenheit < LT)
{
LT = temperatureInFahrenheit; // set low temp
}
  
if (temperatureInFahrenheit < 60 && temperatureInFahrenheit < TE)  
{
  TE =temperatureInFahrenheit; // 24 hr LOW Temp Extreme
 casecop = 0; 
}
 

  //X10 stuff fan
if (temperatureInFahrenheit >= FanONTemp && Fancop == 1)   // Fan ON per X10 when temp over threashold 
{

X10.sendCmd( hcC, (var), cmdOn ); // var = x10 1
whatthing = 3; // (FAN on)
whatthingcop = 0; // reset screen 8 
//Pontime = 20;//change this to alter pump ON time when fan is on (Hotter)
Pontime = Pontime + HotPtime; // longer Pump On Time when hotter)
Fancop = 3; // keep from resending x10 ON signal
}


if (temperatureInFahrenheit < FanONTemp && Fancop == 3) // Fan OFF , cop stops the quick repeating
{

X10.sendCmd( hcC, (var), cmdOff ); // need to adjust Pontime this back to normal once fan is off
whatthing = 4; // (FAN OFF)
whatthingcop = 0; // reset screen 8 
Pontime = Pontime - HotPtime; // reset Pontime (Pump ON Time) to cooler time

if (Pontime < 0)
{Pontime = 15;
  }
delay (2000); // keep from quick on,off,on...
Fancop = 1;  // keep from resending x10 OFF signal
}  

 //X10 Heat stuff

if (temperatureInFahrenheit <= HeatONtemp && Heatcop == 1)   // Heat ON per X10 when temp under threashold 
{

X10.sendCmd( hcC, (Hvar), cmdOn ); //  Hvar = 5
whatthing = 5; // (heater on)
whatthingcop = 0; // reset screen 8 
Heatcop = 3; // keep from resending x10 ON signal

 
}

if (temperatureInFahrenheit > HeatONtemp && Heatcop  == 3) // Heat OFF 
{
X10.sendCmd( hcC, (Hvar), cmdOff ); 
whatthing = 6; // (heater off)
whatthingcop = 0; // reset screen 8 
Heatcop  = 1;  // keep from resending x10 OFF signal
}  

// end of Heat ON

}

void printTime()
{
  Serial.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
    Serial.print('0'); // Print leading '0' for minute
  Serial.print(String(rtc.minute()) + ":"); // Print minute
  if (rtc.second() < 10)
    Serial.print('0'); // Print leading '0' for second
  Serial.print(String(rtc.second())); // Print second
Serial.print(" sec = " ); // Print second  
Serial.print(sec); // Print second
Serial.print(" Pontime = " ); // Print second  
Serial.print(Pontime); // Print second
Serial.print(" HotPtime = " ); // Print second  
Serial.print(HotPtime); // Print second

Serial.print(" pstat = " ); // Print second  
Serial.print(pstat); // Print second


//pstat = 1; // pump ON
//int Pontime = 15; // Pump ON duration per watering, change this to alter pump ON normal (not hot+ )time
//int HotPtime = 10; 


  if (rtc.is12Hour()) // If we're in 12-hour mode
  {
    // Use rtc.pm() to read the AM/PM state of the hour
    if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
    else Serial.print(" AM");
  }
  
  Serial.print(" | ");

  // Few options for printing the day, pick one:
  Serial.print(rtc.dayStr()); // Print day string
  //Serial.print(rtc.dayC()); // Print day character
  //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
  Serial.print(" - ");
#ifdef PRINT_USA_DATE
  Serial.print(String(rtc.month()) + "/" +   // Print month
                 String(rtc.date()) + "/");  // Print date
#else
  Serial.print(String(rtc.date()) + "/" +    // (or) print date
                 String(rtc.month()) + "/"); // Print month
#endif
  Serial.println(String(rtc.year()));        // Print year
}




