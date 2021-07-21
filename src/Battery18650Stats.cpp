#include "Battery18650Stats.h"

Battery18650Stats::Battery18650Stats(int adcPin, double conversionFactor, int reads) {
  _reads = reads;
  _conversionFactor = conversionFactor;
  _adcPin = adcPin;
}

Battery18650Stats::Battery18650Stats(int adcPin, double conversionFactor) {
  _reads = DEFAULT_READS;
  _conversionFactor = conversionFactor;
  _adcPin = adcPin;
}

Battery18650Stats::Battery18650Stats(int adcPin) {
  _reads = DEFAULT_READS;
  _conversionFactor = DEFAULT_CONVERSION_FACTOR;
  _adcPin = adcPin;
}

Battery18650Stats::Battery18650Stats() {
  _reads = DEFAULT_READS;
  _conversionFactor = DEFAULT_CONVERSION_FACTOR;
  _adcPin = DEFAULT_PIN;
}

Battery18650Stats::~Battery18650Stats() {
  delete this->_conversionTable;
}

void Battery18650Stats::_initConversionTable() {
  double ct[101];
  ct[0] = 3.200;
  ct[1] = 3.250;
  ct[2] = 3.300;
  ct[3] = 3.350;
  ct[4] = 3.400;
  ct[5] = 3.450;
  ct[6] = 3.500;
  ct[7] = 3.550;
  ct[8] = 3.600;
  ct[9] = 3.650;
  ct[10] = 3.700;
  ct[11] = 3.703;
  ct[12] = 3.706;
  ct[13] = 3.710;
  ct[14] = 3.713;
  ct[15] = 3.716;
  ct[16] = 3.719;
  ct[17] = 3.723;
  ct[18] = 3.726;
  ct[19] = 3.729;
  ct[20] = 3.732;
  ct[21] = 3.735;
  ct[22] = 3.739;
  ct[23] = 3.742;
  ct[24] = 3.745;
  ct[25] = 3.748;
  ct[26] = 3.752;
  ct[27] = 3.755;
  ct[28] = 3.758;
  ct[29] = 3.761;
  ct[30] = 3.765;
  ct[31] = 3.768;
  ct[32] = 3.771;
  ct[33] = 3.774;
  ct[34] = 3.777;
  ct[35] = 3.781;
  ct[36] = 3.784;
  ct[37] = 3.787;
  ct[38] = 3.790;
  ct[39] = 3.794;
  ct[40] = 3.797;
  ct[41] = 3.800;
  ct[42] = 3.805;
  ct[43] = 3.811;
  ct[44] = 3.816;
  ct[45] = 3.821;
  ct[46] = 3.826;
  ct[47] = 3.832;
  ct[48] = 3.837;
  ct[49] = 3.842;
  ct[50] = 3.847;
  ct[51] = 3.853;
  ct[52] = 3.858;
  ct[53] = 3.863;
  ct[54] = 3.868;
  ct[55] = 3.874;
  ct[56] = 3.879;
  ct[57] = 3.884;
  ct[58] = 3.889;
  ct[59] = 3.895;
  ct[60] = 3.900;
  ct[61] = 3.906;
  ct[62] = 3.911;
  ct[63] = 3.917;
  ct[64] = 3.922;
  ct[65] = 3.928;
  ct[66] = 3.933;
  ct[67] = 3.939;
  ct[68] = 3.944;
  ct[69] = 3.950;
  ct[70] = 3.956;
  ct[71] = 3.961;
  ct[72] = 3.967;
  ct[73] = 3.972;
  ct[74] = 3.978;
  ct[75] = 3.983;
  ct[76] = 3.989;
  ct[77] = 3.994;
  ct[78] = 4.000;
  ct[79] = 4.008;
  ct[80] = 4.015;
  ct[81] = 4.023;
  ct[82] = 4.031;
  ct[83] = 4.038;
  ct[84] = 4.046;
  ct[85] = 4.054;
  ct[86] = 4.062;
  ct[87] = 4.069;
  ct[88] = 4.077;
  ct[89] = 4.085;
  ct[90] = 4.092;
  ct[91] = 4.100;
  ct[92] = 4.111;
  ct[93] = 4.122;
  ct[94] = 4.133;
  ct[95] = 4.144;
  ct[96] = 4.156;
  ct[97] = 4.167;
  ct[98] = 4.178;
  ct[99] = 4.189;
  ct[100] = 4.200;
  double *_conversionTable{ct};
}

int Battery18650Stats::getBatteryChargeLevel(bool useConversionTable) {
  int readValue = _avgAnalogRead(_adcPin, _reads);
  double volts = _analogReadToVolts(readValue);
  return useConversionTable
         ? _getChargeLevel(volts)
         : _calculateChargeLevel(volts);
}

int Battery18650Stats::_avgAnalogRead(int pinNumber, int reads) {
  int totalValue = 0;
  int averageValue = 0;
  for (int i = 0; i < reads; i++) {
    totalValue += analogRead(pinNumber);
  }
  averageValue = totalValue / reads;
  return averageValue;
}

int Battery18650Stats::_getChargeLevel(double volts) {
  if (_conversionTable == nullptr) {
    _initConversionTable();
  }

  int idx = 50;
  int prev = 0;
  int half = 0;
  if (volts >= 4.2) {
    return 100;
  }
  if (volts <= 3.2) {
    return 0;
  }
  while (true) {
    half = abs(idx - prev) / 2;
    prev = idx;
    if (volts >= _conversionTable[idx]) {
      idx = idx + half;
    } else {
      idx = idx - half;
    }
    if (prev == idx) {
      break;
    }
  }
  return idx;
}

int Battery18650Stats::_calculateChargeLevel(double volts) {
  // return int((-233.82*volts**2)+(2021.3*volts)-4266) + (1 if ((volts > 3.755 and volts <= 3.87) or volts >= 3.94) else 0)
  return 0;
}

double Battery18650Stats::_analogReadToVolts(int readValue) {
  return readValue * _conversionFactor / 1000;
}

double Battery18650Stats::getBatteryVolts() {
  int readValue = _avgAnalogRead(_adcPin, _reads);
  return _analogReadToVolts(readValue);
}
