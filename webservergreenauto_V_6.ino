// LM 35 CONNECTIONS 1. VCC 2. ANALOG 0 3. GND ..... 
// LCD 20X4 CONNECTIONS ANALOG 4 AND ANALOG 5 



#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
int tempminaddr = 0;
int tempmaxaddr = 1;
int waterminaddr = 3 ;
int watermaxaddr = 4;

LiquidCrystal_I2C lcd(0x27,20,4); 
byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x00, 0xCD, 0x3E };  //Set your Ethernet Shield's MAC address
byte ip[] = {  
  192,168,0,250 };    // Set your shield's  IP address
byte gateway[] = {  
  192,168,0,1 };   //if you need  a gateway IP
byte subnet[] = { 
  255,255,255,0 };    // Change  to your subnet address

EthernetServer  server(80);                // server port 

//////////////////////// declare variables////////////////////////////////////////////////
int     TEMPERATUREPin = A0;            // analog in 0 for TEMPERATURE sensor
int     tempPin = 0;                   // integer for the analog sensor
int     moisture = A1;                 //moisture sensor
int     voltage = A2;
int     volt=0;
int     moisturePin=0;
int     heaterpin = 5;               // Heater
int     FANpin = 6;                  // me - anolog pin 9 for fan control
int     PUMPPin = 7;                 // water PUMP pin
int     amper=0;
int     watt = 0;
String  readString = String(30);   // string for fetching data from address
boolean HEATON = false;             // LED1 status flag heater status
boolean PUMPON = false;             // ANTENNA status flag  
boolean ACON = false;             // led3 status AE

float tempC;
float tempA;
float tempF;

//////////////////////////// SETUP NOW//////////////////////////////////////////////////////////////////////

void setup()
{    
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  Ethernet.begin(mac, ip, gateway, subnet); // start Ethernet
  server.begin();  //test
  // Serial.print("server is at ");
  //  Serial.println(Ethernet.localIP());
  lcd.setCursor(3,0);
  lcd.print("SERVER IS AT");
  lcd.setCursor(2,3);
  lcd.print(Ethernet.localIP());
  pinMode(PUMPPin, OUTPUT); //Set pin 6 to output
  pinMode(heaterpin, OUTPUT); // set pin 7 to output
  pinMode(FANpin, OUTPUT); // set pin a3 to output
  byte client_ip[4];
  delay (1000);
//  digitalWrite(4, HIGH);
//digitalWrite(13, LOW); 
}


void loop(){
 ///////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////
  int waterlevelmax = 40 ;  
 EEPROM.write (watermaxaddr, waterlevelmax);
 int waterlevelmin = 30 ; 
 EEPROM.write (waterminaddr, waterlevelmin);
 int value = EEPROM.read (waterminaddr);
 int value1 = EEPROM.read (watermaxaddr);
 int value2 = EEPROM.read (tempminaddr);
 int value3 = EEPROM.read (tempmaxaddr);
 //////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////
 
    //// solar panels voltage 
 float volt = analogRead (voltage) * (6/1023.0); // solar pannel's voltage calculation if there will be 10 k and 1 k resistor between them
  amper= volt/1000*1000;
  volt= volt*11;
   watt= amper*amper*11000; // power of solar panel 
for (int i = 0 ; i < 100 ; i++ ){
    tempA = analogRead(TEMPERATUREPin);          
    tempA = (5.0 * tempA * 100.0)/1024.0;
    tempC= tempC + tempA; 
 delay (10); }      
    tempC= tempC/120;
    tempF =tempC*9/5+32;

   if (tempC < 18) {
     digitalWrite(heaterpin, HIGH);
      digitalWrite(FANpin,LOW);  
      HEATON = false;           
lcd.clear();
        lcd.setCursor(0,0);
    lcd.print("   HEATER STARTED");
    lcd.setCursor(0,1);
    lcd.print("TEMPERATURE :");
    lcd.print(tempC);
     lcd.print("");
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(0,2);
    lcd.print("SOLAR PANEL :");
    lcd.print(volt);
    lcd.print("V");
    lcd.setCursor(0,3);
    lcd.print("WATER LEVEL :");
  lcd.print("%");
   lcd.print(moisturePin);
 
    delay(1000);
    }
    else {
      if (tempC > 25) {
        digitalWrite(heaterpin, LOW);
        digitalWrite(FANpin,HIGH);  
        ACON = true;  
    lcd.setCursor(0,0);
    lcd.print("    A/C STARTED");
    lcd.setCursor(0,1);
    lcd.print("TEMPERATURE :");
    lcd.print(tempC);
     lcd.print("");
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(0,2);
    lcd.print("SOLAR PANEL :");
    lcd.print(volt);
    lcd.print("V");
    lcd.setCursor(0,3);
    lcd.print("WATER LEVEL :");
  lcd.print("%");
   lcd.print(moisturePin);
 
    delay(1000);  
    }
      else {
        digitalWrite(heaterpin, LOW);
        digitalWrite(FANpin,LOW);  
        HEATON = false;
      }
    }

  //////////////////////////////////////
  /////////////////////////////////////////
  moisturePin = analogRead(moisture);
  moisturePin = map(moisturePin, 0, 1023, 0, 100);

  if (moisturePin < 30) {
    digitalWrite(PUMPPin ,HIGH);  
    PUMPON = true;
    lcd.clear();
        lcd.setCursor(0,0);
    lcd.print("WATER PUMP ACTIVATED");
    lcd.setCursor(0,1);
    lcd.print("TEMPERATURE :");
    lcd.print(tempC);
     lcd.print("");
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(0,2);
    lcd.print("SOLAR PANEL :");
    lcd.print(volt);
    lcd.print("V");
    lcd.setCursor(0,3);
    lcd.print("WATER LEVEL :");
  lcd.print("%");
   lcd.print(moisturePin);
 
    delay(1000);
  }

  else {
    digitalWrite(PUMPPin ,LOW);  
    PUMPON = false;  

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  moisturePin = analogRead(moisture);
  moisturePin = map(moisturePin, 0, 1023, 0, 100);

    lcd.clear();
        lcd.setCursor(0,0);
    lcd.print("  GreenAutomation");
    lcd.setCursor(0,1);
    lcd.print("TEMPERATURE :");
    lcd.print(tempC);
     lcd.print("");
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(0,2);
    lcd.print("SOLAR PANEL :");
    lcd.print(volt);
    lcd.print("V");
    lcd.setCursor(0,3);
    lcd.print("WATER LEVEL :");
  lcd.print("%");
   lcd.print(moisturePin);
 
    delay(1000);
  //}

  EthernetClient client = server.available();  // Create a client connection
  if (client) {
    boolean current_line_is_blank = true; //new
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if we've gotten to the end of the line (received a newline character) and the line is blank, the http request has ended, send a reply

        if (readString.length() < 30) { //read char by char HTTP request
          readString.concat(c); 
        }                                 //store characters to string


        // HTML Code 
        if (c == '\n' && current_line_is_blank) {
          // HTML Code
          client.println(F("HTTP/1.1 200 OK")); //output HTML data starting with standard header
          client.println(F("Content-Type: text/html"));
          client.println();
          client.print(F("<body style=background-color:#FFF8DC>")); //set background to 003300 F0F0F0
          client.println(F("<META HTTP-EQUIV=REFRESH CONTENT=20 URL=>"));// Auto refresh webpage every 10 seconds
          client.println("<center>");
                   client.println("<HEAD>");
          client.println("<TITLE>GreenAutomation System</TITLE>");
          client.println("</HEAD>");
          client.println(F("<font color=’DarkBlue’><h1><b><i><u>WELCOME TO AUTOMATION SYSTEM </font></h1></b></i></u>"));//send first heading
          client.println(F("<img src='http://www.pacificenergysales.com/wp-content/uploads/2009/12/green-house.png' width=256 height=256>"));
         // client.println(F("<hr />")); 
        //  client.println(F("<hr />"));
         
          client.println(F("<font color=’#00008B’ size=’5′> "));
          client.println("</center>");

          if (c == '\n') { //if HTTP request has ended
            int Wa = readString.indexOf("W="); //here is a key component of where
            int Al = readString.indexOf("A="); //the status is being read by the arduino
            int Co = readString.indexOf("C="); //  ac string  
            int He = readString.indexOf("H=");

            ///////////////// Check if anything  should be lighted or turned on////////////////////////

            //////////////////////////////////////water PUMP ////////////////////////////////////////////        
            if (Wa > 1){
              if (readString.substring(Wa,(Wa+3)) == "W=1") { //Water PUMP has to be turned ON
                digitalWrite(PUMPPin, HIGH);                   // set the LED on
     for (int i = 60 ; i > 0 ; i-- ){          
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("!!!!WARNING!!!! ");
                lcd.setCursor(2,1);
                lcd.print("   WATER PUMP");
                lcd.setCursor(0,2);
                lcd.print("     ACTIVATED");
                lcd.setCursor(2,3);
                lcd.print("  ");
                lcd.print(i);
                lcd.print("  SECONDS");
   delay(1000);  
   }
                PUMPON = true;
                
              }

              if (readString.substring(Wa,(Wa+3)) == "W=0") 
              {                                               //Water PUMP  has to be turned OFF
                digitalWrite(PUMPPin, LOW);                   
                PUMPON = false;
for (int i = 10 ; i > 0 ; i-- ){  
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("!!!!WARNING!!!! ");
                lcd.setCursor(2,1);
                lcd.print("   WATER PUMP");
                lcd.setCursor(0,2);
                lcd.print("    DEACTIVATED");
                 lcd.setCursor(2,3);
                lcd.print("  ");
                lcd.print(i);
                lcd.print("  SECONDS");
                delay(1000);
}
                PUMPON = true;
                
              }
            }
            //////////////////////////////////////////////////////////heater/////////////////////////////////////////////
            if (He > 1){

              if (readString.substring(He,(He+3)) == "H=1") {
                digitalWrite(heaterpin, HIGH);                  
                digitalWrite(FANpin, LOW);
 for (int i = 10 ; i > 0 ; i-- ){          
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("!!!!WARNING!!!!");
                lcd.setCursor(2,1);
                lcd.print("     HEATER");
                lcd.setCursor(0,2);
                lcd.print("     ACTIVATED");                   
                lcd.setCursor(2,3);
                lcd.print("  ");
                lcd.print(i);
                lcd.print("   SECONDS");
                delay(1000);
              }           
                HEATON = true;
                
              }

              if (readString.substring(He,(He+3)) == "H=0") { 
                digitalWrite(heaterpin, LOW); 

                HEATON = false;
for (int i = 10 ; i > 0 ; i-- ){                 
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("!!!!WARNING!!!!");
                lcd.setCursor(2,1);
                lcd.print("     HEATER");
                lcd.setCursor(0,2);
                lcd.print("   DEACTIVATED");
                lcd.setCursor(2,3);
                lcd.print("  ");
                lcd.print(i);
                lcd.print("   SECONDS"); 
              delay(1000);  
}         
                HEATON = true;
                
              }
            }
            ////////////////////////////////////////////////////ac ///////////////////////////////
            if (Co > 1){

              if (readString.substring(Co,(Co+3)) == "C=1") { 
                digitalWrite(FANpin, HIGH); 
                digitalWrite(heaterpin, LOW);
for (int i = 10 ; i > 0 ; i-- ){                 
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("!!!!WARNING!!!!");
                lcd.setCursor(2,1);
                lcd.print("      A/C");
                lcd.setCursor(0,2);
                lcd.print("     ACTIVATED");
                lcd.setCursor(2,3);
                lcd.print("  ");
                lcd.print(i);
                lcd.print("  SECONDS");
                delay(1000);
              }
                ACON = true;
                        
              }

              if (readString.substring(Co,(Co+3)) == "C=0") {
                digitalWrite(FANpin, LOW); 

                ACON = false;
             for (int i = 10 ; i > 0 ; i-- ){ 
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("!!!!WARNING!!!!");
                lcd.setCursor(2,1);
                lcd.print("      A/C");
                lcd.setCursor(0,2);
                lcd.print("    DEACTIVATED");
                lcd.setCursor(2,3);
                lcd.print("  ");
                lcd.print(i);
                lcd.print("  SECONDS");
                delay(1000);
              }
              }
            }
            //////////////////////////////////////////////////////Close All/////////////////////////////////////////////////////////

            if (Al > 1){

              if (readString.substring(Al,(Al+3)) == "A=1") { 
                ////////////////////////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////////////
                /////// give error here becoz you can not open all of them at the same time //////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////DO NOTHING///////////////////////////////////////////////////////
                   for (int i = 10 ; i > 0 ; i-- ){ 
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print(" AUTOMATION SYSTEM");
                lcd.setCursor(2,1);
                lcd.print("     HAS BEEN");
                lcd.setCursor(0,2);
                lcd.print("      ACTIVATED");
                lcd.setCursor(2,3);
                lcd.print("   ");
                lcd.print(i);
                lcd.print("   SECONDS");
                delay(1000);
              }
              }

              if (readString.substring(Al,(Al+3)) == "A=0") {
                digitalWrite(PUMPPin, LOW); 
                digitalWrite(FANpin, LOW);
                digitalWrite(heaterpin, LOW);
                HEATON = false;
                ACON = false;
                PUMPON = false;
                
                 lcd.clear();
                lcd.setCursor(2,1);
                lcd.print("!!!!WARNING!!!!");
                lcd.setCursor(0,2);
                lcd.print(" AUTOMATION SYSTEM");
                lcd.setCursor(2,3);
                lcd.print("     ");
                lcd.setCursor(2,3);
                lcd.print("  DEACTIVATED"); 
                delay(10000);        

              }
            }     


            //////////////////////////////// detemine Plant-BotsTEMPERATURE/////////////////////// 
            client.println("<center><H4>");  
            client.print("SOLAR PANEL IS PRODUCING  : ");
            client.print(volt);     
            client.print("  VOLT");
             client.println("<br />"); 
            client.print("GreenAutomation Systems Temperature is : ");
            client.print(tempC);
            client.print("  CELCIUS ");
            client.println("<br />"); 
            client.print("GreenAutomation Systems Temperature is : ");
            client.print(tempF);
            client.println("  FAHRENHEIT"); 
client.println("</H4>");
            if (tempC < 18) {
              client.println( F("<H1><center><font color ='deepskyblue' >Plant-Bot IS NOW - *COLD* - </font></center></H1>") );
              digitalWrite(heaterpin, HIGH);
              digitalWrite(FANpin,LOW);  
              HEATON = true; 
              ACON = false;
              client.println(F("<center></center>"));
              client.println(F("<font color=’DarkBlue’> </font>"));
              client.println(F("<form  method=get name=LED>"));
              client.println(F("<button name=H value=0 type=submit style=height:100px;width:260px>PWR Off Heater</button>"));
              client.println(F("</form>"));     
            //  client.println(F("<hr />"));
            //  client.println(F("<hr />"));
            }
            else {
              if (tempC > 25) {
                client.println( F("<H1><center><font color ='red' >Plant-Bot IS NOW  - *HOT* -  </font></center></H1>") );
                digitalWrite(heaterpin, LOW);
                digitalWrite(FANpin,HIGH);  
                ACON = true;  
                client.println(F("<center></center>"));
                client.println(F("<font color=’green’> </font>"));
                client.println(F("<form  method=get name=COO>"));
                client.println(F("<button name=C value=0 type=submit style=height:100px;width:260px>PWR Off A/C</button>"));
                client.println(F("</form>"));     
            //    client.println(F("<hr />"));
            //    client.println(F("<hr />"));
              }
              else {
                digitalWrite(heaterpin, LOW);
                digitalWrite(FANpin,LOW);  
                HEATON = false;


              }
            }
            if (tempC>18 && tempC<25){
              client.println(F("<font color=’green’> </font>"));
              client.println(F("<form  method=get name=HEA>"));
              client.println(F("<button name=H value=1 type=submit style=height:100px;width:130px>PWR On Heater</button>"));
              client.println(F("<button name='C' value='1' type=submit style=height:100px;width:130px>PWR On A/C</button>"));
              client.println(F("</form>"));     
           //   client.println(F("<hr />"));
           //   client.println(F("<hr />"));
            }


            //////////////////////////////////////////////////////////////////////////////WATER LEVEL ///////////////////////////////////////////
            /////////////////////////////////////
            moisturePin = analogRead(moisture);
            moisturePin = map(moisturePin, 0, 1023, 0, 100);
  //         client.print("Water Level of Plant-Bot is : ");
 //           if(moisturePin <100&& moisturePin >0 ){
 //           client.print("DRYING");}
//            if (moisturePin <200 && moisturePin >100 ){
//            client.print("FAIR");
//            }
//        if(moisturePin <300 && moisturePin >200  ){
//          client.print("NORMAL");  
//        }
//    if (moisturePin >300 ){
//          client.print("SMOTHER"); 
//    }
            client.println("<br />"); 

            if ((moisturePin > 0) && (moisturePin < 30)){
              client.println( F("<H1><center><font color ='red' >Water Level of Plant-Bot is - *LESS* - </font></center>") );
             client.println (" %  ");
             client.print(moisturePin);
             client.println ("</H1>");
              digitalWrite(PUMPPin ,HIGH);  
              PUMPON = true;

              client.println(F("<center></center>"));
              client.println(F("<font color=’green’> </font>"));
              client.println("<center>");
              client.println(F("<form  method=get name=WAT>"));
              client.println(F("<button name=W value=0 type=submit style=height:100px;width:260px>PWR Off Water PUMP</button>"));
              client.println(F("</form>"));     
              client.println(F("<hr />"));
              client.println(F("<hr />"));
            }

            if ((moisturePin > 30)&& (moisturePin < 40)) {
              client.println( F("<H1><center><font color ='deepskyblue' >Water Level of Plant-Bot is  - *NORMAL* -  </font></center>") );
             client.println (" %  ");
                client.println(F("<font color=’green’> </font>"));
              client.println(F("<form  method=get name=WAT>"));
              client.println(F("<button name=W value=1 type=submit style=height:100px;width:130px>PWR On WATER PUMP</button>"));
              client.println(F("</form>"));     
             client.print(moisturePin);
             client.println ("</H1>") ;
              PUMPON = false;  
              client.println(F("<center></center>"));
              client.println(F("<font color=’green’> </font>"));
              client.println("<center>");
              client.println(F("<form  method=get name=WAT>"));
              client.println(F("<button name=W value=1 type=submit style=height:100px;width:260px>PWR On Water PUMP</button>"));
              client.println(F("</form>"));     
              client.println(F("<hr />"));
            }
            if ((moisturePin < 100)&& (moisturePin > 40)) {
              client.println( F("<H1><center><font color ='deepskyblue' >Water Level of Plant-Bot is  - *SWIMMING* -  </font></center></H1>") );
              client.println (" %  ");
             client.print(moisturePin);
             client.println ("</H1>")   ;           
             PUMPON = false;  
               client.println(F("<hr />"));
               
            }

            ///////////////////////////////////////////////////////// FOR ALL DEVICES////////////////////////////////////////////////////////////



            client.println(F("<font color=’green’> </font>"));
            client.println("<center>");
            client.println(F("<form  method=get name=ALL>"));
            client.println(F("<button name=A value=0 type=submit style=height:100px;width:200px>Deactivate GreenAutomation System</button>"));
            client.println(F("<button name=A value=1 type=submit style=height:100px;width:200px>Activate GreenAutomation System</button>"));
            client.println(F("</form>"));     
            client.println(F("<hr />"));
            client.println(F("<hr />"));


          }
          /////////////////////// DETERMINE WHAT STATUS TO PRINT///////////////////////////////
client.println("<H4>");
           client.print(F("<font color='#00008B'>Heater Status:      "));
          if (HEATON == true) {
            client.println(F("<font color='#FF0000' size=’5′>WORKING</font>")); //red

          }
          else {
            client.println(F("<font color='#0099FF' size=’5′>OFF</font>")); //blue

          }
          client.println(F("<br />"));

          //printing ant status

          client.print(F("<font color='#00008B'>Water Pump Status:      ")); //green
          if (PUMPON == true) {
            client.println(F("<font color='#FF0000' size=’5′>WORKING</font>")); //red

          }
          else {
            client.println(F("<font color='#0099FF' size=’5′>OFF")); //blue

          }
          client.println(F("<br />"));



          client.print(F("<font size=’5′ font color='#00008B'>A/C Status:      ")); 
          if (ACON == true) {
            client.println(F("<font color='#FF0000' size=’5′>WORKING")); 

          }
          else {
            client.println(F("<font color='#0099FF' size=’5′>OFF"));

          }
            client.println("<center></H4>");       
            
          //  client.println("<br />"); 
         //   client.print("SOLAR PANELS POWER IS : ");
         //   client.print(watt);
          //  client.print("MiliWatt");
         //   client.print(analogRead(voltage));
           //        client.println(F("<br />"));
//client.println(F("<hr />"));

          // WEB PAGE Footer
          //  client.println(F("<hr><center><a>WEBPAGE</a><br />"));
          //client.println(F("<hr><center><a href=http://xxxxxx.com>WEBPAGE</a><br />"));
       //   client.println(F("<hr><center><a><p><b><i><font color=darkcyan>This Page  Designed For Plant-Bot Automation System.</font></center></a><br /></b></i>"));


        
          client.println(F("</body></html>"));
          readString=""; 
//          delay(1000); 
          client.stop(); //stopping client
          client.flush();
        }
//        delay(100);
      }
//      delay(100);
    }
//    delay(100);
  }

//  delay( 100 );
}


