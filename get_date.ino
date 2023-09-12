void get_date() {

  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  
  String formattedTime = timeClient.getFormattedTime();
 
    if( (timeClient.getHours() + 3) < 10  ) {currentHour = "0" + String(timeClient.getHours() + 3) ;}
   else{ currentHour = String(timeClient.getHours() + 3) ; }
   
    if ( (timeClient.getMinutes()) < 10 ) { currentMinute = "0" + String(timeClient.getMinutes()) ; }
    else {currentMinute = String(timeClient.getMinutes()) ; }

    if( (timeClient.getSeconds()) < 10 ) { currentSecond = "0" + String(timeClient.getSeconds()) ; }
    else { currentSecond = String(timeClient.getSeconds()) ; }

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

    if( (ptm->tm_mday) < 10 )  { monthDay = "0" + String(ptm->tm_mday) ;}
    else {monthDay = String(ptm->tm_mday) ;}

    if( (ptm->tm_mon+1) < 10 ) { currentMonth = "0" + String(ptm->tm_mon+1) ; }
    else { currentMonth = String(ptm->tm_mon+1) ; }
  
   currentYear = String(ptm->tm_year+1900) ;

}