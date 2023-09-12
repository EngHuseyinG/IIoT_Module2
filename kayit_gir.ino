void kayit_gir() {




String uid = currentSecond + currentMinute + currentHour + monthDay + currentMonth + currentYear ;
String orderid = currentYear + currentMonth + monthDay + currentHour + currentMinute ;


if(Firebase.ready()) {

FirebaseJson json;
json.set("t1" , t1);
json.set("t2" , t2);
json.set("t3" , t3);
json.set("t4" , t4);
json.set("t5" , t5);
json.set("min" , currentMinute.toInt());
json.set("hour", currentHour.toInt());
json.set("day", monthDay.toInt());
json.set("month", currentMonth.toInt());
json.set("year",  currentYear.toInt());
json.set("orderid", orderid);
json.set("uid", uid);

Firebase.setJSON(fbdo, "/laser/kayitlar/" + monthDay + "/" + uid, json);

}

}