#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

#include "Szenario.h"
#include "loesungsalgorithmen.h"

#if defined(WIN32)
  #include <windows.h>
#elif defined(__UNIX__)
  #include <unistd.h>
#else
#endif



int millisleep(unsigned ms){
#if defined(WIN32)
  SetLastError(0);
  Sleep(ms);
  return GetLastError() ?-1 :0;
#elif defined(LINUX)
  return usleep(1000 * ms);
#else
#error ("no milli sleep available for platform")
  return -1;
#endif
}

// Globale Variable
int programmLaeuft = 1;

void vorEnde(){
    if(istZielErreicht()) {
		printf("Ziel erreicht!\n%i Schritte und %i 90 Grad Drehungen hat der Roboter gemacht.\n",schrittZaehler,drehungsZaehler);
		system("pause");
	} else {
		printf("Das Programm hat sich vor erreichen des Ziels beendet.\n");
		system("pause");
	}
	int i;
    if(szenarioNummer==1){
        for(i=0;i<=schrittZaehler;++i){
            printf("%i ,",speicher1d[i]);
        }
    }
}
/*
    Hilfsfunktionen f�r runSzenario4files;
	Ausnahmsweise in dieser Datei. Ansonsten finden sich solche konkreten Algorithmen in Algorithmen.c/.h
*/
<<<<<<< HEAD
=======

int sucheGespeicherteKoordinaten(int x,int y, char* pfad,int* zeile){
        int i,
            x2,
            y2;
        char *str = malloc(sizeof(char)*150);

        FILE* db = fopen(pfad,"r");
        if(db == NULL){
                printf("Fehler beim laden der Datei: %s\n",pfad);
                return 1;
        }
        // falls die Koordinaten noch nicht gespeichert wurden, schreibe -1 in "zeile"
        *zeile=-1;
        for(i=1;fgets(str,150,db)!= NULL;++i){
            if(i){
                sscanf(str,"%i; %i; %*s ;",&x2,&y2);
                if(x==x2&&y==y2){
                    // falls passende Koordinaten gefunden wurden, schreibe die Zeile "i" in der sie gefunden wurden in "zeile"
                    *zeile=i;
                }
            }
        }
        fclose(db);
        free(db);
        free(str);

        return 0;
}

int anhaengen(char* pfad,char* inhalt){
    FILE* db = fopen(pfad, "a");
    if(db == NULL){
        printf("Fehler bei Dateizugriff!\n");
        return 1;
    }
    fprintf(db, "%s",inhalt);;
    fclose(db);

    free(db);
    return 0;
}

int wieLangIstDatei(char* pfad){
    int len;

    FILE* db = fopen(pfad, "r");
    if(db == NULL){
        printf("Fehler bei Dateizugriff!\n");
        return -1;
    }
    char* str=malloc(sizeof(char)*250);
    for(len=0;fgets(str,250,db)!=NULL;len++);
    fclose(db);

    free(db);
    free(str);
    return len;
}

int einfuegenInZeile(char* pfad,int zeile,char* str){
    int size = wieLangIstDatei(pfad);
    int i,inhaltslaenge=250;
    char** dateiInhalt=malloc(size*sizeof(char*));
    for(i=0;i<size;i++){
        dateiInhalt[i]=malloc(inhaltslaenge*sizeof(char));
    }

    FILE* db = fopen(pfad, "r");
    if(db == NULL){
        printf("Fehler bei Dateizugriff!\n");
        return 1;
    }
    for(i=0;i<size;i++){
        fgets(dateiInhalt[i],inhaltslaenge,db);
    }
    strcpy(dateiInhalt[zeile-1],str);

    fclose(db);
    db = fopen(pfad, "w");
    if(db == NULL){
        printf("Fehler bei Dateizugriff!\n");
        return 1;
    }
    for(i=0;i<size;i++){
        fprintf(db,"%s",dateiInhalt[i]);
    }
    fclose(db);

    for(i=0;i<size;i++){
        free(dateiInhalt[i]);
    }
    free(dateiInhalt);
    free(db);

    return 0;
}

int speichereInDatenbank(char* inhalt,int x,int y,char* pfad){
    int zeile;
    if(sucheGespeicherteKoordinaten(x, y, pfad, &zeile)){
        return 1;
    }
    char* str=malloc(sizeof(char)*250);
    sprintf(str, "%i; %i; %s ;\n",x,y,inhalt);
    if(zeile==-1){
        anhaengen(pfad,str);
    }else{
        einfuegenInZeile(pfad,zeile,str);
    }
    free(str);
    return 0;
}

int holeInhaltAusDatei(char* inhalt,int inhaltLaenge, int x, int y, char* pfad){
    int pos;
    if(sucheGespeicherteKoordinaten(x,y,pfad,&pos)){
        return 1;
    }
    strcpy(inhalt,"F0_N0_O0_S0_W0");
    if(pos!=-1){
        int i;
        FILE* db = fopen(pfad, "r");
        if(db == NULL){
            printf("Fehler bei Dateizugriff!\n");
            return 1;
        }
        for(i=0;i<pos;i++){
            fgets(inhalt,inhaltLaenge,db);
            sscanf(inhalt,"%*i; %*i; %s ;",inhalt);
        }
        fclose(db);
        free(db);
    }
    return 0;
}

void umsehen(char* feldString){
    int blick,
        norden=0,
        osten=0,
        sueden=0,
        westen=0;

    blick=blickrichtung;
    blickrichtung=nord;
    if(frontSensor())norden=1;
    if(rechterSensor())osten=1;
    if(heckSensor())sueden=1;
    if(linkerSensor())westen=1;
    blickrichtung=blick;

    sprintf(feldString,"F1_N%i_O%i_S%i_W%i", norden, osten, sueden, westen);
}

int naechstesFeld(char* datenbank){
    int b=blickrichtung,
        ausgaenge=0,
        feld,
        hinten,
        min,
        res;
    int* fi = malloc(sizeof(int)*8);
    fi[0]=0; fi[1]=9999999;// ausgangNord, Feldwerd nord
    fi[2]=0; fi[3]=9999999;// ausgangWest, Feldwerd west
    fi[4]=0; fi[5]=9999999;// ausgangSued, Feldwerd sued
    fi[6]=0; fi[7]=9999999;// ausgangOst, Feldwerd ost

    char* frontFeld = malloc(sizeof(char)*250);
    char* Feldstr = malloc(sizeof(char)*250);

    if(holeInhaltAusDatei(Feldstr, 250, roboterPosition.x, roboterPosition.y, datenbank)){
        return -1;
    }
    sscanf(Feldstr,"F%i_N%i_O%i_S%i_W%i",&feld, &fi[nord], &fi[ost], &fi[sued], &fi[west]);
    if(fi[nord]){
        blickrichtung=nord;
        if(holeInhaltAusDatei(frontFeld, 250, getFrontPosition().x, getFrontPosition().y, datenbank)){
            return -1;
        }
        sscanf(frontFeld,"F%i_N%*i_O%*i_S%*i_W%*i", &fi[nord+1]);
        ausgaenge++;
    }
    if(fi[ost]){
        blickrichtung=ost;
        if(holeInhaltAusDatei(frontFeld, 250, getFrontPosition().x, getFrontPosition().y, datenbank)){
            return -1;
        }
        sscanf(frontFeld,"F%i_N%*i_O%*i_S%*i_W%*i", &fi[ost+1]);
        ausgaenge++;
    }
    if(fi[sued]){
        blickrichtung=sued;
        if(holeInhaltAusDatei(frontFeld, 250, getFrontPosition().x, getFrontPosition().y, datenbank)){
            return -1;
        }
        sscanf(frontFeld,"F%i_N%*i_O%*i_S%*i_W%*i", &fi[sued+1]);
        ausgaenge++;
    }
    if(fi[west]){
        blickrichtung=west;
        if(holeInhaltAusDatei(frontFeld, 250, getFrontPosition().x, getFrontPosition().y, datenbank)){
            return -1;
        }
        sscanf(frontFeld,"F%i_N%*i_O%*i_S%*i_W%*i", &fi[west+1]);
        ausgaenge++;
    }
    blickrichtung=b;
    hinten = (b+4)%8;

    min=fi[nord+1];
    if(fi[ost+1]<min)min=fi[ost+1];
    if(fi[sued+1]<min)min=fi[sued+1];
    if(fi[west+1]<min)min=fi[west+1];

    //Reihenfolge f�r Bewegung n�chstes Feld liegt:
    if(fi[blickrichtung]&&fi[blickrichtung+1]==min){// vorne
        res=blickrichtung;
    }else{
        drehen(90,0);
        // rechts
        if(fi[blickrichtung]&&fi[blickrichtung+1]==min){
            res=blickrichtung;
        }else{
            drehen(90,0);drehen(90,0);
            b=(b+4)%8;// links
            if(fi[blickrichtung]&&fi[blickrichtung+1]==min){
                res=blickrichtung;
            }else{
                drehen(90,0);drehen(90,0);drehen(90,0);
                // hinten
                if(fi[blickrichtung]&&fi[blickrichtung+1]==min){
                    res=blickrichtung;
                }
            }
        }
    }
    printf("\nAusgaenge: %i, v=%i,h=%i |=>>> res = %i\n",ausgaenge,blickrichtung,hinten,res);
    printf("\n\t\t%i\n\t%i\t%i\t%i\n\t\t%i\n", fi[nord+1], fi[west+1], feld, fi[ost+1], fi[sued+1]);

    //Arrays freigeben
    free(fi);free(frontFeld);free(Feldstr);
    return res;
}


void runSzenario4files(){
    int feld,
        norden,
        osten,
        sueden,
        westen,
        richtungNaechstesFeld;

    char* datenbank = ".\\tmp\\EntscheidungsDatenbank.csv";
    char* feldString = malloc(sizeof(char)*250);
    char* frontFeld = malloc(sizeof(char)*250);

    holeInhaltAusDatei(feldString, 250, roboterPosition.x, roboterPosition.y, datenbank);
    sscanf(feldString,"F%i_N%i_O%i_S%i_W%i",&feld, &norden, &osten, &sueden, &westen);

    // wenn das Feld noch nicht besucht ist, sieh dich um und gehe ein Feld weiter
    if(!feld){
        umsehen(feldString);
        sscanf(feldString,"F%i_N%i_O%i_S%i_W%i",&feld, &norden, &osten, &sueden, &westen);
    }else{
        //falls schon besucht, erh�he den Feld-counter um 1
        feld++;
    }
    //speichere aktuelles Feld
    sprintf(feldString,"F%i_N%i_O%i_S%i_W%i",feld, norden, osten, sueden, westen);
    speichereInDatenbank(feldString,roboterPosition.x,roboterPosition.y,datenbank);

    richtungNaechstesFeld=naechstesFeld(datenbank);

    // berechnet ob man in die entgegengesetzte Richtung gehen soll
    if((4+blickrichtung)%8==richtungNaechstesFeld){
        //wenn der Robotor in einer Sackgasse steckt und nur nach Hinten gehen kann, erh�he den Feld wert noch einmal um Eins,
        //damit nicht im naechsten Feld der Roboter auf die Idee kommt das akt�elle Sackgassenfeld wieder zu besuchen.
        feld++;
        sprintf(feldString,"F%i_N%i_O%i_S%i_W%i",feld, norden, osten, sueden, westen);
        speichereInDatenbank(feldString,roboterPosition.x,roboterPosition.y,datenbank);
    }
    // gucke in die Richtung des n�chsten Feldes
    setBlickrichtung(richtungNaechstesFeld);

	printf("\n x=%i, y=%i feld: %s naechstesFeld %i \n",roboterPosition.x,roboterPosition.y,feldString,richtungNaechstesFeld);
    // und gehe nach vorne
    geheNachVorn(1);
    free(datenbank);free(feldString);free(frontFeld);
}

>>>>>>> aa5fc1d5d6c4dc52d8ed6f220e9ed72fbf7fee9e

void run()
{

//    runSzenario3quickReturn();
//    runSzenario3Karteerstellen();
//    runSzenario3OURL();
//    runSzenario4rightTurn();
//    runSzenario5dotfinder();

    runSzenario4files();


}

int main(int argc, char const *argv[])
{
<<<<<<< HEAD
=======
	//Sollte die Datei nicht vorhanden sein, lege sie mit einer passenden Kopfzeile an
    FILE* datei = fopen(".\\tmp\\EntscheidungsDatenbank.csv", "r");
    if(datei == NULL){
            datei=fopen(".\\tmp\\EntscheidungsDatenbank.csv", "w");
            fprintf(datei, "X; Y; Inhalt ;\n");
    }
    fclose(datei);
    free(datei);
>>>>>>> aa5fc1d5d6c4dc52d8ed6f220e9ed72fbf7fee9e

	int wiederholen=1;
    ladeSzenario(4);
    zeichneKarte();
	if(wiederholen){
		while(!istZielErreicht() && programmLaeuft && !kollision)
		{
			zeichneKarte();
			run();
			nachDurchlauf();
			//system("pause");
			millisleep(500);
		}
	}else{
		run();
	}
	zeichneKarte();
	vorEnde();
    system("pause");
	return 0;
}
