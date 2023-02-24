/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F18875
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
int valor_adc = 0;
int P1_ = 0;
int P2_ = 0;

int uni, dec, cen, unm;
float temp_lm35 = 0.0;
float P11 = 0.0;
float P22 = 0.0;

int x, tiempo;
int valor_pwm = 0;
int p1=0;
int p2=0;
int c=0;
int r=0;



// D0=a    ...   D6=g    D7=punto
uint8_t numeros[10] = {0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000011, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10011000, // 9
};

/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    tiempo = 0;
    // 9bits de PWM = 511
    // 20 es 0 grados
    
    
    
    
    //Pin RA4 del potenciometro 
    
     PWM6_LoadDutyValue(15);
    __delay_ms(1000);
     PWM6_LoadDutyValue(20);
    __delay_ms(1000);
      PWM6_LoadDutyValue(30);
    __delay_ms(1000);



    ADCC_DisableContinuousConversion();

    while (1) {
        

         
        // Add your application code
        //tiempo = tiempo + 20;
        // adc -> 10bits. Valor max. 1023
         
        P1_ = ADCC_GetSingleConversion(P1);
        P2_ = ADCC_GetSingleConversion(P2);
        valor_adc = ADCC_GetSingleConversion(LM35);
       
        //convertimos de float a int
        
         P11= ((((float) P1_)));
         P22= ((((float) P2_)));
         temp_lm35 = ((((float) valor_adc)));



        // si pulsante P1 pin RA4 es presionado  incrementa en 15
        if (P11>0){
            p1=p1+15;
            valor_pwm=p1;
            __delay_ms(1000);
        }
        // si pulsante P2 pin RA5 es presionado  decrementa en 5
       
         if (P22>0){
            p2=p2-5;
            valor_pwm=p2;
            __delay_ms(1000);
        }
        
        //condicion para que no sea menor a cero
        if(p1<0){
            p1=10;
            valor_pwm=p1;
        }
        
          if(p2>255){
            p2=255;
            valor_pwm=p2;
        }
        // el valor es almacenado en el PWM6 PIN RA7
        PWM6_LoadDutyValue(valor_pwm);
        
           
        
       // si el potenciometro varia de 0 a 1024 realiza
       // a siguiente condicion 
        
        if (temp_lm35 >= 100 & temp_lm35 <=512) { // cada 1000 ms = 1 seg
        
            L1_SetHigh();
            L2_SetLow();
            L3_SetLow();
       }
     
       if (temp_lm35 >= 513 & temp_lm35 <=800) { // cada 1000 ms = 1 seg
        
            L1_SetLow();
            L2_SetHigh();
            L3_SetLow();
       }
        
      if (temp_lm35 >= 800 & temp_lm35 <=1024) { // cada 1000 ms = 1 seg
        
            L1_SetLow();
            L2_SetLow();
            L3_SetHigh();
       }
        

        // la siguiente variable x toma el valor pwm del servo motor
        x = valor_pwm; 
       
        unm = (x / 1000); // 1234/1000 = 1
        cen = (x - unm * 1000) / 100; // (1234 - 1*1000)/100 = 2
        dec = (x - unm * 1000 - cen * 100) / 10; //(1234 - 1*1000 - 2*100)/10
        uni = (x - unm * 1000 - cen * 100 - dec * 10);
        
        
        
    
            for (int i = 0; i < 15; i++) {
            DIS_U_SetHigh();
            DIS_D_SetLow();
            DIS_C_SetLow();
            DIS_UM_SetLow();
            LATD = numeros[uni];
            __delay_ms(4);

            DIS_U_SetLow();
            DIS_D_SetHigh();
            DIS_C_SetLow();
            DIS_UM_SetLow();
            LATD = numeros[dec];
            __delay_ms(5);

            DIS_U_SetLow();
            DIS_D_SetLow();
            DIS_C_SetHigh();
            DIS_UM_SetLow();
            LATD = numeros[cen];
           // LATDbits.LATD7 = 0; // para prender el punto
            __delay_ms(5);

            DIS_U_SetLow();
            DIS_D_SetLow();
            DIS_C_SetLow();
            DIS_UM_SetLow();
            LATD = numeros[unm];
            __delay_ms(5);
        
        
        }
            
        
        
        
        
    }
}
/**
 End of File
 */