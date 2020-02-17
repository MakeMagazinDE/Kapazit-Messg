![GitHub Logo](http://www.heise.de/make/icons/make_logo.png)

Maker Media GmbH

***

# Akku-Kapazitäts-Messgerät

### für Li-Ion- und NiMH-Zellen, Make 1/2020 S. 100

![Picture](https://github.com/MakeMagazinDE/Kapazit-Messg/blob/master/kapmess_gh.JPG)

R5..R7 sind für blaue Displays mit weißer Hintergrundbeleuchtung bemessen. Ggf. ist R5 bei unzureichender Kontrast-Einstellbarkeit kleiner zu wählen (2k2..4k7). R6 und R7 sind so zu bemessen, dass der Nennstrom der Hintergrundbeleuchtung nicht überschritten wird.

R13 ist nur bei erhöhten Anforderungen an die Genauigkeit erforderlich (Referenzspannung auf +2,50V gegenüber Masse an PL3 einstellen).

Mit dem billigen OpAmp LM358 für U5 wird der maximal einstellbare Strom (2A) nicht ganz erreicht (nur ca. 1,5A). Wenn Sie Entladeströme bis 2A benötigen, wählen Sie für U5 den teureren TLC272.

Q1 benötigt einen Kühlkörper für TO220 mit weniger als 10 K/W, beispielsweise die Gehäuse-Seitenwand. Für höhere Verlustleistungen können auch zwei MOSFETs parallel geschaltet werden, wenn in jede Gate-Leitung ein 1k-Widerstand eingefügt wird (siehe R11 im Schaltplan).

Für die Genauigkeit der Messung ist R10 entscheidend. Statt eines teuren Präzisionswiderstands (Lötpads hierfür sind vorhanden, z.B. für Widerstände mit 4-Leiter-Anschluss von Isabellenhütte) bestellen Sie einfach mehrere 1R-Widerstände und wählen den genauesten aus. Am besten eignet sich hierfür ein Tischmultimeter mit 4-Leiter-Messung.

### Stückliste

	Halbleiter
	U1  Arduino Nano oder kompatibel
	U2  LCD 2x16 Zeichen, beleuchtet
	U3  MCP4911 EP DIL (Microchip, Reichelt MCP 4911-E/P)
	U4  LM336-2,5
	U5  TLC272 DIP oder LM358 DIP (siehe Text)
	Q1  IRLZ44N oder IRLIZ44 (isol. Gehäuse)
	LED1  LED 3mm, grün
  
	Widerstände
	R1, R5, R8, R12 10k 1%
	R2  470R
	R3  Trimmer 10x5mm, 2k2 oder 2k5 (Reichelt PT 10-L 2,5K)
	R4  20k 1%
	R6, R7  100R (siehe Text)
	R9  2k2
	R10	1R 5W axial (siehe Text)
	R11 1k
	R13 Präz. Trimmer stehend, 10k (siehe Text, Reichelt 64W-10K)
  
	Kondensatoren
	C1, C3, C6  100n 50V ker., RM 5,08
	C2  47µ/16V
	C4, C5  10µ/35V
	C7  1n 100V RM 5,08
  
	Sonstiges
	PL1 USB-A Buchse, Print (Reichelt USB AW)
	PL2 Anschlussklemme 2pol. steckbar, RM 5,08 (Reichelt AKL 249-02 und AKL 230-02)
	SW1..SW4 Print-Taster DT6, ITT Schadow (Reichelt DT 6)
  
  
  
  
  
  
