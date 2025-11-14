# fix(icm456xx): ICM-45686/45605 accelerometer scaling and DMA fixes

Fixes #14712 - ICM-45686 accelerometer reads 0.5g when flat instead of 1.0g, causing auto-leveling failures.

## Summary

Addresses critical accelerometer scaling issue and supporting driver fixes discovered during #14712 investigation. Primary fix: accelerometer now reads correct 1.0g when level (was 0.5g) by configuring hardware for 16g mode instead of incorrectly attempting 32g mode. Also resolves DMA buffer corruption, initialization sequencing, and error handling issues.

## Changes

### 1. Accelerometer Scaling (PRIMARY FIX)
- Hardware only supports ±16g mode (2048 LSB/g); datasheet incorrectly documents ±32g as available
- Set `acc_1G = 2048` and move ACCEL_CONFIG0 write to GyroInit (after sensor power-on)
- Result: Both ICM-45686 and ICM-45605 now read 1.00g when flat
- Formal bug report submitted to TDK regarding datasheet discrepancy

### 2. DMA & Data Reading
- Fixed accel reading gyro data in DMA mode (buffer indices 1-6 for accel, 7-12 for gyro)
- Implemented combined 12-byte DMA reads for accel+gyro contiguous data
- Proper little-endian byte ordering and buffer management
- Unified non-DMA read paths for consistency

### 3. Initialization & Sequencing
- Removed invalid bank select writes (register 0x75 not in ICM-45686 datasheet)
- Proper sensor enable sequencing: power on → configure → read
- Datasheet-compliant startup delays (10ms accel, 35ms gyro, 1ms enable)
- AAF/Interpolator initialization with graceful fallback on failure

### 4. Error Handling & Stability
- IREG write operations with 5ms timeout and elapsed-time tracking
- AAF/LPF configuration error handling with fallback to disabled state
- SPI return value checking in default accel read case
- Production-ready error handling per Betaflight style

## Testing

✅ ICM-45686: Z-axis reads 1.00g when flat (was 0.50g)  
✅ ICM-45605: Z-axis reads 1.00g when flat  
✅ DMA and non-DMA modes working correctly  
✅ No buffer corruption, jitter, or data errors  
✅ Compiles without warnings, follows Betaflight code style

## Technical Notes

Hardware supports only ±16g mode (2048 LSB/g); datasheet (DS-000577) incorrectly lists ±32g, ±8g, ±4g, ±2g modes as available. Register writes to select unsupported modes are silently ignored. Configured for 16g explicitly per actual hardware capability.

## Files Modified
- `src/main/drivers/accgyro/accgyro_spi_icm456xx.c` (146 insertions, 101 deletions)

## Related
- Issue: #14712
- Datasheets: DS-000577 (ICM-45686), DS-000576 (ICM-45605)

