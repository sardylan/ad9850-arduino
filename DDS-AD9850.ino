/*
 * 
 * Arduino sketch for DDS AD9850
 * Copyright (C) 2019  Luca Cireddu (sardylan@gmail.com)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#define PIN_DDS_RESET 8
#define PIN_DDS_DATA 9
#define PIN_DDS_FU_UD 10
#define PIN_DDS_W_CLK 11

#define PIN_LED LED_BUILTIN

#define DELAY_US 500

#define DDS_CLOCK 125000000

int incoming_byte;
char cmd[128];
char frequency[128];
int i = 0;

void setup() {
    pinMode(PIN_DDS_RESET, OUTPUT);
    pinMode(PIN_DDS_DATA, OUTPUT);
    pinMode(PIN_DDS_FU_UD, OUTPUT);
    pinMode(PIN_DDS_W_CLK, OUTPUT);
    pinMode(PIN_LED, OUTPUT);

    digitalWrite(PIN_DDS_RESET, LOW);
    digitalWrite(PIN_DDS_DATA, LOW);
    digitalWrite(PIN_DDS_FU_UD, LOW);
    digitalWrite(PIN_DDS_W_CLK, LOW);
    digitalWrite(PIN_LED, LOW);
    
    Serial.begin(115200, SERIAL_8N1);

    memset(cmd, '\0', sizeof(cmd));
    memset(frequency, '\0', sizeof(frequency));
}

void loop() {
    if(!Serial.available())
        return;

    incoming_byte = Serial.read();
    cmd[i] = incoming_byte;
    i++;
    
    if(incoming_byte == '\n' || incoming_byte == '\r') {
        digitalWrite(PIN_LED, HIGH);
        delay(125);
        parse_cmd();
        delay(125);
        digitalWrite(PIN_LED, LOW);

        memset(cmd, '\0', sizeof(cmd));
        i = 0;
    }
}

void parse_cmd() {
    if(cmd[0] == 'F') {
        for(int i = 1; i < strlen(cmd); i++)
            frequency[i - 1] = cmd[i];

        String freq_str = String(frequency);
        unsigned long freq_word = freq_str.toInt();
        unsigned long word = word_compute(freq_word);

        Serial.write("Frequency (Hz): ");
        Serial.write(String(freq_word).c_str());
        Serial.write(" - Number: ");
        Serial.write(String(word).c_str());
        Serial.write(" - Sending to DDS: ");

        send(word);

        Serial.write(" - Completed!\r\n");
    } else if(cmd[0] == 'R') {
        Serial.write("DDS reset... ");
        reset_tick();
        Serial.write(" - Completed!\r\n");
    }
}

void send(unsigned long word) {
    for(int i = 0; i < 32; i++) {
        int val = word & 0b1;
        Serial.write(val ? "1" : "0");
        send_bit(val);
        word >>= 1;
    }

    // Control bits
    send_bit(0);
    send_bit(0);

    // Power-down
    send_bit(0);

    // Phase
    send_bit(0);
    send_bit(0);
    send_bit(0);
    send_bit(0);
    send_bit(0);
    
    freq_update_tick();
}

void send_bit(int value) {
    if(value)
        digitalWrite(PIN_DDS_DATA, HIGH);
    else
        digitalWrite(PIN_DDS_DATA, LOW);

    clock_tick();
}

void clock_tick() {
    delayMicroseconds(DELAY_US);
    digitalWrite(PIN_DDS_W_CLK, HIGH);
    delayMicroseconds(DELAY_US);
    digitalWrite(PIN_DDS_W_CLK, LOW);
}
void reset_tick() {
    digitalWrite(PIN_DDS_RESET, HIGH);
    delayMicroseconds(DELAY_US);
    digitalWrite(PIN_DDS_RESET, LOW);
    delayMicroseconds(DELAY_US);
}

void freq_update_tick() {
    digitalWrite(PIN_DDS_FU_UD, HIGH);
    delayMicroseconds(DELAY_US);
    delayMicroseconds(DELAY_US);
    digitalWrite(PIN_DDS_FU_UD, LOW);
}

unsigned long word_compute(unsigned long frequency) {
    return (4294967296 * frequency) / DDS_CLOCK;
}
