#define Ncyl 6.0      // 6 injectors
#define Ninjection 1  // injection once per cycle

#if defined(SDCARD) // DEFINE module for writing to the SD card
#include "SdFat.h"
#include <SPI.h>
#define SS 5 // SS pin number for SD module
#define FILE_BASE_NAME "Data"   // file name template
#define error(msg) sd.errorHalt(F(msg)) // errors when working with SD
#endif

// DEFINE OBD READER
#define  MY_HIGH  HIGH // LOW    // I have inverted the Eng line using an Opto-Coupler, if yours isn't then reverse these low & high defines.
#define  MY_LOW   LOW // HIGH
#define  TOYOTA_MAX_BYTES  24
#define OBD_INJ 1 // Injector pulse width (INJ)
#define OBD_IGN 2 // Ignition timing angle (IGN)
#define OBD_IAC 3 // Idle Air Control (IAC)
#define OBD_RPM 4 // Engine speed (RPM)
#define OBD_MAP 5 // Manifold Absolute Pressure (MAP)
#define OBD_ECT 6 // Engine Coolant Temperature (ECT)
#define OBD_TPS 7 // Throttle Position Sensor (TPS)
#define OBD_SPD 8 // Speed (SPD)
#define OBD_OXSENS 9 // Oxygen sensor 1
//#define OBD_OXSENS2 10 // Oxygen sensor 2 for V-shaped engines. I don't have it.

#if defined(INJECTOR)
// based on the injector signals
#endif
