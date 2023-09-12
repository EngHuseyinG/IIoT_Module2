void setup_firebase() {

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.reconnectWiFi(true);

  // required for large file data, increase Rx size as needed.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  /*
  while ((auth.token.uid) == "") {
    mySerial.print('.');
    delay(1000);
  }
  */
  String uid;
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);
  delay(500);

if(Firebase.setInt(fbdo, "/laser/command/c", 10));
else{ ESP.restart();}


}