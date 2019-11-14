void connect_to_WIFI(){
  int flag=1;
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(wifi_ssid, wifi_pwd);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED || WiFi.status()==WL_CONNECT_FAILED){
    delay(500);
    Serial.print(".");
  }
  if(WiFi.status()==WL_CONNECTED){
    Serial.println("Connected to the WiFi network");
    ledFlag=1;
    ledUpdate();
  }
  else{
    
  }
  Serial.println("Connected to the WiFi network");
}