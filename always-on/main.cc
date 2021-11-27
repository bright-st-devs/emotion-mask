#include <Arduino.h>

#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

#define ROWS 16
#define COLUMNS 16
#define BYTES_PER_ROW 2
#define LED_PINS 4

typedef unsigned char byte;

const byte initialState[ROWS][BYTES_PER_ROW] = {
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00}};

const byte heart[ROWS][BYTES_PER_ROW] = {
    {0b11111111, 0b11111111},
    {0b11111111, 0b11111111},
    {0b11111111, 0b11111111},
    {0b11100001, 0b10000111},
    {0b11000000, 0b00000011},
    {0b10000000, 0b00000001},
    {0b10000000, 0b00000001},
    {0b10000000, 0b00000001},
    {0b11000000, 0b00000011},
    {0b11100000, 0b00000111},
    {0b11110000, 0b00001111},
    {0b11111000, 0b00011111},
    {0b11111100, 0b00111111},
    {0b11111110, 0b01111111},
    {0b11111111, 0b11111111},
    {0b11111111, 0b11111111}};

const byte scanLines[ROWS][LED_PINS] = {
    {LOW, LOW, LOW, LOW},
    {LOW, LOW, LOW, HIGH},
    {LOW, LOW, HIGH, LOW},
    {LOW, LOW, HIGH, HIGH},
    {LOW, HIGH, LOW, LOW},
    {LOW, HIGH, LOW, HIGH},
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, HIGH, HIGH},
    {HIGH, LOW, LOW, LOW},
    {HIGH, LOW, LOW, HIGH},
    {HIGH, LOW, HIGH, LOW},
    {HIGH, LOW, HIGH, HIGH},
    {HIGH, HIGH, LOW, LOW},
    {HIGH, HIGH, LOW, HIGH},
    {HIGH, HIGH, HIGH, LOW},
    {HIGH, HIGH, HIGH, HIGH}};

void setup()
{
    setupPins();
    writeAllRows(initialState);
}

void loop()
{
    writeAllRows(heart);
}

void setupPins()
{
    pinMode(LEDARRAY_D, OUTPUT);
    pinMode(LEDARRAY_C, OUTPUT);
    pinMode(LEDARRAY_B, OUTPUT);
    pinMode(LEDARRAY_A, OUTPUT);
    pinMode(LEDARRAY_G, OUTPUT);
    pinMode(LEDARRAY_DI, OUTPUT);
    pinMode(LEDARRAY_CLK, OUTPUT);
    pinMode(LEDARRAY_LAT, OUTPUT);
}

// Each bit in leds represents 1 LED. 1 = on, 0 = off
void writeAllRows(const byte leds[ROWS][BYTES_PER_ROW])
{
    for (int row = 0; row < ROWS; ++row)
    {
        digitalWrite(LEDARRAY_G, HIGH);

        writeRow(leds[row]);

        // TODO make this a function with a name explaining what it does
        digitalWrite(LEDARRAY_LAT, HIGH);
        delayMicroseconds(1);

        digitalWrite(LEDARRAY_LAT, LOW);
        delayMicroseconds(1);

        scanLine(row);

        digitalWrite(LEDARRAY_G, LOW);
        delayMicroseconds(100);
    }
}

void scanLine(int row)
{
    const byte *scanLine = scanLines[row];

    digitalWrite(LEDARRAY_D, scanLine[0]);
    digitalWrite(LEDARRAY_C, scanLine[1]);
    digitalWrite(LEDARRAY_B, scanLine[2]);
    digitalWrite(LEDARRAY_A, scanLine[3]);
}

void writeRow(const byte *leds)
{
    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    for (int halfRow = BYTES_PER_ROW - 1; halfRow >= 0; --halfRow)
    {
        for (int led = 0; led < 8; ++led)
        {
            writeLed(leds[halfRow], led);
        }
    }
}

void writeLed(const byte halfRow, int led)
{
    digitalWrite(LEDARRAY_CLK, LOW);
    delayMicroseconds(1);

    if (halfRow & HIGH << led)
    {
        digitalWrite(LEDARRAY_DI, HIGH);
    }
    else
    {
        digitalWrite(LEDARRAY_DI, LOW);
    }

    // Prepare Matrix for next LED
    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, HIGH);
    delayMicroseconds(1);
}