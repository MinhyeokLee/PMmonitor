#include <SoftwareSerial.h>

SoftwareSerial pms7003(2, 3);

unsigned long previousMillis = 0;  
const long interval = 1000;  

void setup() {
    Serial.begin(9600);
    pms7003.begin(9600);
}

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;  

        int pm10 = 0, pm25 = 0;
        if (readPMS7003(pm10, pm25)) {  
            Serial.print(pm10);
            Serial.print("-");
            Serial.print(pm25);
            Serial.println();
        }
    }
}

bool readPMS7003(int &pm10, int &pm25) {
    while (pms7003.available() > 32) {  
        pms7003.read();  // 불필요한 데이터 삭제
    }

    if (pms7003.available() >= 32) {
        uint8_t buffer[32];
        for (int i = 0; i < 32; i++) {
            buffer[i] = pms7003.read();
        }

        if (buffer[0] == 0x42 && buffer[1] == 0x4D) {
            pm10 = (buffer[10] << 8) + buffer[11];
            pm25 = (buffer[12] << 8) + buffer[13];
            return true;
        }
    }
    return false;
}
