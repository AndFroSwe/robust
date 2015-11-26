// Långlab Mikro, Kurs Elektroteknik

#include "mik.h"

char prog[] = "Mikro-lab";              // Textsträng med programmets namn
int ver     = 1;                        // *** ÖKA gärna numret för varje version ni testkör! ***

int main(void)                          // Själva programslingan 
{
    init_mik();                         // Initiera hårdvaran

    move_cursor( 1, 5 );                // Displaymarkören till rad 1, kolumn 5
    dprintf( "%s v.%i", prog, ver );    // Skriv ut programmets namn och version

    /*********************************
    **                              **
    ** Vartefter som ni behöver     **
    ** variabler deklarerar ni dem  **
    ** här nedanför.                **
    **                              **
    *********************************/
    

    
    /*********************************
    **                              **
    ** Konfigurera GPIO som in- och **
    ** utgångar här nedanför med    **
    ** funktionen init_pin.         **
    **                              **
    *********************************/




    while( 1 )                          // Evighetsloop
    {











    }
}                                        // Programslingans slut
