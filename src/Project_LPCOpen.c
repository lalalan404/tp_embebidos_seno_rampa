#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "sapi_dac.h"
#include "sapi_systick.h"
#include "sapi_gpio.h"
#include "math.h"
// TODO: insert other include files here
int opcion=0;
int a=0;
uint16_t cont_pasos=0;
uint16_t paso=102;
uint16_t ampl=1023;
uint32_t frec=100000;

int vector[1000];
double y=0;
double x=0;
int c=0;
uint16_t z=0;

// TODO: insert other definitions and declarations here
void inicio_syst_cambio(void);

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif


int tec=0;
gpioInit( 0, 6, GPIO_INPUT );
gpioInit( 0, 7, GPIO_INPUT );
gpioInit( 0, 8, GPIO_INPUT );
gpioInit( 0, 9, GPIO_INPUT );
gpioInit( 0, 0, GPIO_INPUT );
//calculo y parametros de la seno
for(int i=0;i<10;i++){
y=sin(x)+1;
z=(1023*y)/2; // transformacion de la seno a valores de escalones
vector[i]=z;  // vector con valores de seno
x=x+0.628;    //cada 36 grados toma una muestra

dacInit( DAC_ENABLE);
Init_Tick(frec);

}

    while(1) {
         //configuracion de puertos de entrada
    	if(gpioRead(0,6) == 1){tec=1;}
        if(gpioRead(0,7) == 1){tec=2;}
    	if(gpioRead(0,8) == 1){tec=3;}
    	if(gpioRead(0,9) == 1){tec=4;}
    	if(gpioRead(0,0) == 1){tec=5;}

    	switch (tec){
    		case 1: // triangular 10kH-3.3V
    			ampl=1023;
    			paso=102;
    			frec=100000;
    			inicio_syst_cambio();
    			tec=0;
    			a=0;
    			break;
    		case 2: // triangular 1kH-3.3V
    			ampl=1023;
    			paso=102;
    			frec=10000;
    			inicio_syst_cambio();
    			tec=0;
    			a=0;
    			break;
    		case 3: // triangular 10kH-1.65V
    			ampl=512;
    			paso=51;
    			frec=100000;
    			inicio_syst_cambio();
    			tec=0;
    			a=0;
    		    break;
    		case 4: // triangular 1kH-1.65V
    			ampl=512;
    			paso=51;
    			frec=10000;
    			inicio_syst_cambio();
    			tec=0;
    			a=0;
    		    break;
    		case 5: // funcion seno 10khz -3.3v
    			a=1;
    			frec=100000;
    		    inicio_syst_cambio();
    		    tec=0;

    			break;

    			    }

    if(opcion==1){               //construccion de la rampa
    	if(cont_pasos>= ampl){
    		cont_pasos=0;}
    	cont_pasos=cont_pasos +paso;
    	dacWrite( DAC , cont_pasos);
    	opcion=0;
    	       }
    if(opcion==2){              //construccion de la seno
    	if(c==9){
    		c=0; }
    dacWrite( DAC , vector[c]);
    c++;
    }




    }
    return 0 ;
}
void inicio_syst_cambio(void){
	Init_Tick(frec);
}


void SysTick_Handler(void){
	if(a==1){
		opcion=2;
	}
	else opcion=1;
    }

