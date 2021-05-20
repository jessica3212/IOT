

boolean connect_ESP()
{  
  Serial.println("CONNECTING");  
  ESP8266.print(ESP8266_CONNECT);
   
  if(read_until_ESP(keyword_OK,sizeof(keyword_OK),5000,0))
  {
    serial_dump_ESP();
    Serial.println("CONNECTED");
    ESP8266.print("AT+CIPSEND=0,");
    ESP8266.print(payload_size);
    serial_dump_ESP();
    ESP8266.print("\r\n\r\n");
    
    if(read_until_ESP(keyword_carrot,sizeof(keyword_carrot),5000,0))/
    {
      Serial.println("READY TO SEND");
      for(int i=0; i<payload_size; i++)//print the payload to the ESP
      {
        ESP8266.print(payload[i]);    
        Serial.print(payload[i]); 
      }
    
      if(read_until_ESP(keyword_sendok,sizeof(keyword_sendok),5000,0))//go wait for 'SEND OK'
      {
        Serial.println("SENT");//yay, it was sent
        return 1;//get out of here, data is about to fly out of the ESP
      }// got the SEND OK
    
      else// SEND OK
      Serial.println("FAILED TO SEND");  
    }//got the back carrot >
    
    else
    Serial.println("FAILED TO GET >");
    
  }//First OK
  
  else
  {
    Serial.println("FAILED TO CONNECT");//something went wrong
    setup_ESP();
  }
  
}
