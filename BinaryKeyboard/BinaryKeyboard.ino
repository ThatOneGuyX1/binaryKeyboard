#include <BleKeyboard.h>
#include <BLEDevice.h>  // Include BLE Device library


hw_timer_t* timer = NULL;            // Define the timer for checking bluetooth in main loop
volatile bool conditionMet = false;  // Shared flag

const int ZERO = 22;
const int ONE = 23;
const int ENTER = 9;
const int MODE = 36;


const char* hexTable[256] = {
  "0x00", "0x01", "0x02", "0x03", "0x04", "0x05", "0x06", "0x07",
  "0x08", "0x09", "0x0A", "0x0B", "0x0C", "0x0D", "0x0E", "0x0F",
  "0x10", "0x11", "0x12", "0x13", "0x14", "0x15", "0x16", "0x17",
  "0x18", "0x19", "0x1A", "0x1B", "0x1C", "0x1D", "0x1E", "0x1F",
  "0x20", "0x21", "0x22", "0x23", "0x24", "0x25", "0x26", "0x27",
  "0x28", "0x29", "0x2A", "0x2B", "0x2C", "0x2D", "0x2E", "0x2F",
  "0x30", "0x31", "0x32", "0x33", "0x34", "0x35", "0x36", "0x37",
  "0x38", "0x39", "0x3A", "0x3B", "0x3C", "0x3D", "0x3E", "0x3F",
  "0x40", "0x41", "0x42", "0x43", "0x44", "0x45", "0x46", "0x47",
  "0x48", "0x49", "0x4A", "0x4B", "0x4C", "0x4D", "0x4E", "0x4F",
  "0x50", "0x51", "0x52", "0x53", "0x54", "0x55", "0x56", "0x57",
  "0x58", "0x59", "0x5A", "0x5B", "0x5C", "0x5D", "0x5E", "0x5F",
  "0x60", "0x61", "0x62", "0x63", "0x64", "0x65", "0x66", "0x67",
  "0x68", "0x69", "0x6A", "0x6B", "0x6C", "0x6D", "0x6E", "0x6F",
  "0x70", "0x71", "0x72", "0x73", "0x74", "0x75", "0x76", "0x77",
  "0x78", "0x79", "0x7A", "0x7B", "0x7C", "0x7D", "0x7E", "0x7F",
  "0x80", "0x81", "0x82", "0x83", "0x84", "0x85", "0x86", "0x87",
  "0x88", "0x89", "0x8A", "0x8B", "0x8C", "0x8D", "0x8E", "0x8F",
  "0x90", "0x91", "0x92", "0x93", "0x94", "0x95", "0x96", "0x97",
  "0x98", "0x99", "0x9A", "0x9B", "0x9C", "0x9D", "0x9E", "0x9F",
  "0xA0", "0xA1", "0xA2", "0xA3", "0xA4", "0xA5", "0xA6", "0xA7",
  "0xA8", "0xA9", "0xAA", "0xAB", "0xAC", "0xAD", "0xAE", "0xAF",
  "0xB0", "0xB1", "0xB2", "0xB3", "0xB4", "0xB5", "0xB6", "0xB7",
  "0xB8", "0xB9", "0xBA", "0xBB", "0xBC", "0xBD", "0xBE", "0xBF",
  "0xC0", "0xC1", "0xC2", "0xC3", "0xC4", "0xC5", "0xC6", "0xC7",
  "0xC8", "0xC9", "0xCA", "0xCB", "0xCC", "0xCD", "0xCE", "0xCF",
  "0xD0", "0xD1", "0xD2", "0xD3", "0xD4", "0xD5", "0xD6", "0xD7",
  "0xD8", "0xD9", "0xDA", "0xDB", "0xDC", "0xDD", "0xDE", "0xDF",
  "0xE0", "0xE1", "0xE2", "0xE3", "0xE4", "0xE5", "0xE6", "0xE7",
  "0xE8", "0xE9", "0xEA", "0xEB", "0xEC", "0xED", "0xEE", "0xEF",
  "0xF0", "0xF1", "0xF2", "0xF3", "0xF4", "0xF5", "0xF6", "0xF7",
  "0xF8", "0xF9", "0xFA", "0xFB", "0xFC", "0xFD", "0xFE", "0xFF"
};

const char* asciiTable[128] = {
  "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
  "BS", "TAB", "LF", "VT", "FF", "CR", "SO", "SI",
  "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
  "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US",
  "SPACE", "!", "\"", "#", "$", "%", "&", "'",
  "(", ")", "*", "+", ",", "-", ".", "/",
  "0", "1", "2", "3", "4", "5", "6", "7",
  "8", "9", ":", ";", "<", "=", ">", "?",
  "@", "A", "B", "C", "D", "E", "F", "G",
  "H", "I", "J", "K", "L", "M", "N", "O",
  "P", "Q", "R", "S", "T", "U", "V", "W",
  "X", "Y", "Z", "[", "\\", "]", "^", "_",
  "`", "a", "b", "c", "d", "e", "f", "g",
  "h", "i", "j", "k", "l", "m", "n", "o",
  "p", "q", "r", "s", "t", "u", "v", "w",
  "x", "y", "z", "{", "|", "}", "~", "DEL"
};

const int LEDPIN_0 = 1;  // Corrected LED pin assignment

enum KEYBOARD_STATE {
  DISCONNECTED,
  CONNECTING,
  BINARY,
  HEXDEC,
  ASCII
};

BleKeyboard BKBD("01Keyboard", "XS Labs", 100);

//BleKeyboard BKBD; // Uses default name and manufacturer

int buttonCount = 0;
int DEVICE_STATE = DISCONNECTED;

void setup() {

  Serial.begin(115200);
  Serial.println("BOOTING");
  //pinMode(LEDPIN_0, OUTPUT);  // Mode indicator LED setup

  pinMode(ZERO, INPUT_PULLDOWN);
  pinMode(ONE, INPUT_PULLDOWN);
  pinMode(MODE, INPUT_PULLDOWN);




  DEVICE_STATE = BINARY;

  // Initialize BLE device before starting
  //BLEDevice::init("01Keyboard");
  BKBD.begin();
  delay(100);
  /*
    bool connectFlag = false;
    int count = 0;

    // Attempt connection 100 times
    while (!connectFlag && count < 100) {
        connectFlag = BKBD.isConnected();
        count++;

        if (count % 10 == 0) {
            Serial.printf("Connection has been attempted %d times.\n", count);
        }

        delay(10);  // To be replaced with non-blocking millis()
    }

    // Update device state based on connection result
    if (connectFlag) {
        Serial.println("DEVICE CONNECTED");
        DEVICE_STATE = BINARY;
    } else {
        Serial.println("DEVICE NOT CONNECTED");
        DEVICE_STATE = DISCONNECTED;
    }
    */

  updateModeLED();
}

void loop() {
  char buffer = 0;
  buttonCount = 0;
  if (digitalRead(MODE)) {

    switch (DEVICE_STATE) {
      case BINARY:
        DEVICE_STATE = HEXDEC;
        break;
      case HEXDEC:
        DEVICE_STATE = ASCII;
        break;
      default:
      case ASCII:
        DEVICE_STATE = BINARY;
        break;
    }
  }
  if (BKBD.isConnected()) {
    switch (DEVICE_STATE) {
      case DISCONNECTED:
        Serial.println("DISCONNECTED");
        break;
      case CONNECTING:
        Serial.println("CONNECTING");
        break;
      case BINARY:
        Serial.println("BINARY");
        if (digitalRead(ZERO)) BKBD.write(KEY_NUM_0);
        if (digitalRead(ONE)) BKBD.write(KEY_NUM_1);
        Serial.print("HIHHWEH");
        break;
      case HEXDEC:
        Serial.println("HEXDEC");

        while (buttonCount < 8) {
          Serial.println(buttonCount);
          Serial.println((int)buffer, BIN);
          delay(90);


          if (digitalRead(ZERO)) {
            buffer = (buffer << 1) | 0;  // Shift left and add 0
            buttonCount++;
          }

          if (digitalRead(ONE)) {
            buffer = (buffer << 1) | 1;  // Shift left and add 1
            buttonCount++;
          }
        }
          BKBD.print(hexTable[buffer]);  // Send the buffer as a stringq
          break;
          case ASCII:
            Serial.println("ASCII");
            while (buttonCount < 8) {
              Serial.println(buttonCount);
              Serial.println((int)buffer);
              delay(90);

              if (digitalRead(ZERO)) {
                buffer = (buffer << 1) | 0;  // Shift left and add 0
                buttonCount++;
              }

              if (digitalRead(ONE)) {
                buffer = (buffer << 1) | 1;  // Shift left and add 1
                buttonCount++;
              }
            }
              BKBD.print(hexTable[buffer]);  // Send the buffer as a stringq
              break;
            }
        }

        delay(100);  // Adjust delay as needed
        Serial.println("WAITING");
    }

    void updateModeLED() {
      // Implement LED feedback for different states
    }
