#include <Wire.h>
#include <EEPROM.h>
#include <GravityTDS.h> // library TDS
#include <OneWire.h> // library OneWire
#include <DallasTemperature.h> // library DS18B20
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 5
#define TdsSensorPin 35  // GPIO35 on ESP32

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Skripsi Mustain");
    delay(3000);
    lcd.clear();

    //inisialisasi DS18B20
    sensors.begin();
    
    //inisialisasi TDS
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(3.3);
    gravityTds.setAdcRange(4096);
    gravityTds.begin();

    Serial.println("TDS Sensor Initialized");
}

void loop() {
    // Ambil data suhu
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);

    // Update nilai TDS
    gravityTds.setTemperature(temperature);
    gravityTds.update();
    tdsValue = gravityTds.getTdsValue();

    // Tampilkan ke Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(temperature);
    Serial.print(" C, TDS: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");

    // Tampilkan ke LCD
    lcd.setCursor(0, 0);
    lcd.print("Suhu: ");
    lcd.print(temperature, 1);
    lcd.print((char)223); // Simbol derajat
    lcd.print("C     "); // Spasi untuk bersihin sisa karakter

    lcd.setCursor(0, 1);
    lcd.print("TDS: ");
    lcd.print(tdsValue, 0);
    lcd.print(" ppm   ");

    delay(5000);
}
