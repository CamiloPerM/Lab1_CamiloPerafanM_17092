/*
 * File:   Lab1.c
 * Author: Camilo Perafán Montoya
 *
 * Created on 22 de enero de 2020, 06:11 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 4000000


#include <xc.h>

//********************************************************************************************************
// Prototipos de Funciones y Declaración de Variables
//********************************************************************************************************

void init (void);
char J1;
char puertoJ1;
char J2;
char puertoJ2;
char BTNJ1;
char BTNJ2;
char BTNIN;

//********************************************************************************************************
// Función Principal
//********************************************************************************************************

void main(void) {
    init(); // Llama a la función que configura las entradas y salidas del PIC
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    J1 = 0;
    puertoJ1 = 0; 
    J2 = 0;
    puertoJ2 = 0; 
    BTNJ1 = 0; 
    BTNJ2 = 0;
    BTNIN = 0; //Se inicializan los puertos y las variables en 0
    
    unsigned int seg[] ={ // Configura los valores que se muestran en el display de 7 segmentos
        0X3F, // Representa el 0
        0X06, // Representa el 1
        0X5B, // Representa el 2
        0X4F, // Representa el 3
    };
    
    unsigned int cont[] ={ //Configura los valores para el contador de decadas
        0b00000000,
        0b10000000,
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001,
    };
    
    if (PORTAbits.RA7 == 1){ // Si el pushbutton ubicado en el Pin A6 se oprime haga lo siguinte
                             // Si no, mantenga todo en 0
            BTNIN = 1; //Prenda la bandera de que se oprimio este botón
            __delay_ms(5); //Espere 5 ms (Para darle tiempo al programa de ajustarse)
                 
            if (PORTAbits.RA7 == 0 && BTNIN == 1) { //Pregunta si el botón ya se soltó y si la bandera está encendida, cumpliendo una función antirrebote     
                PORTAbits.RA0 = 1; // Prenda la LED ubicada en el pin A0
                PORTD = seg[3]; // Muestre el número 3 en el display
                __delay_ms(1000); // Espere 1 segundo

                PORTAbits.RA0 = 0; // Apague la LED ubicada en el pin A0
                PORTAbits.RA1 = 1; // Prenda la LED ubicada en el pin A1
                PORTD = seg[2]; // Muestre el número 2 en el display
                __delay_ms(1000); // Espere 1 segundo

                PORTAbits.RA1 = 0; // Apague la LED ubicada en el pin A1
                PORTAbits.RA2 = 1; // Prenda la LED ubicada en el pin A2
                PORTD = seg[1]; // Muestre el número 1 en el display
                __delay_ms(1000); // Espere 1 segundo

                PORTAbits.RA0 = 1; // Prenda la LED ubicada en el pin A0
                PORTAbits.RA1 = 1; // Prenda la LED ubicada en el pin A1
                PORTD = seg[0]; // Muestre el número 0 en el display
               __delay_ms(500); // Espere 500 ms
               
               PORTD = 0; // Apague el Display
               PORTAbits.RA0 = 0; // Apague la LED ubicada en el pin A0
               PORTAbits.RA1 = 0; // Apague la LED ubicada en el pin A1
               PORTAbits.RA2 = 0; // Apague la LED ubicada en el pin A2
               
               Juego: //Se usa un label para que cuando termine la cuenta regresiva se entre a otro estado
               
               //Jugador 1
               if (PORTAbits.RA6 == 0){ //Si el pushbutton en A6 no es presionado, mande el valor guardado
                                        //en la variable puertoJ1 a PORTC
                   PORTC = puertoJ1;
                   
               }else{ //Si el pushbutton se oprime
                   BTNJ1 = 1; //Prenda la bandera de que se oprimio este botón
                   __delay_ms(5); //Espere 5 ms (Para darle tiempo al programa de ajustarse)
                   
                   if (PORTAbits.RA6 == 0 && BTNJ1 == 1){ //Pregunta si el botón ya se soltó y si la bandera está encendida, cumpliendo una función antirrebote
                        J1 = J1 + 1; //La variable J1 se inicializa para que cada vez que se oprima el pushbutton, esta vaya subiendo de a 1 su valor
                        puertoJ1=cont[J1]; //El valor que tenga J1 se mapea en el array cont y se guarda en la variable puertoJ1
                        BTNJ1 = 0; //Se limpia la bandera
                        
                   if (J1 >= 8 & J1>J2){ //Cuando la variable J1 llegue a la ultima posición del array y esta sea mayor que la variable J2
                       J1 = 0; //Reinicie el valor de la variable J1 a cero
                       PORTEbits.RE0 = 1; //Encienda la LED en el pin E0, indicando que J1 es el ganador
                       PORTC = 0; //Se limpia PORTC
                       PORTCbits.RC0 = 1; // Se enciende el ultimo LED del conteo de J1
                       PORTD = seg[1]; //Muestra en el display que el jugador 1 ganó
                       __delay_ms(3000); // Se esperan 3 segundos
                       return; // Se reinicia automaticamente el juego

                   }
                   }
               }
               
               //Jugador 2
               if (PORTAbits.RA5 == 0){ //Si el pushbutton en A5 no es presionado, mande el valor guardado
                                        //en la variable puertoJ2 a PORTB
                   PORTB = puertoJ2;
                   
               }else{ //Si el pushbutton se oprime
                   BTNJ2 = 1; //Prenda la bandera de que se oprimio este botón
                   __delay_ms(5); //Espere 5 ms (Para darle tiempo al programa de ajustarse)
                   
                   if (PORTAbits.RA5 == 0 && BTNJ2 == 1){ //Pregunta si el botón ya se soltó y si la bandera está encendida, cumpliendo una función antirrebote
                        J2 = J2 + 1; //La variable J2 se inicializa para que cada vez que se oprima el pushbutton, esta vaya subiendo de a 1 su valor
                        puertoJ2=cont[J2]; //El valor que tenga J2 se mapea en el array cont y se guarda en la variable puertoJ2
                        BTNJ2 = 0; //Se limpia la bandera
                        
                   if (J2 >= 8 & J2>J1){ //Cuando la variable J2 llegue a la ultima posición del array y esta sea mayor que la variable J1
                       J2 = 0; //Reinicie el valor de la variable J2 a cero
                       PORTEbits.RE1 = 1; //Encienda la LED en el pin E1, indicando que J2 es el ganador
                       PORTB = 0; //Se limpia PORTB
                       PORTBbits.RB0 = 1; //Se enciende el ultimo LED del conteo de J2
                       PORTD = seg[2]; //Muestra en el display que el jugador 2 ganó
                       __delay_ms(3000); //Se esperan 3 segundos
                       return; //Se reinicia automaticamente el juego
                       
                   }
                   }
               }
               
               goto Juego; //Vuelva otra vez al label juego
            }    
        }
     
    return;
}


//********************************************************************************************************
//Función de Inicialización de Puertos
//********************************************************************************************************

void init (void){
    TRISAbits.TRISA0 = 0; // PIN A0 de PORTA configurado como salida
    TRISAbits.TRISA1 = 0; // PIN A1 de PORTA configurado como salida
    TRISAbits.TRISA2 = 0; // PIN A2 de PORTA configurado como salida
    TRISAbits.TRISA3 = 0; // PIN A3 de PORTA configurado como salida
    TRISAbits.TRISA4 = 0; // PIN A4 de PORTA configurado como salida
    TRISAbits.TRISA5 = 1; // PIN A5 de PORTA configurado como entrada
    TRISAbits.TRISA6 = 1; // PIN A6 de PORTA configurado como entrada
    TRISAbits.TRISA7 = 1; // PIN A7 de PORTA configurado como entrada
    TRISD = 0; // PORTD configurado como salida
    TRISC = 0; // PORTC configurado como salida
    TRISB = 0; // PORTB configurado como salida
    TRISE = 0; // PORTE configurado como salida
    ANSEL = 0; // Pines connfigurados como entradas digitales
    ANSELH = 0; // Pines connfigurados como entradas digitales

}
