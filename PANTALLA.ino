#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary
#include <RTClib.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#define RXD2 16
#define TXD2 17
#include <Wire.h>
#include <Adafruit_ADS1015.h>

// mostrar data///
int muestras=0;
float suma=0;
float promedio=0;
long humedad=0;
 EasyNex myNex(Serial);

//reloj///
RTC_DS3231 rtc;
const int REFRESH_TIME = 1000;           // time to refresh the Nextion page every 100 ms
unsigned long refresh_timer = millis();  // timer for refreshing Nextion's page
//sd card
String trama_g="";
String fechaA="";
#define SD_CS 5
/// analogico
Adafruit_ADS1115 ads(0x48);
// typo de suelo
int type=0;
//tipo de sensor
int typeSen=0;
//time sleep
int timeS=0;
void setup() {
  
// Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);// serial1 pines 4 y 2
 
   myNex.begin(9600);
   if (rtc.begin()) {
    
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } 
  }

    SD.begin(SD_CS);  
   if (!SD.begin(SD_CS)) {
     Serial.println("falla sd");
    return;    // init failed
    
  }else{
    ads.begin();
    int co=10;
   
   while(co<=100){
    co=co+1;
    myNex.writeNum("j0.val", co);
    delay(50);
   }
   myNex.writeStr("page 1");
   
  fecha();
  }
   
  // put your setup code here, to run once:

}
void fecha(){
  DateTime now = rtc.now();
   char buf2[] = "MM-DD-YY,hh:mm";
   fechaA=now.toString(buf2);
   myNex.writeStr("t6.txt",fechaA);
}




void loop() {
  // put your main code here, to run repeatedly:
myNex.NextionListen(); // This function must be called repeatedly to response touch events
 if((millis()-refresh_timer) > REFRESH_TIME){ 
   fecha();
   // myNex.writeNum("j0.val",50);
  NivelBat();
  refresh_timer = millis(); 
 }
     
}

void trigger0(){
 humedad=random(1,100);
  muestras=muestras+1;
  suma=suma+humedad;
  promedio=suma/muestras;
  
  /* Create a button on Nextion
   * Write in the Touch Release Event of the button
   * this command:    printh 23 02 54 00
   * Every time the button is pressed, the trigger0() function will run
   * and the code inside will be executed once
   */

   //Serial.println("20");
    mostrarP1();
     myNex.writeNum("bt1.val",0);
  
}
bool mostrarP1(){
  String salH=String(humedad);
    String salM=String(muestras);
    String salP=String(promedio);
    myNex.writeStr("t1.txt", salH); // Set button b0 text to "ON"
    myNex.writeStr("t3.txt", salM);
    myNex.writeStr("t5.txt", salP);
    trama_g=fechaA+","+salM+","+salP+"\r\n";
}
void trigger1(){
  if (humedad!=0){
  muestras=muestras-1;
  suma=suma-humedad;
  promedio=suma/muestras;
  humedad=0;
  
  /* Create a button on Nextion
   * Write in the Touch Release Event of the button
   * this command:    printh 23 02 54 00
   * Every time the button is pressed, the trigger0() function will run
   * and the code inside will be executed once
   */

   mostrarP1();
  }else{
    myNex.writeStr("t1.txt", "--");
  }
  myNex.writeNum("bt0.val",0);
  
  
    
  
}

void trigger5(){
  DateTime now = rtc.now();
   char buf3[] = "MM-DD-YY";
   String fechaf=now.toString(buf3);
  String namef="/archivos/"+fechaf+".csv";
  guardar(namef,trama_g);
  //delay(50);
 
   
}
//configuracion
void iniconfig(){
 String fic="/config/config.txt";
   String salida=readFile(SD,"/config/config.txt");
   String date[5]={};
   extraer_carga(salida,date,",");
   type=date[0].toInt();//tipo de suelo
   
   typeSen=date[1].toInt();
  int pri=date[2].toInt();
    myNex.writeNum("n0.val",pri);

    timeS=date[3].toInt();
}
void trigger4(){
   myNex.writeStr("page 2");
   delay(100);
   String fic="/config/config.txt";
   String salida=readFile(SD,"/config/config.txt");
   String date[5]={};
   extraer_carga(salida,date,",");
   int pri=date[0].toInt();
   String types="error";
   switch(pri){
    case 0:
      types= myNex.readStr("bt0.txt");
      myNex.writeNum("bt0.val",1);
      break;
    case 1:
      types= myNex.readStr("bt1.txt");
      myNex.writeNum("bt1.val",1);
      break;
    case 2:
      types= myNex.readStr("bt2.txt");
      myNex.writeNum("bt2.val",1);
      break;
    default:
    // statements
    break;
   }
   myNex.writeStr("t0.txt", types );
  

   pri=date[1].toInt();
   switch(pri){
    case 0:
      myNex.writeStr("t1.txt", myNex.readStr("bt6.txt"));
      myNex.writeNum("bt6.val",1);
      break;
    case 1:
      myNex.writeStr("t1.txt", myNex.readStr("bt7.txt"));
      myNex.writeNum("bt7.val",1);
      break;
    case 2:
      myNex.writeStr("t1.txt", myNex.readStr("bt8.txt"));
      myNex.writeNum("bt8.val",1);
      break;
    default:
    // statements
    break;
   }
    pri=date[2].toInt();
    myNex.writeNum("n0.val",pri);

    pri=date[3].toInt();
   switch(pri){
    case 0:
      myNex.writeNum("bt10.val",1);
      break;
    case 1:
      
      myNex.writeNum("bt11.val",1);
      break;
    case 2:

      myNex.writeNum("bt12.val",1);
      break;
    default:
    // statements
    break;
   }
}
