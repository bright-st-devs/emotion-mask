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
typedef unsigned short unit16_t;

const unit16_t initialState[ROWS] = {
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000};

const unit16_t heart[ROWS] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1110000110000111,
    0b1100000000000011,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1100000000000011,
    0b1110000000000111,
    0b1111000000001111,
    0b1111100000011111,
    0b1111110000111111,
    0b1111111001111111,
    0b1111111111111111,
    0b1111111111111111};

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

// Arduino function that gets called at initialization
void setup()
{
    setupPins();
    writeAllRows(initialState);
}

// Arduino function that gets called repeatedly
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
void writeAllRows(const unit16_t leds[ROWS])
{
    for (int row = 0; row < ROWS; ++row)
    {
        digitalWrite(LEDARRAY_G, HIGH);

        writeRow(leds[row]);

        finalizeRow(row);
    }
}

void finalizeRow(int row)
{
    digitalWrite(LEDARRAY_LAT, HIGH);
    delayMicroseconds(1);

    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    scanLine(row);

    digitalWrite(LEDARRAY_G, LOW);
    delayMicroseconds(100);
}

void scanLine(int row)
{
    const byte *scanLine = scanLines[row];

    digitalWrite(LEDARRAY_D, scanLine[0]);
    digitalWrite(LEDARRAY_C, scanLine[1]);
    digitalWrite(LEDARRAY_B, scanLine[2]);
    digitalWrite(LEDARRAY_A, scanLine[3]);
}

void writeRow(const unit16_t leds)
{
    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    // Here we take the first byte and asign it to the second element in the array
    // then we take the second byte and asign it to the first element in the array.
    // We do this because the LED matrix takes the second byte first.
    byte halfRows[BYTES_PER_ROW];
    halfRows[1] = leds >> 8;
    halfRows[0] = leds;

    for (int halfRow = 0; halfRow < BYTES_PER_ROW; ++halfRow)
    {
        for (int led = 0; led < 8; ++led)
        {
            writeLed(halfRows[halfRow], led);
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