#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <array>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <ctime>

using std::cout;
using std::cin;
using std::string;
using std::chrono::system_clock;
using std::to_string;
using std::chrono::duration;

//Graphics
int Screensize[2] = { 12,6 };
string Screen;

//Input
int MousePos[2];

//Other
bool Running = true;
int Turn = 0;

void FontSize(int Size[]) {
    static CONSOLE_FONT_INFOEX  fontex;
    fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, 0, &fontex);
    fontex.FontWeight = 700;
    fontex.dwFontSize.X = Size[0];
    fontex.dwFontSize.Y = Size[1];
    SetCurrentConsoleFontEx(hOut, NULL, &fontex);
    return;
}

char CheckScreen(int Pos[]) {
    return Screen[Pos[0] + Pos[1] * Screensize[0]];
}

void EditScreen(int Pos[], char Character) {
    Screen[Pos[0] + Pos[1] * Screensize[0]] = Character;
}

void ClearScreen()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;
    cursorInfo.dwSize = 1;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void EmptyScreen() {
    string Line;
    Screen = "";
    for (int x = 0; x < Screensize[0]; x++) {
        Line += " ";
    }
    for (int y = 0; y < Screensize[1]; y++) {
        Screen += Line;
    }
}

void MakeFrame() {
    for (int n = 0; n < Screensize[1]; n++) {
        int Pos[2] = { 0,n };
        EditScreen(Pos, '|');
    }
    for (int n = 0; n < Screensize[1]; n++) {
        int Pos[2] = { Screensize[0] - 1 ,n };
        EditScreen(Pos, '|');
    }
    for (int n = 0; n < Screensize[0]; n++) {
        int Pos[2] = { n, Screensize[1] - 1 };
        EditScreen(Pos, '-');
    }
    for (int n = 0; n < 2; n++) {
        int Pos[2] = { n * (Screensize[0] - 1), Screensize[1] - 1 };
        EditScreen(Pos, '+');
    }
}

void Setup() {
    EmptyScreen();
    MakeFrame();
    ShowConsoleCursor(false);
    int Size[2] = { 10,15 };
    FontSize(Size);
}

int Clamp(int min, int max, int num) {
    if (num < min) {
        num = min;
    }
    if (num > max) {
        num = max;
    }
    return num;
}

bool Floating(int Pos[]) {
    for (int n = Screensize[1] - 1; n > Pos[1]; n--) {
        int TempPos[2] = { Pos[0], n };
        if (CheckScreen(TempPos) == ' ') {
            return true;
        }
    }
    return false;
}

void InputLogic() {
    //Mouse
    POINT p;
    if (GetCursorPos(&p)) {
        if (ScreenToClient(GetConsoleWindow(), &p))
        {
            MousePos[0] = floor(p.x / (85 / 12));
            MousePos[1] = floor(p.y / (90 / 6));

            MousePos[0] = Clamp(0, Screensize[0] - 1, MousePos[0]);
            MousePos[1] = Clamp(0, Screensize[1] - 1, MousePos[1]);
            cout << "\n" + to_string(p.x) + "," + to_string(p.y) + "      ";
        }
    }
    if (GetKeyState(27) & 0x8000) //if escape is pressed
    {
        Running = false;
    }
    if (GetKeyState('X') & 0x8000) //if escape is pressed
    {
        if (CheckScreen(MousePos) == ' ' && Turn == 0 && Floating(MousePos) == false) {
            EditScreen(MousePos, 'X');
            Turn = 1;
        }
        if (CheckScreen(MousePos) == ' ' && Turn == 1 && Floating(MousePos) == false) {
            EditScreen(MousePos, 'O');
            Turn = 0;
        }
    }
}


char CheckforWin() {
    //Check horizontals
    for (int y = 0; y < Screensize[1]; y++) {
        int Pos[2] = { 0,y};
        string Temp;
        Temp += CheckScreen(Pos);
        for (int x = 1; x < Screensize[0]; x++) {
            int Pos[2] = { x,y };
            if (CheckScreen(Pos) == Temp[0]) {
                Temp += Temp[0];
                if (Temp.size() >= 4 && (Temp[0] == 'X' || Temp[0] == 'O')) {
                    return Temp[0];
                }
            }
            else {
                Temp = "";
                Temp += CheckScreen(Pos);
            }
        }
    }
    //Check verticals
    for (int x = 0; x < Screensize[0]; x++) {
        int Pos[2] = { x,0 };
        string Temp;
        Temp += CheckScreen(Pos);
        for (int y = 1; y < Screensize[1]; y++) {
            int Pos[2] = { x,y };
            if (CheckScreen(Pos) == Temp[0]) {
                Temp += Temp[0];
                if (Temp.size() >= 4 && (Temp[0] == 'X' || Temp[0] == 'O')) {
                    return Temp[0];
                }
            }
            else {
                Temp = "";
                Temp += CheckScreen(Pos);
            }
        }
    }
    //Check diagonal up
    for (int n = 0; n < Screensize[0] + Screensize[1] - 1; n++) {
        int Pos[2] = { Clamp(0,Screensize[0] - 1,n),Clamp(0,Screensize[1] - 1, n - Screensize[0] - 1)};
        string Temp;
        for (int m = 0; m < Screensize[0] * Screensize[1];m++ ) { //Very bad practice
            if (CheckScreen(Pos) == Temp[0]) {
                Temp += Temp[0];
                if (Temp.size() >= 4 && (Temp[0] == 'X' || Temp[0] == 'O')) {
                    return Temp[0];
                }
            }
            else {
                Temp = "";
                Temp += CheckScreen(Pos);
            }

            Pos[0] -= 1;
            Pos[1] += 1;
            if (Pos[0] < 0) {
                break;
            }
            if (Pos[1] > Screensize[1] - 1) {
                break;
            }
        }
    }
    //Check diagonal down
    for (int n = 0; n < Screensize[0] + Screensize[1] - 1; n++) {
        int Pos[2] = { Clamp(0,Screensize[0] - 1, n - Screensize[1] - 1),Clamp(0,Screensize[1] - 1, n) };
        string Temp;
        for (int m = 0; m < Screensize[0] * Screensize[1]; m++) { //Very bad practice
            if (CheckScreen(Pos) == Temp[0]) {
                Temp += Temp[0];
                if (Temp.size() >= 4 && (Temp[0] == 'X' || Temp[0] == 'O')) {
                    return Temp[0];
                }
            }
            else {
                Temp = "";
                Temp += CheckScreen(Pos);
            }

            Pos[0] += 1;
            Pos[1] += 1;
            if (Pos[0] > Screensize[0] - 1) {
                break;
            }
            if (Pos[1] > Screensize[1] - 1) {
                break;
            }
        }
    } 
    return ' ';
}

void RenderScreen(string printstring) {
    ClearScreen();

    auto Start = system_clock::now();

    for (int n = 0; n < Screensize[1]; n++) {
        int Pos = Screensize[0] * (n + 1) + n;
        printstring.insert(Pos, "\n");
    }

    cout << printstring;

    duration<double> duration = system_clock::now() - Start;
    cout << "time: " + to_string(duration.count() * 1000) + " ms \n";
    cout << "Winner: ";
    cout << CheckforWin();
}

void ExitLogic() {
    ShowConsoleCursor(true);
}

int main()
{
    //Setup the Project
    Setup();

    //Frame logic
    while (Running) {
        //Input logic
        InputLogic();

        //Game logic
        

        //Graphics logic
        RenderScreen(Screen);
        
    }

    //Exit game
    ExitLogic();
    return 0;
}