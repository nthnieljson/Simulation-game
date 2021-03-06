
#include <stdio.h>
#include <stdlib.h>

#include "./MesinKata/mesinkata.h"
#include "./Point/point.h"
#include "./Jam/jam.h"
#include "./ADTLain/instruction.h"
#include "./Stack/stackins.h"
#include "./ADTLain/command.h"
#include "./Array/arraybahan.h"
#include "utils.h"
#include "./MesinKata/constants.h"
#include "./BinTree/bintree.h"
#include "./ADTLain/wahanagame.h"
#include "./ListBerkait/listwahanagame.h"
#include "./ListBerkait/listupgrade.h"
#include "./ADTLain/wahanaplayer.h"
#include "./Array/arraywahanaplayer.h"
#include "./ADTLain/pengunjung.h"
#include "./Graph/graph.h"


#define ENDL printf("\n")


int main()
{
	
	
	// Initialize Game

	//Initialisasi GameCommand
	InitGameCommand();


	TabCommand ArrayCommand = InitArrayCommand();

	//Initialize Materials
	TabBahan ShopBahan = FileToListBahan(FILE_MATERIAL);

	//Initialize Map
	Graph G = InitMapGraph();

	//Initialize Wahana Game
	STARTKATA(FILE_WAHANA);

	ListWG ListWahanaGame;
	ListWahanaGame = FileToListTreeWahana(FILE_WAHANA, ShopBahan);

    // Player state

	Kata Username;

	int Day = 1;

	boolean PrepPhase;

	boolean Play;

	JAM CurrentTime = MakeJAM(21, 0, 0);
	JAM TimeSpent = MakeJAM(0, 0, 0);

	JAM PrepStartTime = MakeJAM(21, 0, 0);
	JAM PrepEndTime = MakeJAM(9, 0, 0);

	JAM MainStartTime = MakeJAM(9, 0, 0);
	JAM MainEndTime = MakeJAM(21, 0, 0);

	TabBahan BahanPlayer = CreateEmptyBahan(ShopBahan);
	IdxType idxBP;
	for (idxBP = GetFirstIdxListBahan(BahanPlayer); idxBP <= GetLastIdxListBahan(BahanPlayer); ++idxBP)
	{
		Val(Elmt(BahanPlayer, idxBP)) = 999;
	}

	int MoneyPlayer = 99999;

	POINT PlayerPosition = MakePOINT(1, 1);

	int PlayerMap = 1;
	int PlayerMapPrev;

	TabWahanaPlayer ArrWahanaPlayer;
	MakeEmptyListWP(&ArrWahanaPlayer);

	// Main program 

	PrintTitle();
	ENDL;

	PrintMainMenu();
	ENDL;

	STARTKATA(" ");

	if (IsKataSama(CKata, CNew))
	{
		printf("Memulai permainan baru...\n");

		printf("Masukkan nama:\n");
		STARTKATA(" ");
		CopyKata(CKata, &Username);

		Play = true;

		PrepPhase = true;

		while (Play)
		{

			if (PrepPhase)
			{
				// Prep state

				StackI InstructionStack;
				CreateEmpty(&InstructionStack);

				Instruction NewInstruction;

				int MoneyNeededTotal = 0;
				int MoneyNeeded;

				JAM TimeNeededTotal = MakeJAM(0, 0, 0);
				int TimeNeeded; //dalam menit (satuan waktu terkecil)

				int AksiTotal = 0;

				TabBahan ListBahanNeededTotal = CreateEmptyBahan(ShopBahan);
				TabBahan ListBahanNeeded;

				Graph GPrep = G;



				while (Play && PrepPhase)
				{

					printf("Preparation phase day %d\n", Day);

					PrintMap(PlayerMap, PlayerPosition, MapG(SearchNodeG(G, PlayerMap)));
					ENDL;
					ENDL;

					PrintPlayerStat(Username, MoneyPlayer);
					ENDL;

					PrintTime(CurrentTime, PrepEndTime, true);
					ENDL;

					PrintPrepStat(AksiTotal, TimeNeededTotal, MoneyNeededTotal, ListBahanNeededTotal);
					ENDL;
					ENDL;

					printf("Masukkan perintah:\n");

					STARTKATA(" ");

					if (IsKataSama(CKata, CW))
					{
						MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 'w', &PlayerPosition);
						if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition)) != -1)
						{
							PlayerMapPrev = PlayerMap;
							PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition));
							PlayerPosition = MakePOINT(1,1);
						}						

					} else if (IsKataSama(CKata, CA))
					{
						MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 'a', &PlayerPosition);
						if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition)) != -1)
						{
							PlayerMapPrev = PlayerMap;
							PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition));
							PlayerPosition = MakePOINT(1,1);
						}						

					} else if (IsKataSama(CKata, CS))
					{
						MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 's', &PlayerPosition);
						if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition)) != -1)
						{
							PlayerMapPrev = PlayerMap;
							PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition));
							PlayerPosition = MakePOINT(1,1);
						}						

					} else if (IsKataSama(CKata, CD))
					{
						MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 'd', &PlayerPosition);
						if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition)) != -1)
						{
							PlayerMapPrev = PlayerMap;
							PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition));
							PlayerPosition = MakePOINT(1,1);
						}						

					} else if (IsKataSama(CKata, CBuild))
					{
						BinTree WahanaGameBasicTree;
						WahanaGame WahanaGameBasic;
						boolean SuccesBuild;

						PrintBuildWahana(ListWahanaGame);
						ENDL;

						printf("Akhiri command dengan ';' !\n");
						STARTKALIMAT(" ");

						WahanaGameBasicTree = FindBasicWahana(ListWahanaGame, CKata);

						if (WahanaGameBasicTree != NilBinTree) // Wahana basic ditemukan
						{
							SuccesBuild = true;

							WahanaGameBasic = Akar(WahanaGameBasicTree);

							MoneyNeeded = MoneyCost(WahanaGameBasic);
							ListBahanNeeded = BahanCost(WahanaGameBasic);

							if (!(MoneyCukup(MoneyPlayer, (MoneyNeeded + MoneyNeededTotal))))
							{
								SuccesBuild = false;
								printf("Uang anda tidak mencukupi!");
								ENDL;
							}

							if (!(BahanCukup(BahanPlayer, AddListBahan(ListBahanNeededTotal, ListBahanNeeded))))
							{
								SuccesBuild = false;
								printf("Bahan anda tidak mencukupi!");
								ENDL;
							}

							if (SuccesBuild) // Bahan dan uang cukup untuk membangun
							{
								if (GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition) == '-') // Lahan kosong
								{
									SetElementMap(&MapG(SearchNodeG(GPrep, PlayerMap)), PlayerPosition, 'W');

									NewInstruction = CreateInstruction(CBuild, PlayerPosition, Type(WahanaGameBasic), 1, PlayerMap, MoneyNeeded, ListBahanNeeded);
									Push(&InstructionStack, NewInstruction);

									TimeNeeded = FindDuration(ArrayCommand, CBuild) * 60;
									TimeNeededTotal = NextNDetik(TimeNeededTotal, TimeNeeded);

									MoneyNeededTotal += MoneyNeeded;

									ListBahanNeededTotal = AddListBahan(ListBahanNeededTotal, ListBahanNeeded);

									AksiTotal += 1;

									PrintKata(Type(WahanaGameBasic));
									printf(" dapat dibangun!");
									ENDL;	

								} else // Sudah terdapat wahana ditempat berdiri
								{

									printf("Anda sudah merencanakan untuk memabngun wahana disini, bangun di tempat lain!");
									ENDL;
								}
								

							} else
							{
								PrintKata(Type(WahanaGameBasic));
								printf(" tidak dapat dibangun!");
								ENDL;

							}

						} else
						{
							printf("Type wahana yang anda input tidak ditemukan!");
							ENDL;

						}

					} else if (IsKataSama(CKata, CUpgrade))
					{
						FindAround(PlayerPosition, MapG(SearchNodeG(G, PlayerMap)), 1, ArrWahanaPlayer);

						WahanaPlayer WPUpgrade = SearchWahanaPlayerName(CKata, ArrWahanaPlayer);

						if (TotalPlayW(WPUpgrade) != TotalPlayW(MakeWahanaPlayerUndef()))
						{
							BinTree LeftWPUpgrade = Left(StatW(WPUpgrade));
							BinTree RightWPUpgrade = Right(StatW(WPUpgrade));

							if ((LeftWPUpgrade != NilBinTree) || (RightWPUpgrade != NilBinTree))
							{

								printf("Pilihan upgrade: ");
								ENDL;
								if (LeftWPUpgrade != NilBinTree)
								{
									printf("- ");
									PrintKata(Type(Akar(LeftWPUpgrade)));
									ENDL;
								}

								if (RightWPUpgrade != NilBinTree)
								{
									printf("- ");
									PrintKata(Type(Akar(RightWPUpgrade)));
									ENDL;
								}

								printf("Akhiri command dengan ';' !\n");
								STARTKALIMAT(" ");

								if ((IsKataSama(CKata, Type(Akar(LeftWPUpgrade)))) || (IsKataSama(CKata, Type(Akar(RightWPUpgrade)))))
								{

									BinTree WPUpgradeNow;

									if (IsKataSama(CKata, Type(Akar(LeftWPUpgrade))))
									{
										WPUpgradeNow = LeftWPUpgrade;
									} else
									{
										WPUpgradeNow = RightWPUpgrade;
									}


									WahanaGame WahanaGameUpgrade = Akar(WPUpgradeNow);

									MoneyNeeded = MoneyCost(WahanaGameUpgrade);
									ListBahanNeeded = BahanCost(WahanaGameUpgrade);

									boolean SuccesUpgrade = true;

									if (!(MoneyCukup(MoneyPlayer, (MoneyNeeded + MoneyNeededTotal))))
									{
										SuccesUpgrade = false;
										printf("Uang anda tidak mencukupi!");
										ENDL;
									}

									if (!(BahanCukup(BahanPlayer, AddListBahan(ListBahanNeededTotal, ListBahanNeeded))))
									{
										SuccesUpgrade = false;
										printf("Bahan anda tidak mencukupi!");
										ENDL;
									}

									if (SuccesUpgrade) // Bahan dan uang cukup untuk membangun
									{
										if (GetElementMap(MapG(SearchNodeG(GPrep, PlayerMap)), LocW(WPUpgrade)) == 'W') // Lahan kosong
										{
											SetElementMap(&MapG(SearchNodeG(GPrep, PlayerMap)), LocW(WPUpgrade), 'U');

											NewInstruction = CreateInstruction(CUpgrade, LocW(WPUpgrade), Type(WahanaGameUpgrade), 1, MapW(WPUpgrade), MoneyNeeded, ListBahanNeeded);
											Push(&InstructionStack, NewInstruction);

											TimeNeeded = FindDuration(ArrayCommand, CUpgrade) * 60;
											TimeNeededTotal = NextNDetik(TimeNeededTotal, TimeNeeded);

											MoneyNeededTotal += MoneyNeeded;

											ListBahanNeededTotal = AddListBahan(ListBahanNeededTotal, ListBahanNeeded);

											AksiTotal += 1;

										} else 
										{

											printf("Anda sudah merencanakan untuk upgrade wahana disini!");
											ENDL;
										}
										

									} else
									{
										printf("Upgrade gagal dilakukan!");
										ENDL;
									}

								} else
								{
									printf("Type upgrade tidak ditemukan!");
									ENDL;
								}

							} else
							{
								printf("Tidak ada pilihan upgrade lagi");
								ENDL;

							}

						} else
						{

							printf("Nama wahana tidak ditemukan!");
							ENDL;

						}


					} else if (IsKataSama(CKata, CBuy))
					{
						PrintBahanPlayer(BahanPlayer);
						ENDL;

						PrintBuyBahan(ShopBahan);
						ENDL;

						Bahan CBahan;
						STARTKATA(" ");
						Val(CBahan) = KataToInteger(CKata);
						ADVKATA(" ");
						Name(CBahan) = CKata;				

						MoneyNeeded = PriceBuyBahan(Name(CBahan), Val(CBahan), ShopBahan);
						if (MoneyNeeded == -999) //Nama bahan tidak ditemukan
						{
							printf("Bahan yang ingin anda beli tidak tersedia\n");

						} else // Nama bahan ditemukan
						{
							if ((MoneyNeeded + MoneyNeededTotal) <= MoneyPlayer) // Uang mencukupi
							{

								NewInstruction = CreateInstruction(CBuy, PlayerPosition, Name(CBahan), Val(CBahan), PlayerMap, MoneyNeeded, ListBahanNeeded);
								Push(&InstructionStack, NewInstruction);

								TimeNeeded = FindDuration(ArrayCommand, CBuy) * 60;
								TimeNeededTotal = NextNDetik(TimeNeededTotal, TimeNeeded);

								MoneyNeededTotal += MoneyNeeded;

								AksiTotal += 1;

								printf("You ");
								PrintKata(Function(InfoTop(InstructionStack)));
								printf(" %d ", Quantity(InfoTop(InstructionStack)));
								PrintKata(Detail(InfoTop(InstructionStack)));
								ENDL;	
							} else // Uang tidak mencukupi
							{
								printf("Uang tidak mencukupi!");
								ENDL;

							}
							

						}

					} else if (IsKataSama(CKata, CUndo))
					{
						if (IsEmpty(InstructionStack))
						{
							printf("Anda tidak memiliki perintah sama sekali, undo gagal!");
							ENDL;

						} else
						{
							Pop(&InstructionStack, &NewInstruction);

							if (IsKataSama(CBuild, Function(NewInstruction)))
							{

								SetElementMap(&MapG(SearchNodeG(GPrep, PlayerMap)), Point(NewInstruction), '-');

								TimeNeeded = FindDuration(ArrayCommand, CBuild) * 60;
								TimeNeededTotal = PrevNDetik(TimeNeededTotal, TimeNeeded);

								MoneyNeededTotal -= MCost(NewInstruction);

								ListBahanNeededTotal = MinusListBahan(ListBahanNeededTotal, BCost(NewInstruction));


							} else if (IsKataSama(CBuy, Function(NewInstruction)))
							{

								TimeNeeded = FindDuration(ArrayCommand, CBuy) * 60;
								TimeNeededTotal = PrevNDetik(TimeNeededTotal, TimeNeeded);

								MoneyNeededTotal -= MCost(NewInstruction);


							} else if (IsKataSama(CUpgrade, Function(NewInstruction)))
							{
								SetElementMap(&MapG(SearchNodeG(GPrep, PlayerMap)), Point(NewInstruction), 'W');

								TimeNeeded = FindDuration(ArrayCommand, CUpgrade) * 60;
								TimeNeededTotal = PrevNDetik(TimeNeededTotal, TimeNeeded);

								MoneyNeededTotal -= MCost(NewInstruction);

								ListBahanNeededTotal = MinusListBahan(ListBahanNeededTotal, BCost(NewInstruction));
							
							}
							AksiTotal -= 1;

						}
		

					} else if (IsKataSama(CKata, CExecute))
					{
						if (JLT(DetikToJAM(Durasi(PrepStartTime, PrepEndTime)), TimeNeededTotal))
						{
							printf("Waktu yang dibutuhkan melebihi waktu yang tersisa!");
							ENDL;

						} else
						{
							StackI TmpStackI = ReverseStack(InstructionStack);

							while (!IsEmpty(TmpStackI))
							{
								Pop(&TmpStackI, &NewInstruction);

								if (IsKataSama(CBuild, Function(NewInstruction)))
								{
									SetElementMap(&MapG(SearchNodeG(G, PlayerMap)), Point(NewInstruction), 'W');

									MoneyPlayer -= MCost(NewInstruction);

									BahanPlayer = MinusListBahan(BahanPlayer, BCost(NewInstruction));


									BinTree WGBuild;
									WGBuild = FindListWahana(Detail(NewInstruction), ListWahanaGame);

									Kata NamaWBuild = GenerateWahanaName(Type(Akar(WGBuild)), Point(NewInstruction), Map(NewInstruction));

									WahanaPlayer NewWP;

									ListU ListUBuild;
									CreateEmptyListU(&ListUBuild);

									NewWP = MakeWahanaPlayer(WGBuild, NamaWBuild, Point(NewInstruction), Map(NewInstruction), ListUBuild, 0, 0, 0, 0, false);


									ElmtWP(ArrWahanaPlayer, GetLastIdxListWP(ArrWahanaPlayer) + 1) = NewWP;

								} else if (IsKataSama(CBuy, Function(NewInstruction)))
								{

									MoneyPlayer -= MCost(NewInstruction);

									Bahan NewBahan;
									Name(NewBahan) = Detail(NewInstruction);
									Val(NewBahan) = Quantity(NewInstruction);
									BahanPlayer = AddBahan(BahanPlayer, NewBahan);

								} else if (IsKataSama(CUpgrade, Function(NewInstruction)))
								{

									MoneyPlayer -= MCost(NewInstruction);

									BahanPlayer = MinusListBahan(BahanPlayer, BCost(NewInstruction));

									int i;
									i = GetFirstIdxListWP(ArrWahanaPlayer);
									while(i <= GetLastIdxListWP(ArrWahanaPlayer))
									{
								        if (EQ(LocW(ElmtWP(ArrWahanaPlayer,i)), Point(NewInstruction)) && MapW(ElmtWP(ArrWahanaPlayer,i)) == Map(NewInstruction))
								        {
								    		
								    		BinTree WGUpgrade;
											WGUpgrade = FindListWahana(Detail(NewInstruction), ListWahanaGame);
											StatW(ElmtWP(ArrWahanaPlayer,i)) = WGUpgrade;

											InsVFirstListU(&HUpgradeW(ElmtWP(ArrWahanaPlayer,i)), NamaW(ElmtWP(ArrWahanaPlayer,i)));

											Kata NamaWUpgrade = GenerateWahanaName(Type(Akar(WGUpgrade)), Point(NewInstruction), Map(NewInstruction));
											ENDL;
											NamaW(ElmtWP(ArrWahanaPlayer,i)) = NamaWUpgrade;

								        }
								        i++;
								    }

								}
							}



							PrepPhase = false;
						}

							

					} else if (IsKataSama(CKata, CMain))
					{
						PrepPhase = false;

					} else if (IsKataSama(CKata, CExit))
					{
						Play = false;

					}
					ENDL;	
				}

				CurrentTime = MainStartTime;
				

			} else // Main Phase
			{

				int TimeSkipVal;
				TimeSkipVal = 10;

				PrioQueueChar QueueP;
				MakeEmptyPQ(&QueueP, 10);

				Pengunjung P;

				boolean MainCommandSucces = true;

				boolean FailServe = false;


				while (!PrepPhase && Play)
				{
					if (MainCommandSucces)
					{
						QueueP = MinusKesabaranQueue(QueueP, TimeSkipVal);

						if (NbElmtListWP(ArrWahanaPlayer) > 0)
						{
							if (!IsFullPQ(QueueP))
							{
								if (GenerateP(TimeSkipVal))
								{

									P = GeneratePengunjung(ArrWahanaPlayer);
									Enqueue(&QueueP, P);	
								}	
							}
						}	
					}

					printf("Main phase day %d\n", Day);

					PrintMap(PlayerMap, PlayerPosition, MapG(SearchNodeG(G, PlayerMap)));
					ENDL;
					ENDL;

					PrintPlayerStat(Username, MoneyPlayer);
					ENDL;

					PrintTime(CurrentTime, MainEndTime, false);
					ENDL;

					PrintMainQueue(QueueP);
					ENDL;

					if (FailServe)
					{
						printf("Broken: ");
						PrintKata(ElmtWPengunjung(P, CurrWPeng(P)));
						ENDL;
					}

					ENDL;

					printf("Masukkan perintah:\n");

					STARTKATA(" ");
					if (IsKataSama(CKata, CServe))
					{
						MainCommandSucces = true;
						if (IsAround(PlayerPosition, MapG(SearchNodeG(G, PlayerMap)), 'A'))
						{
							if (!IsEmptyPQ(QueueP))
							{
								// random rusak atau tidak
								if ((rand() % 10) < 1) //rusak
								{
									Dequeue(&QueueP, &P);

									WahanaPlayer WPeng = SearchWahanaPlayerName(ElmtWPengunjung(P, CurrWPeng(P)), ArrWahanaPlayer);

									int i;
									i = GetFirstIdxListWP(ArrWahanaPlayer);
									while(i <= GetLastIdxListWP(ArrWahanaPlayer))
									{
								        if (IsKataSama(NamaW(ElmtWP(ArrWahanaPlayer,i)), ElmtWPengunjung(P, CurrWPeng(P))))
								        {
								    		BrokenW(ElmtWP(ArrWahanaPlayer, i)) = true;
								        }
								        i++;
								    }

								    Prio(P) += 1;
									Enqueue(&QueueP, P);

									TimeSkipVal = (rand() % DurationW(Akar(StatW(WPeng)))) + 1;
									CurrentTime = NextNDetik(CurrentTime, TimeSkipVal * 60);

									FailServe = true;

								} else
								{
									if (!FailServe)
									{
										Dequeue(&QueueP, &P);

										WahanaPlayer WPeng = SearchWahanaPlayerName(ElmtWPengunjung(P, CurrWPeng(P)), ArrWahanaPlayer);

										int i;
										i = GetFirstIdxListWP(ArrWahanaPlayer);
										while(i <= GetLastIdxListWP(ArrWahanaPlayer))
										{
									        if (IsKataSama(NamaW(ElmtWP(ArrWahanaPlayer,i)), ElmtWPengunjung(P, CurrWPeng(P))));
									        {
									    		TotalPlayW(ElmtWP(ArrWahanaPlayer,i)) += 1;
									    		TotalPlayDayW(ElmtWP(ArrWahanaPlayer,i)) += 1;
									    		TotalMoneyW(ElmtWP(ArrWahanaPlayer,i)) += Price(Akar(StatW(ElmtWP(ArrWahanaPlayer,i))));
									    		TotalMoneyDayW(ElmtWP(ArrWahanaPlayer,i)) += Price(Akar(StatW(ElmtWP(ArrWahanaPlayer,i))));
									        }
									        i++;
									    }

									    MoneyPlayer += Price(Akar(StatW(ElmtWP(ArrWahanaPlayer,i - 1))));

										CurrWPeng(P) += 1;

										if (CurrWPeng(P) != TotalWPeng(P))
										{
											Prio(P) += 1;
											Enqueue(&QueueP, P);
										}




										TimeSkipVal = DurationW(Akar(StatW(WPeng)));
										CurrentTime = NextNDetik(CurrentTime, TimeSkipVal * 60);	
									} else
									{
										printf("Wahana rusak, perbaiki dahulu!");
										ENDL;
										MainCommandSucces = false;
									}

								}		
							} else
							{
								printf("Tidak ada pengunjung!");
								MainCommandSucces = false;
								ENDL;
							}
							
						} else
						{

							printf("Anda jauh dari antrian!");
							MainCommandSucces = false;
							ENDL;
						}

					} else 
					{
						MainCommandSucces = true;

						if (IsKataSama(CKata, CW))
						{
							MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 'w', &PlayerPosition);
							if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition)) != -1)
							{
								PlayerMapPrev = PlayerMap;
								PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition));
								PlayerPosition = MakePOINT(1,1);
							}

						} else if (IsKataSama(CKata, CA))
						{
							MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 'a', &PlayerPosition);
							if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition)) != -1)
							{
								PlayerMapPrev = PlayerMap;
								PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition));
								PlayerPosition = MakePOINT(1,1);
							}

						} else if (IsKataSama(CKata, CS))
						{
							MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 's', &PlayerPosition);
							if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition)) != -1)
							{
								PlayerMapPrev = PlayerMap;
								PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition));
								PlayerPosition = MakePOINT(1,1);
							}
						} else if (IsKataSama(CKata, CD))
						{
							MovePlayer(MapG(SearchNodeG(G, PlayerMap)), 'd', &PlayerPosition);
							if (PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition)) != -1)
							{
								PlayerMapPrev = PlayerMap;
								PlayerMap = PlayerTunnel(PlayerMap, GetElementMap(MapG(SearchNodeG(G, PlayerMap)), PlayerPosition));
								PlayerPosition = MakePOINT(1,1);
							}
						} else if (IsKataSama(CKata, CRepair))
						{
							if (IsAround(PlayerPosition, MapG(SearchNodeG(G, PlayerMap)), 'W'))
							{
								boolean FoundBroken;
								POINT Up, Down, Left, Right;

								Up = NextY(PlayerPosition);
								Left = PrevX(PlayerPosition);
								Down = PrevY(PlayerPosition);
								Right = NextX(PlayerPosition);

								FoundBroken = false;

								int i;
							    i=GetFirstIdxListWP(ArrWahanaPlayer);
							    while(i<=GetLastIdxListWP(ArrWahanaPlayer)){
							        if (EQ(LocW(ElmtWP(ArrWahanaPlayer,i)), Up)&& MapW(ElmtWP(ArrWahanaPlayer,i))==PlayerMap){
							            if (BrokenW(ElmtWP(ArrWahanaPlayer,i)))
							            {
							            	BrokenW(ElmtWP(ArrWahanaPlayer,i)) = false;
							            	FailServe = false;
							            	FoundBroken = true;	
							            }
							        }
							        i++;
							    }

							    i=GetFirstIdxListWP(ArrWahanaPlayer);
							    while(i<=GetLastIdxListWP(ArrWahanaPlayer)){
							        if (EQ(LocW(ElmtWP(ArrWahanaPlayer,i)), Down)&& MapW(ElmtWP(ArrWahanaPlayer,i))==PlayerMap){
							            if (BrokenW(ElmtWP(ArrWahanaPlayer,i)))
							            {
							            	BrokenW(ElmtWP(ArrWahanaPlayer,i)) = false;
							            	FailServe = false;
							            	FoundBroken = true;	
							            }
							        }
							        i++;
							    }

							    i=GetFirstIdxListWP(ArrWahanaPlayer);
							    while(i<=GetLastIdxListWP(ArrWahanaPlayer)){
							        if (EQ(LocW(ElmtWP(ArrWahanaPlayer,i)), Right)&& MapW(ElmtWP(ArrWahanaPlayer,i))==PlayerMap){
							            if (BrokenW(ElmtWP(ArrWahanaPlayer,i)))
							            {
							            	BrokenW(ElmtWP(ArrWahanaPlayer,i)) = false;
							            	FailServe = false;
							            	FoundBroken = true;	
							            }
							        }
							        i++;
							    }

							    i=GetFirstIdxListWP(ArrWahanaPlayer);
							    while(i<=GetLastIdxListWP(ArrWahanaPlayer)){
							        if (EQ(LocW(ElmtWP(ArrWahanaPlayer,i)), Left)&& MapW(ElmtWP(ArrWahanaPlayer,i))==PlayerMap){
							            if (BrokenW(ElmtWP(ArrWahanaPlayer,i)))
							            {
							            	BrokenW(ElmtWP(ArrWahanaPlayer,i)) = false;
							            	FailServe = false;
							            	FoundBroken = true;	
							            }
							        }
							        i++;
							    }

							    if (!FoundBroken)
							    {
							 		printf("Tidak ada yang bisa diperbaiki disini!");
									MainCommandSucces = false;
									ENDL;   	
							    }


							} else
							{
								printf("Tidak ada yang bisa diperbaiki disini!");
								MainCommandSucces = false;
								ENDL;
							}

						} else if (IsKataSama(CKata, CDetail))
						{
							printf("Detail\n");
							FindAroundDetail(PlayerPosition, MapG(SearchNodeG(G, PlayerMap)), 1, ArrWahanaPlayer);

						} else if (IsKataSama(CKata, COffice))
						{
							printf("Anda memasuki Office\n");
							printf("Masukkan command Details, Report, atau exit: ");
							STARTKATA(" ");
							ENDL; ENDL;

							if(IsKataSama(CKata, CDetails)) 
							{
								//Details
								int i;
								WahanaPlayer WP;

								printf("List Wahana : \n");
								for (i=0; i<NbElmtListWP(ArrWahanaPlayer); i++){
									printf("  -");
									PrintKata(ElmtWP(ArrWahanaPlayer,i).NamaW);
									printf("\n");
								}

								printf("\nAkhiri command dengan ';' !\n");
								printf("Masukkan Nama Wahana yang ingin dilihat detailnya: ");
								STARTKALIMAT(" ");

								WP =  SearchWahanaPlayerName(CKata, ArrWahanaPlayer);
								//Asumsi inputan selalu benar 
								printf("Nama      : "); PrintKata(NamaW(WP)); 					ENDL;
								printf("Tipe      : "); PrintKata(Akar(StatW(WP)).Type); 		ENDL;
								printf("Harga     : %d", Akar(StatW(WP)).Price); 				ENDL; 
								printf("Lokasi    : (%d,%d)", LocW(WP).X, LocW(WP).Y);			ENDL;
								printf("Deskripsi : "); PrintKata(Akar(StatW(WP)).Description);	ENDL;
								printf("Kapasitas : %d", Akar(StatW(WP)).Capacity); 			ENDL;
								printf("History Upgrade : "); PrintKataListU(HUpgradeW(WP));	ENDL;
								printf("Durasi Wahana: %d", Akar(StatW(WP)).DurationW); 		ENDL;

							}

							else if (IsKataSama(CKata, CReport))
							{
								//Report
								int i;
								WahanaPlayer WP;

								printf("Report Wahana\n");
								printf("List Wahana : \n");
								for (i=0; i<NbElmtListWP(ArrWahanaPlayer); i++){
									printf("  -");
									PrintKata(ElmtWP(ArrWahanaPlayer,i).NamaW);
									printf("\n");
								}
								
								printf("\nAkhiri command dengan ';' !\n");
								printf("Masukkan Nama Wahana yang ingin dilihat reportnya: ");
								STARTKALIMAT(" ");
								WP = SearchWahanaPlayerName(CKata, ArrWahanaPlayer);
								printf("Jumlah wahana dinaiki:  %d\n", TotalPlayW(WP));
								printf("Total penghasilan dari wahana ini : %d\n", TotalMoneyW(WP));
								printf("Jumlah wahana dinaiki hari ini: %d\n", TotalPlayDayW(WP));
								printf("Total Penghasilan dari wahana hari ini: %d\n", TotalMoneyDayW(WP));
								ENDL;

							}

							else if(IsKataSama(CKata, CExit))
							{
								printf("Anda Keluar dari Office\n\n");
							}

						} else if (IsKataSama(CKata, CPrepare))
						{
							PrepPhase = true;
							Day += 1;
						
						} else if (IsKataSama(CKata, CExit))
						{
							Play = false;
						}

						if (MainCommandSucces)
						{
							CurrentTime = TimeSkip(CurrentTime, ArrayCommand, CKata);
							TimeSkipVal = FindDuration(ArrayCommand, CKata);
						}	
					}

					PrepPhase = JGT(CurrentTime, MainEndTime);
										
				}

				CurrentTime = PrepStartTime;
				
			}

			 
		}	


	} else if (IsKataSama(CKata, CLoad))
	{
		printf("load game\n");
	} else if (IsKataSama(CKata, CExit))
	{
		printf("exit\n");
	}

}