#if defined(INJECTOR)
void InjectorInit() {
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // set entire TCCR1B register to 0
  TCCR1B |= (1 << CS12); // prescaler 256 --> Increment time = 256/16,000,000 = 16us
  TIMSK1 |= (1 << TOIE1); // enable Timer1 overflow interrupt
  TCNT1 = 3036; // counter initial value so as to overflow every 1 sec: 65536 - 3036 = 62500 * 16us = 1 sec (65536 maximum value of the timer)
  // set and initialize the TIMER1
}

void InjectorTime() { // it is called every time a change occurs at the gasoline injector signal and calculates gasoline injector opening time during the 1 sec interval
  if (digitalRead(INJECTOR_PIN) == LOW) {
    InjectorTime1 = micros();
  }
  if (digitalRead(INJECTOR_PIN) == HIGH) {
    InjectorTime2 = micros();
  }
  if (InjectorTime2 > InjectorTime1) {
    if ((InjectorTime2 - InjectorTime1) > 500 && (InjectorTime2 - InjectorTime1) < 15000) {  // It has been observed that during a sudden drop in RPM, spikes > 15ms = 15000us occur. I assume this is engine braking and the injectors are turned off.
      Injector_Open_Duration += (InjectorTime2 - InjectorTime1) * Ncyl; // Summing the opening time of Ncyl injectors to calculate the total fuel consumption in microseconds.
      INJ_TIME = InjectorTime2 - InjectorTime1; // injection duration
      num_injection++;
    }
  }
}

ISR(TIMER1_OVF_vect) { // TIMER1 overflow interrupt -- occurs every 1 sec -- it holds the time (in seconds) and also prevents the overflowing of some variables
  total_duration_inj += (float)Injector_Open_Duration / 1000; // Total injector open time in milliseconds stored in EEPROM.
  current_duration_inj += (float)Injector_Open_Duration / 1000; // Injector open time in milliseconds for the current trip.
  total_consumption_inj = total_duration_inj / 1000 * Ls * Ncyl; // total fuel consumed in liters
  current_consumption_inj = current_duration_inj / 1000 * Ls * Ncyl; // fuel consumed in liters for the trip
  current_time_inj += 1000; // current machine running time
  total_time_inj += 1000; // total machine running time
  rpm_inj = num_injection * 2;
  LPH_INJ = (float)Injector_Open_Duration / 1000 * Ls * Ncyl / 2 * 1.2;
  // | fuel injected per second | in liters | for 6 cylinders | coefficients are empirical
  Injector_Open_Duration = 0; // injectors open duration for 1 sec
  TCNT1 = 3036;
}
#endif
