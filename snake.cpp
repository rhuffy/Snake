/*
Snake
Raymond Huffman
Matthew O'Cadiz
7/12/2013
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <stdio.h>


using namespace std;

const int DIM=76;
const int YDIM=DIM/2;

WORD GetConsoleTextAttribute (HANDLE hCon)
{
  CONSOLE_SCREEN_BUFFER_INFO con_info;
  GetConsoleScreenBufferInfo(hCon, &con_info);
  return con_info.wAttributes;
}

void displayGame(char board[][YDIM]);
bool isDead(char board[][YDIM],char f);
bool moveSnake(char board[][YDIM]);
void turnSnake(char board[][YDIM],int headx,int heady,char d);
char setTail(char board[][YDIM],int x,int y);
void spawnFood(char board[][YDIM]);
bool isFood (char board[][YDIM],char f);
int score=0;

int headx,heady,tailx,taily;
 
int main()
{
	ifstream highscores;
	ofstream newscores;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL));  
	char file[200]="D:\\Snake\\highscore.txt";
	char n = 'y';
	cout << "\n - - - - - Welcome to Snake - - - - -\n\nProgrammed by Raymond Huffman and Matthew O'Cadiz\n\nUse W S A D to control your snake.\n\nBe sure to maximize your window.\n\n";
	int m;
	system("pause");
	while(n=='y')
	{
		char board [DIM][YDIM];
		char d,in;
		int level;
		bool ep=false;
		bool tr=false;
		bool end=false;
		score=0;
		system("cls");
		SetConsoleTextAttribute(hConsole,7);
		cout << "Please chose a color, (g)reen, (b)lue, (r)ed, (p)urple, (y)ellow, or (w)hite.\n";
		in=getch();
		while(in!='g'&&in!='b'&&in!='r'&&in!='p'&&in!='y'&&in!='w')
		{
			cout << "Please pick a valid color.\n";
			in=getch();
		}
		if(in=='g')
			SetConsoleTextAttribute(hConsole,10);
		if(in=='b')
			SetConsoleTextAttribute(hConsole,11);
		if(in=='r')
			SetConsoleTextAttribute(hConsole,12);
		if(in=='p')
			SetConsoleTextAttribute(hConsole,13);
		if(in=='y')
			SetConsoleTextAttribute(hConsole,14);
		if(in=='w')
			SetConsoleTextAttribute(hConsole,15);

		cout << "\nPick a level, (e)asy, (m)edium, (h)ard, e(p)ilepsy, or (t)rouser\n";
		in=getch();
		while(in!='e'&&in!='m'&&in!='h'&&in!='p'&&in!='t')
		{
			cout << "Please pick a valid level\n";
			in=getch();
		}
		if(in=='e')
		{
			level=100;
			m=0;
		}
		if(in=='m')
		{
			level=50;
			m=1;
		}
		if(in=='h')
		{
			level=10;
			m=2;
		}
		if(in=='p')
		{
			level=5;
			ep=true;
			system("cls");
			cout << "WARNING\n\nSome people (about 1 in 4000) may have seizures or blackouts triggered by light\nflashes or patterns, such as while watching TV or playing video games,\neven if they have never had a seizure before. \nAnyone who has had a seizure, loss of awareness, or other symptom linked to an\nepileptic condition should consult a doctor before playing a video game.\n\n";
			m=3;
		}
		if(in=='t')
		{
			level=10;
			tr=true;
			m=4;
		}
	
		memset(board,0,DIM*YDIM);
		board[DIM/2+1][YDIM/2]='R';
		headx=DIM/2+1;
		heady=YDIM/2;
		board[DIM/2][YDIM/2]='h';
		board[DIM/2-1][YDIM/2]='h';
		board[DIM/2-2][YDIM/2]='l';
		if(!tr)
		{
			tailx=DIM/2-2;
			taily=YDIM/2;
			spawnFood(board);
		}

		cout << "Press any key to begin . . .";

		while (!end)
		{
			d=getch();
			while(!kbhit()&&!end)
			{
				if(d=='p')
				{
					system("pause");
					d='\0';
				}
				turnSnake(board,headx,heady,d);//Snake turns
				displayGame(board);//Display the game
				end=moveSnake(board);//Snake moves - Snake grows
				if(ep)
					SetConsoleTextAttribute(hConsole,rand() % (256));
				if(tr)
					score++;
				Sleep(level);//Clock
			}
		}
	if(ep==true)
		SetConsoleTextAttribute(hConsole,7);
	cout << "\n - - - - - - - - - - YOU LOSE - - - - - - - - - -\n\nYou got "<< score <<" points!\n\n";//You Lose	
	char highscore1[200];

	int highscore[5];
	highscores.clear();
	highscores.open(file);
	while(!highscores)
	{
		cout << "Could not open highscores file.  Please enter the correct file path: ";
		cin >> file;
		highscores.open(file);
	}
	highscores.seekg(0);
	int c;
	for(c=0;c<5;c++)
	{
		highscores.getline(highscore1,199);
		highscore[c] = atoi (highscore1);
	}
	highscores.close();
	if(score>highscore[m])
	{
		highscore[m]=score;
		cout << "New highscore!\n";

		newscores.open(file);
		for(c=0;c<5;c++)
		{
			newscores << highscore[c] << "\n";
		}
		newscores.close();
	}
	else
	{
		cout << "The highscore is " << highscore[m] << ".\n";
	}
	cout << "Would you like to play again, (y)es or (n)o?\n";
	n=getch();
	while(n!='y'&&n!='n')
	{
		cout << "Please choose y or n\n";
		n=getch();
	}
	}
	system ("pause");
	return 0;
}

bool moveSnake(char board[][YDIM])
{
	//Spawns new head
	char f;
	if(board[headx][heady]=='U')
	{
		f=board[headx][heady-1];
		board[headx][heady-1]='U';
		board[headx][heady]='t';
		heady=heady-1;
	}
	else if(board[headx][heady]=='D')
	{
		f=board[headx][heady+1];
		board[headx][heady+1]='D';
		board[headx][heady]='g';
		heady=heady+1;
	}
	else if(board[headx][heady]=='L')
	{
		f=board[headx-1][heady];
		board[headx-1][heady]='L';
		board[headx][heady]='f';
		headx=headx-1;
	}
	else if(board[headx][heady]=='R')
	{
		f=board[headx+1][heady];
		board[headx+1][heady]='R';
		board[headx][heady]='h';
		headx=headx+1;
	}


	//Deletes old tail
	if(!isFood(board,f))
	{
		if(board[tailx][taily]=='i')
		{
			board[tailx][taily-1]=setTail(board,tailx,taily-1);
			board[tailx][taily]='\0';
			taily=taily-1;
		}
		else if(board[tailx][taily]=='k')
		{
			board[tailx][taily+1]=setTail(board,tailx,taily+1);
			board[tailx][taily]='\0';
			taily=taily+1;
		}	
		else if(board[tailx][taily]=='j')
		{
			board[tailx-1][taily]=setTail(board,tailx-1,taily);
			board[tailx][taily]='\0';
			tailx=tailx-1;
		}	
		else if(board[tailx][taily]=='l')
		{
			board[tailx+1][taily]=setTail(board,tailx+1,taily);
			board[tailx][taily]='\0';
			tailx=tailx+1;
		}
	}
	else 
		score++;

	if(isDead(board,f))//Is snake dead?
		return true;
	else
		return false;
}

char setTail(char board[][YDIM],int x,int y)
{
	if(board[x][y]=='t')
	{
		return('i');
	}
	else if(board[x][y]=='g')
	{
		return('k');
	}
	else if(board[x][y]=='f')
	{
		return('j');
	}
	else if(board[x][y]=='h')
	{
		return('l');
	}
}

void displayGame(char board[][YDIM])
{
	int x;
	int y;
	system("cls");
	for(y=0;y<YDIM;y++)
	{
		for(x=0;x<DIM;x++)
		{
			if(board[x][y]=='t'||board[x][y]=='g'||board[x][y]=='f'||board[x][y]=='h'||board[x][y]=='i'||board[x][y]=='k'||board[x][y]=='j'||board[x][y]=='l')
			{
				cout << "*";
			}
			else if(board[x][y]=='U'||board[x][y]=='D'||board[x][y]=='L'||board[x][y]=='R')
			{
				cout << "O";
			}
			else if(board[x][y]=='F')
			{
				cout << "X";
			}
			else if(x==0||y==0||x==DIM-1||y==YDIM-1)
			{
				cout << "\xDB";
			}
			else
			{
				cout << " ";
			}
		}
		cout << "\n";
	}
}

void turnSnake(char board [][YDIM], int headx, int heady, char d)
{
	if (d == 'w' && board[headx][heady] != 'D')
	{
	 board [headx] [heady] = 'U';
	}

	else if (d == 'd' && board[headx][heady] != 'L')
	{
		board [headx] [heady] = 'R';
	}

	else if (d == 's' && board[headx][heady] != 'U')
	{
	board [headx] [heady] = 'D';
	}

	else if(d == 'a' && board[headx][heady] != 'R')
	{
	board [headx] [heady] = 'L';
	}


}

void spawnFood(char board [][YDIM])
{
int foodx = rand() % (DIM-2)+1;
int foody = rand() % (YDIM-2)+1;
board [foodx][foody] = 'F';
}

bool isDead (char board [][YDIM],char f )
{
	if(f == 't' || f == 'g' || f == 'f' || f == 'h' || f == 'i' || f == 'k' || f == 'j' || f == 'l' || headx == 0 || headx == DIM-1 || heady == 0 || heady == YDIM-1)
		return true;
	else
		return false; 
}

bool isFood (char board [][YDIM],char f)
{
	if(f=='F')
	{
		spawnFood(board);
		return true;
	}
	else
		return false;
}

