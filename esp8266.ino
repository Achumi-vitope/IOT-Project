//DMD headers
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <DMD2.h>
//Fonts
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Droid_Sans_12.h>
#include <fonts/Droid_Sans_16.h>
#include <fonts/Droid_Sans_24.h>
//NTPC headers
#include <NTPClient.h>
#include <TimeLib.h>

#include <WiFiUdp.h>

//Set up config:
SPIDMD dmd(1,1);                
DMD_TextBox box(dmd,0, 0); //Can include x, y, width, and height parameters

//Setting up time config
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org",19800);

//Set Wifi credentials
const char * SSID = "Tetso C Floor";
const char * PASSWORD = "tetso@pass";

//Rountine
//Improvement -- Instead of raw codding the Rountine arrays 5 times, just create arrays for Time, Subject, and Teacher and make adjustments accordingly
//
const char * Mon[] = {
  " Monday : 8:45AM to 9:30AM NET 404 and SWL 405 (Bidiyapati)",
  " Monday : 9:30AM to 10:15AM NET 404 and SWL 405 (Bidiyapati)",
  " Monday : 10:15AM to 11:00AM DMS 401 (Visato)",
  " Monday : 11:00AM to 11:45AM Lunch Break :)",
  " Monday : 11:45AM to 12:30PM PPD 402 (Heulungdailie)",
  " Monday : 12:30PM to 1:15PM SWL 406 (Nepoleon)",
  };

const char * Tue[] = {
  " Tuesday : 8:45AM to 9:30AM DMS 401 (Visato)",
  " Tuesday : 9:30AM to 10:15AM NET 404 and SWL 405 (Bidiyapati)",
  " Tuesday : 10:15AM to 11:00AM SWL 406 (Nepoleon)",
  " Tuesday : 11:00AM to 11:45AM Lunch Break :)",
  " Tuesday : 11:45AM to 12:30PM PPD 402 (Heulungdailie)",
  " Tuesday : 12:30PM to 1:15PM NIELIT (Abhigyan)",
};

const char * Wed[] =
{
  " Wednesday : 8:45AM to 9:30AM NET 404 and SWL 405 (Bidiyapati)",
  " Wednesday : 9:30AM to 10:15AM NET 404 and SWL 405 (Bidiyapati)",
  " Wednesday : 10:15AM to 11:00AM SWL 406 (Nepoleon)",
  " Wednesday : 11:00AM to 11:45AM Lunch Break :)",
  " Wednesday : 11:45PM to 12:30PM NIELIT (Abhigyan)",
  " Wednesday : 12:30PM to 1:15PM SAD 403 (Bidiyapati)"
};

const char * Thu[] =
{
  " Thursday : 8:45AM to 9:30AM SAD 403 (Bidiyapati)",
  " Thursday : 9:30AM to 10:15AM DMS 401 (Visato)",
  " Thursday : 10:15AM to 11:00AM NET 404 (Bidiyapati)",
  " Thursday : 11:00AM to 11:45AM Lunch Break :)",
  " Thursday : 11:45PM to 12:30PM NIELIT (Abhigyan)",
  " Thursday : 12:30PM to 1:15PM SWL 406 (Nepoleon)"
};

const char * Fri[] =
{
  " Friday : 8:45AM to 9:30AM NIELIT (Abhigyan)",
  " Friday : 9:30AM to 10:15AM NET 404 and SWL 405 (Bidiyapati)",
  " Friday : 10:15AM to 11:00AM NET 404 and SWL 405 (Bidiyapati)",
  " Friday : 11:00AM to 11:45AM Lunch Break :)",
  " Friday : 11:45PM to 12:30PM PPD 402 (Heulungdailie)",
  " Friday : 12:30PM to 1:15PM SAD 403 (Bidiyapati)"
};

const char * noClass = " BCA 4th Semester :)";
const char * Message;
int getDays;
int getMinutes;
int getHours;
int phase = 0;
int currentDay;
int day_phase;
void setup()
{
  Serial.begin(115200);
  dmd.setBrightness(255);//set brightness 0-255
  dmd.selectFont(Arial_Black_16);
  //Connecting to router
  WiFi.begin(SSID, PASSWORD);
  WiFi.setAutoReconnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.persistent(true);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    box.print(".");
    Serial.print("Connecting...");
  }
  Serial.print("Connected");
  timeClient.begin();
  dmd.begin();
}

void loop()
{
  timeClient.update();
  // //displaying the time table
  getHours = timeClient.getHours();
  getMinutes = timeClient.getMinutes();
  getDays = timeClient.getDay();
  Serial.print("\n" + getHours);
  Serial.print("\n" + getMinutes);
  Serial.print("\n" + getDays);
  condition();
}

void display()
{
  const char * next = Message;
  while(*next)
  {
    box.print(*next);
    Serial.print(*next);
    delay(300);
    next++;
  }
}

void condition()
{ 
  //This function processess the current Hour, minutes and day and sets the day_phase for the function call
  currentDay = getDays + 1;

  //Get current week
  switch(currentDay)
  {
    case 1:
      day_phase = 1;//Sunday
      break;
    case 2:
      day_phase = 2;
      break;
    case 3:
      day_phase = 3;
      break;
    case 4:
      day_phase = 4;
      break;
    case 5:
      day_phase = 5;
      break;
    case 6:
      day_phase = 6;
      break;
    case 7:
      day_phase = 7;
      break;
    default:
      break;
  }

  //Check for ongoing period : set phase
  currentPeriod();
  datas();
}

void currentPeriod()
{
  if(getHours == 8 && getMinutes >= 45)
    phase = 1;
  else if((getHours == 9 && getMinutes >= 30) || (getHours == 10 && getMinutes < 15))
    phase = 2;
  else if((getHours == 10 && getMinutes >= 15))
    phase = 3;
  else if(getHours == 11 && getMinutes >= 0 && getMinutes < 45)
    phase = 4;
  else if(getHours == 11 && getMinutes >= 45 || (getHours == 12 && getMinutes < 30))
    phase = 5;
  else if(getHours == 12 && getMinutes >= 30 || (getHours == 1 && getMinutes <= 15))
    phase = 6;
  else
    phase = 0;
}

void datas()
{
  if(day_phase == 2)
  {
    Monday();
  }else if(day_phase == 3)
  {
    Tuesday();
  }else if(day_phase == 4)
  {
    Wednesday();
  }else if(day_phase == 5)
  {
    Thursday();
  }else if(day_phase == 6)
  {
    Friday();
  }else
  {
    Message = noClass;
  }
  display();
}

void Monday()
{
  switch(phase)
    {
      case 1:
        Message = Mon[0];
        break;
      case 2:
        Message = Mon[1];
        break;
      case 3:
        Message = Mon[2];
        break;
      case 4:
        Message = Mon[3];
        break;
      case 5:
        Message = Mon[4];
      case 6:
        Message = Mon[5];
        break;
      default:
      Message = noClass;
        break;
    }
}

void Tuesday()
{
      switch(phase)
    {
      case 1:
        Message = Tue[0];
        break;
      case 2:
        Message = Tue[1];
        break;
      case 3:
        Message = Tue[2];
        break;
      case 4:
        Message = Tue[3];
        break;
      case 5:
        Message = Tue[4];
        break;
      case 6:
        Message = Tue[5];
      default:
      Message = noClass;
        break;
    }
}

void Wednesday()
{
  switch(phase)
    {
      case 1:
        Message = Wed[0];
        break;
      case 2:
        Message = Wed[1];
        break;
      case 3:
        Message = Wed[2];
        break;
      case 4:
        Message = Wed[3];
        break;
      case 5:
        Message = Wed[4];
        break;
      case 6:
        Message = Wed[5];
        break;
      default:
      Message = noClass;
        break;
    }
}
void Thursday()
{
  switch(phase)
    {
      case 1:
        Message = Thu[0];
        break;
      case 2:
        Message = Thu[1];
        break;
      case 3:
        Message = Thu[2];
        break;
      case 4:
        Message = Thu[3];
        break;
      case 5:
        Message = Thu[4];
        break;
      case 6:
        Message = Thu[5];
        break;
      default:
      Message = noClass;
        break;
    }
}

void Friday()
{
    switch(phase)
    {
      case 1:
        Message = Fri[0];
        break;
      case 2:
        Message = Fri[1];
        break;
      case 3:
        Message = Fri[2];
        break;
      case 4:
        Message = Fri[3];
        break;
      case 5:
        Message = Fri[4];
        break;
      case 6:
        Message = Fri[5];
        break;
      default:
      Message = noClass;
        break;
    }
}
}

void Wednesday()
{
  switch(phase)
    {
      case 1:
        Message = Wed[0];
        break;
      case 2:
        Message = Wed[1];
        break;
      case 3:
        Message = Wed[2];
        break;
      case 4:
        Message = Wed[3];
        break;
      case 5:
        Message = Wed[4];
        break;
      case 6:
        Message = Wed[5];
        break;
      default:
      Message = " BCA 4th Sem";
        break;
    }
}
void Thursday()
{
  switch(phase)
    {
      case 1:
        Message = Thu[0];
        break;
      case 2:
        Message = Thu[1];
        break;
      case 3:
        Message = Thu[2];
        break;
      case 4:
        Message = Thu[3];
        break;
      case 5:
        Message = Thu[4];
        break;
      case 6:
        Message = Thu[5];
        break;
      default:
      Message = " BCA 4th Sem";
        break;
    }
}

void Friday()
{
    switch(phase)
    {
      case 1:
        Message = Fri[0];
        break;
      case 2:
        Message = Fri[1];
        break;
      case 3:
        Message = Fri[2];
        break;
      case 4:
        Message = Fri[3];
        break;
      case 5:
        Message = Fri[4];
        break;
      case 6:
        Message = Fri[5];
        break;
      default:
      Message = " BCA 4th Sem";
        break;
    }
}
