//import library
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

//inisialisasi LCD
LiquidCrystal_I2C lcd(0x27,16,2);

//inisialisasi pin input dan output
const int SoilSensor = A0;
const int pompa = 2 ;

int kelembabanRockwool;
float suhu,humi;
float kp = 0.0153;
float ki = 0.0085;
float kd = 0.034;
float Sp =60;
double priError = 0;
double toError = 0;
double error,plus_error, P,I,D, error_sebelum,tambah_error;

void printLCD(String out)
{
  lcd.clear();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Kelembaban: ");
  lcd.print(kelembabanRockwool);
  lcd.print("% ");
  lcd.setCursor(1,1);
  lcd.print(out);
  return;
}

void setup() {
  Serial.begin(9600);
  pinMode(pompa, OUTPUT);
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  }

void loop(){
  
  int hasilPembacaan = analogRead(SoilSensor);
  kelembabanRockwool = map(hasilPembacaan,1024,0,0,100); 
  
  if(Serial.available()){
    String baca = Serial.readString();
    Sp  = baca.toFloat();
    Serial.println("Nilai Setpoint: " + String(Sp));
  }

  error_sebelum  =  error;
  error = Sp - kelembabanRockwool;
  tambah_error = tambah_error + error;
  P  = kp * error;
  I  = ki * tambah_error;
  D  = kd *(error - error_sebelum);

  float PID = P + I + D;
  
  
  if(PID < 0.1){
    PID=0.1;
  }
  else if(PID > 5){
    PID=5;
  }
  
  Serial.print("Kelembaban");
  Serial.print(",");
  Serial.println("Delay");
  
  Serial.print(kelembabanRockwool);
  Serial.print(",");
  Serial.print(PID);
  Serial.println();
  
  if(kelembabanRockwool < Sp)
  { 
    printLCD("Mulai ");
    digitalWrite(pompa,HIGH);
  }
      
  delay(PID*100);
  digitalWrite(pompa, LOW);
  
  printLCD("Henti");
  delay((5-PID)*100);      
}
