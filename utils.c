
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define ENDL printf("\n")

void GetBrsKolFileMatriks(int *NB, int *NK, char *filename)
/* I.S. NBrs, NKol, terdefinisi dan sembarang, filename adalah nama file map
   F.S. NBrs adalah jumlah baris pada filemap, begitu jg dengan NKol adalah jumlah kolom*/
{
	STARTKATA(filename);
	*NK = CKata.Length;

	*NB = 0;
	while (!EndKata)
	{
		*NB += 1;
		ADVKATA(filename);
	}
}

MATRIKS FileToMatriks(char *filename)
/* Membaca file yang berisi matriks dan mengembalikan matriks tersebut */
{
	//KAMUS
	int i, j, NBrs, NKol;
	MATRIKS Map;

	//ALGORITMA
	GetBrsKolFileMatriks(&NBrs, &NKol, filename);

	MakeMATRIKS(NBrs, NKol, &Map);

	i = GetFirstIdxBrs(Map);

	STARTKATA(filename);
	while (!EndKata)
	{
		for (j = GetFirstIdxKol(Map); j < NKol; ++j)
		{
			ElmtM(Map, i, j) = CKata.TabKata[j];
		}
		i += 1;
		ADVKATA(filename);
	}

	return Map;
}

char GetElementMap(MATRIKS Map, POINT Loc)
/* Mengembalikan elemen yang ada pada Map pada koordinat Loc */
{
	return ElmtM(Map, Ordinat(Loc), Absis(Loc));
}

void SetElementMap(MATRIKS *Map, POINT Loc, char el)
/* I.S. Map, Loc, el terdefinisi
   F.S. Map pada koordinat Loc, di set element nya menjadi el */
{
	ElmtM(*Map, Ordinat(Loc), Absis(Loc)) = el;
}

void MovePlayer(MATRIKS Map, char Command, POINT *Loc)
/* I.S. Map adalah matriks map yang akan diubah posisi player nya,
        Command adalah command yg di input pengguna 'w'. 'a'. 's'. 'd'.
        PlayerLoc adalah lokasi player sebelumnya
   F.S. Posisi pemain berubah jika tidak menabrak pagar yaitu char '*',
   		Map dan PlayerLoc akan berubah jika tidak menabrak*/
{
	//KAMUS
	POINT NewLoc;

	//ALGORITMA
	switch (Command)
	{
	case 'w':
		NewLoc = PrevY(*Loc);
		break;

	case 'a':
		NewLoc = PrevX(*Loc);
		break;

	case 's':
		NewLoc = NextY(*Loc);
		break;

	case 'd':
		NewLoc = NextX(*Loc);
		break;
	}

	if ((GetElementMap(Map, NewLoc) != '*') && (GetElementMap(Map, NewLoc) != 'W') && (GetElementMap(Map, NewLoc) != 'A'))
	{
		*Loc = NewLoc;
	}
}

TabBahan FileToListBahan(char *filename)
/* Membaca file yang berisi nama bahan beserta harganya */
/* Mengembalikan list bahan  */
{
	//KAMUS
	TabBahan ListBahan;
	int i;
	Bahan CBahan;

	//ALGORITMA
	MakeEmptyListBahan(&ListBahan);

	STARTKATA(filename);
	Val(CBahan) = KataToInteger(CKata);
	ADVKATA(filename);
	Name(CBahan) = CKata;

	i = 0;
	while (!EndKata)
	{
		Elmt(ListBahan, i) = CBahan;
		i += 1;

		ADVKATA(filename);
		if (!EndKata)
		{
			Val(CBahan) = KataToInteger(CKata);
			ADVKATA(filename);
			Name(CBahan) = CKata;
		}
	}

	return ListBahan;
}

TabBahan CreateEmptyBahan(TabBahan ListBahan)
/* Membuat inventory utk penyimpanan bahan player
   Semua val pada elemen list diset ke 0 */
{
	//KAMUS
	IdxType i;

	//ALGORITMA
	for (i = GetFirstIdxListBahan(ListBahan); i <= GetLastIdxListBahan(ListBahan); ++i)
	{
		Val(Elmt(ListBahan, i)) = 0;
	}

	return ListBahan;
}

void PrintBahanPlayer(TabBahan ListBahan)
/* I.S. ListBahan terdefinisi
   F.S Isi dari ListBahan ditampilkan ke layar dengan format "{name}: {qty}"*/
{
	//KAMUS
	IdxType i;

	//ALGORITMA

	printf("Inventory Bahan\n");
	printf("List: (Nama): (qty)\n");

	for (i = GetFirstIdxListBahan(ListBahan); i <= GetLastIdxListBahan(ListBahan); ++i)
	{
		printf("    - ");
		PrintKata(Name(Elmt(ListBahan, i)));
		printf(": %d\n", Val(Elmt(ListBahan, i)));
	}
}

void PrintBuyBahan(TabBahan ListBahan)
/* I.S. ListBahan terdefinisi
   F.S Isi dari ListBahan ditampilkan ke layar dengan format "{name}: {value}"*/
{
	//KAMUS
	IdxType i;

	//ALGORITMA

	printf("Ingin membeli apa?\n");
	printf("List: (Nama): (Harga)\n");

	for (i = GetFirstIdxListBahan(ListBahan); i <= GetLastIdxListBahan(ListBahan); ++i)
	{
		printf("    - ");
		PrintKata(Name(Elmt(ListBahan, i)));
		printf(": %d\n", Val(Elmt(ListBahan, i)));
	}
}

int PriceBuyBahan(Kata name, int qty, TabBahan ShopBahan)
/* Menghitung harga yang perlu dibayar untuk membeli bahan dengan quantity tertentu
   Mengembalikan total harga jika bahan yang ingin dibeli pengguna (name) ada di
   shop bahan
   Jika tidak ada di shop bahan, akan mengembalikan total harga -999*/
{
	//KAMUS
	IdxType i;
	boolean found;
	int price;

	//ALGORITMA

	i = GetFirstIdxListBahan(ShopBahan);
	found = false;

	while ((i <= GetLastIdxListBahan(ShopBahan)) && (!found))
	{
		if (IsKataSama(name, Name(Elmt(ShopBahan, i))))
		{
			price = qty * Val(Elmt(ShopBahan, i));
			found = true;
		}

		i += 1;
	}

	if (!found)
	{
		price = -999;
	}

	return price;
}

void PitaToTreeWahanaGame(BinTree *T, char *filename, TabBahan ListBahan)
{

	//KAMUS
	WahanaGame WGame;
	Kata Type;
	int Price;
	int Capacity;
	int DurationW;
	Kata Description;
	int MoneyCost;
	TabBahan BahanCost;

	int i;

	//ALGORITMA
	ADVKATA(filename);
	if (CKata.TabKata[0] == ')')
	{
		(*T) = NilBinTree;
	}
	else
	{
		Price = KataToInteger(CKata);

		ADVKALIMAT(filename);
		Type = CKata;

		ADVKATA(filename);
		Capacity = KataToInteger(CKata);

		ADVKATA(filename);
		DurationW = KataToInteger(CKata);

		ADVKALIMAT(filename);
		Description = CKata;

		ADVKATA(filename);
		MoneyCost = KataToInteger(CKata);

		BahanCost = CreateEmptyBahan(ListBahan);
		for (i = GetFirstIdxListBahan(BahanCost); i <= GetLastIdxListBahan(BahanCost); ++i)
		{
			ADVKATA(filename);
			Val(Elmt(BahanCost, i)) = KataToInteger(CKata);
		}

		WGame = MakeWahanaGame(Type, Price, Capacity, DurationW, Description, MoneyCost, BahanCost);
		(*T) = AlokNode(WGame);

		ADVKATA(filename);
		PitaToTreeWahanaGame(&(Left(*T)), filename, ListBahan);
		PitaToTreeWahanaGame(&(Right(*T)), filename, ListBahan);
	}
	ADVKATA(filename);
}
/* Dipakai jika input dari pita karakter */
/* I.S. CC berisi ‘(‘ */
/* F.S. T terdefinisi */
/* Proses: Membaca isi pita karakter dan membangun pohon secara rekursif, hanya
membutuhkan mesin karakter */

ListWG FileToListTreeWahana(char *filename, TabBahan ListBahan)
/* Membaca file wahana yang berisi beberapa tree wahana, setiap tree wahana akan
   di-insert ke List wahana game. Mengembalikan list wahana game */
{
	//KAMUS
	BinTree T;
	ListWG LWG;

	//ALGORITMA

	CreateEmptyListWG(&LWG);

	while (!EndKata)
	{
		PitaToTreeWahanaGame(&T, filename, ListBahan);
		InsVFirstListWG(&LWG, T);
	}

	return LWG;
}

void PrintTitle()
/*Menampilkan ke layar pembukaan dari game*/
{
	printf("// Welcome to Willy wangky's fum factory!!//");
}

void PrintMainMenu()
/*Menampilkan main menu ke layar*/
{
	printf("// New game / load game / exit? //");
}

void PrintMap(int PlayerMap, POINT PlayerLoc, MATRIKS Map1)
/*Menampilkan map beserta legend ke layar*/
{
	//KAMUS
	indeks i, j;
	MATRIKS Map;
	//ALGORITMA
	//Menampilkan map

	Map = Map1;

	for (i = GetFirstIdxBrs(Map); i <= GetLastIdxBrs(Map); ++i)
	{
		for (j = GetFirstIdxKol(Map); j <= GetLastIdxKol(Map); ++j)
		{
			if ((i == Ordinat(PlayerLoc)) && (j == Absis(PlayerLoc)))
			{
				printf(" P");
			}
			else
			{
				printf(" %c", ElmtM(Map, i, j));
			}
		}
		if (i != GetLastIdxBrs(Map))
		{
			printf("\n");
		}
	}
	ENDL;

	//Menampilkan legend
	printf("Legend:\n");
	printf("A = Antrian\n");
	printf("P = Player\n");
	printf("W = Wahana\n");
	printf("O = Office\n");
	printf("<, ^, >, v = Gerbang");
}

void PrintPlayerStat(Kata username, int money)
/* Menampilkan stat utama pemain seperti nama, money, dan time */
{
	printf("Name: ");
	PrintKata(username);
	ENDL;

	printf("Money: %d", money);
}

void PrintTime(JAM CurrentTime, JAM EndTime, boolean PrepPhase)
/* Menampilkan waktu untuk preparation phase */
{
	//KAMUS
	JAM TR; // TR = Time Remaining

	//ALGORITMA
	printf("Current Time: ");
	TulisJAM(CurrentTime);
	ENDL;

	if (PrepPhase)
	{
		printf("Opening Time: ");
	}
	else
	{
		printf("Closing Time: ");
	}
	TulisJAM(EndTime);
	ENDL;

	TR = DetikToJAM(Durasi(CurrentTime, EndTime));
	printf("Time Remaining: %d hour(s) %d minute(s) %d second(s)", Hour(TR), Minute(TR), Second(TR));
}

void PrintPrepStat(int Aksi, JAM J, int Money, TabBahan ListBahan)
/* Menampilkan stat khusus preparation day, total aksi waktu
   dan uang yang dibutuhkan */
{
	//KAMUS
	IdxType i;

	//ALGORITMA
	printf("Total aksi yang akan dilakukan: %d", Aksi);
	ENDL;

	printf("Total waktu yang dibutuhkan: %d hour(s) %d minute(s) %d second(s)", Hour(J), Minute(J), Second(J));
	ENDL;

	printf("Total uang yang dibutuhkan: %d", Money);
	ENDL;

	printf("List bahan yang dibutuhkan: (Nama) (Harga)\n");

	for (i = GetFirstIdxListBahan(ListBahan); i <= GetLastIdxListBahan(ListBahan); ++i)
	{
		printf("    - ");
		PrintKata(Name(Elmt(ListBahan, i)));
		printf(": %d\n", Val(Elmt(ListBahan, i)));
	}
}

void PrintMainQueue(PrioQueueChar Q)
/* Menampilkan  antrian pengunjung saat main phase*/
{
	Pengunjung P;

	printf("Antrian [%d/%d]:\n", NBElmtPQ(Q), MaxElPQ(Q));

	while (!IsEmptyPQ(Q))
	{
		Dequeue(&Q, &P);
		PrintPengunjung(P);
		ENDL;
	}
}

void PrintBuildWahana(ListWG ListWahanaGame)
/*  I.S. ListWahana terdefinisi
    F.S. menampilkan list wahana game ke layar */
{
	printf("ingin membangun apa?\n");
	printf("List:\n");

	addressListWG P = FirstListWG(ListWahanaGame);
	while (P != NilListWG)
	{
		printf("   -");
		PrintKata(Type(Akar(InfoListWG(P))));
		printf("\n");
		P = NextListWG(P);
	}
}

BinTree FindBasicWahana(ListWG ListWahanaGame, Kata K)
/* Mencari tree wahana basic, dengan type tertentu, yaitu K, kalau ditemukan akan mengembalikan
   Alamat pohon tersebut, jika tidak akan mengembalikan NilBinTree */
{
	boolean found;
	BinTree T;
	addressListWG P;

	P = FirstListWG(ListWahanaGame);
	found = false;

	while ((P != NilListWG) && (!found))
	{
		if (IsKataSama(Type(Akar(InfoListWG(P))), K))
		{
			T = InfoListWG(P);
			found = true;
		}
		P = NextListWG(P);
	}

	if (found)
	{
		return T;
	}
	else
	{
		return NilBinTree;
	}
}

TabCommand InitArrayCommand()
{
	/* Mengembalikan List/Array Command yang setiap elemen bertipe Commtype */
	/* Commtype : < perintah : Kata, duration : integer > */
	TabCommand T;
	createEmptyListCommand(&T);
	InsertNewCommand(&T, makeCommtype(CNew, 0));
	InsertNewCommand(&T, makeCommtype(CLoad, 0));
	InsertNewCommand(&T, makeCommtype(CExit, 0));
	InsertNewCommand(&T, makeCommtype(CW, 1));
	InsertNewCommand(&T, makeCommtype(CA, 1));
	InsertNewCommand(&T, makeCommtype(CS, 1));
	InsertNewCommand(&T, makeCommtype(CD, 1));
	InsertNewCommand(&T, makeCommtype(CBuild, 75));
	InsertNewCommand(&T, makeCommtype(CUpgrade, 3));
	InsertNewCommand(&T, makeCommtype(CBuy, 60));
	InsertNewCommand(&T, makeCommtype(CUndo, 0));
	InsertNewCommand(&T, makeCommtype(CExecute, 0));
	InsertNewCommand(&T, makeCommtype(CMain, 0));
	InsertNewCommand(&T, makeCommtype(CServe, 2));
	InsertNewCommand(&T, makeCommtype(CRepair, 5));
	InsertNewCommand(&T, makeCommtype(CDetail, 0));
	InsertNewCommand(&T, makeCommtype(COffice, 0));
	InsertNewCommand(&T, makeCommtype(CPrepare, 0));
	InsertNewCommand(&T, makeCommtype(CDetails, 0));
	InsertNewCommand(&T, makeCommtype(CReport, 0));

	return T;
}

int FindDuration(TabCommand T, Kata K)
{
	/* Mengembalikan durasi sebuah command jika terdapat di TabCommand */
	/* Jika tidak ada, return DurasiUndef=-999 */
	IdxType i = IdxMinCommand;
	boolean found = false;

	while (i < NbElmtCommand(T) && !found)
	{
		if (IsKataSama(Command(T, i), K))
		{
			found = true;
		}
		else
		{
			i++;
		}
	}
	if (found)
	{
		return DurasiCommand(T, i);
	}
	else
	{
		return DurasiUndef;
	}
}

boolean BahanCukup(TabBahan BahanPlayer, TabBahan BahanCost)
{
	/* Mengecek apakah player memiliki bahan yang cukup */

	int panjangBahanPlayer = NbElmtListBahan(BahanPlayer);
	int panjangBahanCost = NbElmtListBahan(BahanCost);

	if (panjangBahanPlayer != panjangBahanCost)
	{
		return false;
	}
	else
	{
		int i = 0;
		boolean cukup = true;

		while (i < panjangBahanPlayer && cukup)
		{
			if (Elmt(BahanPlayer, i).Val < Elmt(BahanCost, i).Val)
			{
				cukup = false;
			}
			else
			{
				i++;
			}
		}
		return cukup;
	}
}

boolean MoneyCukup(int MoneyPlayer, int MoneyCost)
{
	/* Mengecek apakah player memiliki uang yang cukup */
	return MoneyPlayer >= MoneyCost;
}

TabBahan AddBahan(TabBahan ListBahan, Bahan B)
/* Menambahkan bahan kepada list bahan, bahan sudah pasti terdefinisi di list bahan */
{
	IdxType i;
// 	MakeEmptyListBahan(&Hasil);

	for (i = GetFirstIdxListBahan(ListBahan); i <= GetLastIdxListBahan(ListBahan); ++i)
	{
		if (IsKataSama(Name(Elmt(ListBahan, i)), Name(B)))
		{
			Val(Elmt(ListBahan, i)) += Val(B);
		}
	}

	return ListBahan;
}

TabBahan AddListBahan(TabBahan ListBahan1, TabBahan ListBahan2)
{
	/* Menjumlahkan elemen tiap bahan pada 2 List */
	/* Asumsi panjang ListBahan1=ListBahan2 */

	TabBahan Hasil;
	MakeEmptyListBahan(&Hasil);
	IdxType i;

	//Me-assign Nama Bahan dan jumlah bahan ke TabBahan Hasil;
	for (i = GetFirstIdxListBahan(ListBahan1); i <= GetLastIdxListBahan(ListBahan1); ++i)
	{
		Elmt(Hasil, i).Name = Elmt(ListBahan1, i).Name;
		Elmt(Hasil, i).Val = Elmt(ListBahan1, i).Val + Elmt(ListBahan2, i).Val;
	}

	return Hasil;
}

TabBahan MinusBahan(TabBahan ListBahan, Bahan B)
// Menagurangi bahan kepada list bahan, bahan sudah pasti terdefinisi di list bahan 
{
	IdxType i;

	for (i = GetFirstIdxListBahan(ListBahan); i <= GetLastIdxListBahan(ListBahan); ++i)
	{
		if (IsKataSama(Name(Elmt(ListBahan, i)), Name(B)))
		{
			Val(Elmt(ListBahan, i)) -= Val(B);
		}
	}

	return ListBahan;
}

TabBahan MinusListBahan(TabBahan ListBahan1, TabBahan ListBahan2)
/* Mengurangi elemen tiap bahan pada  List 1 dengan List 2 */
/* Asumsi panjang ListBahan1=ListBahan2 */
{
	TabBahan Hasil;
	MakeEmptyListBahan(&Hasil);
	IdxType i;

	//Me-assign Nama Bahan dan jumlah bahan ke TabBahan Hasil;
	for (i = GetFirstIdxListBahan(ListBahan1); i <= GetLastIdxListBahan(ListBahan1); ++i)
	{

		Elmt(Hasil,i).Name = Elmt(ListBahan1,i).Name;
		Elmt(Hasil,i).Val = Elmt(ListBahan1,i).Val - Elmt(ListBahan2,i).Val;
	}

	return Hasil;
}

boolean SearchNodeWG(BinTree T, Kata K)
{
	/* Mengirimkan true jika ada node dari T yang memiliki info.Type = K */
	if (IsTreeEmpty(T))
	{
		return false;
	}
	else if (IsKataSama(Type(T->info), K))
	{
		return true;
	}
	else
	{
		return (SearchNodeWG(Left(T), K) || SearchNodeWG(Right(T), K));
	}
}

addrNode findTypeBinTree(Kata TypeYangDicari, BinTree T)
{
	/* Mengembalikas addrNode dari Node yang memiliki Type sama dengan
	TypeYangDicari, kalau tidak ada return NilBinTree */

	if (IsTreeEmpty(T))
	{
		return NilBinTree;
	}
	else if (IsKataSama(Type(T->info), TypeYangDicari))
	{
		return T;
	}
	else
	{
		if (SearchNodeWG(Left(T), TypeYangDicari))
		{
			return findTypeBinTree(TypeYangDicari, Left(T));
		}
		else
		{
			return findTypeBinTree(TypeYangDicari, Right(T));
		}
	}
}

addrNode FindListWahana(Kata K, ListWG ListWahanaGame)
/* Mencari tree wahana game tertentu dengan type = T, pada suatu list of wahanagame */
{
	addressListWG P;
	boolean found;
	addrNode T;


	P = FirstListWG(ListWahanaGame);
	found = false;

	while ((P != NilListWG) && (!found))
	{
		T = findTypeBinTree(K, InfoListWG(P));

		if (T != NilBinTree)
		{
			found = true;
		}

		P = NextListWG(P);
	}

	return T;

}

Kata GenerateWahanaName(Kata TypeWahana, POINT PlayerLoc, int PlayerMap)
{
	/* Mengenerate Nama Wahana yang unik dari Type(Nama) Wahana Standar berdasarkan
	PlayerLoc dan Map */
	Kata Nama_Unik;

	int panjang_awal = TypeWahana.Length;
	Nama_Unik.Length = panjang_awal + 6;

	int i;
	for (i = 0; i < panjang_awal; i++)
	{
		Nama_Unik.TabKata[i] = TypeWahana.TabKata[i];
	}

	int a = PlayerLoc.X / 10;
	char aC = a + '0';

	int b = PlayerLoc.X % 10;
	char bC = b + '0';

	Nama_Unik.TabKata[i] = ' '; //Spasi
	Nama_Unik.TabKata[i + 1] = aC;
	Nama_Unik.TabKata[i + 2] = bC;

	a = PlayerLoc.Y / 10;
	aC = a + '0';

	b = PlayerLoc.Y % 10;
	bC = b + '0';

	char mapC = PlayerMap + '0';

	Nama_Unik.TabKata[i + 3] = aC;
	Nama_Unik.TabKata[i + 4] = bC;
	Nama_Unik.TabKata[i + 5] = mapC;

	return Nama_Unik;
}

int PlayerTunnel(int MapIdPlayer, char Symbol)
/* Menghasilkan perpindahan player dari symbol yang dimasukkan */
/* Jika input invalid maka akan mengembalikan nilai Nil (-1) */
{
	if ((MapIdPlayer == 1) && (Symbol == '>'))
	{
		return 2;
	}
	else if ((MapIdPlayer == 1) && (Symbol == 'v'))
	{
	
		return 3;
	}
	else if ((MapIdPlayer == 2) && (Symbol == '<'))
	{
	
		return 1;
	}
	else if ((MapIdPlayer == 2) && (Symbol == 'v'))
	{
	
		return 4;
	}
	else if ((MapIdPlayer == 3) && (Symbol == '>'))
	{
	
		return 4;
	}
	else if ((MapIdPlayer == 3) && (Symbol == '^'))
	{
	
		return 1;
	}
	else if ((MapIdPlayer == 4) && (Symbol == '<'))
	{
	
		return 3;
	}
	else if ((MapIdPlayer == 4) && (Symbol == '^'))
	{
	
		return 2;
	}
	else
	{
		return -1;
	}
}

POINT MoveMap(int MapIdPlayer, char Symbol)
{
	if ((MapIdPlayer == 1) && (Symbol == '>'))
	{
		return MakePOINT(3,0);
	}
	else if ((MapIdPlayer == 1) && (Symbol == 'v'))
	{
		return MakePOINT(0,3);

	}
	else if ((MapIdPlayer == 2) && (Symbol == '<'))
	{
		return MakePOINT(3,6);
	
	}
	else if ((MapIdPlayer == 2) && (Symbol == 'v'))
	{
		return MakePOINT(0,3);
	
	}
	else if ((MapIdPlayer == 3) && (Symbol == '>'))
	{
		return MakePOINT(3,0);

	}
	else if ((MapIdPlayer == 3) && (Symbol == '^'))
	{
		return MakePOINT(6,3);

	}
	else if ((MapIdPlayer == 4) && (Symbol == '<'))
	{
		return MakePOINT(3,6);
	
	}
	else if ((MapIdPlayer == 4) && (Symbol == '^'))
	{
		return MakePOINT(6,3);
	
	}
}

WahanaPlayer SearchWahanaPlayer(int Map, POINT Loc, TabWahanaPlayer ArrayWP)
/*  I.S. Wahana Player pasti ada
    F.S. Mencari Wahana Player yang memiliki Map dan Loc sama dengan input */
{
	int i;
    i=GetFirstIdxListWP(ArrayWP);
    while(i<=GetLastIdxListWP(ArrayWP)){
        if (EQ(LocW(ElmtWP(ArrayWP,i)),Loc)&& MapW(ElmtWP(ArrayWP,i))==Map){
            return ElmtWP(ArrayWP,i);
        }
        i++;
    }

    return ElmtWP(ArrayWP,i);
}

WahanaPlayer SearchWahanaPlayerName(Kata Name, TabWahanaPlayer ArrayWP)
/* Mengreturn elemen wahanaplayer mempunyain nama Name. jika tidak adaa, akan mengembalikan wahanaundef */
{
	int i;
    i=GetFirstIdxListWP(ArrayWP);
    while(i<=GetLastIdxListWP(ArrayWP)){
        if (IsKataSama(Name, NamaW(ElmtWP(ArrayWP,i)))){
            return ElmtWP(ArrayWP,i);
        }
        i++;
    }

    return ElmtWP(ArrayWP,i);
}


void FindAround(POINT P, MATRIKS MapMat, int MapInt, TabWahanaPlayer ArrayWP){
/*	I.S. Player ada di posisi P di Map MapInt dengan Matriks MapMat 
	F.S. Output daftar semua wahana di sekitar Player
		 Jika tidak ada wahana di sekitar player, output pesan "Tidak ada wahana di sekitar anda" */
	POINT Up, Down, Left, Right;
	char CUp, CDown, CLeft, CRight;
	Kata Nama;

	Up = NextY(P);
	Left = PrevX(P);
	Down = PrevY(P);
	Right = NextX(P);

	CUp = GetElementMap(MapMat, Up);
	CLeft = GetElementMap(MapMat, Left);
	CDown = GetElementMap(MapMat, Down);
	CRight = GetElementMap(MapMat, Right);

	if ((CUp != 'W') && (CRight != 'W') && (CDown != 'W')  && (CLeft != 'W')) {
		printf("Tidak ada wahana di sekitar anda\n");
	} else {
		printf("List:\n");
		if (CUp == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Up, ArrayWP));
			printf("- ");
			PrintKata(Nama);
			printf("\n");
		}
		if (CRight == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Right, ArrayWP));
			printf("- ");
			PrintKata(Nama);
			printf("\n");
		}
		if (CDown == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Down, ArrayWP));
			printf("- ");
			PrintKata(Nama);
			printf("\n");
		}
		if (CLeft == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Left, ArrayWP));
			printf("- ");
			PrintKata(Nama);
			printf("\n");
		}
		printf("Akhiri command dengan ';' !\n");
		STARTKALIMAT(" ");
	}
}


void FindAroundDetail(POINT P, MATRIKS MapMat, int MapInt, TabWahanaPlayer ArrayWP){
/*  Sama kayak FindAround cuman untuk command detail*/
	POINT Up, Down, Left, Right;
	char CUp, CDown, CLeft, CRight;
	Kata Nama;

	Up = NextY(P);
	Left = PrevX(P);
	Down = PrevY(P);
	Right = NextX(P);

	CUp = GetElementMap(MapMat, Up);
	CLeft = GetElementMap(MapMat, Left);
	CDown = GetElementMap(MapMat, Down);
	CRight = GetElementMap(MapMat, Right);

	if ((CUp != 'W') && (CRight != 'W') && (CDown != 'W')  && (CLeft != 'W')) {
		printf("Tidak ada wahana di sekitar anda\n");
	} else {
		WahanaPlayer WP;
		printf("List:\n");
		if (CUp == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Up, ArrayWP));
			WP = SearchWahanaPlayerName(Nama, ArrayWP);
			Printdetail(WP);

		}
		if (CRight == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Right, ArrayWP));
			WP = SearchWahanaPlayerName(Nama, ArrayWP);
			Printdetail(WP);
		}
		if (CDown == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Down, ArrayWP));
			WP = SearchWahanaPlayerName(Nama, ArrayWP);
			Printdetail(WP);
		}
		if (CLeft == 'W') {
			Nama = NamaW(SearchWahanaPlayer(MapInt, Left, ArrayWP));
			WP = SearchWahanaPlayerName(Nama, ArrayWP);
			Printdetail(WP);
		}
	}
}

JAM TimeSkip(JAM Sekarang, TabCommand T, Kata K){
	/* Mengembalikan JAM yang sudah di-timeskip setelah menjalankan command */
	int durasi  = FindDuration(T,K);

	if(durasi!=-999){	//ketemu
		int durasi2detik = durasi * 60;
		return NextNDetik(Sekarang, durasi2detik);
	}
	else{
		return Sekarang;
	}
}

PrioQueueChar MinusKesabaranQueue(PrioQueueChar Q, int X)
/* Mengurangi nilai kesabaran dari setiap pengunjung pada antrian sebanyak X
   jika nilai kesabaran kurang dari 0, maka pengunjung akan keluar*/
{
	//KAMUS
	PrioQueueChar QNew;
	Pengunjung P;


	//ALGORITMA
	MakeEmptyPQ(&QNew, MaxElPQ(Q));

	while (!IsEmptyPQ(Q))
	{
		Dequeue(&Q, &P);
		Kesabaran(P) -= X;

		if (Kesabaran(P) > 0)
		{
			Enqueue(&QNew, P);
		}
	}

	return QNew;

}

boolean GenerateP(int TimeSkipVal)
/* Menentukan apakah akan digenerate pengunjung atau tidak, dengan random */
{
	int Maks;

	Maks = 10;
	return (rand() % 10) < TimeSkipVal;
}

boolean IsAround(POINT Loc, MATRIKS Map, char C)
/* return true jika disekitar (atas bawah krii kanan) Loc terdapat C pada Map */
{
	POINT Up, Down, Left, Right;
	char CUp, CDown, CLeft, CRight;
	Kata Nama;

	Up = NextY(Loc);
	Left = PrevX(Loc);
	Down = PrevY(Loc);
	Right = NextX(Loc);

	CUp = GetElementMap(Map, Up);
	CLeft = GetElementMap(Map, Left);
	CDown = GetElementMap(Map, Down);
	CRight = GetElementMap(Map, Right);

	return !((CUp != C) && (CRight != C) && (CDown != C)  && (CLeft != C)) ;
}

void PrintKataListU(ListU L){
	/* Mencetak elemen ListU (Kata) pada setiap node */
	addressListU P;
	P = FirstListU(L);

	while (P!=NilListU)
	{
		PrintKata(InfoListU(P));
		printf(" <-- ");
		P = NextListU(P);
	}
	
}

void Printdetail(WahanaPlayer WP){
/* Mencetak Detail Wahana Player untuk command detail */
	printf("Nama      : "); PrintKata(NamaW(WP)); ENDL;
	printf("Lokasi    : (%d,%d)", LocW(WP).X, LocW(WP).Y);	ENDL;
	printf("Upgrade(s): [");

	if (Left(StatW(WP)) != NilBinTree)
	{
		PrintKata(Akar(Left(StatW(WP))).Type);
	}

	if ( (Left(StatW(WP)) != NilBinTree) && (Right(StatW(WP)) != NilBinTree) )
	{
		printf(",");
	}
	
	if (Right(StatW(WP)) != NilBinTree)
	{
		PrintKata(Akar(Right(StatW(WP))).Type);
	}
	printf("]\n");

	printf("History   : "); PrintKataListU(HUpgradeW(WP));	ENDL;
	printf("Status    : ");
	if (BrokenW(WP)){
		printf("Tidak berfungsi");
	}
	else{
		printf("Berfungsi");
	}
	ENDL; ENDL;
}

Graph InitMapGraph()
/* Meng return graph dengan node node nya merupakan id dan map, yang sudah terhubungkan */
{

	MATRIKS Map1 = FileToMatriks(FILE_MAP_1);
	MATRIKS Map2 = FileToMatriks(FILE_MAP_2);
	MATRIKS Map3 = FileToMatriks(FILE_MAP_3);
	MATRIKS Map4 = FileToMatriks(FILE_MAP_4);

	Graph G = CreateGraph(1, Map1);

	InsertNodeG(G, 2, Map2);
	InsertNodeG(G, 3, Map3);
	InsertNodeG(G, 4, Map4);

	return G;
}