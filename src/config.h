
// Analog input pin connected to ZMCT103C current sensor output
#define ZMCT103C_PIN              A1

// Calibration factor for the ZMCT103C module
#define ZMCT103C_CAL              3.0

// Current threshold in amperes, readings below this are considered noise/no charging
#define ZMCT103C_MINCURRENT       0.01

// Analog input pin for battery voltage divider module
#define BATTERY_VOLTAGE_PIN       A2

// Calibration factor for the battery voltage module
#define BATTERY_VOLTAGE_FACTOR    4.69

// Battery empty threshold in volts (represents 0% state of charge)
#define BATTERY_MINVOLTAGE        12.0

// Battery full threshold in volts (represents 100% state of charge) 
#define BATTERY_MAXVOLTAGE        14.4

// Expected battery discharge time in hours under typical load conditions
#define BATTERY_AVGRUNTIME        5.0

// Expected charging duration in hours to go from empty to full charge
#define BATTERY_AVGCHARGETIME     6.0

