//*****************************************************************************************************************************//
//-------------------          By  MastrMarco    Versione 1.0        ISCRIVITI AL CANALE YOUTUBE       ------------------------//
//
//                                  https://www.youtube.com/channel/UCpQb1Iz6M229ylkyXpdJPlw
//*****************************************************************************************************************************//
/* Libreria per DisplayOled_I2C  */
#include <Arduino.h>
#include <Wire.h>
#include <MicroLCD.h>
LCD_SSD1306 lcd;
//--------------------------------------
int PrecisioneDecimale = 2;             // Posizioni decimali ES:(0.00) per tutti i valori mostrati nel monitor seriale 

int TermoresistenzaPin = A1;            // Il pin di ingresso analogico per la misurazione della temperatura
float PartitoreTensioneR2 = 10000;      // Valore del resistore R2 del partitore di tensione  
float BValue = 3470;                    // Il "BValue" del termistore per l'intervallo di misurazione della temperatura
float R1 = 1000;                        // Valore nominale del resistore del termistore (NTC) alla temperatura di base (25 gradi centigradi) Lo schema é stato testato con 1K ohm, 5K ohm, 10K ohm (Vaolore NOMINALE)
float T1 = 298.15;                      // Temperatura base "T1" in Kelvin (l'impostazione predefinita dovrebbe essere di 25 gradi )
float R2 ;                              // Resistenza del termistore (in ohm) alla temperatura di misurazione 
float T2 ;                              // Temperatura misurata "T22 in Kelvin

float a ;                               /* Utilizzare per il calcolo in temperatura */
float b ;                               /* Utilizzare per il calcolo in temperatura */
float c ;                               /* Utilizzare per il calcolo in temperatura */
float d ;                               /* Utilizzare per il calcolo in temperatura */
float e = 2.718281828 ;                 // Il valore "e" utilizzare per il calcolo in Temperatura

float tempSampleRead  = 0;              // Per leggere il valore di un campione incluso il valore tempSampleSum 
float tempSampleSum   = 0;              // Accumulo di letture di campioni 
float tempMean ;                        // Per calcolare il valore medio di tutti i campioni, in valori analogici
 
//-----------------------------------------------------Delay non Bloccante-----------------------------------------------------//              
float tempLastSample  = 0;                              
float tempSampleCount = 0;                                        

void setup() {
  Serial.begin(9600);
  lcd.begin();                          // Avvio comunicazione con il Display
}

void loop(){                                      /* Misura della temperatura  */  
          
  if (millis() >= tempLastSample + 1){                                    /* Funzione Delay non Bloccante */
    tempSampleRead = analogRead(TermoresistenzaPin);                      // legge il valore analogico dal sensore
    tempSampleSum = tempSampleSum + tempSampleRead;                       // Aggiunge tutto il valore analogico per calcolare la media in seguito
    tempSampleCount = tempSampleCount + 1;                                /* Funzione Delay non Bloccante*/
    tempLastSample = millis();                                            /* Funzione Delay non Bloccante*/
  }

  if (tempSampleCount == 1000){                                           /* Funzione Delay non Bloccante*/
    tempMean = tempSampleSum / tempSampleCount;                           // Trova il valore analogico medio da questi dati
    R2 = (PartitoreTensioneR2 * tempMean) / (1023 - tempMean);            // Convertire il valore analogico medio in valore di resistenza

    a = 1 / T1;                                                           /* Utilizzare per il calcolo in temperatura */
    b = log10(R1 / R2);                                                   /* Utilizzare per il calcolo in temperatura */
    c = b / log10(e);                                                     /* Utilizzare per il calcolo in temperatura */
    d = c / BValue ;                                                      /* Utilizzare per il calcolo in temperatura */
    T2 = 1 / (a - d);                                                     // Il valore di temperatura misurato in base al calcolo (in Kelvin)
    Serial.print(T2 - 273.15, PrecisioneDecimale);                        // Visualizza nel Serial monitorare la temperatura in Celcius 
    Serial.println("°C");

    tempSampleSum = 0;                                                    // Resettare tutto il valore analogico totale a 0 per il conteggio successivo
    tempSampleCount = 0;                                                  // Reimpostare il numero totale di campioni prelevati a 0 per il conteggio successivo 

  /* Display_OLED  */
  //lcd.clear();
  lcd.setCursor(35, 11);
  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.print(T2 - 273.15, PrecisioneDecimale);
  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.println("C");
  }
}
