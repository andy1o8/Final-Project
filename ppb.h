#ifndef PPB_H_
#define PPB_H_

#include <stdint.h>

#define PPB                     ((volatile uint32_t *)0xE000E000)

// SysTick registers
enum {
    SYST_CSR   = (0x010 >> 2),   // SysTick Control and Status
    SYST_RVR   = (0x014 >> 2),   // SysTick Reload Value
    SYST_CVR   = (0x018 >> 2),   // SysTick Current Value
    SYST_CALIB = (0x01C >> 2),   // SysTick Calibration
};

// Bit definitions
#define SYST_CSR_COUNTFLAG     (1 << 16)  // Returns 1 if timer counted to 0
#define SYST_CSR_CLKSOURCE     (1 << 2)   // 1 = core clock, 0 = external
#define SYST_CSR_TICKINT       (1 << 1)   // Enable interrupt
#define SYST_CSR_ENABLE        (1 << 0)   // Enable counter

#endif /* PPB_H_ */
