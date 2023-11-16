#include <ESP8266WiFi.h>

String apiWritekey = "BBFPI64DRCJ4ISKR"; // this is THINGSPEAK WRITEAPI key 
const char* ssid = "jane";
const char* password = "0987654321";
const char* host = "maker.ifttt.com";
const char* server = "api.thingspeak.com";
float resolution=3.3/1023; // 3.3 is the supply volt  & 1023 is max analog read value
int IRSensor = 4; // connect ir sensor to nodemcu digital pin 2

void setup() 
{
    pinMode (IRSensor, INPUT); // sensor pin INPUT
    Serial.begin(115200);
    delay(1000);
    connectWiFi();   
}

void connectWiFi()
{ 
  pinMode(2, OUTPUT);
  int i=0;
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to wifi......");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    digitalWrite(2,HIGH);
    delay(300);
    digitalWrite(2,LOW);
    delay(200);
        Serial.println(" - ");
    i++;
    if( i>10 )    
    { 
     return;
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("NodeMCU Local IP is : ");
    Serial.print((WiFi.localIP()));
  }
  
  digitalWrite(2,HIGH);
  delay(400);
  digitalWrite(2,LOW);

}

void loop()
{
      WiFiClient client; 
      const int httpPort = 80;  
      float temp = (analogRead(A0) * resolution) * 100;
      if (client.connect(server,80))
      {  
           String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(temp);
           tsData += "\r\n\r\n";
           Serial.println("Temperature: ");
           Serial.println(temp);
           Serial.println("uploaded to Thingspeak server....");
      }
      client.stop();
      if (!client.connect(host, httpPort)) 
      {  
         Serial.println("connection failed");  
         return;
      }
      if (digitalRead (IRSensor) == 0)
      {             
            String url = "trigger/IR_detection/json/with/key/hm8mMsZPLoc5Jhl5sqzOUYdJHiRAW2cgJnvh1syNcsO"; 
            Serial.print("Requesting URL: ");
            Serial.println(url); 
            Serial.println("An object Detected");                
            client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");                    
      }  
      else
      {
           Serial.println("Object Not Detected");            
      }
          
      delay(5000);  
  
    while((!(WiFi.status() == WL_CONNECTED)))
    {
      connectWiFi();
    } 
}
