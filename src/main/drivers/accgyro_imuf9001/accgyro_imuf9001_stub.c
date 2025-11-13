/*
 * IMUF9001 Stub Driver for Betaflight
 * This is a minimal stub to allow compilation.
 * Full implementation requires significant API adaptation from EmuFlight.
 */

#include "platform.h"

#ifdef USE_GYRO_IMUF9001

#include "drivers/accgyro/accgyro.h"
#include "accgyro_imuf9001.h"

// Global variables required by header
volatile uint32_t isImufCalibrating = 0;
volatile uint16_t imufCurrentVersion = 0;

// Stub functions for compilation
uint8_t imuf9001SpiDetect(const gyroDev_t *gyro) {
    UNUSED(gyro);
    return 0;  // Not detected (stub)
}

bool imufSpiAccDetect(accDev_t *acc) {
    UNUSED(acc);
    return false;  // Not detected (stub)
}

bool imufSpiGyroDetect(gyroDev_t *gyro) {
    UNUSED(gyro);
    return false;  // Not detected (stub)
}

void imufSpiGyroInit(gyroDev_t *gyro) {
    UNUSED(gyro);
}

void imufSpiAccInit(accDev_t *acc) {
    acc->acc_1G = 512 * 4;  // Standard scale
}

void imufStartCalibration(void) {
}

void imufEndCalibration(void) {
}

#endif // USE_GYRO_IMUF9001
