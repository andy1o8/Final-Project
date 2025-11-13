#ifndef _SYSCTL_H
#define _SYSCTL_H

#include <stdint.h>

// Peripheral base addresses.
#define SYSCTL          ((volatile uint32_t *)0x400FE000)

// Peripheral register offsets and special fields.
enum {
    SYSCTL_DID0         = (0x000 >> 2),
#define   SYSCTL_DID0_VER_M     (0x7 << 28)
#define   SYSCTL_DID0_CLASS_M   (0xFFFF << 16)
#define   SYSCTL_DID0_CLASS_TM4C129  (0x5 << 16)

    SYSCTL_RCGCGPIO     = (0x608 >> 2),
#define   SYSCTL_RCGCGPIO_PORTC   (1 << 2)
#define   SYSCTL_RCGCGPIO_PORTD   (1 << 3)
#define   SYSCTL_RCGCGPIO_PORTE   (1 << 4)
#define   SYSCTL_RCGCGPIO_PORTH   (1 << 7)
#define   SYSCTL_RCGCGPIO_PORTJ   (1 << 8)
#define   SYSCTL_RCGCGPIO_PORTK   (1 << 10)   // PORTK added
#define   SYSCTL_RCGCGPIO_PORTL   (1 << 11)
#define   SYSCTL_RCGCGPIO_PORTN   (1 << 12)

    SYSCTL_RCGCTIMER    = (0x604 >> 2),
#define   SYSCTL_RCGCTIMER_R3     (1 << 3)    // Timer 3

    SYSCTL_RIS          = (0x050 >> 2),
#define   SYSCTL_RIS_MOSCPUPRIS   (1 << 8)

    SYSCTL_MISC         = (0x058 >> 2),
#define   SYSCTL_MISC_MOSCPUMIS   (1 << 8)

    SYSCTL_MOSCCTL      = (0x07C >> 2),
#define   SYSCTL_MOSCCTL_OSCRNG   (1 << 4)
#define   SYSCTL_MOSCCTL_PWRDN    (1 << 3)
#define   SYSCTL_MOSCCTL_NOXTAL   (1 << 2)
#define   SYSCTL_MOSCCTL_MOSCIM   (1 << 1)
#define   SYSCTL_MOSCCTL_CVAL     (1 << 0)

    SYSCTL_RSCLKCFG     = (0x0B0 >> 2),
#define   SYSCTL_RSCLKCFG_MEMTIMU (1 << 31)
#define   SYSCTL_RSCLKCFG_NEWFREQ (1 << 30)
#define   SYSCTL_RSCLKCFG_USEPLL  (1 << 28)
#define   SYSCTL_RSCLKCFG_PLLSRC_M     (0xF << 24)
#define   SYSCTL_RSCLKCFG_PLLSRC_MOSC  (3 << 24)
#define   SYSCTL_RSCLKCFG_OSCSRC_M     (0xF << 20)
#define   SYSCTL_RSCLKCFG_OSCSRC_MOSC  (3 << 20)
#define   SYSCTL_RSCLKCFG_PSYSDIV_M    (0x3FF << 0)

    SYSCTL_MEMTIM0      = (0x0C0 >> 2),
#define   SYSCTL_MEMTIM0_FBCHT_M   (0xF << 6)
#define   SYSCTL_MEMTIM0_FBCHT_3_5 (6 << 6)
#define   SYSCTL_MEMTIM0_FWS_M     (0xF << 0)
#define   SYSCTL_MEMTIM0_FWS_6     (6 << 0)

    SYSCTL_PLLFREQ0     = (0x160 >> 2),
#define   SYSCTL_PLLFREQ0_PLLPWR   (1 << 23)
#define   SYSCTL_PLLFREQ0_MFRAC_M  (0x3FF << 10)
#define   SYSCTL_PLLFREQ0_MINT_M   (0x3FF << 0)

    SYSCTL_PLLFREQ1     = (0x164 >> 2),
#define   SYSCTL_PLLFREQ1_Q_M      (0x1F << 8)
#define   SYSCTL_PLLFREQ1_N_M      (0x1F << 0)

    SYSCTL_PLLSTAT      = (0x168 >> 2),
#define   SYSCTL_PLLSTAT_LOCK      (1 << 0)
};

// Helper: Wait for PLL lock
#define WAIT_FOR_PLL_LOCK() \
    while ((SYSCTL[SYSCTL_PLLSTAT] & SYSCTL_PLLSTAT_LOCK) == 0) {}

// Helper: Configure 120 MHz from 25 MHz crystal
__inline void SysCtlClockSet_120MHz(void) {
    // 1. Use MOSC, no crystal fail action
    SYSCTL[SYSCTL_MOSCCTL] = SYSCTL_MOSCCTL_OSCRNG | SYSCTL_MOSCCTL_CVAL;

    // 2. Wait for MOSC stable
    while ((SYSCTL[SYSCTL_RIS] & SYSCTL_RIS_MOSCPUPRIS) == 0) {}

    // 3. Configure PLL: Fvco = 480 MHz, SysDiv = 4 → 120 MHz
    SYSCTL[SYSCTL_PLLFREQ0] = (4 << 10) |  // MFRAC = 0
                              (120 << 0);  // MINT = 120
    SYSCTL[SYSCTL_PLLFREQ1] = (4 << 8) |   // Q = 4
                              (0 << 0);    // N = 0

    // 4. Power up PLL
    SYSCTL[SYSCTL_PLLFREQ0] |= SYSCTL_PLLFREQ0_PLLPWR;

    // 5. Wait for lock
    WAIT_FOR_PLL_LOCK();

    // 6. Configure clock: Use PLL, PSYSDIV=3 (480/4=120 MHz)
    SYSCTL[SYSCTL_RSCLKCFG] = SYSCTL_RSCLKCFG_USEPLL |
                              SYSCTL_RSCLKCFG_PLLSRC_MOSC |
                              SYSCTL_RSCLKCFG_OSCSRC_MOSC |
                              (3 << 0);  // PSYSDIV = 3

    // 7. Update memory timings
    SYSCTL[SYSCTL_MEMTIM0] = SYSCTL_MEMTIM0_FBCHT_3_5 | SYSCTL_MEMTIM0_FWS_6;
    SYSCTL[SYSCTL_RSCLKCFG] |= SYSCTL_RSCLKCFG_MEMTIMU | SYSCTL_RSCLKCFG_NEWFREQ;
}

#endif // _SYSCTL_H
