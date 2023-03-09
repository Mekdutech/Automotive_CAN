//essential libraries

#include <Serial_CAN_Module.h> 
#include <SoftwareSerial.h> 
#include <TinyGPSPlus.h> 
#include <TimeLib.h>


//Initialising a CAN interface
Serial_CAN can; 
#define can_tx 2 // tx of serial can module connect to D2 
#define can_rx 3 // rx of serial can module connect to D3

// variables to store CAN payload
unsigned long id = 0; 
unsigned char data[8];

int count = 0;

//Variables for JSON Payload
String jsonPayload, hum = "{ \"Battery\": ", temp = ", \"temperature\": ", lt = ", \"latitude\": ", lg = ", \"longitude\": ", sped = ", \"speed\": ", ip =", \"ipAddress\": ", ipAdd;

int humidity, temperature;
// long longitude, latitude, spd;
float longitude, latitude, spd;

//Initialising a GPS interface
TinyGPSPlus gps;
static const int RXPin = 6, TXPin = 7; 
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);
SoftwareSerial expresslink_serial(5,4);
 


// Variable to keep track of time
long last_time;

void setup() {
  // put your setup code here, to run once:
  jsonPayload.reserve(2000);
  ss.begin(GPSBaud);
  can.begin(can_tx, can_rx, 9600);
  Serial.begin(9600); 
  Serial.println("Initiating..");
  expresslink_serial.begin(115200);
  expressLinkConnect();
  last_time = millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  can.begin(can_tx, can_rx, 9600);
  while((last_time + 5000) > millis())
  {
   if(can.recv(&id, data)) 
    {

      humidity = data[0];
     // humidity = humidity * 0.33;
      temperature = data[1];
      // break;
     } 
  }

  last_time = millis();
  ss.begin(GPSBaud);


  while( (last_time + 5000) > millis() )
  {
    while (ss.available() > 0)
    if (gps.encode(ss.read()))
    if (gps.location.isValid() && gps.speed.isValid())  {
    

      longitude = gps.location.lng();
      latitude = gps.location.lat(); 

      spd = gps.speed.kmph();
      
      // break;

    }

  //  jsonPayload = "AT+SEND embeddedWorld/data " + hum + humidity + temp + temperature + lt + latitude + lg + longitude + sped + spd + "}";
  //  Serial.println(jsonPayload); 
  //  //SendCommand(jsonPayload);
  //  //jsonPayload = "";

  }
    ipAdd = "85.255.236.114";
    jsonPayload = "AT+SEND device/test-1/location/ipbased " + hum + humidity + temp + temperature + lt + latitude + lg + longitude + sped + spd + ip + "\"" + ipAdd + "\"" + "}";
    Serial.println(jsonPayload); 
    //SendCommand(jsonPayload);
    jsonPayload = "";
    last_time = millis();
    


  
}







String SendCommand(String command){
  expresslink_serial.print(command+"\n");
  String response = expresslink_serial.readString();
  return response;  
  }

  
bool expressLinkConnect(){
  String response, response2;
  bool finished = false;

  response = SendCommand("AT+CONNECT?");
  if(response.substring(0,4) == "OK 1"){
    Serial.println("Connected");
    }
  else {
    do {
      response2 = SendCommand("AT+CONNECT");
      }
    while (response2.substring(0,4) != "OK 1");
    Serial.println("Connected");
    }
    return true;
   
  }