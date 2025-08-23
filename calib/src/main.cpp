#include <PowerMonitor.h>

// Analog input pin connected to ZMCT103C current sensor output
#define ZMCT103C_PIN              A1

// Calibration factor for the ZMCT103C module
#define ZMCT103C_CAL              3.0

// Analog input pin for battery voltage divider module
#define BATTERY_VOLTAGE_PIN       A2

// Enter the measured battery voltage. Factor will be calculated automatically
#define BATTERY_KNOWNVOLTAGE      13.6

PowerMonitor pm;

void setup() {
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(ZMCT103C_PIN, INPUT);

    // Initialize serial
    Serial.begin(57600);

    // Initialize PowerMonitor (ZMCT103C)
    pm.initCurrentSensor(ZMCT103C_PIN, ZMCT103C_CAL);
}

void loop() {

    // Read current output from ZMCT103C
    pm.sampleAndCalculate();

    Serial.print("AC Mains Current: ");
    Serial.println(pm.Irms, 4);

    // Read voltage output from voltage module, without factor
    float volt = ((float) analogRead(BATTERY_VOLTAGE_PIN)) / 1024 * 5.0;

    // Estimate factor based on known voltage
    float fact = BATTERY_KNOWNVOLTAGE / volt;

    Serial.print("Calculated BATTERY_VOLTAGE_FACTOR: ");
    Serial.println(fact, 2);
    Serial.println("");

    delay(1000);
}
