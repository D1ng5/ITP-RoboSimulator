#ifndef ALGORITHMEN_H_INCLUDED
#define ALGORITHMEN_H_INCLUDED


/*
Liest die Anweisungen f�r den Roboter aus der gegebenen Datei. Bei jedem Aufruf wird genau ein Steuerbefehl abgearbeitet. Ein Beispiel f�r eine solche Datei, ist die Datei "Befehlsdatei-Beispiel.txt".

Setzt programmLaeuft auf 0, wenn das Dateiende erreicht ist, ohne dass eine Schleife eingebaut worden ist.
*/
void ausDatei(char* pfad, int* programmLaeuft);

/*
* Gibt wahr (1) zur�ck, wenn das gegebene Array mit der L�nge laenge, das gegebene Zeichen enthaelt.
*/
int enthaelt(char zeichenArray[], int laenge, char zeichen);

#endif // ALGORITHMEN_H_INCLUDED
