#include <FS.h>  // this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ModbusMaster.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



const char*  ssid  = "ssid";
const char* password = "pw";
unsigned long wifiledtimer;



#define ledred 5
#define ledyellow 4
#define ledgreen1 0
#define ledgreen2 12
#define relay1 15
#define relay2 13
#define rst_btn 16
bool boolrelay1 = false ;
bool boolrelay2 = false ;




int t1 = 0;
int t2 = 0;
int t3 = 0;
int t4 = 0;
int t5 = 0;

int command = 0;



unsigned long kayit_timer = 0;

//////////////////////////////////////////////////////////////////////////////////////Setup Firebase////////////////////////////////////////////////
/* 2. Define the API Key */
#define API_KEY "API_KEY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


///////////////////////////////////////////////////////Setup Modbus  identities//////////////////////////////////////////////////////////////////////////////////////////

#define MAX485_RE_NEG 14
ModbusMaster node;



//////////////////////////////////////////////////////////////NTP DATE////////////////////////////////////////////////////////////////////////////////////////////////

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

//Month names
String months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

String currentHour;
String currentMinute;
String monthDay;
String currentMonth;
String currentYear;
String currentSecond;

unsigned long modbus_timer;

unsigned long sendprevmillis ;

int loopcounter;

int alarm_temp;

void setup() {

  Serial.begin(38400);

  pinMode(ledred, OUTPUT);
  pinMode(ledyellow, OUTPUT);
  pinMode(ledgreen1, OUTPUT);
  pinMode(ledgreen2, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(rst_btn, INPUT);

  digitalWrite(ledred, LOW);
  digitalWrite(ledyellow, LOW);
  digitalWrite(ledgreen1, LOW);
  digitalWrite(ledgreen2, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  setup_wifi();
  setup_firebase();
  setup_modbus();
}

void loop() {

  ///////////////////////////////////////////////////////////////////////////////DATABASEDE BİR DEĞERİ DİNLEYİP KOMUT ANLAMA//////////////////////////
  //10 komut yok // 20 Röle 1 Çektir // 21 Röle 1 Bırak // 

 if(loopcounter%3 == 0) {
    if (Firebase.getInt(fbdo, "/laser/command/c")) {
    if (fbdo.dataType() == "int") {
      command = fbdo.intData();

      if (command == 20) {
        digitalWrite(relay1, HIGH);
        boolrelay1 = true;
        Firebase.setBool(fbdo, "/laser/anlik/r1", boolrelay1);

      } else if (command == 21) {
        digitalWrite(relay1, LOW);
        boolrelay1 = false;
         Firebase.setBool(fbdo, "/laser/anlik/r1", boolrelay1);
      } else if (command == 22) {
       if(Firebase.getInt(fbdo, "/laser/command/alarm_temp")) {
         if(fbdo.dataType() == "int") {
           alarm_temp = fbdo.intData();
         }
       }
      } 
    }

    if(boolrelay1 == HIGH || boolrelay2 == HIGH) {digitalWrite(ledgreen1, HIGH);}
    else {digitalWrite(ledgreen1,LOW);}

    Firebase.setInt(fbdo, "/laser/command/c", 10);
  }
 }




////////////////////BQTEK İLE OKUNAN 5 ADET PT100 Sensörü RS485 COM3 ile HMI 'a Çekildi ve HMI Makrosunda $1,2,3,4,5'e yazıldı.COM3'ten 38600 Baudrate RTU 8 Bits 1 Bits None Protokolü ile buraya çekildi///////////////////////////
  digitalWrite(ledyellow,HIGH);

  modbus_timer = millis();

  uint8_t result;
  result = node.readHoldingRegisters(1, 6);
  t1 = node.getResponseBuffer(0);
  t2 = node.getResponseBuffer(1);
  t3 = node.getResponseBuffer(2);
  t4 = node.getResponseBuffer(3);
  t5 = node.getResponseBuffer(4);

    if(modbus_timer - millis() > 2000) {digitalWrite(ledred,HIGH);}
    else{digitalWrite(ledred,LOW);}

    if (Firebase.ready() && (millis() - sendprevmillis > 1000 || sendprevmillis == 0) ) { 
        sendprevmillis = millis() ;
        FirebaseJson json;
        json.set("t1", t1);
        json.set("t2", t2);
        json.set("t3", t3);
        json.set("t4", t4);
        json.set("t5", t5);
        Firebase.updateNode(fbdo, "/laser/anlik", json);
    }

//////////////////////////////////////////////////////////////////////////////////////Anlık DAtabase'e yazılan sıcaklık verileri 10 dk ya bir ayrı bir bloğa kaydediliyor////////////////////////////////////
  if (millis() - kayit_timer >= 600000) {
    if (t1 != 0 || t2 != 0 || t3 != 0 || t4 != 0 || t5 != 0) {
      //////////////////////////////////////////////10 DK DA BİR SICAKLIKLARI DATABASE'E KAYDETMEK İÇİN////////////////////////
      
      get_date();

      kayit_gir();

      kayit_timer = millis();
    }
  }

  digitalWrite(ledyellow,LOW);


  if(alarm_temp <= t1 || alarm_temp <= t2 || alarm_temp <= t3 || alarm_temp <= t4 || alarm_temp <= t5 ) {
    digitalWrite(relay2,HIGH);
    if(boolrelay2 == false) {
       boolrelay2 = true;
     Firebase.setBool(fbdo, "/laser/anlik/r2", boolrelay2);
    }
  }
  else {
    digitalWrite(relay2, LOW);
    if(boolrelay2 == true) {
     boolrelay2 =  false;
    Firebase.setBool(fbdo, "/laser/anlik/r2", boolrelay2);
    }
  }


  if(loopcounter == 100) {loopcounter==0;}
  loopcounter++;

}
