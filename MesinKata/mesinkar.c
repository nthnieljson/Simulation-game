/* File: mesinkar.c */
/* Implementasi Mesin Karakter */

#include "mesinkar.h"
#include <stdio.h>

char CC;
boolean EOP;

static FILE * pita;
static int retval;

void START(char * filename) {
/* Mesin siap dioperasikan. Pita disiapkan untuk dibaca.
   Karakter pertama yang ada pada pita posisinya adalah pada jendela.
   I.S. : sembarang
   F.S. : CC adalah karakter pertama pada pita. Jika CC != MARK maka EOP akan padam (false).
          Jika CC = MARK maka EOP akan menyala (true) */

	/* Algoritma */
  // fopen(stdin);
	
  if (filename[0] == ' ')
  {
    pita = stdin;  
  } else
  {
    pita = fopen(filename, "r");
  }
	ADV();
}

void ADV() {
/* Pita dimajukan satu karakter. 
   I.S. : Karakter pada jendela = 
          CC, CC != MARK
   F.S. : CC adalah karakter berikutnya dari CC yang lama, 
          CC mungkin = MARK.
		  Jika  CC = MARK maka EOP akan menyala (true) */

	/* Algoritma */
	retval = fscanf(pita,"%c",&CC);
	
  if (pita != stdin)
  {
    EOP = (CC == MARKFILE);
    if (EOP)
    {
      fclose(pita);
    }
  } else 
  {
    EOP = (CC == MARK);
  }
	// if (EOP) {
 //       fclose(pita);
 // 	}
}
