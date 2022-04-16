#include <SoftwareSerial.h>

#include<Wire.h>
#include<ds3231.h>
struct ts t;
int timeCheck = 0;
int timePeriod = 1;



SoftwareSerial SIM900A(10,11);
char inchar;
byte q;
char state[5]="STATE";
String readString;

 void SendMessage()
{
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900A.println("AT+CMGS=\"+919847359440\"\r"); // Replace x with mobile number9207639389
  delay(1000);
  SIM900A.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
   SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


 void RecieveMessage()
{
  SIM900A.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1);
  if (SIM900A.available()>0)
  {
    q=SIM900A.read();
    Serial.write(q);
  }
 } 

void setup()
{
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);   // Setting the baud rate of Serial Monitor (Arduino)
  Wire.begin();  
  delay(100);

  DS32321_init(DS3231_CONTROL_INTCN);
  t.hour=12;
  t.min=30;
  t.sec=0;
  timeCheck=t.min;
  DS3231_set(t);
  delay(10000);
  Serial.println("GSM is ready");
  //----------------------------------------------------------------
    
  for(int j=0;j<5;j++){
    Serial.print(state[j]);
  }
  Serial.println("");
  delay(1000);
}


void loop()
{
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      Serial.println("sending mode");
      SendMessage();
      break;
    case 'r':
      Serial.println("recieving mode");
      RecieveMessage();
      break;
  }

  
//------------------------------------------------------------------



    while(SIM900A.available() > 0) {inchar = SIM900A.read(); readString+=inchar;delay(1);} ///can be a delay up to (10) so you can get a clear reading
    Serial.print(readString);  /// Declare a string    " String readString; "
    for (int i=0; i<200; i++){  /// Serch for the txt you sent up to (200) times it depends how long your ""readString" is
      if(readString.substring(i,i+6)=="STATUS"){ //// I am looking for the word RING sent from my phone
        SendMessage();
        break;
      }
    }
    readString = ""; 

    

//------------------------------------------------------------------



  if((t.min==timeCheck+timePeriod)){
   
    SIM900.println("AT+CMGF=1");                              //Sets the GSM Module in Text Mode
    delay(1000);                                                             // Delay of 1000 milli seconds or 1 second
    SIM900.println("AT+CMGS=\"+919847359440\"\r");            // Replace x with mobile number
    delay(1000);
    SIM900.println("SUPPLY IS ON");                           // The SMS text you want to send
    delay(100);
    SIM900.println((char)26);                                 // ASCII code of CTRL+Z
    delay(1000);
   
   
    if ((timeCheck+timePeriod>=60)){
    t.min=0;
    timeCheck=(t.min);
    }
    else
    timeCheck=t.min;
    }


   
  DS3231_get(&t);
//  Serial.print("\t Hour : ");
//  Serial.print(t.hour);
//  Serial.print(":");
//  Serial.print(t.min);
//  Serial.print(".");
//  Serial.println(t.sec);
//
//  delay(1000);



     
}
