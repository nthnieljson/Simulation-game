#include "pengunjung.h"
#include <stdlib.h>

Pengunjung GeneratePengunjung (TabWahanaPlayer T){
/* Mengenerate seorang pengunjung dengan melakukan randomisasi
jumlah wahana dan wahana apa saja yang dinaiki pengunjung tersebut */

    Pengunjung P;

    //Random jumlah wahana yang dinaiki pengunjung
    int NWahana = (rand() %3) + 1;
    
    //Random Wahana yang didapatkan
    int i;
    for (i=0; i<NWahana; i++){
        int idxWahanaRandom = (rand() % NbElmtListWP(T));
        ElmtWPengunjung(P, i) = Elmt(T, idxWahanaRandom).NamaW;
    }

    Prio(P) = 5;
    TotalWPeng(P) = NWahana;
    CurrWPeng(P) = 0;
    Kesabaran(P) = 5;

    return P;

}

void PrintPengunjung(Pengunjung P){
    /* Mencetak Wahana apa saja yang ingin dinaiki pengunjung dan tingkat kesabarannya */
    int i;
    printf("(");
    for (i=0; i< TotalWPeng(P); i++){
        PrintKata(ElmtWPengunjung(P, i));
        
        if (i!=TotalWPeng(P)-1){
            printf(",");
        }
    }
    printf("), kesabaran: %d\n", Kesabaran(P));
}