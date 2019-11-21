#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <time.h>


#define FIREBASE_HOST "ayam-broiler-b62d1.firebaseio.com"
#define FIREBASE_AUTH "pbbAnowPluCwzQp6aWhE0Q9Gn9uaKafdNi4V0M45"
#define WIFI_SSID "WIFI.HOME"
#define WIFI_PASSWORD "11223344"
//#define WIFI_SSID "Bon Cabe"
//#define WIFI_PASSWORD "12345678"


#define lampuPin  2  //D4

//deklarasi waktu
const char* ntpServer      = "us.pool.ntp.org";
const long  gmtOffset_sec   = 3600*7;//GMT+7
const int   daylightOffset_sec  = 0;
char pukul[69], tanggal[69];

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   pinMode(lampuPin, OUTPUT);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {

      Serial.print("not connected");
      
      delay(500);
      
      }
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


//  setup waktu  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  unsigned timeout = 5000;
  unsigned start = millis();
  while (!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
  }

  
    
}

void loop() {
  // put your main code here, to run repeatedly:
  //  set waktu
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (pukul, 80, "%H", timeinfo);
 
  Serial.print("pukul: ");
  Serial.println(atol(pukul));
  int jam_mati = Firebase.getInt("/jam_mati");
  int jam_nyala = Firebase.getInt("/jam_nyala");
  Serial.print("mati: ");
  Serial.println(jam_mati);
  Serial.print("nyala: ");
  Serial.println(jam_nyala);
//  convert char to int
  int waktu = atol(pukul);

   Serial.println(waktu);
  if(waktu <=  jam_mati && waktu >= jam_nyala){
    digitalWrite(lampuPin, HIGH);
    Firebase.setString("status", "menyala");
    Serial.println("trueee");
    
    }
   else{
    digitalWrite(lampuPin, LOW);
    Firebase.setString("status", "mati");
    Serial.println("falseee");
    }

}
