/******************************************************************************
 * @file:    system_LPC11xx.c
 * @purpose: CMSIS Cortex-M0 Device Peripheral Access Layer Source File
 *           for the NXP LPC11xx Device Series 
 *----------------------------------------------------------------------------*/
 
#include "type.h"
#include "LPC11xx.h"

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*--------------------- Clock Configuration ----------------------------------
//
// <e> Clock Configuration
//   <h> System Controls and Status Register (SCS)
//     <o1.4>    OSCRANGE: Main Oscillator Range Select
//                     <0=>  1 MHz to 20 MHz
//                     <1=> 15 MHz to 24 MHz
//     <e1.5>       OSCEN: Main Oscillator Enable
//     </e>
//   </h>
//
//   <h> Clock Source Select Register (CLKSRCSEL)
//     <o2.0..1>   CLKSRC: PLL Clock Source Selection
//                     <0=> Internal RC oscillator
//                     <1=> Main oscillator
//                     <2=> RTC oscillator
//   </h>
//
//   <e3> PLL0 Configuration (Main PLL)
//     <h> PLL0 Configuration Register (PLL0CFG)
//                     <i> F_cco0 = (2 * M * F_in) / N
//                     <i> F_in must be in the range of 32 kHz to 50 MHz
//                     <i> F_cco0 must be in the range of 275 MHz to 550 MHz
//       <o4.0..14>  MSEL: PLL Multiplier Selection
//                     <6-32768><#-1>
//                     <i> M Value
//       <o4.16..23> NSEL: PLL Divider Selection
//                     <1-256><#-1>
//                     <i> N Value
//     </h>
//   </e>
//
//   <e5> PLL1 Configuration (USB PLL)
//     <h> PLL1 Configuration Register (PLL1CFG)
//                     <i> F_usb = M * F_osc or F_usb = F_cco1 / (2 * P)
//                     <i> F_cco1 = F_osc * M * 2 * P
//                     <i> F_cco1 must be in the range of 156 MHz to 320 MHz
//       <o6.0..4>   MSEL: PLL Multiplier Selection
//                     <1-32><#-1>
//                     <i> M Value (for USB maximum value is 4)
//       <o6.5..6>   PSEL: PLL Divider Selection
//                     <0=> 2
//                     <1=> 4
//                     <2=> 8
//                     <3=> 16
//                     <i> P Value
//     </h>
//   </e>
//
//   <h> CPU Clock Configuration Register (CCLKCFG)
//     <o7.0..7>  CCLKSEL: Divide Value for CPU Clock from PLL0
//                     <0-255>
//                     <i> Divide is CCLKSEL + 1. Only 0 and odd values are valid.
//   </h>
//
//   <h> USB Clock Configuration Register (USBCLKCFG)
//     <o8.0..3>   USBSEL: Divide Value for USB Clock from PLL1
//                     <0-15>
//                     <i> Divide is USBSEL + 1
//   </h>
//
// </e>
*/

#define MAIN_PLL_SETUP        1
#define MAIN_CLKSRCSEL_Val    0x00000000
#define MAIN_PLL_M_Val        0x00000003
#define MAIN_PLL_P_Val        0x00000001
#define SYS_AHB_DIV_Val       1         /* 1 through 255, typical is 1 or 2 or 4 */


/*
//-------- <<< end of configuration section >>> ------------------------------
*/

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL_OSC    ( 4000000UL)        /* Internal RC oscillator frequency   */
#define IRC_OSC     ( 12000000UL)        /* Internal RC oscillator frequency   */
#define WDT_OSC     (  250000UL)        /* WDT oscillator frequency           */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t ClockSource = IRC_OSC;
uint32_t SystemFrequency = IRC_OSC; /*!< System Clock Frequency (Core Clock)  */
uint32_t SystemAHBFrequency = IRC_OSC;

/**
 * Misc. clock generation modules
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void Main_PLL_Setup ( void )
{
  uint32_t regVal;

  ClockSource = IRC_OSC;
  LPC_SYSCON->SYSPLLCLKSEL = MAIN_CLKSRCSEL_Val;   /* Select system OSC */
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;                 /* Update clock source */
  LPC_SYSCON->SYSPLLCLKUEN = 0x00;                 /* toggle Update register once */
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;
  while ( !(LPC_SYSCON->SYSPLLCLKUEN & 0x01) ); /* Wait until updated */

  regVal = LPC_SYSCON->SYSPLLCTRL;
  regVal &= ~0x1FF;
  LPC_SYSCON->SYSPLLCTRL = (regVal | (MAIN_PLL_P_Val<<5) | MAIN_PLL_M_Val);
  
  /* Enable main system PLL, main system PLL bit 7 in PDRUNCFG. */
  LPC_SYSCON->PDRUNCFG &= ~(0x1<<7);
  while ( !(LPC_SYSCON->SYSPLLSTAT & 0x01) );   /* Wait until it's locked */

  LPC_SYSCON->MAINCLKSEL = 0x03;      /* Select PLL clock output */
  LPC_SYSCON->MAINCLKUEN = 0x01;      /* Update MCLK clock source */
  LPC_SYSCON->MAINCLKUEN = 0x00;      /* Toggle update register once */
  LPC_SYSCON->MAINCLKUEN = 0x01;
  while ( !(LPC_SYSCON->MAINCLKUEN & 0x01) );   /* Wait until updated */

  LPC_SYSCON->SYSAHBCLKDIV = SYS_AHB_DIV_Val;   /* SYS AHB clock, typical is 1 or 2 or 4 */

#if MAIN_PLL_SETUP
  SystemFrequency = ClockSource * (MAIN_PLL_M_Val+1);//12mhz irc = 48mhz system freq
#else
  SystemFrequency = ClockSource;
#endif
  SystemAHBFrequency = (uint32_t)(SystemFrequency/SYS_AHB_DIV_Val);//same as 48mhz from irc
  return;
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void SystemInit (void)
{
  uint32_t i;

#ifdef __DEBUG_RAM    
  LPC_SYSCON->SYSMEMREMAP = 0x1;      /* remap to internal RAM */ 
#else
#ifdef __DEBUG_FLASH    
  LPC_SYSCON->SYSMEMREMAP = 0x2;      /* remap to internal flash */
#endif
#endif

  /* bit 0 default is crystal bypass, 
  bit1 0=0~20Mhz crystal input, 1=15~50Mhz crystal input. */
  LPC_SYSCON->SYSOSCCTRL = 0x00;

  /* main system OSC run is cleared, bit 5 in PDRUNCFG register */
  LPC_SYSCON->PDRUNCFG &= ~(1<<5);
  /* Wait 200us for OSC to be stablized, no status 
  indication, dummy wait. */
  for ( i = 0; i < 0x100; i++ );

  Main_PLL_Setup();  



  /* System clock to the IOCON needs to be enabled or
  most of the I/O related peripherals won't work. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
  return;
}
