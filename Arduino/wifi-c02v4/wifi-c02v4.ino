#include<stdlib.h>
#include <LiquidCrystal.h>
#define SSID "Arduino"
#define PASS "edje1961"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=V7ACK4C0O5U9KPOS&field1="; //Thingspeak KEY = V7ACK4C0O5U9KPOS
int errorCount = 0;
int i=0;

LiquidCrystal lcd(12,11,5,4,3,2);
void(* resetFunc) (void) = 0; 

void setup()
{
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.print("WiFi Module :)");
  lcdWrite("Zoeken...        ",1);
  Serial.println("AT");
  while(Serial.available() == 0&&i<1000)
  {
    i++;
    delay(5);
    lcd.setCursor(15,1);
    lcd.print(i);
  }
    connectWiFi();
  if(Serial.find("OK")){
    lcdWrite("Gevonden       ",1);
    connectWiFi();
  } else {
    lcdWrite("Niet gevonden!  ",1);
    delay(2000);
    resetFunc();
  }
}

void loop(){
  String sensorValue=String(analogRead(A0));
  updateData(sensorValue);
  delay(5000);
}

void updateData(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
  }
}

void lcdWrite(String cmd, int regel)
{
  lcd.setCursor(0,regel); 
  lcd.print(cmd);   
}
 
boolean connectWiFi(){
  lcdWrite("Koppelen...        ",1);
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=";
  cmd+=SSID;
  cmd+=",";
  cmd+=PASS;
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    lcdWrite("Verbinden...    ",1);
    return true;
  }else{
    lcdWrite("Koppelfout      ",1);    
    return false;
  }
}
