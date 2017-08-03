// Statki.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BattleShips.h"
#include <ctime>
#include <cstdlib>
#include <Uxtheme.h>
#pragma comment(lib,"UxTheme.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HINSTANCE hInst;
int gracze = 0;
/*	1-bot
	2-hot seat
	3-"online"
*/
int gMod = 0;
int gsTryb = 0, gsTryb2 = 0;;
int oGracz = 1;
int p1[100] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
int p2[100] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
int rLicznik = 0, rLicznik2 = 1, rRuch = 0, rStatek = 4;
int sObecny[4] = { -1,-1,-1,-1 };
int tryb = 0;
int znalezione = 0, znalezione2 = 0;
bool CALLBACK losowaPlansza();
bool CALLBACK sprawdzObecny(int);
bool CALLBACK sprawdzOkolice(int, int*);
bool CALLBACK sprawdzStatek(int);
bool CALLBACK stworzStatek(int);
bool CALLBACK ustawPlansze(int, int*);
int CALLBACK wykonajRuch();
void CALLBACK cofnijZmiany();
void CALLBACK mieszajTablice(int*, int);
void CALLBACK nowaGra(HWND);
void CALLBACK resetPlanszy(int*, int);
void CALLBACK resetZmienne();
void CALLBACK schowajPlansze(HWND, int);
INT_PTR CALLBACK Pomoc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Statki(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Wybor(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	HWND hDlg;
	MSG msg;
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_STATKI), 0, Statki, 0);
	ShowWindow(hDlg, nCmdShow);
	SetWindowTheme(hDlg, L"Explorer", NULL);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
bool CALLBACK losowaPlansza() {
	bool s = false;
	srand((unsigned int)time(0));
	do {
		resetPlanszy(p2, 100);
		s = stworzStatek(4);
		if (s == true) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					s = stworzStatek(3);
					if (s == true)break;
					else cofnijZmiany();
				}
				if (s == false)break;
			}
		}
		if (s == true) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 15; j++) {
					s = stworzStatek(2);
					if (s == true)break;
					else cofnijZmiany();
				}
				if (s == false)break;
			}
		}
		if (s == true) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 30; j++) {
					s = stworzStatek(1);
					if (s == true)break;
					else cofnijZmiany();
				}
				if (s == false)break;
			}
		}
	} while (s == false);
	return true;
}
bool CALLBACK sprawdzObecny(int x) {
	for (int i = 0; i < 4; i++) {
		if (sObecny[i] == x)return true;
	}
	return false;
}
bool CALLBACK sprawdzOkolice(int x, int* p) {
	if (p[x] == 1)return false;
	if (x % 10 > 0) {
		if (p[x - 1] == 1 && sprawdzObecny(x - 1) == false)return false;//lewo
		if (x > 10) {
			if (p[x - 11] == 1 && sprawdzObecny(x - 11) == false)return false;//gora lewo
		}
		if (x < 90) {
			if (p[x + 9] == 1 && sprawdzObecny(x + 9) == false)return false;//dol lewo
		}
	}
	if (x % 10 < 9) {
		if (p[x + 1] == 1 && sprawdzObecny(x + 1) == false)return false;//prawo
		if (x > 9) {
			if (p[x - 9] == 1 && sprawdzObecny(x - 9) == false)return false;//gora prawo
		}
		if (x < 89) {
			if (p[x + 11] == 1 && sprawdzObecny(x + 11) == false)return false;//gora dol
		}
	}
	if (x > 9) {
		if (p[x - 10] == 1 && sprawdzObecny(x - 10) == false)return false;//gora
	}
	if (x < 90) {
		if (p[x + 10] == 1 && sprawdzObecny(x + 10) == false)return false;//dol
	}
	return true;
}
bool CALLBACK sprawdzStatek(int x) {
	int tmp = 0;
	for (int i = 0; i < 4; i++) {
		tmp = sObecny[i];
		if (tmp == -1)break;//i tutaj
		if (gMod == 0) {
			if ((tmp == x - 1 && x % 10 > 0) || (tmp == x + 1 && x % 10 < 9) || tmp == x + 10 || tmp == x - 10)return true;
		}
		else if (gMod == 1) {
			if (gsTryb == 1) {
				if ((tmp == x - 1 && x % 10 > 0) || (tmp == x + 1 && x % 10 < 9))return true;
			}
			else if (gsTryb == 2) {
				if (tmp == x + 10 || tmp == x - 10)return true;
			}
			else {
				if ((tmp == x - 1 && x % 10 > 0) || (tmp == x + 1 && x % 10 < 9)) {
					gsTryb = 1;
					return true;
				}
				else if (tmp == x + 10 || tmp == x - 10) {
					gsTryb = 2;
					return true;
				}
			}
		}
	}
	return false;
}
bool CALLBACK stworzStatek(int r) {
	resetPlanszy(sObecny, 4);
	int ruchy[4] = { 1,-1,10,-10 };
	if (gMod == 1)gsTryb2 = std::rand() % 2+1;
	else gsTryb2 = 0;
	if (gMod==1&&gsTryb2==2) {
		ruchy[0] = 1;
		ruchy[1] = -1;
		ruchy[2] = 1;
		ruchy[3] = -1;
		gsTryb = 1;
	}
	else if(gMod==1&&gsTryb2==1){
		gsTryb = 2;
		ruchy[0] = 10;
		ruchy[1] = -10;
		ruchy[2] = 10;
		ruchy[3] = -10;
	}
	mieszajTablice(ruchy, 3);
	bool s = false;
	int p = std::rand() % 100;
	int pp = 0;
	int licznik = 0;
	int tmp = p;
	s = sprawdzOkolice(tmp, p2);
	if (s == false)return false;
	sObecny[0] = tmp;
	p2[tmp] = 1;
	for (int i = 1; i < r; i++) {
		licznik = 0;
		do {
			for (int j = 0; j < 4; j++) {
				tmp = sObecny[licznik] + ruchy[j];
				if (tmp >= 0 && tmp < 100) {
					s = sprawdzStatek(tmp);
					if (s == true) {
						s = sprawdzOkolice(tmp, p2);
						if (s == true) {
							sObecny[i] = tmp;
							p2[tmp] = 1;
							break;
						}
					}
				}
			}
			if (s == true)break;
			licznik++;
		} while (licznik < pp);
		if (s == false)return false;
		pp++;
		mieszajTablice(ruchy, 3);
		if (i > 1)mieszajTablice(sObecny, i - 1);
	}
	return true;
}
bool CALLBACK ustawPlansze(int x, int* p) {
	bool s = true;
	if (rRuch > 0)s = sprawdzStatek(x);//tutaj
	if (s == true) {
		s = sprawdzOkolice(x, p);
		if (s == true) {
			p[x] = 1;
			sObecny[rRuch] = x;
			rRuch++;
			if (rRuch == rStatek) {
				rLicznik++;
				if (rLicznik == rLicznik2) {
					rLicznik = 0;
					rStatek--;
					rLicznik2++;
				}
				rRuch = 0;
				gsTryb = 0;
				resetPlanszy(sObecny, 4);
			}
			return true;
		}
	}
	return false;
}
int CALLBACK wykonajRuch() {
	bool s = false;
	int ruchy[4] = { 1,-1,10,-10 };
	mieszajTablice(ruchy, 3);
	for (int i = 0; i < 3; i++) {
		if (sObecny[i] != -1) {
			s = true;
			break;
		}
	}
	if (s == true) {
		for (int i = 0; i < 4; i++) {
			if (sObecny[i] == -1)break;
			for (int j = 0; j < 4; j++) {
				int tmp = sObecny[i] + ruchy[j];
				if (tmp > 99 || tmp < 0)continue;
				if (p1[tmp] == -1)return tmp;
				else if (p1[tmp] == 1) {
					sObecny[rRuch] = tmp;
					rRuch++;
					return tmp;
				}
			}
		}
	}
	int r;
	do {
		r = std::rand() % 100;
	} while (p1[r] == 2);
	rRuch = 1;
	if (p1[r] == 1)sObecny[0] = r;
	return r;
}
void CALLBACK cofnijZmiany() {
	for (int i = 0; i < 4; i++) {
		int tmp = sObecny[i];
		if (tmp != -1) {
			p2[tmp] = -1;
		}
	}
}
void CALLBACK mieszajTablice(int* p, int s) {
	for (int i = 0; i < 5; i++) {
		int los = (std::rand() % s) + 1;
		int xx = p[0];
		p[0] = p[los];
		p[los] = xx;
	}
}
void CALLBACK nowaGra(HWND hDlg) {
	resetPlanszy(p1, 100);
	resetPlanszy(p2, 100);
	schowajPlansze(hDlg, 1000);
	schowajPlansze(hDlg, 1100);
	resetZmienne();
	SetWindowText(GetDlgItem(hDlg, IDC_INFO), "");
}
void CALLBACK resetPlanszy(int* p, int r) {
	for (int i = 0; i < r; i++)p[i] = -1;
}
void CALLBACK resetZmienne() {
	oGracz = 1;
	tryb = 0;
	rStatek = 4;
	rRuch = 0;
	rLicznik = 0, rLicznik2 = 1;
	znalezione = 0, znalezione2 = 0;
	gsTryb = 0;
	gsTryb = 0;
}
void CALLBACK schowajPlansze(HWND hDlg, int x) {
	HBITMAP y = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WATER));
	for (int i = 0; i < 100; i++)SendMessage(GetDlgItem(hDlg, x + i), BM_SETIMAGE, 0, (LPARAM)y);
	DeleteObject(y);
}
INT_PTR CALLBACK Pomoc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK||LOWORD(wParam)==IDCANCEL)
		{
			EndDialog(hDlg, 0);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Statki(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		nowaGra(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDM_EXIT)DestroyWindow(hDlg);
		else if (LOWORD(wParam) == IDM_HELP) {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP), hDlg, Pomoc);
		}
		else if (LOWORD(wParam) == IDM_STATKI1) {
			gracze = 1;
			gMod = 1;
			nowaGra(hDlg);
		}
		else if (LOWORD(wParam) == IDM_STATKI2) {
			gracze = 2;
			gMod = 1;
			nowaGra(hDlg);
		}
		else if (LOWORD(wParam) == IDM_STATKI3) {
			gracze = 3;
			gMod = 1;
			nowaGra(hDlg);
		}
		else if (LOWORD(wParam) == IDM_MSTATKI1) {
			gracze = 1;
			gMod = 0;
			nowaGra(hDlg);
		}
		else if (LOWORD(wParam) == IDM_MSTATKI2) {
			gracze = 2;
			gMod = 0;
			nowaGra(hDlg);
		}
		else if (LOWORD(wParam) == IDM_MSTATKI3) {
			gracze = 3;
			gMod = 0;
			nowaGra(hDlg);
		}
		else if (LOWORD(wParam) >= 1000 && LOWORD(wParam) < 1100 && gracze > 0) {
			if (tryb == 0) {//ustawianie 1 planszy
				bool s = ustawPlansze(LOWORD(wParam) - 1000, p1);
				if (s == true) {
					HBITMAP x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIP));
					SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETIMAGE, 0, (LPARAM)x);
					DeleteObject(x);
				}
				if (rStatek == 0) {
					if (gracze == 1) {
						tryb = 1;
						losowaPlansza();
						resetPlanszy(sObecny, 4);
						SetWindowText(GetDlgItem(hDlg, IDC_INFO), "Have fun");
					}
					else if (gracze == 2) {//hot seat
						schowajPlansze(hDlg, 1000);
						resetZmienne();
						tryb = 2;
					}
					else if (gracze == 3)tryb = 4;
				}
			}
			else if (tryb == 3) {//ruch gracza nr 2
				if (gracze == 2 && oGracz == 2 && p1[LOWORD(wParam) - 1000] != 2) {
					HBITMAP x;
					if (p1[LOWORD(wParam) - 1000] == 1) {
						x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HIT));
						znalezione2++;
					}
					else x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MISS));
					SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETIMAGE, 0, (LPARAM)x);
					DeleteObject(x);
					if (znalezione2 == 20) {
						SetWindowText(GetDlgItem(hDlg, IDC_INFO), "Player 2 won.");
						tryb = -1;
						break;
					}
					oGracz = 1;
					p1[LOWORD(wParam) - 1000] = 2;
				}
			}
		}
		else if (LOWORD(wParam) >= 1100 && LOWORD(wParam) < 1200 && gracze > 0) {
			if ((tryb == 1 || tryb == 3) && oGracz == 1 && p2[LOWORD(wParam) - 1100] != 2) {//ruch gracza nr 1
				HBITMAP x;
				if (p2[LOWORD(wParam) - 1100] == 1) {
					x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HIT));
					znalezione++;
				}
				else x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MISS));
				SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETIMAGE, 0, (LPARAM)x);
				DeleteObject(x);
				if (znalezione == 20) {
					SetWindowText(GetDlgItem(hDlg, IDC_INFO), "Player 1 won.");
					tryb = -1;
					break;
				}
				p2[LOWORD(wParam) - 1100] = 2;
				if (gracze == 1) {
					int xyz = wykonajRuch();
					if (xyz != -1) {
						if (p1[xyz] == -1)x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MISS));
						else {
							x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HIT));
							znalezione2++;
						}
						p1[xyz] = 2;
						SendMessage(GetDlgItem(hDlg, 1000 + xyz), BM_SETIMAGE, 0, (LPARAM)x);
						DeleteObject(x);
						if (znalezione2 == 20) {
							SetWindowText(GetDlgItem(hDlg, IDC_INFO), "You lost!");
							tryb = -1;
							break;
						}
						if (sObecny[3] != -1)resetPlanszy(sObecny, 4);
					}
				}
				else if (gracze == 2) {
					oGracz = 2;
				}
			}
			else if (tryb == 2) {
				bool s = ustawPlansze(LOWORD(wParam) - 1100, p2);
				if (s == true) {
					HBITMAP x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIP));
					SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETIMAGE, 0, (LPARAM)x);
					DeleteObject(x);
				}
				if (rStatek == 0) {
					tryb = 3;
					schowajPlansze(hDlg, 1100);
					SetWindowText(GetDlgItem(hDlg, IDC_INFO), "Have fun.");
				}
			}
			else if (tryb == 4) {
				int id = DialogBox(hInst, MAKEINTRESOURCE(IDD_WYBOR), hDlg, Wybor);
				if (id > 0) {
					HBITMAP x;
					if (id == IDC_PUDLO)x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MISS));
					else if (id == IDC_TRAF) {
						x = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HIT));
						znalezione++;
						if (znalezione == 20) {
							SetWindowText(GetDlgItem(hDlg, IDC_INFO), "You won!");
							tryb = -1;
						}
					}
					SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETIMAGE, 0, (LPARAM)x);
					DeleteObject(x);
				}
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Wybor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		POINT pt;
		GetCursorPos(&pt);
		SetWindowPos(hDlg, 0, pt.x - 60, pt.y - 80, 0, 0, SWP_NOSIZE);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_PUDLO || LOWORD(wParam) == IDC_TRAF)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}