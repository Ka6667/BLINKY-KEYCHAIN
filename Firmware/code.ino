#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

// ---- OLED Display config ----
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

// ---- I2C pins (XIAO RP2040) ----
#define I2C_SDA 4
#define I2C_SCL 5

// ---- LED pins ----
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define LED5 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  // LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  // I2C on custom pins, then bring it up once
  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // halt here only, doesn't block the rest of setup
  }
  display.clearDisplay();
  display.display();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Automatically set RTC to compile time if it lost power
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void blink() {
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED3, HIGH);
  delay(500);
  digitalWrite(LED4, HIGH);   // was LED1 again — fixed
  delay(500);
  digitalWrite(LED5, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  delay(500);
  digitalWrite(LED2, LOW);
  delay(500);
  digitalWrite(LED3, LOW);
  delay(500);
  digitalWrite(LED4, LOW);
  delay(500);
  digitalWrite(LED5, LOW);
  delay(500);
}

void allblink() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  delay(200);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  delay(200);
}

// Renamed from random() — that name collides with Arduino's built-in RNG
void randomLED() {
  int pins[] = {LED1, LED2, LED3, LED4, LED5};
  int pin = pins[random(0, 5)]; // random(min, max) is exclusive of max, so 0..4 covers all 5
  digitalWrite(pin, HIGH);
  delay(200);
  digitalWrite(pin, LOW);
}

void screen() {
  DateTime now = rtc.now();

  display.clearDisplay();

  // Display Date
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 10);
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);

  // Display Time
  display.setTextSize(2);
  display.setCursor(15, 30);

  if (now.hour() < 10) display.print('0');
  display.print(now.hour(), DEC);
  display.print(':');

  if (now.minute() < 10) display.print('0');
  display.print(now.minute(), DEC);
  display.print(':');

  if (now.second() < 10) display.print('0');
  display.print(now.second(), DEC);

  display.display();
  delay(1000);
}

void loop() {
  screen();
  blink();
  allblink();
  randomLED();
}