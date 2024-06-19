#define BLYNK_TEMPLATE_ID "TMPL6J1OuKeRg"
#define BLYNK_TEMPLATE_NAME "IOT Ultrasonic Pest Detector"
#define BLYNK_AUTH_TOKEN "9r-nKnqH1j6nJQMz4Gm1oUxyodNPWw3P"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Ultrasonic.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";  
char pass[] = "";

BlynkTimer timer;

Ultrasonic ultrasonic(27, 17);
int ledPin = 12;
int buzzerPin = 14;
double jarak = 0;
int switchBuzz = 0;

void myTone(int pin){
  ledcAttachPin(pin, 0);
  ledcWriteNote(0, NOTE_F, 8);
}

void myNoTone(int pin){
  ledcDetachPin(pin);
}

BLYNK_WRITE(V1){
  int pin_value = param.asInt();
  Serial.println(pin_value);
  if (pin_value == 1){
      switchBuzz = 1;
  }else {
      switchBuzz = 0;
  }
}

void sendSensor(){
  delay(1000);
  double jarak = ultrasonic.read(CM);
  Blynk.virtualWrite(V0, jarak);

  Serial.print("Jarak : ");
  Serial.print(jarak);
  Serial.println("Cm");

  if(jarak < 200){
    if(switchBuzz == 1){
      digitalWrite(ledPin, HIGH);
      Serial.println("Hama terdeteksi pada radius 2 Meter!");
      Blynk.virtualWrite(V2, 1);
      myTone(buzzerPin);
      Blynk.logEvent("notification", "Peringatan Hama terdeteksi pada radius 2 Meter!");
    }else{
      digitalWrite(ledPin, LOW);
      Serial.println(("Alarm dan Notifikasi berhasil dimatikan."));
      Blynk.virtualWrite(V2, 0);
      myNoTone(buzzerPin);
    }
  }else{
    digitalWrite(ledPin, LOW);
      Serial.println(("Tidak ada Hama yang Terdeteksi."));
      Blynk.virtualWrite(V2, 0);
      myNoTone(buzzerPin);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  delay(1000);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2500L, sendSensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000); // this speeds up the simulation

  Blynk.run();
  sendSensor();
  timer.run();
}
