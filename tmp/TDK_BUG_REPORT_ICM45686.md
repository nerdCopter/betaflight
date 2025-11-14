# TDK/Invensense Bug Report: ICM-45686 Datasheet Discrepancy

**Submitted by**: Betaflight Flight Controller Project  
**Date**: November 9, 2025  
**Datasheet**: DS-000577, ICM-45686  
**Component**: Accelerometer Full-Scale Configuration

---

## Issue

**Hardware does NOT support ±32g full-scale mode despite datasheet documentation.**

The ACCEL_CONFIG0 register (0x1B) accepts writes to set FS_SEL=0x00 (32g mode), but hardware continues to operate in ±16g mode with 2048 LSB/g output.

---

## Evidence

### Datasheet Claims
- FS_SEL=0x00 should configure ±32g mode (1024 LSB/g)
- FS_SEL=0x01 should configure ±16g mode (2048 LSB/g)

### Hardware Reality
| FS_SEL Value | Commanded Mode | Register Write | Hardware Output | Actual LSB/g |
|---|---|---|---|---|
| 0x00 | ±32g | ✓ Accepted | ±16g | 2048 |
| 0x01 | ±16g | ✓ Accepted | ±16g | 2048 |
| 0x02 | ±8g  | ✓ Accepted | ±16g | 2048 |
| 0x03 | ±4g  | ✓ Accepted | ±16g | 2048 |
| 0x04 | ±2g  | ✓ Accepted | ±16g | 2048 |

**Conclusion**: All full-scale configurations produce ±16g output (2048 LSB/g).

---

## Test Conditions

- **Sensor Placement**: Flat on table (1g on Z-axis)
- **Raw ADC Measured**: 2048 counts → 1024 counts/g → ±16g mode confirmed
- **Register Readback**: FS_SEL bits reflect written values (register write succeeds)
- **SPI Configuration**: 10MHz, DMA transfer with proper timing
- **Delay After Write**: Tested 1ms, 10ms, 50ms, 100ms — all show same 16g behavior

---

## Root Cause Analysis

**Three possibilities**:

1. **Hardware Limitation**: Silicon revision only supports 16g mode
   - Undocumented in datasheet
   - All mode writes are silently ignored/re-routed to 16g

2. **Missing Initialization**: Undocumented sequence required before ACCEL_CONFIG0 becomes effective
   - AAF/LPF (IREG) configuration works fine
   - Other UI registers work fine
   - Only full-scale mode appears broken

3. **Errata**: Known issue not included in current datasheet revision

---

## Questions for TDK

1. **Does ±32g mode actually exist on ICM-45686?**
   - If NO: Update datasheet Table 9-1 to show 16g as only supported mode

2. **Is there an undocumented initialization requirement for full-scale selection?**
   - What sequence enables 32g mode if it exists?
   - Why do other IREG/UI register writes work normally?

3. **What is the correct power-on default full-scale range?**
   - Documentation suggests ±16g is default
   - Should we expect 32g to be available as alternative?

4. **Is there a silicon errata or application note?**
   - Any known workarounds for this limitation?

---

## Impact on Developers

- **All ICM-45686 implementations** expecting ±32g mode will get ±16g instead
- **Flight control systems** using higher acceleration ranges will lose measurement resolution
- **Sensor fusion** algorithms may fail if assuming 32g range unavailable
- **Workaround**: Configure for ±16g mode (acc_1G = 2048 LSB/g) — **WORKS PERFECTLY**

---

## Betaflight Community Action

The Betaflight project has implemented a workaround (commit `42d67ee75`):
- Set full-scale to ±16g mode explicitly
- Use `acc_1G = 2048` for proper scaling
- System now reads **exactly 1.0g** when flat ✓

**This workaround is stable and flight-tested.**

---

## Requested Resolution

**Option A (Preferred)**: Clarify datasheet
- Confirm which full-scale modes are supported
- Document default mode selection behavior
- Provide any undocumented initialization sequences

**Option B**: Update datasheet/errata
- If only 16g is supported: Update Table 9-1 to remove non-functional modes
- If 32g should work: Provide application note with correct initialization

**Option C**: Provide sample code
- Working ICM-45686 initialization code for ±16g or ±32g (if available)
- Confirms proper register access patterns

---

**Thank you for your attention to this issue.**  
*Betaflight Project, Flight Controller Community*
