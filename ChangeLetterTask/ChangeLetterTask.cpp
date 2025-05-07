#include <windows.h>
#include <string>
#include <vector>
#include <windowsx.h>
#include <commctrl.h>

#define IDC_WORD_EDIT 101
#define IDC_REPLACE_BUTTON 102
#define IDC_RESULT_EDIT 103

HINSTANCE hInst;
HWND hWordEdit, hReplaceButton, hResultEdit;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ReplaceChars(HWND hwnd);
std::wstring ReplaceAWithO(const std::wstring& input);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    const wchar_t CLASS_NAME[] = L"CharReplacerClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    
    HWND hwnd = CreateWindowEx(
        0,                          
        CLASS_NAME,                 
        L"Заміна літер 'а' на 'о'",  
        WS_OVERLAPPEDWINDOW,        
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL,        
        NULL,       
        hInstance,  
        NULL        
    );

    if (hwnd == NULL) {
        return 0;
    }

    
    CreateWindow(
        L"STATIC",
        L"Введіть слово:",
        WS_VISIBLE | WS_CHILD,
        20, 20, 460, 20,
        hwnd, NULL, hInstance, NULL
    );

    hWordEdit = CreateWindow(
        L"EDIT",
        L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        20, 50, 460, 25,
        hwnd, (HMENU)IDC_WORD_EDIT, hInstance, NULL
    );

    hReplaceButton = CreateWindow(
        L"BUTTON",
        L"Замінити 'а' на 'о'",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        150, 90, 200, 30,
        hwnd, (HMENU)IDC_REPLACE_BUTTON, hInstance, NULL
    );

    CreateWindow(
        L"STATIC",
        L"Результат:",
        WS_VISIBLE | WS_CHILD,
        20, 140, 460, 20,
        hwnd, NULL, hInstance, NULL
    );

    hResultEdit = CreateWindow(
        L"EDIT",
        L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
        20, 170, 460, 25,
        hwnd, (HMENU)IDC_RESULT_EDIT, hInstance, NULL
    );

    
    CreateWindow(
        L"STATIC",
        L"Програма замінює всі літери 'а' на 'о' у введеному слові.",
        WS_VISIBLE | WS_CHILD,
        20, 220, 460, 20,
        hwnd, NULL, hInstance, NULL
    );

   
    ShowWindow(hwnd, nCmdShow);

    
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_REPLACE_BUTTON && HIWORD(wParam) == BN_CLICKED) {
            ReplaceChars(hwnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void ReplaceChars(HWND hwnd) {
    
    int wordLength = GetWindowTextLength(hWordEdit);
    if (wordLength == 0) {
        MessageBox(hwnd, L"Будь ласка, введіть слово!", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }

    std::vector<wchar_t> wordBuffer(wordLength + 1);
    GetWindowText(hWordEdit, wordBuffer.data(), wordLength + 1);
    std::wstring word(wordBuffer.data());

    
    std::wstring result = ReplaceAWithO(word);

    SetWindowText(hResultEdit, result.c_str());
}

std::wstring ReplaceAWithO(const std::wstring& input) {
    std::wstring result = input;

    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] == L'а' || result[i] == L'А') {  
            result[i] = (result[i] == L'а') ? L'о' : L'О';
        }
        else if (result[i] == L'a' || result[i] == L'A') {  
            result[i] = (result[i] == L'a') ? L'o' : L'O';
        }
    }

    return result;
}