/////////////////////////////////////////////////////////////////////
//This code uses the input from a remote and sends information to the Blynk app via wifi.
///////////////////////////////////////////////////////////////////////

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

// WiFi credentials:
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";//remote auth token

char* ssid[] = {"SSID1", "SSID2", "SSID3"};
char* pass[] = {"password1", "password2", "password3"};
const int nbKnownAP = 3;//number of elements in ssid[] and pass[]
char ssidChosen[20] = "\0";//increase the number of characters if longest ssid is longer than 20 characters
char passChosen[20] = "\0";//increase the number of characters if longest pass is longer than 20 characters

IPAddress ip (45,55,96,146); //This depends on where you are located. Ping http://blynk-cloud.com/ to know the ip address for you

//Global variables:
const int buttonPin[4] = {4, 5, 12, 13};
const int ledPin[3] = {16, 15, 14};
short int buttonState[4] = {0, 0, 0, 0};
short int switchState[4] = {0, 0, 0, 0};
bool isFirstConnect = true;

int timerId;

BlynkTimer timer;

WidgetBridge bridge0(V0);
WidgetBridge bridge1(V1);
WidgetBridge bridge2(V2);
WidgetBridge bridge3(V3);
WidgetBridge bridge4(V4);
WidgetBridge bridge5(V5);
WidgetBridge bridge6(V6);
WidgetBridge bridge7(V7);
WidgetBridge bridge8(V8);
WidgetBridge bridge9(V9);
WidgetBridge bridge10(V10);
WidgetBridge bridge11(V11);
WidgetBridge bridge12(V12);
WidgetBridge bridge13(V13);
WidgetBridge bridge14(V14);
WidgetBridge bridge15(V15);

BLYNK_CONNECTED()
{
  if (isFirstConnect)
  {
    timerId = timer.setInterval(200, checkInputs);//check input state every 200 milliseconds
  
    Blynk.syncAll();//Syncs all values from the app

    isFirstConnect = false;
  }

  bridge0.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge1.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge2.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge3.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge4.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge5.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge6.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge7.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge8.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge9.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge10.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge11.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge12.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge13.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge14.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  bridge15.setAuthToken("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void checkInputs()
{
  int i = 0;
  int thisPin = 0;
  
  for(i=0; i<4; i++)
  {    
    thisPin = digitalRead(buttonPin[i]);//Reads value of button i

    if(thisPin != buttonState[i])//if the button has just been pressed or released
    {
      buttonState[i] = thisPin;//Stores value of button in buttonState[i]. This is to send the button state only once when the button is pressed or relaeased
      if(buttonState[i])//if the button is pressed
      {
        switchState[i] = !switchState[i];//toggles the state of the switch
        sendState(i);//sends the new state of the switch to the linked relays      
      }
    }
  }
}



void sendState(int pin)
{
  switch(pin)
  {
    case 0://button 1
      bridge9.virtualWrite(V0, switchState[pin]);
      bridge12.virtualWrite(V0, switchState[pin]);
      bridge15.virtualWrite(V0, switchState[pin]);
      break;
    case 1://button 2
      bridge11.virtualWrite(V0, switchState[pin]);
      bridge14.virtualWrite(V0, switchState[pin]);
      break;
    case 2://button 3
      bridge2.virtualWrite(V0, switchState[pin]);
      break;
    case 3://"turn off everything" button
      bridge0.virtualWrite(V0, 0);
      bridge1.virtualWrite(V0, 0);
      bridge2.virtualWrite(V0, 0);
      bridge3.virtualWrite(V0, 0);
      bridge4.virtualWrite(V0, 0);
      bridge5.virtualWrite(V0, 0);
      bridge6.virtualWrite(V0, 0);
      bridge7.virtualWrite(V0, 0);
      bridge8.virtualWrite(V0, 0);
      bridge9.virtualWrite(V0, 0);
      bridge10.virtualWrite(V0, 0);
      bridge11.virtualWrite(V0, 0);
      bridge12.virtualWrite(V0, 0);
      bridge13.virtualWrite(V0, 0);
      bridge14.virtualWrite(V0, 0);
      bridge15.virtualWrite(V0, 0);
      break;
    default:
      break;
  }
  digitalWrite(ledPin[pin], !switchState[pin]);//updates feedback LED
}



void setup()
{
  int numSsid = 0;//Number of ssid found
  int i = 0;
  int j = 0;

  for(i=0; i<3; i++)
    pinMode(ledPin[i], OUTPUT);

  for(i=0; i<4; i++)
    pinMode(buttonPin[i], INPUT);

  Serial.begin(115200);

  do//loop as long as we don't have a known wifi network
  {
    Serial.println("** Scan Networks **");
    numSsid = WiFi.scanNetworks();

    // print the list of networks seen:
    Serial.print("number of available networks:");
    Serial.println(numSsid);

    // print the network number and name for each network found:
    for (i = 0; i < numSsid; i++)
    {
      Serial.print(i);
      Serial.print(") ");
      Serial.print(WiFi.SSID(i));
      Serial.print("\tSignal: ");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm");//*/
      for(j=0; j<nbKnownAP; j++)
      {
        if(WiFi.SSID(i) == ssid[j])//in case 1 of the networks found is a known network
        {
          Serial.print(ssid[j]);
          Serial.println(" is a known network!");//*/
          strcpy(ssidChosen, ssid[j]);
          strcpy(passChosen, pass[j]);
        }
      }
    }
  
    if(strlen(ssidChosen) == 0)//in case no known network was found
    {
      Serial.println("None of the known networks has been found");
      delay(2000);
    }
  } while(strlen(ssidChosen) == 0);//Loops back as long as no known wifi is found

  Blynk.begin(auth, ssidChosen, passChosen, ip, 8442);//Connects to the chosen wifi
  
  while(Blynk.connected() == false);//Waits to be connected
}

void loop()
{  
  Blynk.run();
  timer.run();
}