#include <string>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <io.h>

using namespace std;

HANDLE hStdin;
DWORD fdwSaveOldMode;


VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);

bool ShowDir(TCHAR path[]) {
    //Показ содержимого текущей директории
    _finddata64i32_t find;
    TCHAR pathfind[MAX_PATH];
    _tcscpy(pathfind, path);
    _tcscat(pathfind, "\\*.*");
    char info[MAX_PATH];
    //Начало Поиска
    int result = _findfirst64i32(pathfind, &find);
    //Очистка экрана
    system("cls");
    int flag = result;
    if (flag == -1) {
        strcpy(info, "Такой Директории Нет");
        //RussianMessage(info);
        printf("%s\n", info);
        return false;
    }
    while (flag != -1) {
        if (strcmp(find.name, ".") && strcmp(find.name, "..")) {
            //Проверяем Директория или Нет
            find.attrib & _A_SUBDIR ? strcpy(info, " Каталог ") :
                strcpy(info, " Файл ");
            //RussianMessage(info);
            //RussianMessage(find.name);
            printf("%30s %10s\n", find.name, info);
        }
        //Продолжаем Поиск
        flag = _findnext64i32(result, &find);
    }
    //ShowCurrentDir(path, info);
    //Очищаем ресурсы, выделенные под поиск
    _findclose(result);
    return true;
}

int main(VOID)
{
    TCHAR path[MAX_PATH];
    //path = "C:\\";
    int getDir = GetCurrentDirectory(sizeof(path), path);
    
    //Показ содержимого текущей директории
    ShowDir(path);

    cout << "path: " << getDir << endl;

    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];
    int counter = 0;

    // Get the standard input handle.
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    // Enable the window and mouse input events.
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    //if (!SetConsoleMode(hStdin, fdwMode))
        //ErrorExit("SetConsoleMode");

    // Loop to read and handle the next 100 input events.
    while (counter++ <= 100)
    {
        // Wait for the events.

        if (!ReadConsoleInput(
            hStdin,      // input buffer handle
            irInBuf,     // buffer to read into
            128,         // size of read buffer
            &cNumRead)) // number of records read
        {
            //ErrorExit("ReadConsoleInput");
        }

        // Dispatch the events to the appropriate handler.

        for (i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case KEY_EVENT: // keyboard input
                KeyEventProc(irInBuf[i].Event.KeyEvent);
                break;

            case MOUSE_EVENT: // mouse input
                MouseEventProc(irInBuf[i].Event.MouseEvent);
                break;

            case FOCUS_EVENT:  // disregard focus events

            case MENU_EVENT:   // disregard menu events
                break;

            default:
                //ErrorExit("Unknown event type");
                break;
            }
        }
    }

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
    printf("Key event: ");

    if (ker.bKeyDown) {
        printf("key pressed %u\n", ker.wVirtualKeyCode);
    }
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
    printf("Mouse event: ");

    switch (mer.dwEventFlags)
    {
    case 0:

        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            printf("left button press \n");
        }
        else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
        {
            printf("right button press \n");
        }
        else
        {
            printf("button press\n");
        }
        break;
    case DOUBLE_CLICK:
        printf("double click\n");
        break;
    case MOUSE_HWHEELED:
        printf("horizontal mouse wheel\n");
        break;
    case MOUSE_MOVED:
        printf("mouse moved\n");
        break;
    case MOUSE_WHEELED:
        printf("vertical mouse wheel\n");
        break;
    default:
        printf("unknown\n");
        break;
    }
}