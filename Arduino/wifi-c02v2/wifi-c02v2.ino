#include<stdlib.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
/*#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);</p><p>*/
#define SSID "Arduino"
#define PASS "edje1961"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=V7ACK4C0O5U9KPOS&field1="; //Thingspeak KEY = V7ACK4C0O5U9KPOS
//SoftwareSerial monitor(11, 12); // RX, TX
LiquidCrystal lcd(12,11,5,4,3,2);
SoftwareSerial wificom(7, 6); // RX, TX

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 10;  // variable to store the value coming from the sensor
int selectPin = 8;
String receivedData;

void setup()
{
  Serial.begin(115200);
  wificom.begin(115200); //300,1200,2400,4800,9600,19200,28800,38400,57600,74880,115200,230400,250000
  lcd.begin(16,2);
  lcd.print("WiFi Module :)");
  
//  sensors.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(selectPin, OUTPUT);
  digitalWrite(selectPin, HIGH);
  sendDebug("AT");
  digitalWrite(ledPin, HIGH);
  delay(5000);
  lcd.setCursor(0,1);
  lcd.print("Searching module");
  WiFiDebug();
  if(receivedData.substring(4,5)="OK"){
    digitalWrite(selectPin, LOW);
    Serial.println("RECEIVED: OK");
    digitalWrite(selectPin, HIGH);
    connectWiFi();
  }
  else
  {
    digitalWrite(selectPin, LOW);
    Serial.println("NO RESPONSE: WIFI MODULE NOT DETECTED");
    digitalWrite(selectPin, HIGH);
  }
  digitalWrite(ledPin, LOW);
}

void loop(){
  digitalWrite(ledPin, HIGH);
//  sensorValue = analogRead(sensorPin);
//  updateMeting(sensorValue);
  delay(100);
  digitalWrite(ledPin, LOW);
  if(Serial.available()){
    receivedData=Serial.readString();
    sendDebug(receivedData);
    WiFiDebug();
  }
}
/*  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  tempC = DallasTemperature::toFahrenheit(tempC);
  char buffer[10];
  String tempF = dtostrf(tempC, 4, 1, buffer);
  updateTemp(tempF);
  delay(60000);
} */

void updateMeting(int Package){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  digitalWrite(selectPin, HIGH);
  sendDebug(cmd);
  delay(2000);
  if(wificom.find("Error")){
    digitalWrite(selectPin, LOW);
    Serial.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += Package;
  cmd += "\r\n";
  wificom.print("AT+CIPSEND=");
  wificom.println(cmd.length());
  if(wificom.find(">")){
    digitalWrite(selectPin, LOW);
    Serial.print(">");
    Serial.print(cmd);
    digitalWrite(selectPin, HIGH);
    wificom.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(wificom.find("OK")){
    digitalWrite(selectPin, LOW);
    Serial.println("RECEIVED: OK");
  }else{
    digitalWrite(selectPin, LOW);
    Serial.println("RECEIVED: Error");
  }
}
/* OLD TEMPUPDATE FUNCTION
void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(Serial.find("Error")){
    monitor.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    monitor.print(">");
    monitor.print(cmd);
    Serial.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(Serial.find("OK")){
    monitor.println("RECEIVED: OK");
  }else{
    monitor.println("RECEIVED: Error");
  }
} */

void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  lcd.setCursor(0,0);
  lcd.print(cmd);
  wificom.println(cmd);
} 

void WiFiDebug(){
  while(wificom.available() == 0);
  receivedData = wificom.readString();
  Serial.println(receivedData);
  lcd.setCursor(0,1);
  lcd.print(receivedData); 
}

boolean connectWiFi()
{
  sendDebug("AT+CWMODE=1");
  WiFiDebug();  
/*  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";  
*/  
  String cmd="AT+CWJAP=";
  cmd+=SSID;
  cmd+=",";
  cmd+=PASS ;
  sendDebug(cmd);
  WiFiDebug();
  if(receivedData.substring(4,5)="OK"){
    Serial.println("RECEIVED: OK");
    return true;
  }else{
    Serial.println("RECEIVED: Error");
    return false;
  }
}
