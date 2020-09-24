#include "Accelerometer.h"

#define ADR_FS_2 0x00
#define ADR_FS_4 0x10
#define ADR_FS_8 0x30

#define G 9.8

LIS331DLH::LIS331DLH(uint8_t addr)
    : IMU(addr) { }

void LIS331DLH::begin() {
    // подключаемся к шине I²C
    WIRE_IMU.begin();
    // включаем координаты x, y, z
    _ctrlReg1 |= (1 << 0);
    _ctrlReg1 |= (1 << 1);
    _ctrlReg1 |= (1 << 2);
    // включаем аксселерометр
    _ctrlReg1 |= (1 << 5);
    // устанавливаем максимальное измеряемое ускорение в G
    setRange(RANGE_2G);
    writeCtrlReg1();
}

void LIS331DLH::setRange(uint8_t range) {
    switch (range) {
    case RANGE_2G: {
        _ctrlReg4 = ADR_FS_2;
        _mult = RANGE_2G / 32767.0;
        break;
    }
    case RANGE_4G: {
        _ctrlReg4 = ADR_FS_4;
        _mult = RANGE_4G / 32767.0;
        break;
    }
    case RANGE_8G: {
        _ctrlReg4 = ADR_FS_8;
        _mult = RANGE_8G / 32767.0;
        break;
    }
    default: {
        _mult = RANGE_2G / 32767.0;
    } break;
    }
    writeCtrlReg4();
}

void LIS331DLH::sleep(bool enable) {
    if (enable)
        _ctrlReg1 &= ~(1 << 5);
    else
        _ctrlReg1 |= (1 << 5);

    writeCtrlReg1();
}

float LIS331DLH::readGX() { return readX() * _mult; }

float LIS331DLH::readGY() { return readY() * _mult; }

float LIS331DLH::readGZ() { return readZ() * _mult; }

float LIS331DLH::readAX() { return readGX() * G; }

float LIS331DLH::readAY() { return readGY() * G; }

float LIS331DLH::readAZ() { return readGZ() * G; }

void LIS331DLH::readGXYZ(float* gx, float* gy, float* gz) {
    int16_t x, y, z;
    readXYZ(&x, &y, &z);
    *gx = x * _mult;
    *gy = y * _mult;
    *gz = z * _mult;
}

void LIS331DLH::readAXYZ(float* ax, float* ay, float* az) {
    readGXYZ(ax, ay, az);
    (*ax) *= G;
    (*ay) *= G;
    (*az) *= G;
}
