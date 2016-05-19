#include<stdlib.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
/*#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);</p><p> */
#define SSID "Edje-boven"
#define PASS "edje1961"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=V7ACK4C0O5U9KPOS&field1="; //Thingspeak KEY = V7ACK4C0O5U9KPOS
int errorCount = 0;
int i=0;

SoftwareSerial monitor(10, 11); // RX, TX
LiquidCrystal lcd(12,11,5,4,3,2);
void(* resetFunc) (void) = 0; 

void setup() //Instellen en starten van het programma
{
  lcd.begin(16,2);
  lcd.print("WiFi Module");
  lcd.setCursor(0,1);
  lcd.print("Zoeken...");
  Serial.begin(115200);
  pinMode(A0, INPUT);
  sendDebug("AT");
  while(Serial.available() == 0&&i<1000)
  {
    i++;
    delay(5);
    lcd.setCursor(15,1);
    lcd.print(i);
  }
  sendDebug("AT");
  if(Serial.find("OK")){
    lcdWrite("Gevonden     ",1);
    connectWiFi();
    delay(500);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("ER: WiFi-module"); 
    lcd.setCursor(0,1);
    lcd.print("not connected!):");
    delay(2000);
    resetFunc();   
  }
}

void loop(){
  String sensor=String(analogRead(A0));
  updateTemp(sensor);
    if(errorCount<1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("WiFi Connected:)"); 
      lcd.setCursor(0,1);
      lcd.print("Value: ");
      lcd.print(sensor);
    } 
    else{
      errorCheck();
    }
  delay(1000);
}

void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  while(Serial.available() == 0&&i<1000)
  {
    i++;
    delay(5);
  }
  if(Serial.find("Error")){
    lcdWrite("RECEIVED: Error",1);
    errorCount++;
    delay(500);
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    lcd.setCursor(0,1);
    lcd.print(">"); 
    lcd.print(cmd);
    Serial.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(Serial.find("OK")){
    lcd.setCursor(0,0);  
    lcd.println("Verzonden :)");
    errorCount=0;
    delay(500);
  }else{
    lcd.setCursor(0,0);
    lcd.println("Pakket Verloren ");
    errorCount++;
    delay(500);
  }
}

void sendDebug(String cmd){ //Verzend data en laat het zien indien fouten
  if(errorCount>=1)
  {
    lcdWrite(cmd,1);
  }
  Serial.println(cmd);
} 

void lcdWrite(String cmd, int regel){
   lcd.setCursor(0,regel);
   lcd.print(cmd);
   return;
}

void errorCheck(){
  if(errorCount>10){
      lcd.clear();
      lcdWrite("Not connected :(",0);
      lcdWrite("Resetting... ",1);
      delay(5000);
      resetFunc();
    }
    return;
}
 
boolean connectWiFi(){
  sendDebug("AT+CWMODE=1");
  i = 0;
  while(Serial.available() == 0&&i<1000)
  {
    lcdWrite("Koppelen...",1);
    i++;
    lcd.setCursor(15,1);
    lcd.print(i);
  }
  Serial.read();
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  i = 0;
  while(Serial.available() == 0&&i<1000)
  {
    lcdWrite("Verbinden...",1);
    i++;
    lcd.setCursor(15,1);
    lcd.print(i);
  }
  lcd.setCursor(0,1);
  if(Serial.find("OK")){
    return true;
  }else{
    lcdWrite("RECEIVED: Error",1);
    errorCount++;
    delay(1000);
    return false;
  }
}
