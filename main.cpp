#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string>
#include<fstream>
#include<time.h>
using namespace std;

#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_SPACE 32
#define KEY_ENTER 13
#define KEY_ESC 27
#define ATTR1 014
#define MAIN_MENU_OX 20
#define SETTINGS_MENU_OX 32
#define SOLVE_MENU_OX 15
#define MENU_OY 13
#define MENU_START 0
#define MENU_SOLVE 1
#define MENU_ABOUT 2
#define MENU_HELP 3
#define MENU_SETTINGS 4
#define MENU_EXIT 5
#define SETTINGS_LEVEL 0
#define SETTINGS_VELOCITY 1
#define SETTINGS_ATTRIBUTES 2
#define SETTINGS_EXIT 3
#define SOLVE_LA 0
#define SOLVE_MA 1
#define SOLVE_DM 2
#define SOLVE_ML 3
#define SOLVE_EXIT 4

#define VELOCITY_FAST 10
#define VELOCITY_MEDIUM 50
#define VELOCITY_SLOW 250
#define TIME_BEFORE_RAIN 2500
#define DROPS 10

char *main_menu[] = {"start", "solve", "about", "help", "settings", "exit"};
char *settings_menu[] = {"level", "velocity", "attributes", "exit"};
char *solve_maths_menu[] = {"linear algebra", "maths analysis", "discrete maths", "maths logic", "exit"};

bool back_to_menu = false;
int level = 1;
double VELOCITY = VELOCITY_MEDIUM;
int AMOUNT_OF_DROPS = 10;
int LIVES_LEFT = 3;
int LIVES_SAVED = 0;
int SOLVED_TASKS = 0;
string style_d = "dark";
string style_s = "special";
string style_w = "white";
string STYLE = style_d;


using namespace std;

CONSOLE_SCREEN_BUFFER_INFO cs;

void draw_settings_menu(HANDLE h, COORD curr, int chosen_item, int m2_amount_of_items, int len, int fl, char ch,
                        char *menu[]);

void help(HANDLE h, COORD curr);

void about(HANDLE h, COORD c);

void game(HANDLE h);

void draw_border(HANDLE h);

void drawHorizLine(COORD t, COORD b, int j);

void drawVertLine(HANDLE h, COORD t, COORD b, int k);

void messageAboutAcidRain(HANDLE h);

enum direction {
    RIGHT, LEFT
};

class textRect {
    HANDLE h;
    COORD rectCoord;
    char ch_attr[3] = {char(254), char(219), char(179)};
    char clear[3] = {' ', ' ', ' '};
    int size_X = 2;
    int size_Y = 3;

    void drawline(COORD cursorPos, char ch[]);

public:
    textRect(HANDLE h, COORD Pos) {
        this->h = h;
        rectCoord = Pos;
        draw();
    }

    void draw();

    void move(direction dir);

    short getWidth() { return rectCoord.X + size_X; }

    short getHeight() { return rectCoord.Y + size_Y; }

    short getX() { return rectCoord.X; }

    short getY() { return rectCoord.Y; }
};

void draw_character(HANDLE h, textRect &rect);

void start_rain(HANDLE h, textRect rect);

bool drops_collided(HANDLE h, textRect rect, COORD *c_array);

void choose_level();

void choose_velocity();

void choose_attr();

void
draw_solve_menu(HANDLE h, COORD curr, int chosen_item, int amount_of_items, int len, int fl, char ch, char *menu[]);

void show_tasks(HANDLE h, const char **tasks);

void draw_menu(HANDLE h, COORD c, int chosen_item, int amount_of_items, int len, char *menu[]) {
    SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
    COORD c2 = c;
    for (int i = 0; i < amount_of_items; i++) {
        c2.X = c.X + i * len;
        SetConsoleCursorPosition(h, c2);
        cout << menu[i];
    }
    c2.X = c.X + chosen_item * len;
    SetConsoleTextAttribute(h, ATTR1);
    SetConsoleCursorPosition(h, c2);
    cout << menu[chosen_item];
}


int main() {
    system("title PATH TO A GAME");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD main_menu_source = {MAIN_MENU_OX, MENU_OY};
    COORD settings_menu_source = {SETTINGS_MENU_OX, MENU_OY};
    COORD solve_menu_source = {SOLVE_MENU_OX, MENU_OY};
    COORD main_curr = main_menu_source;
    COORD set_curr = settings_menu_source;
    COORD solve_curr = solve_menu_source;

    int fl = 1, chosen_item = 0, m1_amount_of_items = 6, m2_amount_of_items = 4, m3_amount_of_items = 5,
            len = 15, maths_len = 20;
    char ch;

    back_to_menu:
    SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
    system("cls");
    draw_menu(h, main_curr, chosen_item, m1_amount_of_items, len, main_menu);
    while (fl) {
        if (_kbhit()) {
            ch = _getch();
            switch (ch) {
                case KEY_ESC: {
                    fl = 0;
                    break;
                }
                case KEY_ENTER: {
                    switch (chosen_item) {
                        case MENU_START: {
                            game(h);
                            break;
                        }
                        case MENU_SOLVE: {
                            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
                            system("cls");
                            chosen_item = 0;
                            draw_solve_menu(h, solve_curr, chosen_item, m3_amount_of_items, maths_len, fl, ch,
                                            solve_maths_menu);
                            if (back_to_menu) {
                                back_to_menu = false;
                                goto back_to_menu;
                            }
                            break;
                        }
                        case MENU_ABOUT: {
                            about(h, main_curr);
                            goto back_to_menu;
                        }
                        case MENU_HELP: {
                            help(h, main_curr);
                            goto back_to_menu;
                        }
                        case MENU_SETTINGS: {
                            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
                            system("cls");
                            chosen_item = 0;
                            draw_settings_menu(h, set_curr, chosen_item, m2_amount_of_items, len, fl, ch,
                                               settings_menu);
                            if (back_to_menu) {
                                back_to_menu = false;
                                goto back_to_menu;
                            }
                            break;
                        }
                        case MENU_EXIT: {
                            fl = 0;
                            break;
                        }
                    }
                    break;
                }
                case KEY_ARROW_RIGHT: {
                    if (chosen_item == m1_amount_of_items - 1)chosen_item = 0;
                    else chosen_item++;
                    break;
                }
                case KEY_ARROW_LEFT: {
                    if (chosen_item == 0)chosen_item = m1_amount_of_items - 1;
                    else chosen_item--;
                    break;
                }
            }
            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
            system("cls");
            draw_menu(h, main_curr, chosen_item, m1_amount_of_items, len, main_menu);
        }
    }
    COORD coord_esc = {30, 25};
    SetConsoleCursorPosition(h, coord_esc);
}

/**
 * This function creates the game
 * @param h HANDLE
 * @param level of game
 */
void game(HANDLE h) {
    if (LIVES_LEFT > 0) {
        AMOUNT_OF_DROPS = DROPS + level;
        system("cls");
        draw_border(h);
        if (level == 1) {
            messageAboutAcidRain(h);
            draw_border(h);
        }

        COORD cursorPos = {3, 25};
        textRect rect(h, cursorPos);
        draw_character(h, rect);
        SetConsoleCursorPosition(h, {static_cast<SHORT>(cs.srWindow.Right - 17), 2});
        cout << "Lives left: " << LIVES_LEFT;
        SetConsoleCursorPosition(h, {2, 2});
        cout << "Lives saved: " << LIVES_SAVED;
        start_rain(h, rect);
        Sleep(TIME_BEFORE_RAIN / 2);
        if (LIVES_LEFT > 0)
            game(h);
        else {
            system("color 4");
            SetConsoleCursorPosition(h, {2, 2});
            cout << "You lost!";
            SetConsoleCursorPosition(h, {2, 10});
        }
    } else {
        system("color 4");
        SetConsoleCursorPosition(h, {2, 2});
        cout << "No lives left. Solve Maths problems to achieve them.";
        SetConsoleCursorPosition(h, {2, 10});
    }
    system("pause");
}


/**
 * This method draws the character
 * @param h HANDLE
 */
void draw_character(HANDLE h, textRect &rect) {
    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    GetConsoleScreenBufferInfo(h, &csbInfo);
    int iKey = 67;
    clock_t start, stop;
    start = clock();
    while (iKey != KEY_SPACE) {
        stop = clock();
        if (stop - start >= TIME_BEFORE_RAIN)
            break;
        if (_kbhit()) {
            iKey = _getch();
            switch (iKey) {
                case KEY_ARROW_RIGHT:
                    if (rect.getWidth() > csbInfo.srWindow.Right - 2);
                    else {
                        rect.move(RIGHT);
                    }
                    break;
                case KEY_ARROW_LEFT:
                    if (rect.getX() <= csbInfo.srWindow.Left + 2);
                    else {
                        rect.move(LEFT);
                    }
                    break;
                case KEY_ESC:
                    exit(0);
            }
        }
    }
}


void textRect::draw() {
    COORD pos = rectCoord;
    for (int j = 0; j < size_X; j++) {
        drawline(pos, ch_attr);
        pos.X++;
    }
    SetConsoleCursorPosition(h, {0, 0});
}

void textRect::move(direction dir) {
    COORD pos = rectCoord;
    switch (dir) {
        case RIGHT:
            drawline(pos, clear);
            pos = rectCoord;
            pos.X += size_X;
            drawline(pos, ch_attr);
            rectCoord.X += 1;
            break;
        case LEFT:
            pos.X += size_X - 1;
            drawline(pos, clear);
            pos = rectCoord;
            pos.X -= 1;
            drawline(pos, ch_attr);
            rectCoord.X -= 1;
            break;
        default:
            break;
    }
    SetConsoleCursorPosition(h, {0, 0});
}

void textRect::drawline(COORD cursorPos, char ch[]) {
    for (int i = 0; i < size_Y; i++) {
        SetConsoleCursorPosition(h, cursorPos);
        cout << ch[i];
        cursorPos.Y++;
    }
}

/**
 * This function draws rain
 * @param h HANDLE
 */
void start_rain(HANDLE h, textRect rect) {
    LPDWORD count;
    GetConsoleScreenBufferInfo(h, &cs);
    COORD c_array[AMOUNT_OF_DROPS];
    char rain_type;
    srand(time(NULL));
    for (int i = 0; i < AMOUNT_OF_DROPS; i++) {
        c_array[i].X = 2 + rand() % cs.srWindow.Right + 1;
        c_array[i].Y = 2;
    }
    int rain_type_int = 32 + rand() % 222;
    rain_type = char(rain_type_int);//from 33 to 254
    COORD last_c;
    int real_velocity = 1;
    for (int i = 0; c_array[0].Y < cs.srWindow.Bottom + real_velocity - 2; i++) {
        for (int j = 0; j < AMOUNT_OF_DROPS; j++) {
            last_c.X = c_array[j].X - real_velocity;
            last_c.Y = c_array[j].Y - real_velocity;
            if (c_array[j].X >= cs.srWindow.Right - 2)
                c_array[j].X = 2;
            FillConsoleOutputCharacter(h, rain_type, 1, c_array[j], reinterpret_cast<PDWORD>(&count));
            c_array[j].X += real_velocity;
            c_array[j].Y += real_velocity;
            FillConsoleOutputCharacter(h, ' ', 1, last_c, reinterpret_cast<PDWORD>(&count));
        }
        Sleep(VELOCITY);
    }
    if (drops_collided(h, rect, c_array)) {
        if (level < 10)
            level++;
        LIVES_LEFT--;
        Beep(523, 500);//beep sound
    } else {
        if (level < 10)
            level++;
        LIVES_SAVED++;
    }
    SetConsoleCursorPosition(h, {static_cast<SHORT>(cs.srWindow.Right - 17), 2});
    cout << "Lives left: " << LIVES_LEFT;
    SetConsoleCursorPosition(h, {2, 2});
    cout << "Lives saved: " << LIVES_SAVED;
    SetConsoleCursorPosition(h, {0, cs.srWindow.Bottom});
}

/**
 * This function checks if drops collided with rect
 * @param h HANDLE
 * @param rect character
 * @param c_array array of drops coordinates
 * @return true if drops collided
 */
bool drops_collided(HANDLE h, textRect rect, COORD *c_array) {
    for (int j = 0; j < AMOUNT_OF_DROPS; j++) {
        if (c_array[j].X >= rect.getX() + 1 && c_array[j].X <= rect.getWidth() + 2) {
            return true;
        }
    }
    return false;
}

/**
 * This function draws the boarder of game
 * @param h HANDLE
 */
void draw_border(HANDLE h) {
    GetConsoleScreenBufferInfo(h, &cs);
    if (STYLE == "dark")
        system("color B");
    else if (STYLE == "white")
        system("color F0");
    else if (STYLE == "special")
        system("color 9E");

    COORD b = {cs.srWindow.Right, cs.srWindow.Bottom};
    COORD t = {cs.srWindow.Left, cs.srWindow.Top};
    b.Y += 3;
    drawHorizLine(t, b, 0);
    drawVertLine(h, t, b, 0);
    drawHorizLine(t, b, 1);
    drawVertLine(h, t, b, 1);
}

/**
 * This function builds horizontal line and corners
 * @param t  coord 1
 * @param b coord 2
 * @param j parameter
 */
void drawHorizLine(COORD t, COORD b, int j) {
    int lenX = b.X - t.X;
    COORD f = t;
    unsigned char c;

    if (j == 0)
        c = 457;
    else if (j == 1)
        c = 456;
    cout << c;


    f.X++;
    c = 461;
    for (int i = 0; i < lenX - 1; i++) {
        cout << c;
    }

    if (j == 0)
        c = 443;
    else if (j == 1)
        c = 444;
    cout << c << endl;
}

/**
 * This function builds vertical line
 * @param h HANDLE
 * @param t Coord 1
 * @param b Coord 2
 * @param k parameter
 */
void drawVertLine(HANDLE h, COORD t, COORD b, int k) {
    int lenY = b.Y - t.Y - 4;
    COORD f = t;
    unsigned char c;

    if (k == 0)
        f = t;
    else if (k == 1)
        f = {b.X, 0};

    c = 442;
    f.Y++;
    for (int i = 0; i < lenY - 1; i++) {
        SetConsoleCursorPosition(h, f);
        cout << c;
        f.Y++;
    }
    cout << endl;
}


/**
 * This function opens file "AcidRainIsGoingToFall.txt" and inputs lines into screen
 * @param h HANDLE
 */
void messageAboutAcidRain(HANDLE h) {
    //B(dark), 9E(blue), F0 (white)
    if (STYLE == "dark")
        SetConsoleTextAttribute(h, FOREGROUND_RED);//background: black
    else if (STYLE == "white")
        SetConsoleTextAttribute(h, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
                                   FOREGROUND_RED);//background: white
    else if (STYLE == "special")
        SetConsoleTextAttribute(h, FOREGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_BLUE);//background: blue


    COORD cc = {static_cast<SHORT>(cs.srWindow.Right / 2 - 20), 3};
    SetConsoleCursorPosition(h, cc);
    ifstream input("AcidRainIsGoingToFall.txt");
    string line;
    while (getline(input, line)) {
        cout << line << endl;
        cc.Y += 3;
        SetConsoleCursorPosition(h, cc);

    }
    SetConsoleCursorPosition(h, cc);
    system("pause");
    system("cls");

    //B(dark), 9E(blue), F0 (white)
    if (STYLE == "dark")
        system("color B");
    else if (STYLE == "white")
        system("color F0");
    else if (STYLE == "special")
        system("color 9E");
}


/**
 * This function opens file "About.txt" and inputs lines into screen
 * @param h HANDLE
 * @param c position
 */
void about(HANDLE h, COORD c) {
    system("cls");
    system("color F0");
    SetConsoleTextAttribute(h, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
                               FOREGROUND_RED | FOREGROUND_BLUE);
    COORD cc = c;
    cc.X = c.X * 27;
    cc.Y = c.Y + 3;
    SetConsoleCursorPosition(h, cc);
    ifstream input("About.txt");
    string line;
    while (getline(input, line)) {
        cout << line << endl;
        cc.Y += 3;
        SetConsoleCursorPosition(h, cc);

    }
    SetConsoleCursorPosition(h, cc);
    printf("Shaposhnikova Sofia");
    printf("\n\n\n\n");
    system("pause");
}

/**
 * This function opens file "Help.txt" and inputs lines into screen
 * @param h HANDLE
 * @param c position
 */
void help(HANDLE h, COORD c) {
    system("cls");
    system("color F0");
    SetConsoleTextAttribute(h, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
                               FOREGROUND_RED | FOREGROUND_BLUE);

    COORD cc = c;
    cc.X = c.X * 25;
    cc.Y = c.Y + 3;
    ifstream input("Help.txt");
    string line;
    SetConsoleCursorPosition(h, cc);
    while (getline(input, line)) {
        cout << line << endl;
        cc.Y += 3;
        SetConsoleCursorPosition(h, cc);
    }
    printf("\n\n\n\n");
    system("pause");
}

/**
 * This function draws the settings menu
 * @param h HANDLE
 * @param curr position coords
 * @param chosen_item chosen item
 * @param m2_amount_of_items  amount of items in settings menu
 * @param len length of one item
 * @param fl bool
 * @param ch char
 */
void draw_settings_menu(HANDLE h, COORD curr, int chosen_item, int m2_amount_of_items,
                        int len, int fl, char ch, char *menu[]) {
    draw_menu(h, curr, chosen_item, m2_amount_of_items, len, menu);
    while (fl) {
        if (_kbhit()) {
            ch = _getch();
            switch (ch) {
                case KEY_ESC: {
                    fl = 0;
                    break;
                }
                case KEY_ARROW_RIGHT: {
                    if (chosen_item == m2_amount_of_items - 1)chosen_item = 0;
                    else chosen_item++;
                    break;
                }
                case KEY_ARROW_LEFT: {
                    if (chosen_item == 0)chosen_item = m2_amount_of_items - 1;
                    else chosen_item--;
                    break;
                }
                case KEY_ENTER: {
                    switch (chosen_item) {
                        case SETTINGS_LEVEL: {
                            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
                            system("cls");
                            choose_level();
                            break;
                        }
                        case SETTINGS_VELOCITY: {
                            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
                            system("cls");
                            choose_velocity();
                            break;
                        }
                        case SETTINGS_ATTRIBUTES: {
                            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
                            system("cls");
                            choose_attr();
                            break;
                        }
                        case SETTINGS_EXIT: {
                            back_to_menu = true;
                            fl = 0;
                            break;
                        }
                    }
                }
            }
            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
            system("cls");
            draw_menu(h, curr, chosen_item, m2_amount_of_items, len, menu);
        }
    }
}

/**
 * This function draws the solve maths menu
 * @param h HANDLE
 * @param curr position coords
 * @param chosen_item chosen item
 * @param m2_amount_of_items  amount of items in solve maths menu
 * @param len length of one item
 * @param fl bool
 * @param ch char
 */
void
draw_solve_menu(HANDLE h, COORD curr, int chosen_item, int amount_of_items, int len, int fl, char ch, char *menu[]) {
    draw_menu(h, curr, chosen_item, amount_of_items, len, menu);
    const char *maths_tasks[4][4] = {{"LA_1.txt", "LA_2.txt", "LA_3.txt"},
                                {"MA_1.txt", "MA_2.txt", "MA_3.txt", "MA_4.txt"},
                                {"DM_1.txt", "DM_2.txt", "DM_3.txt"},
                                {"ML_1.txt"}};
    while (fl) {
        if (_kbhit()) {
            ch = _getch();
            switch (ch) {
                case KEY_ESC: {
                    fl = 0;
                    break;
                }
                case KEY_ARROW_RIGHT: {
                    if (chosen_item == amount_of_items - 1)chosen_item = 0;
                    else chosen_item++;
                    break;
                }
                case KEY_ARROW_LEFT: {
                    if (chosen_item == 0)chosen_item = amount_of_items - 1;
                    else chosen_item--;
                    break;
                }
                case KEY_ENTER: {
                    switch (chosen_item) {
                        case SOLVE_LA: {
                            show_tasks(h, maths_tasks[0]);
                            break;
                        }
                        case SOLVE_MA: {
                            show_tasks(h, maths_tasks[1]);
                            break;
                        }
                        case SOLVE_DM: {
                            show_tasks(h, maths_tasks[2]);
                            break;
                        }
                        case SOLVE_ML: {
                            show_tasks(h, maths_tasks[3]);
                            break;
                        }
                        case SOLVE_EXIT: {
                            back_to_menu = true;
                            fl = 0;
                            break;
                        }
                    }
                }
            }
            SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
            system("cls");
            draw_menu(h, curr, chosen_item, amount_of_items, len, menu);
        }
    }
}

/**
 * This function shows tasks
 * @param h HANDLE
 * @param tasks array of manes of file
 */
void show_tasks(HANDLE h, const char **tasks) {
    system("cls");
    system("color F0");
    SetConsoleTextAttribute(h, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    SetConsoleCursorPosition(h, {2, 2});
    cout << "Tasks solved: " << SOLVED_TASKS;
    COORD cc = {1, 1};
    cc.X = cc.X * 27;
    cc.Y = cc.Y + 3;
    SetConsoleCursorPosition(h, cc);

    int amount_of_tasks = -2;
    if (tasks[0] != "ML_1.txt") {
        for (int i = 0; i < 5; i++) {
            if (tasks[i] != "\0")
                amount_of_tasks++;
        }
    } else {
        amount_of_tasks = 1;
    }
    srand(time(NULL));
    int num_of_curr_task = rand() % amount_of_tasks;
    const char *a = tasks[num_of_curr_task];
    ifstream input(a);
    string line, solution, answer;
    while (getline(input, line)) {
        if (line != "") {
            cout << line << endl;
            cc.Y += 1;
            SetConsoleCursorPosition(h, cc);
        } else {
            cin >> answer;
            getline(input, solution);
            break;
        }
    }
    SetConsoleCursorPosition(h, cc);
    cc.Y += 4;
    if (answer == solution) {
        system("color 2");
        LIVES_LEFT++;
        SOLVED_TASKS ++;
        SetConsoleCursorPosition(h, cc);
        cout << "Right!";
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
        cout << "For a correctly solved task you get a new life!";
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
        cout << "Current amount of lives: " << LIVES_LEFT;
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
    } else {
        system("color 4");
        SetConsoleCursorPosition(h, cc);
        cout << "Wrong answer!";
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
        cout << "Unfortunately, you cannot achieve new life.";
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
        cout << "Try better next time";
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
        cout << "Current amount of lives: " << LIVES_LEFT;
        cc.Y += 2;
        SetConsoleCursorPosition(h, cc);
    }
    system("pause");
}

/**
 * This function allows the user to choose the attribute of game
 */
void choose_attr() {
    cout << "Current attribute is: " << STYLE << endl;
    char new_attr;
    cout << "Choose new attribute " << endl;
    cout << "Input: 'd' - dark, 'w' - white or 's' - special" << endl;
    choose_attr:
    cin >> new_attr;
    switch (new_attr) {
        case 'd': {
            STYLE = style_d;
            break;
        }
        case 'w': {
            STYLE = style_w;
            break;
        }
        case 's': {
            STYLE = style_s;
            break;
        }
        default: {
            cout << "Wrong symbol. Try again" << endl;
            goto choose_attr;
        }
    }
    cout << "New attribute is: " << STYLE << endl << endl << "Start the game to see what has changed" << endl;
    system("pause");
}

/**
 * This function allows the user to choose the velocity of rain
 */
void choose_velocity() {//10 50 250
    string curr_vel;
    char new_velocity;
    if (VELOCITY == 10)
        curr_vel = "fast";
    else if (VELOCITY == 50)
        curr_vel = "medium";
    else if (VELOCITY == 250)
        curr_vel = "slow";
    cout << "Current velocity is: " << curr_vel << endl;
    cout << "Input: 's' - slow, 'm' - medium or 'f' - fast to choose velocity" << endl;
    input_velocity:
    cin >> new_velocity;
    switch (new_velocity) {
        case 's': {
            VELOCITY = VELOCITY_SLOW;
            break;
        }
        case 'm': {
            VELOCITY = VELOCITY_MEDIUM;
            break;
        }
        case 'f': {
            VELOCITY = VELOCITY_FAST;
            break;
        }
        default: {
            cout << "Wrong symbol. Try again" << endl;
            goto input_velocity;
        }
    }
    string new_vel;
    if (VELOCITY == 10)
        new_vel = "fast";
    else if (VELOCITY == 50)
        new_vel = "medium";
    else if (VELOCITY == 250)
        new_vel = "slow";
    cout << "New velocity is: " << new_vel << endl << endl << "Start the game to see what has changed" << endl;
    system("pause");
}

/**
 * This function allows the user to choose level of game
 */
void choose_level() {
    cout << "Current level: " << level << endl << "Input new number of level from 1 to 10" << endl;
    int change_level;
    while (true) {
        cin >> change_level;
        if (change_level > 0 && change_level < 11) {
            level = change_level;
            break;
        } else
            cout << "Wrong number. Try again" << endl;
    }
    cout << "New level is: " << level << endl << endl << "Start the game to see what has changed" << endl;
    system("pause");
}
