#include <windows.h>
#include <string>
#include <vector>
#include <windowsx.h>
#include <commctrl.h>

// Контрольні айдішки
#define IDC_NAME_EDIT 101
#define IDC_PROCESS_BUTTON 102
#define IDC_RESULT_EDIT 103
#define IDC_INITIALS_EDIT 104

// Глобальні змінні
HINSTANCE hInst;
HWND hNameEdit, hProcessButton, hResultEdit, hInitialsEdit;

// Прототипи для функцій
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ProcessName(HWND hwnd);
std::vector<std::wstring> SplitString(const std::wstring& input, wchar_t delimiter);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    const wchar_t CLASS_NAME[] = L"StudentNameProcessorClass";

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
        L"Обробка ПІБ студента",    
        WS_OVERLAPPEDWINDOW,        
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        NULL,           
        NULL,      
        hInstance,  
        NULL     
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Форма для введення даних
    CreateWindow(
        L"STATIC",
        L"Введіть прізвище, ім'я та по батькові (розділені пробілом):",
        WS_VISIBLE | WS_CHILD,
        20, 20, 500, 20,
        hwnd, NULL, hInstance, NULL
    );

    hNameEdit = CreateWindow(
        L"EDIT",
        L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        20, 50, 540, 25,
        hwnd, (HMENU)IDC_NAME_EDIT, hInstance, NULL
    );

    hProcessButton = CreateWindow(
        L"BUTTON",
        L"Обробити",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        230, 90, 120, 30,
        hwnd, (HMENU)IDC_PROCESS_BUTTON, hInstance, NULL
    );

    CreateWindow(
        L"STATIC",
        L"Довжини компонентів ПІБ:",
        WS_VISIBLE | WS_CHILD,
        20, 140, 500, 20,
        hwnd, NULL, hInstance, NULL
    );

    hResultEdit = CreateWindow(
        L"EDIT",
        L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY,
        20, 170, 540, 100,
        hwnd, (HMENU)IDC_RESULT_EDIT, hInstance, NULL
    );

    CreateWindow(
        L"STATIC",
        L"Прізвище та ініціали:",
        WS_VISIBLE | WS_CHILD,
        20, 290, 500, 20,
        hwnd, NULL, hInstance, NULL
    );

    hInitialsEdit = CreateWindow(
        L"EDIT",
        L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
        20, 320, 540, 25,
        hwnd, (HMENU)IDC_INITIALS_EDIT, hInstance, NULL
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
        if (LOWORD(wParam) == IDC_PROCESS_BUTTON && HIWORD(wParam) == BN_CLICKED) {
            ProcessName(hwnd);
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

void ProcessName(HWND hwnd) {
    //Отримання тексту з textbox
    int nameLength = GetWindowTextLength(hNameEdit);
    if (nameLength == 0) {
        MessageBox(hwnd, L"Будь ласка, введіть прізвище, ім'я та по батькові!", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }

    std::vector<wchar_t> nameBuffer(nameLength + 1);
    GetWindowText(hNameEdit, nameBuffer.data(), nameLength + 1);
    std::wstring fullName(nameBuffer.data());

    //Поділ слів на частини
    std::vector<std::wstring> nameParts = SplitString(fullName, L' ');

    if (nameParts.size() < 3) {
        MessageBox(hwnd, L"Введіть повне ПІБ (прізвище, ім'я, по батькові)!", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }

    std::wstring surname = nameParts[0];
    std::wstring firstName = nameParts[1];
    std::wstring patronymic = nameParts[2];

    // Обчислення довжин
    int surnameLength = surname.length();
    int firstNameLength = firstName.length();
    int patronymicLength = patronymic.length();
    int fullNameLength = fullName.length();

    std::wstring result =
        L"Довжина прізвища: " + std::to_wstring(surnameLength) + L" символів\r\n" +
        L"Довжина імені: " + std::to_wstring(firstNameLength) + L" символів\r\n" +
        L"Довжина по батькові: " + std::to_wstring(patronymicLength) + L" символів\r\n" +
        L"Загальна довжина: " + std::to_wstring(fullNameLength) + L" символів";

    std::wstring initials = surname + L" " + firstName.substr(0, 1) + L"." + patronymic.substr(0, 1) + L".";

    // Виведення результатів
    SetWindowText(hResultEdit, result.c_str());
    SetWindowText(hInitialsEdit, initials.c_str());
}

// Поділ стрічки за певним символом
std::vector<std::wstring> SplitString(const std::wstring& input, wchar_t delimiter) {
    std::vector<std::wstring> result;
    std::wstring current;

    for (wchar_t ch : input) {
        if (ch == delimiter) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            current += ch;
        }
    }

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;
}