#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Wire.h" 
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C LCD(0x27,16,2);

RF24 radio(9, 53);

const int a = 4;
const int b = 6;

const char alphabet[] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  '[', ']', ' ', '°', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

const int mod = sizeof(alphabet);

byte address[][6] = {"0xB4B5B6B7F1", "2Node", "3Node", "4Node", "5Node", "6Node"};

int modInverse(int a, int m) {
  for (int x = 1; x < m; x++) {
    if ((a * x) % m == 1) {
      return x;
    }
  }
  return -1;
}

int getIndex(char c) {
  for (int i = 0; i < mod; i++) {
    if (alphabet[i] == c) {
      return i;
    }
  }
  return -1;
}

bool isInAlphabet(char c, String alphabet) {
  return alphabet.indexOf(c) >= 0;
}

int a_inv = modInverse(a, mod);

void setup() {
  Serial.begin(9600);
  LCD.init();
  LCD.backlight();
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(0, 15);
  radio.enableAckPayload();
  radio.setPayloadSize(32);
  radio.openReadingPipe(1, address[0]);
  radio.setChannel(0x59);
  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_250KBPS);
  radio.powerUp();
  radio.startListening();
  
}
 
void loop() {
  if (radio.available()) {
    byte receivedData[32];

    radio.read(receivedData, sizeof(receivedData));

    String receivedString = "";
    for (int i = 0; i < sizeof(receivedData); i++) {
      if (i == sizeof(receivedData)) break;
      receivedString += (char)receivedData[i];
    }

    String new_word = "";
    for (int i = 0; i < receivedString.length(); i++) {
    char currentChar = receivedString[i];
    int index = getIndex(currentChar);
    if (index != -1) {
      char decryptedChar = alphabet[(a_inv * (index - b + mod)) % mod];
      new_word += decryptedChar;
    } else {
      new_word += currentChar;
      }
    }
    LCD.setCursor(0, 0);
    for(int i = 0; i < 16; ++i){
      LCD.print(new_word[i]);
    }
    LCD.setCursor(0, 1);
    for(int i = 0; i < 32; ++i){
      LCD.print(new_word[i+16]);
    }
  }
  delay(1000);
}
