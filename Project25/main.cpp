#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"
#include <time.h>
#include <algorithm>
#include <vector>
#include <CommCtrl.h> 


HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


const int numberButtons = 16;
int countMove = 0;
int timer = 0;
int tempTimer = 0;
int countTimer = 0;
int remainderTimer = 0;
bool oneStep = true;
int buttonsId[numberButtons] = { IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4, 
							   IDC_BUTTON5, IDC_BUTTON6, IDC_BUTTON7, IDC_BUTTON8, 
							   IDC_BUTTON9, IDC_BUTTON10, IDC_BUTTON11, IDC_BUTTON12,
							   IDC_BUTTON13, IDC_BUTTON14, IDC_BUTTON15, IDC_BUTTON16
							 };
std::vector<int> numbers = {};

HWND hSpin1, hEdit1, hList1, hProgress1;
HWND dlgButtons[numberButtons] = {};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE: {
			EndDialog(hwnd, 0);

			return TRUE;
		}
		case WM_INITDIALOG: {
			SetWindowText(hwnd, _TEXT("GAME"));

			for (int i = 0; i < numberButtons; i++) {
				dlgButtons[i] = GetDlgItem(hwnd, buttonsId[i]);
				SendMessage(dlgButtons[i], WM_SETTEXT, 0, LPARAM(_TEXT("")));

				EnableWindow(dlgButtons[i], FALSE);
			}

			hSpin1 = GetDlgItem(hwnd, IDC_SPIN1);
			hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
			hList1 = GetDlgItem(hwnd, IDC_LIST1);
			hProgress1 = GetDlgItem(hwnd, IDC_PROGRESS1);

			SendMessage(hSpin1, UDM_SETRANGE32, 0, 100);

			SendMessage(hProgress1, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
			SendMessage(hProgress1, PBM_SETPOS, 0, 0);

			SendMessage(hSpin1, UDM_SETBUDDY, WPARAM(hEdit1), 0);

			return TRUE;
		}
		case WM_COMMAND: {
			if (LOWORD(wParam) == IDC_NEWGAME && HIWORD(wParam) == BN_CLICKED) {
				_TCHAR buff[250] = _TEXT("");

				SendMessage(hEdit1, WM_GETTEXT, sizeof(buff) / sizeof(TCHAR), LPARAM(buff));

				countTimer = timer = _ttoi(buff);

				if (countTimer == 0) {
					MessageBox(hwnd, _TEXT("Enter timer value!"), _TEXT("Message"), MB_ICONINFORMATION);

					return TRUE;
				}

				int randNumber = 0;
				int startOfRange = 0;
				int endOfRange = 100;

				countMove = 0;

				srand(time(NULL));

				numbers.clear();

				for (int i = 0; i < numberButtons; i++) {
					EnableWindow(dlgButtons[i], TRUE);

					SendMessage(dlgButtons[i], WM_SETTEXT, 0, LPARAM(_TEXT("")));

					numbers.push_back(rand() % (endOfRange - startOfRange) + startOfRange);
					wsprintf(buff, _TEXT("%d"), numbers[i]);

					SendMessage(dlgButtons[i], WM_SETTEXT, 0, LPARAM(buff));
				}

				std::sort(numbers.begin(), numbers.end());

				SendMessage(hList1, LB_RESETCONTENT, 0, 0);

			
				SetTimer(hwnd, 1, 1000, NULL);

				SendMessage(hProgress1, PBM_SETPOS, 0, 0);

				wsprintf(buff, _TEXT("%d"), countTimer);
				SetWindowText(hwnd, buff);

				oneStep = true;
			}
			else if (LOWORD(wParam) >= IDC_BUTTON1 && LOWORD(wParam) <= IDC_BUTTON16 && HIWORD(wParam) == BN_CLICKED) {
		
				int buttonID = LOWORD(wParam);

				HWND hButton = GetDlgItem(hwnd, buttonID);

				_TCHAR buff[250] = _TEXT("");
				GetWindowText(hButton, buff, sizeof(buff) / sizeof(_TCHAR));

				int value = _ttoi(buff);

				if (value == numbers[countMove]) {
					countMove++;

					EnableWindow(hButton, FALSE);

					SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));
				}

				if (countMove == numberButtons) {
					KillTimer(hwnd, 1);

					MessageBox(hwnd, _TEXT("You WIN!"), _TEXT("Game"), MB_ICONINFORMATION);
				}
			}
			return TRUE;
		}
		case WM_TIMER: {
			countTimer--;

			if (countTimer + 1 == 0) {
				KillTimer(hwnd, 1);

				MessageBox(hwnd, _TEXT("You lose!("), _TEXT("Game"), MB_ICONINFORMATION);

				for (int i = 0; i < numberButtons; i++) {
					EnableWindow(dlgButtons[i], FALSE);
				}

				return TRUE;
			}

			_TCHAR buff[250] = _TEXT("");

			wsprintf(buff, _TEXT("%d"), countTimer);
			SetWindowText(hwnd, buff);

			if (oneStep) {
				oneStep = false;

				int remainder = 100 % timer;

				SendMessage(hProgress1, PBM_SETSTEP, remainder, 0);
				SendMessage(hProgress1, PBM_STEPIT, 0, 0);

				SendMessage(hProgress1, PBM_SETSTEP, 100 / timer, 0);
			}

			SendMessage(hProgress1, PBM_STEPIT, 0, 0);

			return TRUE;
		}
	}

	return FALSE;
}