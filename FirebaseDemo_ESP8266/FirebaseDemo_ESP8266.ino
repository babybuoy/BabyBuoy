#include <ESP8266WiFi.h> //TO BE CHANGED TO THE ESP32 HEADER FILE 
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "babybuoy.firebaseio.com"
#define FIREBASE_AUTH "mXS1eKO9f478AK9cqBRKrepxyoNAnxDxbbsTIFrl"
#define WIFI_SSID "NETGEAR65" //TO BE CHANGED TO YOUR WIFI NAME
#define WIFI_PASSWORD "pinkstreet495"// TO BE CHANGED TO YOUR WIFI PASSWORD

int Temp = 0; // Temerature reader

void setup() {
  Serial.begin(9600);
 pinMode(D0,OUTPUT);//LED
 pinMode(D2, INPUT);  //PIR
 pinMode(D1, OUTPUT); //Buzzer
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //If Firebase is connected Turn on LED
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  
  if(Firebase.success()){
    digitalWrite(0,HIGH);
  }
  else {
    digitalWrite(0,LOW);
  }

  //VARIABLE NAMES SET INSIDE OF FIREBASE
  Firebase.set("Temperature",0);
  Firebase.set("PIR_SENSOR",0);

    
}



int n = 0;

void loop() {
  // set value
  int reading = analogRead(Temp);


  //Read Temperature
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 + 10 ; // +10 is added to retrieve a more accurate reading (NEEDS ADJUSTMENTS)
  
  if (temperatureC>50){
  Firebase.set("Temperature",temperatureC);
  delay(5000);
  }


    //PIR and Buzzer Detection (NEEDS EDITING)
    int Value = digitalRead(2);
     if( Value== HIGH)
     {
       Serial.println("buzzer ON");
       digitalWrite(1,HIGH);
       Value = digitalRead(2);
     }
  
    if(Value==LOW){
       Serial.println("buzzer OFF");
        digitalWrite(1,LOW);
       Value = digitalRead(2);
     }
 
}


  

 
