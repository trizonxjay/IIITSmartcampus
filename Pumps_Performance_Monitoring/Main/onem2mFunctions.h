StaticJsonDocument<200> jsonBuffer;

String cse_ip = "http://onem2m.iiit.ac.in";
String cse_port = "443";
String server = "http://"+cse_ip+":"+cse_port+"/~/in-cse/in-name/";
void ledUpdate(){
  if(ledFlag==1){
    digitalWrite(LED_R,0);
    digitalWrite(LED_G,1);
  }
  else{
    digitalWrite(LED_R,1);
    digitalWrite(LED_G,0);
  }
}

// posting data (val) to onem2m server/ae/cnt
String createCI(String server, String ae, String cnt, String val)
{
  HTTPClient http;
  http.begin(server + ae + "/" + cnt + "/");
  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");
  http.addHeader("Connection", "close");
  int code = http.POST("{\"m2m:cin\": {\"cnf\": \"text/plain:0\",\"con\": "+ String(val) +"}}");
  http.end();
  Serial.println(code);
  
  // error handling
  if(code==-1){
    Serial.println("UNABLE TO CONNECT TO THE SERVER");
    ledFlag=0;
    ledUpdate();
  }
  delay(300);
}

// format the data as per onem2m standard and create node for new data (push the data)
void pushMyData(String pathh, String val){
  val = "\"" + val + "\"";
  pathh = "pr_4_esp32_1/"+pathh;
  createCI(server, "Team9_Pumps_performance_monitoring", pathh, val);  
}