float getOBDdata(byte OBDdataIDX) {
  float returnValue;
  switch (OBDdataIDX) {
    case 0: // UNKNOWN
      returnValue = ToyotaData[0];
      break;
    case OBD_INJ: // Injector pulse width = X * 0.125 (ms)
      returnValue = ToyotaData[OBD_INJ] * 0.125; // Injector pulse width
      break;
    case OBD_IGN: // Ignition timing angle X * 0.47 - 30 (degrees)
      returnValue = ToyotaData[OBD_IGN] * 0.47 - 30;
      break;
    case OBD_IAC: // Idle Air Control valve state for different types of IAC valves: X / 255 * 100 (%)
      // X (steps)
      returnValue = ToyotaData[OBD_IAC] * 0.39215; // optimize divide
      break;
    case OBD_RPM: // Engine speed X * 25 (RPM)
      returnValue = ToyotaData[OBD_RPM] * 25;
      break;
    case OBD_MAP: // Manifold Absolute Pressure (MAP/MAF)
      // X * 0.6515 (kPa)
      // X * 4.886 (mmHg)
      // X * 0.97 (kPa) (for turbo engines)
      // X * 7.732 (mmHg) (for turbo engines)
      // X * 2 (g/sec) MAF
      // X / 255 * 5 (Volts) (voltage on the MAF sensor)
      returnValue = ToyotaData[OBD_MAP] * 2; // Raw data
      break;
    case OBD_ECT: // Engine Coolant Temperature (ECT)
      // Depending on the value of X, different formulas:
      // 0..14:          = (X - 5) * 2 - 60
      // 15..38:        = (X - 15) * 0.83 - 40
      // 39..81:        = (X - 39) * 0.47 - 20
      // 82..134:      = (X - 82) * 0.38
      // 135..179:    = (X - 135) * 0.44 + 20
      // 180..209:    = (X - 180) * 0.67 + 40
      // 210..227:    = (X - 210) * 1.11 + 60
      // 228..236:    = (X - 228) * 2.11 + 80
      // 237..242:    = (X - 237) * 3.83 + 99
      // 243..255:    = (X - 243) * 9.8 + 122
      // Temperature in degrees Celsius.
      if (ToyotaData[OBD_ECT] >= 243)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 243) * 9.8) + 122;
      else if (ToyotaData[OBD_ECT] >= 237)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 237) * 3.83) + 99;
      else if (ToyotaData[OBD_ECT] >= 228)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 228) * 2.11) + 80.0;
      else if (ToyotaData[OBD_ECT] >= 210)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 210) * 1.11) + 60.0;
      else if (ToyotaData[OBD_ECT] >= 180)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 180) * 0.67) + 40.0;
      else if (ToyotaData[OBD_ECT] >= 135)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 135) * 0.44) + 20.0;
      else if (ToyotaData[OBD_ECT] >= 82)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 82) * 0.38);
      else if (ToyotaData[OBD_ECT] >= 39)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 39) * 0.47) - 20.0;
      else if (ToyotaData[OBD_ECT] >= 15)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 15) * 0.83) - 40.0;
      else
        returnValue = ((float)(ToyotaData[OBD_ECT] - 15) * 2.0) - 60.0;
      break;
    case OBD_TPS: // Throttle Position Sensor
      // X / 2 (degrees)
      // X / 1.8 (%)
      returnValue = ToyotaData[OBD_TPS] / 1.8;
      break;
    case OBD_SPD: // Vehicle speed (km/h)
      returnValue = ToyotaData[OBD_SPD];
      break;
    // Correction for inline engines / correction for the first half
    case OBD_OXSENS:
      returnValue = (float)ToyotaData[OBD_OXSENS] * 0.01953125;
      break;
    // Correction for the second half
    /*
    case OBD_OXSENS2: // Lambda2 tst
      returnValue = (float)ToyotaData[OBD_OXSENS2] * 0.01953125;
      break;*/
    // read flag bytes bitwise
    case 11:
      returnValue = bitRead(ToyotaData[11], 0);  // Over-enrichment after start 1-On
      break;
    case 12:
      returnValue = bitRead(ToyotaData[11], 1); // Cold engine 1-Yes
      break;
    case 13:
      returnValue = bitRead(ToyotaData[11], 4); // Detonation 1-Yes
      break;
    case 14:
      returnValue = bitRead(ToyotaData[11], 5); // Feedback on the lambda probe 1-Yes
      break;
    case 15:
      returnValue = bitRead(ToyotaData[11], 6); // Additional enrichment 1-Yes
      break;
    case 16:
      returnValue = bitRead(ToyotaData[12], 0); // Starter 1-Yes
      break;
    case 17:
      returnValue = bitRead(ToyotaData[12], 1); // Idle indicator (Throttle) 1-Yes (Closed)
      break;
    case 18:
      returnValue = bitRead(ToyotaData[12], 2); // Air conditioning 1-Yes
      break;
    case 19:
      returnValue = bitRead(ToyotaData[13], 3); // Neutral 1-Yes
      break;
    case 20:
      returnValue = bitRead(ToyotaData[14], 4); // Mixture first half 1-Rich, 0-Lean
      break;
    case 21:
      returnValue = bitRead(ToyotaData[14], 5); // Mixture second half 1-Rich, 0-Lean
      break;
    default: // DEFAULT CASE (in no match to number)
      // send "error" value
      returnValue = 9999.99;
  } // end switch
  // send value back
  return returnValue;
} // end void getOBDdata

void ChangeState() {
  static uint8_t ID, EData[TOYOTA_MAX_BYTES];
  static boolean InPacket = false;
  static unsigned long StartMS;
  static uint16_t BitCount;
  int state = digitalRead(ENGINE_DATA_PIN);
  digitalWrite(LED_PIN, state);
  if (InPacket == false) {
    if (state == MY_HIGH) {
      StartMS = millis();
    } else { // else if (state == MY_HIGH)
      if ((millis() - StartMS) > (15 * 8)) {
        StartMS = millis();
        InPacket = true;
        BitCount = 0;
      } // end if ((millis() - StartMS) > (15 * 8))
    } // end if (state == MY_HIGH)
  } else { // else if (InPacket == false)
    uint16_t bits = ((millis() - StartMS) + 1) / 8; // The +1 is to cope with slight time errors
    StartMS = millis();
    // process bits
    while (bits > 0) {
      if (BitCount < 4) {
        if (BitCount == 0)
          ID = 0;
        ID >>= 1;
        if (state == MY_LOW) // inverse state as we are detecting the change!
          ID |= 0x08;
      } else { // else if (BitCount < 4)
        uint16_t bitpos = (BitCount - 4) % 11;
        uint16_t bytepos = (BitCount - 4) / 11;
        if (bitpos == 0) {
          // Start bit, should be LOW
          if ((BitCount > 4) && (state != MY_HIGH)) { // inverse state as we are detecting the change!
            ToyotaFailBit = BitCount;
            InPacket = false;
            break;
          } // end if ((BitCount > 4) && (state != MY_HIGH))
        } else if (bitpos < 9) { // else if (bitpos == 0)
          EData[bytepos] >>= 1;
          if (state == MY_LOW) // inverse state as we are detecting the change!
            EData[bytepos] |= 0x80;
        } else { // else if (bitpos == 0)
          // Stop bits, should be HIGH
          if (state != MY_LOW) { // inverse state as we are detecting the change!
            ToyotaFailBit = BitCount;
            InPacket = false;
            break;
          } // end if (state != MY_LOW)
          if ((bitpos == 10) && ((bits > 1) || (bytepos == (TOYOTA_MAX_BYTES - 1)))) {
            ToyotaNumBytes = 0;
            ToyotaID = ID;
            for (uint16_t i = 0; i <= bytepos; i++)
              ToyotaData[i] = EData[i];
            ToyotaNumBytes = bytepos + 1;
            if (bits >= 16) // Stop bits of the last byte were 1's so detect preamble for the next packet
              BitCount = 0;
            else {
              ToyotaFailBit = BitCount;
              InPacket = false;
            }
            break;
          }
        }
      }
      ++BitCount;
      --bits;
    } // end while
  } // end if (InPacket == false)
} // end void ChangeState
