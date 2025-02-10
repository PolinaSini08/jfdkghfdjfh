#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

DHT dht(2, DHT11);

const uint64_t pipe = 0xF0F1F2F3F4LL;
RF24 radio(9, 10);

byte address[][6] = {"0xB4B5B6B7F1", "2Node", "3Node", "4Node", "5Node", "6Node"};

#define PIN_PHOTO_SENSOR A0
#define solPin A1

const int a = 4;
const int b = 6;

const char alphabet[] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  '[', ']', ' ', '°', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

const int mod = sizeof(alphabet);

int getIndex(char c) {
  for (int i = 0; i < sizeof(alphabet); i++) {
    if (alphabet[i] == c) {
      return i;
    }
  }
  return -1;
}

int maxLength = 32;

String addLeadingZeros(String str, int maxLength) {
    int zerosToAdd = maxLength - str.length();
    if (zerosToAdd <= 0) {
        return str;
    }
    String leadingZeros = "";
    for (int i = 0; i < zerosToAdd; i++) {
        leadingZeros += 'v';
    }
    return str + leadingZeros;
}

void setup() {
    dht.begin();
    pinMode(solPin, INPUT);
    Serial.begin(9600);
    radio.begin();
    radio.setAutoAck(1);
    radio.setRetries(0, 15);
    radio.enableAckPayload();
    radio.setPayloadSize(32);
    radio.openWritingPipe(address[0]);
    radio.setChannel(0x59);
    radio.setPALevel (RF24_PA_MAX);
    radio.setDataRate (RF24_250KBPS);
    radio.powerUp();
  radio.stopListening();
}

void loop() {

    String word_1 = "[Temperature] ";
    String word_2 = "[Light] ";
    String word_3 = "[Humidity] ";
    String word_4 = "C ";

    int h = analogRead(solPin);
    int t = dht.readTemperature();
    int l = analogRead(PIN_PHOTO_SENSOR);

    String t_izm = word_1 + String(t) + word_4;
    String l_izm;
    String h_izm;

    if (l < 342) l_izm = "low light. Not enought lighting ";
    else if (l <= 683) l_izm = "normal light";
    else l_izm = "high light";

    if (h < 512) h_izm = word_3 + "NOT OK";
    else h_izm = word_3 + "OK ";

    byte byteArray_1[32];
    byte byteArray_2[32];
    byte byteArray_3[32];
    byte byteArray_4[32];

    String new_word_4 = "";
    for (int i = 0; i < h_izm.length(); i++) {
      char currentChar = h_izm[i];
      int index = getIndex(currentChar);
      if (index != -1) {
        char encryptedChar = alphabet[(a * index + b) % mod];
        new_word_4 += encryptedChar;
      } else {
        new_word_4 += currentChar;
      }
    }
    String resultString_4 = addLeadingZeros(new_word_4, maxLength);
    resultString_4.getBytes(byteArray_4, resultString_4.length() + 1);
    radio.write(byteArray_4, sizeof(byteArray_4));
    delay(2000);

    String new_word_1 = "";
    for (int i = 0; i < t_izm.length(); i++) {
      char currentChar = t_izm[i];
      int index = getIndex(currentChar);
      if (index != -1) {
        char encryptedChar = alphabet[(a * index + b) % mod];
        new_word_1 += encryptedChar;
      } else {
        new_word_1 += currentChar;
      }
    }
    String resultString_1 = addLeadingZeros(new_word_1, maxLength);
    resultString_1.getBytes(byteArray_1, resultString_1.length() + 1);
    radio.write(byteArray_1, sizeof(byteArray_1));
    delay(2000);

    String new_word_2 = "";
    for (int i = 0; i < word_2.length(); i++) {
      char currentChar = word_2[i];
      int index = getIndex(currentChar);
      if (index != -1) {
        char encryptedChar = alphabet[(a * index + b) % mod];
        new_word_2 += encryptedChar;
      } else {
        new_word_2 += currentChar;
      }
    }
    String resultString_2 = addLeadingZeros(new_word_2, maxLength);
    resultString_2.getBytes(byteArray_2, resultString_2.length() + 1);
    radio.write(byteArray_2, sizeof(byteArray_2));
    delay(2000);

    String new_word_3 = "";
    for (int i = 0; i < l_izm.length(); i++) {
      char currentChar = l_izm[i];
      int index = getIndex(currentChar);
      if (index != -1) {
        char encryptedChar = alphabet[(a * index + b) % mod];
        new_word_3 += encryptedChar;
      } else {
        new_word_3 += currentChar;
      }
    }
    String resultString_3 = addLeadingZeros(new_word_3, maxLength);
    resultString_3.getBytes(byteArray_3, resultString_3.length() + 1);
    radio.write(byteArray_3, sizeof(byteArray_3));
    delay(2000);

}
