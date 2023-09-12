void setup_wifi() {
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {

if(millis() - wifiledtimer > 0 and  millis() - wifiledtimer < 1000 ){digitalWrite(ledgreen2 ,HIGH);}
else if(millis() - wifiledtimer >= 1000 and  millis() - wifiledtimer <= 2000 ){digitalWrite(ledgreen2 , LOW);}
delay(500);
if(millis() - wifiledtimer > 2000) {wifiledtimer = millis();}

}

Serial.print(WiFi.localIP());

digitalWrite(ledgreen2, HIGH);



}