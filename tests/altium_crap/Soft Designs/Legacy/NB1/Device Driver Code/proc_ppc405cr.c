#include "proc_ppc405cr.h"

#define TBL  0x11C
#define TBU  0x11D
#define TSR  0x3D8
#define TCR  0x3DA
#define PIT  0x3DB
#define EVPR 0x3D6

//-------------------------------------------------------------------------------
// Timer Status Register (TSR)
//-------------------------------------------------------------------------------
// ENW  0--- ---- ---- ---- ---- ---- ---- ---- Enable Next Watchdog
// WIS  -0-- ---- ---- ---- ---- ---- ---- ---- Watchdog Interrupt Status
// WRS  --00 ---- ---- ---- ---- ---- ---- ---- Watchdog Reset Status
// PIS  ---- 1--- ---- ---- ---- ---- ---- ---- PIT Interrupt Status
// FIS  ---- -0-- ---- ---- ---- ---- ---- ---- FIT Interrupt Status
// RES  ---- -000 0000 0000 0000 0000 0000 0000 Reserved
//-------------------------------------------------------------------------------
//      0000 1000 0000 0000 0000 0000 0000 0000
//      0    8    0    0    0    0    0    0
#define TSR_ClearPITFlag  0x08000000
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Timer Control Register (TCR)
//-------------------------------------------------------------------------------
// WP   00-- ---- ---- ---- ---- ---- ---- ---- Watchdog Period
// WRC  --00 ---- ---- ---- ---- ---- ---- ---- Watchdog Reset Control
// WIE  ---- 0--- ---- ---- ---- ---- ---- ---- Watchdog Interrupt Enable
// PIE  ---- -1-- ---- ---- ---- ---- ---- ---- PIT Interrupt Enable
// FP   ---- --00 ---- ---- ---- ---- ---- ---- FIT Period
// FIE  ---- ---- 0--- ---- ---- ---- ---- ---- FIT Interrupt Enable
// ARE  ---- ---- -1-- ---- ---- ---- ---- ---- Auto Reload Enable
// RES  ---- ---- --00 0000 0000 0000 0000 0000 Reserved
//-------------------------------------------------------------------------------
//      0000 0100 0100 0000 0000 0000 0000 0000
//      0    4    4    0    0    0    0    0
#define TCR_EnablePITWithReload  0x04400000
#define TCR_DisablePITWithReload 0x00000000
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Machine State Register (MSR)
//-------------------------------------------------------------------------------
// RES  0000 0000 0000 0--- ---- ---- ---- ---- Reserved
// WSE  ---- ---- ---- -0-- ---- ---- ---- ---- Wait State Enable
// CIE  ---- ---- ---- --0- ---- ---- ---- ---- Critical Interrupt Enable
// RES  ---- ---- ---- ---0 ---- ---- ---- ---- Reserved
// EE   ---- ---- ---- ---- 1--- ---- ---- ---- External Interrupt Enable
// PS   ---- ---- ---- ---- -0-- ---- ---- ---- Problem State
// RES  ---- ---- ---- ---- --0- ---- ---- ---- Reserved
// MCE  ---- ---- ---- ---- ---0 ---- ---- ---- Machine Check Enable
// RES  ---- ---- ---- ---- ---- 0--- ---- ---- Reserved
// DWE  ---- ---- ---- ---- ---- -0-- ---- ---- Debug Wait Enable
// DIE  ---- ---- ---- ---- ---- --0- ---- ---- Debug Interrupts Enable
// RES  ---- ---- ---- ---- ---- ---0 00-- ---- Reserved
// IR   ---- ---- ---- ---- ---- ---- --0- ---- Instruction Relocate
// DR   ---- ---- ---- ---- ---- ---- ---0 ---- Date Relocate
// RES  ---- ---- ---- ---- ---- ---- ---- 0000 Reserved
//-------------------------------------------------------------------------------
//      0000 0000 0000 0000 1000 0000 0000 0000
//      0    0    0    0    8    0    0    0
#define MSR_EnableExternalInterrupt 0x00008000
#define MSR_DisableExternalInterrupt 0x00000000
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
unsigned int ppc405cr_get_exception_vector_prefix (void)
{
    unsigned int ret = 0;
    __asm("mfspr %0, %1":"=r"(ret): "i"(EVPR): );
    return ret;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void ppc405cr_set_exception_vector_prefix(unsigned int absolute_address)
{
    __asm("mtevpr %0": :"r"(absolute_address): );
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void ppc405cr_set_programmable_interval_timer(unsigned int value)
{
    __asm("mtspr %0, %1": :"i"(PIT),"r"(value): );
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void ppc405cr_clear_programmable_interval_timer_flag (void)
{
    unsigned int temp;

    temp = TSR_ClearPITFlag;
    __asm("mtspr %0, %1": :"i"(TSR),"r"(temp): );
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void ppc405cr_enable_programmable_interval_timer (void)
{
    unsigned int temp;

    temp = TCR_EnablePITWithReload;
    __asm("mtspr %0, %1": :"i"(TCR),"r"(temp): );
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void ppc405cr_disable_programmable_interval_timer(void)
{
    unsigned int temp;

    temp = TCR_DisablePITWithReload;
    __asm("mtspr %0, %1": :"i"(TCR),"r"(temp): );
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void ppc405cr_enable_interrupts(void)
{
    unsigned int temp;

    temp = MSR_EnableExternalInterrupt;
    __asm("mtmsr %0": :"r"(temp): );
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void ppc405cr_disable_interrupts(void)
{
    unsigned int temp;

    temp = MSR_DisableExternalInterrupt;
    __asm("mtmsr %0": :"r"(temp): );
}
//-------------------------------------------------------------------------------

// mfmsr
