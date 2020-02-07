/*  Kapazitaetsmessgeraet für Powerbanks und Li-Ionen Zellen
 *  Autor: Clemens Verstappen
 *  Sketch steuert einen Messgerät zur Bestimmung der Kapazität (mAh) und Energie (Wh) von USB Powerbanks und Li-Ion Zellen
 *  Hardware: 1 LCD 16/2, ein D/A Umsetzer MCP4911, eine FET IRLZ44 als Stromsenke und vier Taster
 *  Board: Arduino Nano
 *  Version: 1.00  07.03.2019 
 *  Licence: 
 *  This software is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any later version.
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <LiquidCrystal.h>                        // Bibliotheken für das LCD
#include <DAC_MCP49xx.h>                          // Bibliothek des D/A Umsetzers

#define   AD0                 0                   // PIN Arduino Nano, Spannung am Shunt  
#define   AD1                 1                   // PIN Arduino Nano, vom Spannungsteiler der Akkuspannung 
#define   ENTLADE_LED         2                   // LED, die blinkt, während der Entladung
#define   SHUNTWIDERSTAND     1.00                // Shuntwiderstand in Ohm
#define   REFERENZ_SPANNUNG   2.48                // Referenz Spannung für den Controller und D/A Umsetzer, realer Messwert 
#define   SS_PIN              10                  // Arduino Pin des SS_PIN zum D/A Umsetzer
#define   TASTER_I            A2                  // Arduino Pin des Tasters zur Entladestromeinstellung
#define   TASTER_U            A3                  // Arduino Pin des Tasters zur Entladeschlussspannungseinstellung
#define   TASTER_START        A4                  // Arduino Pin des Tasters START/HALT
// #define   TESTPIN             7                   // TESTPIN

LiquidCrystal lcd(8, 9, 6, 5, 4, 3);              // Arduino Nano Pin für RS, E, DB4, DB5, DB6, DB7 des LCDs
DAC_MCP49xx dac(DAC_MCP49xx::MCP4911, SS_PIN);    // init D/A Umsetzer

uint8_t count_i = 4, count_u = 5;
uint16_t tewe_test = 0;
int ad_value;
float shunt_voltage, entladestrom_gemessen, akku_spannung, kapazitaet, energy;        // verschiedene globale Gleitkommavariablen
float array_entladeschlussspannung[5+1] =   {0.0, 0.9, 2.0, 2.5, 2.75, 3.0 };         // Entladeschlussspannungen zur Auswahl
float array_entladestom[5+1] =              {0.0, 0.1, 0.2, 0.50, 1.0, 2.0 };         // Ampere Entladestöme zur Auswahl

unsigned long interval    = 150;                    // periodischer Messzyklus bei der Entladung jede Sekunde, für Tastenabfrage 200 ms
unsigned long prevMillis  = millis();               // reset millis
bool blink = false;                                 // variable der blinkenden LED
bool selekt_flag = true, entladen_flag = false;     // flag, die das Entladen signalisiert

void setup() {
  dac.output(0);                                    // Ladestrom in jedem Fall erst mal auf Null 
  lcd.begin(16,2);                                  // Initialisierung des LC Displays, 16 Zeichen, zwei Zeilen
  pinMode(ENTLADE_LED, OUTPUT);                     // LED, die während der Entladung blinkt  
  pinMode(TASTER_U, INPUT);                         // TASTER_U auf input  
  pinMode(TASTER_U, INPUT_PULLUP);                  // TASTER_U Pull up einschalten am Controller  
  pinMode(TASTER_I, INPUT);                         // TASTER_U auf input  
  pinMode(TASTER_I, INPUT_PULLUP);                  // TASTER_U Pull up einschalten am Controller  
  pinMode(TASTER_START, INPUT);                     // TASTER_U auf input  
  pinMode(TASTER_START, INPUT_PULLUP);              // TASTER_U Pull up einschalten am Controller  
  analogReference(EXTERNAL);                        // externe 2,5 Volt Referenz verwenden
  Serial.begin(9600);                               // serielle einschalten
  Serial.println("Spannung Akku V, Entladestrom A, Kapazität mAh, Energie Wh");     // Text an console
}   

void loop() {                                       // Endlosschleife
 if (millis() - prevMillis > interval) {            // Hier abarbeiten, entweder jede Sekunde und jede 150 ms (bei Tastenabfrage)
   if (selekt_flag) {                               
      lcd.setCursor(0, 0);                          // Curser oben rechts 
      lcd.print("Strom   Spannung");                // obere Zeile  
      lcd.setCursor(0, 1);                          // zweite Zeile 
      lcd.print("I = ");                            // wähle Entladestrom
      lcd.print(array_entladestom[count_i]);        // aktueller Wert den Stroms 
      lcd.print(" U = ");                           // Entladeschlussspannung 
      lcd.print(array_entladeschlussspannung[count_u]);  // aktueller Wert ist 
      
      if (digitalRead(TASTER_I)==LOW)               // wurde Taste I gedrückt?
          count_i++;
      if  (count_i == 6)                            // Werte von 1 bis 5 werden aus dem Array ausgewählt.
          count_i = 1;

      if (digitalRead(TASTER_U)==LOW)               // wurde Taste U gedrückt?
          count_u++;
      if  (count_u == 6)
          count_u = 1;                              // Werte von 1 bis 5 werden aus dem Array ausgewählt.
            
      if (digitalRead(TASTER_START)==LOW){          // Schalte Entladung ein
          for (uint8_t t = 1; t <= count_i; t++) {  
            tewe_test = uint16_t(array_entladestom[t] * 1024 / REFERENZ_SPANNUNG);  // Berechne Wert für D/A Umsetzer
            dac.output(tewe_test);                  // Sende Werte über SPI an den DAC MCP4911 10 Bit DAC converter
            delay(100);                             // zum Einschwingen, sonst u.U. Abschaltung wegen Unterschreitung Limit
          } 
          entladen_flag = true;
          selekt_flag = false;   
          interval = 1000;                          // nun setze das Entladeinterval auf 1 Sekunde
      } 
   } 
   if (entladen_flag) { 
    blink = !blink;                                  // toggle each second
    digitalWrite(ENTLADE_LED, blink);                // blink yellow led on arduino nano board
  
    ad_value = analogRead(AD0);                                 // messe Spannung am Shunt, um dem tatsächsich fließenden Strom zu berechnen
    shunt_voltage = ad_value * REFERENZ_SPANNUNG / 1024;        // Spannung am DA0 entspricht Spannung am Shunt von 1 Ohm
    entladestrom_gemessen = shunt_voltage / SHUNTWIDERSTAND;    // I = U/R = Spannung / LOAD_RESISTOR 

    ad_value = analogRead(AD1);                                 // messe Spannung am Akku 
    akku_spannung = ad_value * 3 * REFERENZ_SPANNUNG / 1024;    // Spannung am Akku, * Spannugsteiler * U-Ref/ 10 Bit
    kapazitaet = kapazitaet +  entladestrom_gemessen / 3.6;     // berechne die Kapazität in mAh
    energy = energy + akku_spannung * entladestrom_gemessen / 3600;   // berechne die Energie in Wh 
      
    if (akku_spannung <= array_entladeschlussspannung[count_u]) {     // Tiefentladeschutz bei Li-Zellen. Abschaltung bei Powerbanks
       akku_spannung = 0.0; 
       entladestrom_gemessen = 0.0;
       digitalWrite(ENTLADE_LED, LOW);                // blink LED AUS
       entladen_flag = false;                         
       dac.output(0);                                 // Setze Ausgang des A/D-Umsetzers MCP4911 auf Null
    }
    Serial.print(akku_spannung, 2);                   // Sende Werte an das Terminal
    Serial.print("             ");                     
    Serial.print(entladestrom_gemessen, 2);          
    Serial.print("            ");           
    Serial.print(kapazitaet, 1);                      
    Serial.print("           ");           
    Serial.print(energy, 2);                          
    Serial.println(" ");                               

    lcd.setCursor(0, 0);                              // set curser on lcd 
    lcd.print(akku_spannung,2);                       // print battery voltage on the second line left 
    lcd.print(" V ");                                 // print battery voltage on the second line left 
    lcd.print(entladestrom_gemessen,2);               // print battery voltage on the second line left 
    lcd.print(" A   ");                               // print battery voltage on the second line left 

    lcd.setCursor(0, 1);                              // set curser on lcd 
    lcd.print(kapazitaet,0);                          // print battery voltage on the second line left 
    lcd.print(" mAh ");                               // print battery voltage on the second line left 
    lcd.print(energy,1);                              // print battery voltage on the second line left 
    lcd.print(" Wh    ");                             // print battery voltage on the second line left 
    }
  prevMillis = millis();                              // reset millis 
  }
}
