#include <SoftwareSerial.h>

 
#define DEBUG true
 
 
// Create the object to use Sotfware Serial library
// RX arduino is pin 2
// TX arduino is pin 3
// The TX esp8266 must be connected to pin 2 of the Arduino
// The esp8266 RX must be connected to pin 3 of the Arduino
 
SoftwareSerial esp8266(0,1);

// declare the LED pins rgb
int red =11;
int green =12;
int blue =13;
 


 

 
void setup()
{
  // a small delay in arduino
  // to prevent send commands before esp8266 to the start
  delay(500);
  // Set both serial for 9600
  Serial.begin(9600);
  // (In some cases the speed of its esp8266 may be deformed)
  esp8266.begin(9600);
  
  // declare the pins as output
  pinMode(red,OUTPUT);
  digitalWrite(red,LOW);
  
  pinMode(green,OUTPUT);
  digitalWrite(green,LOW);
  
  pinMode(blue,OUTPUT);
  digitalWrite(blue,LOW);
  
   delay(2000);
  // turn on the blue LED to inform the beginning of AT commands
  digitalWrite(blue,HIGH);
  
 
  
 
 
 
  // Send the AT commands
 
  // Reset the module
  sendData("AT+RST\r\n",2000,DEBUG);
  // Configure the access point and station (both)
  sendData("AT+CWMODE=1\r\n",1000,DEBUG);
  // connect to the router with a password
  // (this configuration must be done, because your router has a different name and my password)
  sendData("AT+CWJAP=\"Arduino\",\"edje1961\"r\n",10000,DEBUG);
  // Returns the IP to which it is connected and the IP Station
  sendData("AT+CIFSR\r\n",1000,DEBUG);
  // Enable multiple connections
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);
  // Enables the server port 80
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  
  // off the blue LED at the end of the commands
  digitalWrite(blue,LOW);
  // turn on the green LED to indicate that will be able to send commands
  // via browser
  digitalWrite(green,HIGH);
  
  delay(2000);
  // off green LED
  digitalWrite(green,LOW);
  
  
}
 
void loop()
{
 
 

 
  // check if the esp8266 sending this message and is available
  if(esp8266.available())
  {
 
   
    if(esp8266.find("+IPD,"))
    {
      
  
 
     delay(1000); 
      // retrieve only ASCII command
     int connectionId = esp8266.read()-48; 
          
      // moves the cursor to the word "pin =" 
     esp8266.find("pin="); 
     
      // store the first number .. Ex: If pin 13 will store 1 and then immediately multiply by 10 to calculate pin
     int pinNumber = (esp8266.read()-48)*10; 
      // store the second number and add to the first ... Ex: If the number is 13 the sum will be 10 + 3 which resulted in the pin which was sent by the Web page
     pinNumber += (esp8266.read()-48); 
      // store the third data to the one corresponding LED status "1" for on and "0" for OFF
     int statusLed =(esp8266.read()-48);
     
      // Write the satus with the digital method Write
      // Ex 121 is received
      // pin 12 will be connected
     //digitalWrite(12, 1);
     digitalWrite(pinNumber, statusLed);
  
     // Ends the command in esp8266
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; 
     closeCommand+="\r\n";
     
     // Terminate connection
     sendData(closeCommand,1000,DEBUG); 
     
     
    
    }

    
  }
}
 
 
// Method that sends commands to the esp8266
String sendData(String command, const int timeout, boolean debug)
{
    // response variable of esp8266
    String response = "";
   
    // Concatenate character by character received from the module esp8266
    esp8266.println(command);
   
    long int time = millis();
   
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
       
 
        //Concatena caracter por caractere recebido do modulo esp8266
        char c = esp8266.read();
        response+=c;
      }  
    }
   
  // response debug esp8266
    if(debug)
    {
      // Prints the esp8266 sent to arduino
      Serial.println("Arduino : " + response);
    }
   
    return response;
}
