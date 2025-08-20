#include <HIDPowerDevice.h>
#include <PowerMonitor.h>
#include "kalman.h"

// Analog pin for the ZMCT103C module
#define ZMCT103C_PIN              A1

// Calibration factor for the ZMCT103C module
#define ZMCT103C_CAL              3.0

// Minimum current threshold needed to qualify as charging
// 0.01 * ~120v = 1.2watts. We need to be drawing at least
// 1.2w from the outlet to qualify as charging
#define ZMCT103C_MINCURRENT       0.01

// Analog pin for the battery voltage module
#define BATTERY_VOLTAGE_PIN       A2

// Calibration factor for the battery voltage module
#define BATTERY_VOLTAGE_FACTOR    4.69

// Minimum voltage that represents 0% charge state
#define BATTERY_MINVOLTAGE        12.0

// Maximum voltage that the battery is set for a full charge
#define BATTERY_MAXVOLTAGE        14.4

// In hours, the average runtime for the battery
#define BATTERY_AVGRUNTIME        5.0

// In hours, the average time it takes to reach full from empty
#define BATTERY_AVGCHARGETIME     6.0

#define DEVICE_CHEMISTRY          "LiFePO4"
#define DEVICE_VENDOR             "ArduinoUPS"
#define DEVICE_SERIAL             "001"

int iIntTimer = 0;
uint16_t iRemainTimeLimit = 600;
uint16_t iRunTimeToEmpty = 0;
uint16_t iPrevRunTimeToEmpty = 0;
int16_t  iDelayBe4ShutDown = -1;
byte iFullChargeCapacity = 100;
byte iRemaining = 100;
byte iPrevRemaining = 0;
byte min_update_interval = 26;

unsigned long v_timer = 0;

PresentStatus iPresentStatus = {}, iPreviousStatus = {};
KalmanFilter kf = KalmanFilter();
PowerMonitor pm;


void setup() {
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(ZMCT103C_PIN, INPUT);

    // Initialize serial
    Serial.begin(57600);

    // Initialize PowerMonitor (ZMCT103C)
    pm.initCurrentSensor(ZMCT103C_PIN, ZMCT103C_CAL);

    // Initialize KalmanFilter
    kf.init(BATTERY_MAXVOLTAGE, BATTERY_MINVOLTAGE, BATTERY_AVGRUNTIME);

    // Initialize PowerDevice

    const char STRING_DEVICECHEMISTRY[] PROGMEM = DEVICE_CHEMISTRY;
    const char STRING_OEMVENDOR[] PROGMEM = DEVICE_VENDOR;
    const char STRING_SERIAL[] PROGMEM = DEVICE_SERIAL; 

    const byte bDeviceChemistry = IDEVICECHEMISTRY;
    const byte bOEMVendor = IOEMVENDOR;
    byte bRechargable = 1;
    byte bCapacityMode = 2;  // units are in %%
    uint16_t year = 2025, month = 8, day = 8;
    uint16_t iManufacturerDate = (year - 1980)*512 + month*32 + day;
    const byte iDesignCapacity = 100;
    byte iWarnCapacityLimit = 10; // warning at 10% 
    byte iRemnCapacityLimit = 5; // low at 5% 
    const byte bCapacityGranularity1 = 1;
    const byte bCapacityGranularity2 = 1;
    byte iAudibleAlarmCtrl = 1; // 1 - Disabled, 2 - Enabled, 3 - Muted
    const uint16_t iConfigVoltage = BATTERY_MAXVOLTAGE * 100;
    uint16_t iVoltage = BATTERY_MAXVOLTAGE * 100;
    uint16_t iAvgTimeToFull = BATTERY_AVGCHARGETIME * 60 * 60;
    uint16_t iAvgTimeToEmpty = BATTERY_AVGRUNTIME * 60 * 60;
    int16_t  iDelayBe4Reboot = -1;

    PowerDevice.begin();
    PowerDevice.setSerial(STRING_SERIAL); // Serial No is set in a special way as it forms Arduino port name
    PowerDevice.setOutput(Serial); // Used for debugging purposes. 
    PowerDevice.setFeature(HID_PD_PRESENTSTATUS, &iPresentStatus, sizeof(iPresentStatus));
    PowerDevice.setFeature(HID_PD_RUNTIMETOEMPTY, &iRunTimeToEmpty, sizeof(iRunTimeToEmpty));
    PowerDevice.setFeature(HID_PD_AVERAGETIME2FULL, &iAvgTimeToFull, sizeof(iAvgTimeToFull));
    PowerDevice.setFeature(HID_PD_AVERAGETIME2EMPTY, &iAvgTimeToEmpty, sizeof(iAvgTimeToEmpty));
    PowerDevice.setFeature(HID_PD_REMAINTIMELIMIT, &iRemainTimeLimit, sizeof(iRemainTimeLimit));
    PowerDevice.setFeature(HID_PD_DELAYBE4REBOOT, &iDelayBe4Reboot, sizeof(iDelayBe4Reboot));
    PowerDevice.setFeature(HID_PD_DELAYBE4SHUTDOWN, &iDelayBe4ShutDown, sizeof(iDelayBe4ShutDown));
    PowerDevice.setFeature(HID_PD_RECHARGEABLE, &bRechargable, sizeof(bRechargable));
    PowerDevice.setFeature(HID_PD_CAPACITYMODE, &bCapacityMode, sizeof(bCapacityMode));
    PowerDevice.setFeature(HID_PD_CONFIGVOLTAGE, &iConfigVoltage, sizeof(iConfigVoltage));
    PowerDevice.setFeature(HID_PD_VOLTAGE, &iVoltage, sizeof(iVoltage));
    PowerDevice.setStringFeature(HID_PD_IDEVICECHEMISTRY, &bDeviceChemistry, STRING_DEVICECHEMISTRY);
    PowerDevice.setStringFeature(HID_PD_IOEMINFORMATION, &bOEMVendor, STRING_OEMVENDOR);
    PowerDevice.setFeature(HID_PD_AUDIBLEALARMCTRL, &iAudibleAlarmCtrl, sizeof(iAudibleAlarmCtrl));
    PowerDevice.setFeature(HID_PD_DESIGNCAPACITY, &iDesignCapacity, sizeof(iDesignCapacity));
    PowerDevice.setFeature(HID_PD_FULLCHRGECAPACITY, &iFullChargeCapacity, sizeof(iFullChargeCapacity));
    PowerDevice.setFeature(HID_PD_REMAININGCAPACITY, &iRemaining, sizeof(iRemaining));
    PowerDevice.setFeature(HID_PD_WARNCAPACITYLIMIT, &iWarnCapacityLimit, sizeof(iWarnCapacityLimit));
    PowerDevice.setFeature(HID_PD_REMNCAPACITYLIMIT, &iRemnCapacityLimit, sizeof(iRemnCapacityLimit));
    PowerDevice.setFeature(HID_PD_CPCTYGRANULARITY1, &bCapacityGranularity1, sizeof(bCapacityGranularity1));
    PowerDevice.setFeature(HID_PD_CPCTYGRANULARITY2, &bCapacityGranularity2, sizeof(bCapacityGranularity2));
    PowerDevice.setFeature(HID_PD_MANUFACTUREDATE, &iManufacturerDate, sizeof(iManufacturerDate));
}

void loop() {

    // Read current output from ZMCT103C
    pm.sampleAndCalculate();
    
    // Assume that we're charging if we're seeing a current draw above MIN_CURRENT
    bool is_charging = pm.Irms >= ZMCT103C_MINCURRENT;

    Serial.print("AC Mains Current: ");
    Serial.println(pm.Irms, 4);

    // Read voltage output from voltage module
    float volt = ((float) analogRead(BATTERY_VOLTAGE_PIN)) / 1024 * 5.0 * BATTERY_VOLTAGE_FACTOR;

    Serial.print("DC Battery Voltage: ");
    Serial.println(volt, 4);

    // Time since last update (usually ~1sec, in hours)
    double dt = (millis() - v_timer) / 1000 / 60 / 60;
    kf.predict(dt);

    // Update filter with new measurement, total elapsed time (in hours)
    kf.update(volt, millis() / 1000 / 60 / 60);
    v_timer = millis();

    Serial.print("Uncertainty: ");
    Serial.println(kf.get_uncertainty(), 2);

    // Voltage correlation for state of charge (linear model)
    iRemaining = ((volt - BATTERY_MINVOLTAGE) / (BATTERY_MAXVOLTAGE - BATTERY_MINVOLTAGE)) * 100.0;
    if (iRemaining > 100.0 || volt > BATTERY_MAXVOLTAGE) {
        iRemaining = 100.0;
    }
    if (iRemaining < 0.0 || volt < BATTERY_MINVOLTAGE) {
        iRemaining = 0.0;
    }

    Serial.print("SoC (%): ");
    Serial.println(iRemaining, DEC);

    // Runtime to empty, convert from hours to seconds
    iRunTimeToEmpty = kf.get_remaining_time() * 60 * 60;

    Serial.print("Runtime: ");
    Serial.println(iRunTimeToEmpty, 2);
    Serial.println("");


    iPresentStatus.BatteryPresent = 1;
    iPresentStatus.Charging = is_charging ? 1 : 0;
    iPresentStatus.ACPresent = is_charging ? 1 : 0;
    iPresentStatus.Discharging = is_charging ? 0 : 1;
    iPresentStatus.FullyCharged = (iRemaining == iFullChargeCapacity);
    iPresentStatus.RemainingTimeLimitExpired = is_charging ? 0 : (iRunTimeToEmpty < iRemainTimeLimit);
    iPresentStatus.ShutdownRequested = iDelayBe4ShutDown > 0 ? 1 : 0;
    iPresentStatus.ShutdownImminent = (iPresentStatus.ShutdownRequested || iPresentStatus.RemainingTimeLimitExpired) ? 1 : 0;

    // Update if status changed or min_update_interval 
    if((iPresentStatus != iPreviousStatus) || (iRemaining != iPrevRemaining) || (iRunTimeToEmpty != iPrevRunTimeToEmpty) || (iIntTimer>min_update_interval) ) {
        PowerDevice.sendReport(HID_PD_REMAININGCAPACITY, &iRemaining, sizeof(iRemaining));
        if(!is_charging) {
            PowerDevice.sendReport(HID_PD_RUNTIMETOEMPTY, &iRunTimeToEmpty, sizeof(iRunTimeToEmpty));
        }
        PowerDevice.sendReport(HID_PD_PRESENTSTATUS, &iPresentStatus, sizeof(iPresentStatus));

        iIntTimer = 0;
        iPreviousStatus = iPresentStatus;
        iPrevRemaining = iRemaining;
        iPrevRunTimeToEmpty = iRunTimeToEmpty;
    }

    delay(1000);
    iIntTimer++;
}
