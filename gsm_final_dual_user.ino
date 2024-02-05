#include <SoftwareSerial.h>
#include <EEPROM.h>
byte value1;
const int relayPin=6;
const int sound=3;
const int led=2;
bool relayStatus=1;
bool ledStatus=1;
const String phone="+918116615379";
const String user2="+918768344713";
SoftwareSerial gsmSerial(10,11);

String responseData;
String senderNumber;

void setup(){
  responseData.reserve(200);
  phone.reserve(20);
  pinMode(relayPin,OUTPUT);
    pinMode(sound,OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(relayPin,HIGH);
  digitalWrite(led,LOW);
  Serial.begin(9600);
  delay(20000);
 // delay(20000);
 // delay(20000);
  gsmSerial.begin(9600);
  delay(100);
  gsmSerial.write("AT\r");// because of SIM800L autobounding mode
  delay(100);
  gsmSerial.write("AT+IPR=9600\r");
  delay(100);
  gsmSerial.write("AT+CMGF=1\r");
  delay(500);
  gsmSerial.write("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  Serial.print(gsmSerial.readString());
  gsmSerial.write("AT+CLIP=1\r\n");
  delay(100);
}

void loop(){
  if (gsmSerial.available()>0) {
    responseData = gsmSerial.readStringUntil('\n');
    Serial.println(responseData);
    parse();
    delay(1000);
  }
}

void parse(){
  if (responseData.indexOf("CLIP:")>0){
  senderNumber=responseData.substring(responseData.indexOf("+CLIP: ") +8,responseData.indexOf("+CLIP: ") +21); //PARSE CALLER ID 
  Serial.println("Caller number   :");
  Serial.println(senderNumber);
   if (senderNumber == phone){
   relayStatus=!relayStatus;
   ledStatus=!ledStatus;
    digitalWrite(relayPin,relayStatus);
    EEPROM.write('V1' ,relayStatus);
    digitalWrite(led,ledStatus);
    gsmSerial.write("ATA\r");  // disconnect call & then send SMS
     if (relayStatus==0)
    {
           digitalWrite(sound,HIGH);
      delay(1000);
      digitalWrite(sound,LOW);
    gsmSerial.print("AT+CMGS=\""+phone+"\"\r");
    delay(1000);
    gsmSerial.print("Relay status ON(USER1)");
    gsmSerial.print(relayStatus);
    delay(200);
    gsmSerial.write(0x1A); //  ctrl+z to send message
    delay(100);
    }
     if (relayStatus==1)
    {
    gsmSerial.print("AT+CMGS=\""+phone+"\"\r");
    delay(1000);
    gsmSerial.print("Relay status OFF(USER1)");
    gsmSerial.print(relayStatus);
    delay(200);
    gsmSerial.write(0x1A); //  ctrl+z to send message
    delay(100);
    }
    }
     if (senderNumber == user2){
    Serial.println("USER 2");
    relayStatus=!relayStatus;
    ledStatus=!ledStatus;
    digitalWrite(relayPin,relayStatus);
     EEPROM.write('V1',relayStatus );
    digitalWrite(led,ledStatus);
    gsmSerial.write("ATA\r");  // disconnect call & then send SMS
     if (relayStatus==0)
    {
      digitalWrite(sound,HIGH);
      delay(1000);
      digitalWrite(sound,LOW);
    gsmSerial.print("AT+CMGS=\""+user2+"\"\r");
    delay(1000);
    gsmSerial.print("Relay status ON(Mr. Rampada)");
    gsmSerial.print(relayStatus);
    delay(200);
    gsmSerial.write(0x1A); //  ctrl+z to send message
    delay(100);
    }
     if (relayStatus==1)
    {
    gsmSerial.print("AT+CMGS=\""+user2+"\"\r");
    delay(1000);
    gsmSerial.print("Relay status OFF(Mr. Rampada)");
    gsmSerial.print(relayStatus);
    delay(200);
    gsmSerial.write(0x1A); //  ctrl+z to send message
    delay(100);
     }
     }
    gsmSerial.write("ATH\r");
    delay(500);
  }
   else 
  {
   value1 = EEPROM.read('V1');
   digitalWrite(relayPin,value1);
   digitalWrite(led,!value1);   
  }
  }
