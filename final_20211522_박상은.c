#define _CRT_SECURE_NO_WARNINGS // 에러를 방지한다
#include<stdio.h> //전반적인 부분의 작성을 위해 필요하다.
#include<windows.h> //sleep사용을 위해 불러온다.
#include<conio.h> //콘솔 입출력 함수를 제공하며 key입력시 사용한다.
#include<stdlib.h> //rand함수의 사용을 위해 불러온다.
#include<time.h> //rand함수를 사용할 때마다 값을 다르게 나오게 하기위해 불러온다.

#define LEFT 75 //좌측 방향키의 아스키 코드 값을 정해둔다.
#define RIGHT 77 //우측 방향키의 아스키 코드 값을 정해둔다.
#define UP 72 //위쪽 방향키의 아스키 코드 값을 정해둔다.
#define DOWN 80 //아래쪽 방향키의 아스키 코드 값을 정해둔다.
#define PAUSE 112 //p 키의 아스키 코드 값을 정해둔다.
#define ESC 27 //esc 키의 아스키 코드 값을 정해둔다.

#define MAP_X 3 
#define MAP_Y 2 
#define MAP_WIDTH 30 //맵의 너비를 정해둔다.
#define MAP_HEIGHT 20 //맵의 높이를 정해둔다.

int x[100], y[100]; //크기 100의 x, y 배열을 만든다.
int food_x, food_y; //food의 좌표값을 저장 
int length; //뱀의 몸통 길이이다.
int speed; //뱀의 속도이다.
int score; //점수를 저장하고, --reset으로 초기화된다.
int best_score = 0; //최고 점수를 저장하며 게임 실행 동안 초기화하지 않는다.
int last_score = 0; //마지막 점수 저장하며 다음 게임에 보여야하기 때문에 초기화하지 않는다.
int dir; //이동방향을 저장한다. 
int key; //입력받은 키를 저장한다. 

void gotoxy(int x, int y, char* s) { //좌표값을 받는 함수를 생성한다.
    COORD pos = { 2 * x,y }; //x값을 2x로 변경한다.
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); // 좌표값에 바로 문자열을 입력할 수 있도록 printf함수 삽입
    printf("%s", s);
}

void title(void); //게임 시작화면 
void reset(void); //게임을 초기화 
void draw_map(void); // 게임판 테두리를 그림 
void move(int dir); //뱀머리를 이동 
void pause(void); //일시정지 
void game_over(void); //게임 오버를 확인 
void food(void); // 음식 생성 
void status(void); // 개발자용 status표시  


int main() {
    title(); //화면 함수를 불러온다.

    while (1) { //계속 반복한다.
        if (_kbhit()) do { key = _getch(); } while (key == 224); //키를 입력받아 key에 저장한다.
        Sleep(speed); //0.00speed 만큼 지연시킨다.

        switch (key) { //입력받은 키를 알기 위해 switch문을 사용한다.
        case LEFT: //좌측
        case RIGHT: //우측
        case UP: //위쪽
        case DOWN: //아래쪽
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
                (dir == DOWN && key != UP))//180회전이동을 방지하기 위한 조건문이다.
                dir = key; //key값을 dir에 넣어라.
            key = 0; // 키값을 저장하는 함수를 reset한다.
            break;
        case PAUSE: // P키를 누르면 일시정지한다.
            pause();
            break;
        
        case ESC: //ESC키를 누르면 프로그램 종료 
            exit(0);
        }
        move(dir);  //move(dir)함수를 가져온다.

    }
}


void title(void) { //시작 화면 함수를 입력한다.
    int i, j; //반복을 위해 i와 j를 정의한다.

    while (_kbhit()) _getch(); //버퍼에 있는 키값을 버린다.

    draw_map();    //맵 테두리를 제작한다.
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // i는 3부터 21까지 1씩 늘려간다.
        for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) // j는 4부터 32까지 1씩 늘려간다.
            gotoxy(j, i, "  "); //(j,i) 위치를 빈칸으로 둔다.
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 5, "+--------------------------+"); //(11,7)위치에 ""문장을 넣는다.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 6, "|    S N A K E  G A M E    |"); //(11,8) 위치에 ""문장을 넣는다.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 7, "+--------------------------+"); //(11,9) 위치에 ""문장을 넣는다.

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > "); //(11,11) 위치에 "" 문장을 넣는다.

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 11, "   ◇ ←,→,↑,↓ : Move    "); //(11,13) 위치에""문장을 넣는다.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, "   ◇ P : Pause             "); //(11,14) 위치에 ""문장을 넣는다.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 13, "   ◇ ESC : Quit              "); //(11,15) 위치에 ""문장을 넣는다.


    while (1) { //계속 반복한다.
        if (_kbhit()) { //키가 눌렸는지 확인하는 함수를 가져와서
            key = _getch(); //받는 문자를 key에 넣는다.
            if (key == ESC) 
                exit(0); // ESC키면 종료한다.
            else 
                break; //이외에는 게임을 시작한다.
        }
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > "); //(11,11)위치에 ""문장을 넣는다.
        Sleep(400); //0.4초를 지연시킨다.
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, "                            "); //"" 문장을 없앤다.
        Sleep(400); //0.4초를 지연시킨다.

    }
    reset(); // reset 함수를 불러온다.
}

void reset(void) { //reset함수를 만든다.
    int i; //반복문을 위해 i를 생성한다.
    system("cls"); //화면을 지운다.
    draw_map(); //맵 테두리를 그리는 함수를 불러온다.
    while (_kbhit()) _getch(); //버퍼에 있는 키값을 버린다.

    dir = LEFT; // 방향을 초기화  
    speed = 100; // 속도 초기화 
    length = 5; //뱀 길이 초기화 
    score = 0; //점수 초기화 
    for (i = 0; i < length; i++) { //뱀 몸통값 입력 
        x[i] = MAP_WIDTH / 2 + i; //배열에 15+i 값을 넣는다.
        y[i] = MAP_HEIGHT / 2; //배열에 10 값을 넣는다.
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "ㅇ"); //(3+x배열값, 2+y배열값)에 뱀 몸통을 입력한다.
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "ㅇ"); //(3+x배열0의 값, 2+y배열0의 값)의 위치에 뱀 머리를 그린다.
    food(); // food함수를 불러온다.  
}

void draw_map(void) { //맵 테두리 그리는 함수를 생성한다.
    int i, j;
    for (i = 0; i < MAP_WIDTH; i++) { //i를 0부터 30까지 1씩 늘린다.
        gotoxy(MAP_X + i, MAP_Y, "■"); //(3+i, 2)위치에 맵 테두리를 만든다. (아래쪽 가로 맵)
    }
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { //i를 3부터 21까지 1씩 늘려간다.
        gotoxy(MAP_X, i, "■"); //(3,i) 위치에 맵 테두리를 만든다. (왼쪽 세로 맵)
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "■"); //(32, i)위치에 맵 테두리를 만든다. (오른쪽 세로 맵)
    }
    for (i = 0; i < MAP_WIDTH; i++) { //i를 0부터 30까지 1씩 늘린다.
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "■"); //(3+i, 21)의 위치에 맵 테두리를 만든다.(위쪽 가로 맵)
    }
}

void move(int dir) { //뱀 이동 함수를 생성한다.
    int i; //반복문에서의 사용을 위해 i를 생성한다.

    if (x[0] == food_x && y[0] == food_y) { //food와 충돌했을 경우 
        score += 10; //점수를 증가한다.
        food(); //새로운 food를 넣는다.
        length++; //뱀 몸통의 길이를 1 크게한다.
        x[length - 1] = x[length - 2]; //새로만든 몸통에 값을 입력해준다.
        y[length - 1] = y[length - 2]; //새로만든 몸통에 값을 입력해준다.
    }
    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1) { //벽과 충돌했을 경우 
        game_over(); //game_over 함수를 불러온다.
        return; //return을 사용하여 빠져나온다.
    }
    for (i = 1; i < length; i++) { //i는 1부터 몸통길이까지 1씩 늘려간다.
        if (x[0] == x[i] && y[0] == y[i]) { //자신의 몸통과 충돌하는 경우
            game_over();//game_over 함수를 불러온다.
            return; //return을 사용하여 빠져나온다.
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //몸통 마지막을 빈칸으로 바꾼다.
    for (i = length - 1; i > 0; i--) { //i를 몸통-1 부터 0까지 1씩 줄여간다.
        x[i] = x[i - 1]; //몸통의 좌표를 한칸씩 이동한다.
        y[i] = y[i - 1]; //몸통의 좌표를 한칸씩 이동한다.
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "ㅇ"); //머리가 있던곳을 몸통으로 고친다.
    if (dir == LEFT) --x[0]; //좌측인 경우 x배열 0의 값을 1 줄인다.
    if (dir == RIGHT) ++x[0]; //우측인 경우 x배열 0의 값을 1 늘린다.
    if (dir == UP) --y[0]; //위쪽인 경우 y배열 0의 값을 1 줄인다.
    if (dir == DOWN) ++y[0]; //아래쪽인 경우 y배열 0의 값을 1 늘린다.
    gotoxy(MAP_X + x[i], MAP_Y + y[i], "ㅇ"); //새로운 머리좌표값에 머리를 그린다.
}

void pause(void) { // p키를 눌렀을 경우의 함수를 생성한다.
    while (1) { //계속 반복한다.
        if (key == PAUSE) { //만약 key값이 p를 받는다면
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "< PAUSE : PRESS ANY KEY TO RESUME > "); //(9,2)위치에 ""을 작성한다.
            Sleep(400); //0.4초 딜레이한다.
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "                                    "); //(9,2)위치를 빈칸으로 바꾼다.
            Sleep(400); //0.4초 딜레이한다.
        }
        else { //만약 key값이 p가 아니면
            draw_map(); //맵을 그리는 함수를 불러온다.
            return; //빠져나간다.
        }
        if (_kbhit()) { //키보드가 눌렸는지 확인하는 함수를 가져온다면
            do {
                key = _getch(); //key값에 입력을 넣어라.
            } while (key == 224); //방향키에서 224값을 반환한다면
        }

    }
}

void game_over(void) { //게임종료 함수를 생성한다.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "+----------------------+"); //(12, 7)좌표에 ""를 넣어라.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "|      GAME OVER..     |"); //(12,8) 좌표에 ""을 넣어라.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "+----------------------+"); //(12,9) 좌표에 ""를 넣어라.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " YOUR SCORE : "); //(12, 10) 좌표에 ""를 넣어라.
    printf("%d", last_score = score); //score를 마지막점수에 넣어 출력한다.

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, " Press any keys to restart.. "); //(11, 14)좌표에 ""를 넣어라.

    if (score > best_score) { //만약 점수가 bestscore보다 높으면
        best_score = score; //bestscore에 넣어라
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 10, "☆ BEST SCORE ☆"); //(14,12)좌표에 ""을 넣어라.
    }
    Sleep(500); //0.5초 딜레이해라
    while (_kbhit()) _getch(); //만약 키를 입력했는지 확인을 하고
    key = _getch(); //key값에 넣어라.
    title(); //title 함수를 불러와라.
}

void food(void) { //food 함수를 생성해라.
    int i; //반복을 위해 i를 생성한다.

    int food_crush_on = 0;//food가 뱀 몸통좌표에 생길 경우 on 
    int r = 0; //난수 생성에 사용되는 변수를 생성한다.
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, " YOUR SCORE: "); //(3, 22)좌표에 ""를 넣어라. 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %3d", score, last_score, best_score); //최대 3자리수의 점수를 표시해라.

    while (1) { //계속 반복한다.
        food_crush_on = 0; //food_crush_on을 off한다.
        srand((unsigned)time(NULL) + r); //난수표를 생성한다.
        food_x = (rand() % (MAP_WIDTH - 2)) + 1;    //난수를 x좌표값에 넣어라.
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1; //난수를 y좌표값에 넣는다.

        for (i = 0; i < length; i++) { //i를 0부터 몸통길이까지 1씩 늘려간다.
            if (food_x == x[i] && food_y == y[i]) { //food가 뱀 몸통과 겹친다면
                food_crush_on = 1; //겹치면 food_crush_on 를 on한다.
                r++; //r을 1 늘린다.
                break; //벗어난다.
            }
        }

        if (food_crush_on == 1) 
            continue; //겹쳤을 경우 while문을 다시 시작한다.

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "$"); //안겹쳤을 경우 좌표값에 food를 넣어라.
        speed -= 3; //속도를 3만큼 증가한다.
        break; //벗어난다.

    }
}

void status(void) { //현재 상태를 가져오는 함수를 생성한다.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "head= "); //(34,2) 위치에 ""를 넣는다.
    printf("%2d,%2d", x[0], y[0]); //배열 0의 값을 표시해라.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "food= "); //(34,3) 위치에 ""를 넣는다.
    printf("%2d,%2d", food_x, food_y); //food의 좌표를 표시해라.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "leng= "); //(34,4) 위치에 ""를 넣는다.
    printf("%2d", length); //뱀의 길이를 3자리수까지 표시해라.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "key= "); //(34,5) 위치에 ""를 넣는다.
    printf("%3d", key); //key값을 3자리수까지 표시해라.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "spd= "); //(34,6) 위치에 ""를 넣는다.
    printf("%3d", speed); //speed를 3자리수까지 표시해라.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 6, "score= "); //(34,7) 위치에 ""를 넣는다.
    printf("%3d", score); //score를 3자리수까지 표시해라.
}