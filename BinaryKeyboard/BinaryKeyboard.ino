#include <BleKeyboard.h>
#include <BLEDevice.h>  // Include BLE Device library


hw_timer_t *timer = NULL;  // Define the timer for checking bluetooth in main loop
volatile bool conditionMet = false;  // Shared flag

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

int buttonCount =0;
int DEVICE_STATE = DISCONNECTED;

void setup() {
    Serial.begin(115200);
    Serial.println("BOOTING");
    pinMode(LEDPIN_0, OUTPUT);  // Mode indicator LED setup

    pinMode(ZERO, INPUT);
    pinMode(ONE, INPUT);
    

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
        switch(DEVICE_STATE) {
            case DISCONNECTED:
                Serial.println("DISCONNECTED");
                break;
            case CONNECTING:
                Serial.println("CONNECTING");
                break;
            case BINARY:
                Serial.println("BINARY");
                if (digitalRead(ZERO)) BKBD.write('0\n');
                if (digitalRead(ONE)) BKBD.write('1\n');
                break;
            case HEXDEC:
                Serial.println("HEXDEC");
                char buffer[8];
                while(buttonCount < 8){
                if (digitalRead(ZERO)){
                        buffer[buttonCount] = '0';  // Store the pressed button in the buffer
                        buttonCount++;
                } 
                if (digitalRead(ONE)){
                    buffer[buttonCount] = '1';  // Store the pressed button in the buffer
                    buttonCount++;
            } 
        }
                BKBD.print(buffer);  // Send the buffer as a stringq
                break;
            case ASCII:
                Serial.println("ASCII");
                break;
        }
        
        
        

    }

    delay(100);  // Adjust delay as needed
    Serial.println("WAITING");
}

void updateModeLED() {
    // Implement LED feedback for different states
}



