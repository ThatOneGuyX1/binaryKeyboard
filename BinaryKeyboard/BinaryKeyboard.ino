#include <BleKeyboard.h>
#include <BLEDevice.h>  // Include BLE Device library

#define ZERO 7
#define ONE 8
#define ENTER 9
#define MODE 6

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

int DEVICE_STATE = DISCONNECTED;

void setup() {
    Serial.begin(115200);
    Serial.println("BOOTING");
    pinMode(LEDPIN_0, OUTPUT);  // Mode indicator LED setup

    DEVICE_STATE = CONNECTING;

    // Initialize BLE device before starting
    //BLEDevice::init("01Keyboard");
    BKBD.begin();

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

    updateModeLED();
}

void loop() {
    if (BKBD.isConnected()) {
        Serial.println("SENDING COMMAND");
        BKBD.print("I am A Robot");

    }

    delay(100);  // Adjust delay as needed
    Serial.println("WAITING");
}

void updateModeLED() {
    // Implement LED feedback for different states
}
