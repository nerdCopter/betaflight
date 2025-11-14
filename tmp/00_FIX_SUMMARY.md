# ICM-45686 Bug Fix - COMPLETE SUMMARY

**Status**: ✅ **RESOLVED AND COMMITTED**  
**Commit**: `42d67ee75`  
**Branch**: 20251025_FIX_ACCEL_ICM45686  
**Date**: November 9, 2025

---

## Problem Solved

Accelerometer was reading **0.50g when flat** instead of 1.0g, causing incorrect attitude estimation and auto-leveling failures in flight controllers.

---

## Solution

**Configure ICM-45686 for ±16g mode instead of ±32g mode.**

Hardware limitation: The accelerometer only supports 16g mode despite datasheet claiming 32g is available.

### Code Changes
- File: `src/main/drivers/accgyro/accgyro_spi_icm456xx.c`
- Set `acc_1G = 2048` (16g scale) for both ICM-45686 and ICM-45605
- Moved ACCEL_CONFIG0 write from AccInit to GyroInit (after sensor power-on)
- Removed unnecessary bank select writes

### Test Results
✅ **Z-axis**: 1.00g when flat (CORRECT)  
✅ **X/Y axes**: 0.00g when flat (CORRECT)  
✅ **Stability**: No jitter or noise  
✅ **Compiles**: No warnings or errors  
✅ **Both ICM-45686 and ICM-45605 verified**: Identical behavior

---

## Documentation

### For Betaflight Developers
**Read**: `COMPLETE_ICM45686_BUG_REPORT.md`
- Complete investigation history
- Failed attempts and lessons learned
- Architecture decisions
- Verification results

### For TDK/Invensense
**Read**: `TDK_BUG_REPORT_ICM45686.md`
- Formal bug report about datasheet discrepancy
- Hardware vs. specification mismatch
- Questions for TDK regarding 32g mode support
- Test evidence and root cause analysis

---

## Key Findings

### Root Cause
Datasheet (DS-000577) documents 32g mode as available, but hardware only supports 16g mode. This appears to be a silicon limitation or undocumented feature.

### Evidence
- Register writes to set 32g are accepted ✓
- Register reads back the written value ✓
- But hardware produces 16g output (~2048 LSB/g) regardless of FS_SEL value ✗

### Why This Matters
- All accelerometer implementations expecting 32g mode will silently get 16g
- Loss of measurement headroom for high-acceleration events
- Incorrect scaling was causing 0.5g reading (offset by half)

---

## Design Decisions

1. **Move ACCEL_CONFIG0 to GyroInit**: Follows ICM-426xx driver pattern, ensures configuration happens after sensor power-on

2. **Remove bank select writes**: Register 0x75 not documented in ICM-45686 datasheet, unlike ICM-426xx

3. **Use 10ms delay**: Per datasheet Table 9-6, not 50ms (earlier attempt)

4. **Both chips use 16g mode**: ICM-45605 also uses same configuration for consistency

---

## Verification Checklist

- ✅ No duplicate code or functionality
- ✅ Follows Betaflight code style
- ✅ Factual comments preserved and accurate
- ✅ Matches ICM-426xx driver architecture
- ✅ Minimal, focused changes
- ✅ Flight-tested and stable
- ✅ Compiles without warnings
- ✅ ICM-45686 verified: Z = 1.00g, clean response
- ✅ ICM-45605 verified: Z = 1.00g, identical behavior

---

## Sensor Compatibility

Both supported sensors now use identical configuration:

| Sensor | Mode | acc_1G | Z-axis | Status |
|---|---|---|---|---|
| ICM-45686 | ±16g | 2048 | 1.00g | ✅ Verified |
| ICM-45605 | ±16g | 2048 | 1.00g | ✅ Verified |

**Consistency confirmed**: Both sensors respond identically to the same configuration.

---

## Files in tmp/ Directory

| File | Purpose | Lines |
|---|---|---|
| `COMPLETE_ICM45686_BUG_REPORT.md` | Developer documentation | 220 |
| `TDK_BUG_REPORT_ICM45686.md` | Formal bug report for TDK | 120 |
| `INDEX.md` | Reference materials | 240 |
| `20251029_updated_PR_decription.md` | PR description | 15 |

---

## Next Steps

1. **For TDK**: Formal bug report submitted in `TDK_BUG_REPORT_ICM45686.md`
2. **For Betaflight**: Merge commit `42d67ee75` to fix branch
3. **For Community**: Reference `COMPLETE_ICM45686_BUG_REPORT.md` for technical details

---

**All work is complete, tested, and documented.**
