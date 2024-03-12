/*
 * keyboard.c
 *
 *  Created on: Mar 9, 2024
 *      Author: cedricpurwin
 */
#include "keyboard.h"

static SPI_HandleTypeDef *keyboard_hspi=NULL;   //SPI-Handler zunaechst NULL, wird in der init gesetzt.
uint8_t key=0;				   					// key zunächst 0 (keine Taste)
static uint8_t lastkey=0, keyrate=0xff;	        // Abtasrate auf max (255ms)

/*  keyboard_init : Uebergibt den SPI-Handler und die Abtastrate (Lesezykus) in ms
 */
void keyboard_init(SPI_HandleTypeDef *hspi, uint8_t ms_rate)
{
	keyboard_hspi=hspi;		// SPI-Handler setzen
    keyrate=ms_rate; 		// Leserate setzen
}

/*  get_key_1ms :  Zyklisches einlesen des Tasturwertes ueber SPI.
 *                 Die Zykluszeit bestimmt die gl.Variable keyrate
 *
 * 	Rückgabe : ueber gloable varaible key
 */
void get_key_1ms(void)
{
	static uint8_t ms_counter=0;
	uint8_t i;
    uint16_t spi_val;

    ms_counter++;
    if(ms_counter>keyrate) // Tasten erst lesen wenn Zeit erreicht
    {
    	ms_counter=0;
		if(NULL!= keyboard_hspi) // ist Handler initiert?
		{
			HAL_SPI_Receive(keyboard_hspi,(uint8_t*) &spi_val, 1,50); //Tastaturwert 1x16Bit lesen, mit 50ms timeout
			key=0;
			for(i=0;i<16;i++) 			// Taste heraus filtern
				if((~spi_val)&(1<<i))
					key=i+1;
		}
		if (0==key)
			lastkey=0;	// Taste wurde losgelassen
    }
}

/*  is_key :  Abfrage wurde eine neue Taste gedrueckt?
 *            Der Tastenwert wird nur einmal zurueckgegeben bis wieder losgelassen.
 *
 * 	Rückgabe : Tastenwert (1-16) oder 0 (keine Taste gedrückt / oder auch wenn Taste festgehalten)
 */
uint8_t is_key(void)
{

  uint8_t k;

   if ((key!=0) && (lastkey==0))    //erstes mal? , dann k ausgeben
	  k = key;
  else 							   //sonst 0
	  k=0;
  lastkey=key;
  return k;
}
