#include <stdio.h>
#include "type.h"
#include <msp430.h>



/*******************************************************************************
 Func Name    :
 Arguments    :
 Return       :
 Description  :   
*******************************************************************************/
void Scan_Buttons(void)
{
  Scan_Rotary_Button();
  Scan_Touch_Button();

}
/*******************************************************************************
 Func Name    :
 Arguments    :
 Return       :
 Description  :   
*******************************************************************************/
void Scan_Rotary_Button(void)
{
   static volatile unsigned char pin,prev_pin;
   static volatile unsigned long break_timer=0;
   static volatile unsigned char btn_state;

   pin = ((P2IN & BIT2)?1u:0u);

   if(!pin && prev_pin)
   {
      //button pressed
      break_timer=0;
      btn_state = 1;
   }
   else if(!pin && !prev_pin)
   {
     //button pressed low
      if(btn_state == 1)
      {
        break_timer++; 
      }
   }
   else if(pin && !prev_pin)
   {
     //button release
     if(break_timer > 0)
     {
        btn_state = 2;
     }       
   }
   else if(pin && prev_pin)
   {
     //released & normal run
     if((btn_state == 2) && (break_timer > 0u) && (break_timer <= BREAK_TIMEOUT))//short press
     {
        break_timer = 0;
        btn_state = 0;
        Radio_Enc_Button(SHORT_PRESS);
     }
     else if((btn_state == 2) && (break_timer > BREAK_TIMEOUT))//long press
     {
        break_timer = 0;
        btn_state = 0;
        Radio_Enc_Button(LONG_PRESS);
     }
   }
   prev_pin = pin;
}
/*******************************************************************************
 Func Name    :
 Arguments    :
 Return       :
 Description  :   
*******************************************************************************/
void Scan_Touch_Button(void)
{
   static volatile unsigned char pin1,prev_pin1;
   static volatile unsigned long break_timer1=0;
   static volatile unsigned char btn_state1;

   pin1 = !((P2IN & BIT3)?1u:0u);

   if(!pin1 && prev_pin1)
   {
      //button pressed
      break_timer1=0;
      btn_state1 = 1;
   }
   else if(!pin1 && !prev_pin1)
   {
     //button pressed low
      if(btn_state1 == 1)
      {
        break_timer1++; 
      }
   }
   else if(pin1 && !prev_pin1)
   {
     //button release
      if(break_timer1 > 0)
      { 
         btn_state1 = 2;
      }         
   }
   else if(pin1 && prev_pin1)
   {
     //released & normal run
     if((btn_state1 == 2) && (break_timer1 > 0u) && (break_timer1 <= BREAK_TIMEOUT1))//short press
     {
        break_timer1 = 0;
        btn_state1 = 0;
        Radio_Touch_Button(SHORT_PRESS);
     }
     else if((btn_state1 == 2) && (break_timer1 > BREAK_TIMEOUT1))//long press
     {
        break_timer1 = 0;
        btn_state1 = 0;
        Radio_Touch_Button(LONG_PRESS);
     }
   }
   prev_pin1 = pin1;
}
