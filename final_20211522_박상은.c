#define _CRT_SECURE_NO_WARNINGS // ������ �����Ѵ�
#include<stdio.h> //�������� �κ��� �ۼ��� ���� �ʿ��ϴ�.
#include<windows.h> //sleep����� ���� �ҷ��´�.
#include<conio.h> //�ܼ� ����� �Լ��� �����ϸ� key�Է½� ����Ѵ�.
#include<stdlib.h> //rand�Լ��� ����� ���� �ҷ��´�.
#include<time.h> //rand�Լ��� ����� ������ ���� �ٸ��� ������ �ϱ����� �ҷ��´�.

#define LEFT 75 //���� ����Ű�� �ƽ�Ű �ڵ� ���� ���صд�.
#define RIGHT 77 //���� ����Ű�� �ƽ�Ű �ڵ� ���� ���صд�.
#define UP 72 //���� ����Ű�� �ƽ�Ű �ڵ� ���� ���صд�.
#define DOWN 80 //�Ʒ��� ����Ű�� �ƽ�Ű �ڵ� ���� ���صд�.
#define PAUSE 112 //p Ű�� �ƽ�Ű �ڵ� ���� ���صд�.
#define ESC 27 //esc Ű�� �ƽ�Ű �ڵ� ���� ���صд�.

#define MAP_X 3 
#define MAP_Y 2 
#define MAP_WIDTH 30 //���� �ʺ� ���صд�.
#define MAP_HEIGHT 20 //���� ���̸� ���صд�.

int x[100], y[100]; //ũ�� 100�� x, y �迭�� �����.
int food_x, food_y; //food�� ��ǥ���� ���� 
int length; //���� ���� �����̴�.
int speed; //���� �ӵ��̴�.
int score; //������ �����ϰ�, --reset���� �ʱ�ȭ�ȴ�.
int best_score = 0; //�ְ� ������ �����ϸ� ���� ���� ���� �ʱ�ȭ���� �ʴ´�.
int last_score = 0; //������ ���� �����ϸ� ���� ���ӿ� �������ϱ� ������ �ʱ�ȭ���� �ʴ´�.
int dir; //�̵������� �����Ѵ�. 
int key; //�Է¹��� Ű�� �����Ѵ�. 

void gotoxy(int x, int y, char* s) { //��ǥ���� �޴� �Լ��� �����Ѵ�.
    COORD pos = { 2 * x,y }; //x���� 2x�� �����Ѵ�.
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); // ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����
    printf("%s", s);
}

void title(void); //���� ����ȭ�� 
void reset(void); //������ �ʱ�ȭ 
void draw_map(void); // ������ �׵θ��� �׸� 
void move(int dir); //��Ӹ��� �̵� 
void pause(void); //�Ͻ����� 
void game_over(void); //���� ������ Ȯ�� 
void food(void); // ���� ���� 
void status(void); // �����ڿ� statusǥ��  


int main() {
    title(); //ȭ�� �Լ��� �ҷ��´�.

    while (1) { //��� �ݺ��Ѵ�.
        if (_kbhit()) do { key = _getch(); } while (key == 224); //Ű�� �Է¹޾� key�� �����Ѵ�.
        Sleep(speed); //0.00speed ��ŭ ������Ų��.

        switch (key) { //�Է¹��� Ű�� �˱� ���� switch���� ����Ѵ�.
        case LEFT: //����
        case RIGHT: //����
        case UP: //����
        case DOWN: //�Ʒ���
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
                (dir == DOWN && key != UP))//180ȸ���̵��� �����ϱ� ���� ���ǹ��̴�.
                dir = key; //key���� dir�� �־��.
            key = 0; // Ű���� �����ϴ� �Լ��� reset�Ѵ�.
            break;
        case PAUSE: // PŰ�� ������ �Ͻ������Ѵ�.
            pause();
            break;
        
        case ESC: //ESCŰ�� ������ ���α׷� ���� 
            exit(0);
        }
        move(dir);  //move(dir)�Լ��� �����´�.

    }
}


void title(void) { //���� ȭ�� �Լ��� �Է��Ѵ�.
    int i, j; //�ݺ��� ���� i�� j�� �����Ѵ�.

    while (_kbhit()) _getch(); //���ۿ� �ִ� Ű���� ������.

    draw_map();    //�� �׵θ��� �����Ѵ�.
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // i�� 3���� 21���� 1�� �÷�����.
        for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) // j�� 4���� 32���� 1�� �÷�����.
            gotoxy(j, i, "  "); //(j,i) ��ġ�� ��ĭ���� �д�.
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 5, "+--------------------------+"); //(11,7)��ġ�� ""������ �ִ´�.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 6, "|    S N A K E  G A M E    |"); //(11,8) ��ġ�� ""������ �ִ´�.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 7, "+--------------------------+"); //(11,9) ��ġ�� ""������ �ִ´�.

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > "); //(11,11) ��ġ�� "" ������ �ִ´�.

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 11, "   �� ��,��,��,�� : Move    "); //(11,13) ��ġ��""������ �ִ´�.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, "   �� P : Pause             "); //(11,14) ��ġ�� ""������ �ִ´�.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 13, "   �� ESC : Quit              "); //(11,15) ��ġ�� ""������ �ִ´�.


    while (1) { //��� �ݺ��Ѵ�.
        if (_kbhit()) { //Ű�� ���ȴ��� Ȯ���ϴ� �Լ��� �����ͼ�
            key = _getch(); //�޴� ���ڸ� key�� �ִ´�.
            if (key == ESC) 
                exit(0); // ESCŰ�� �����Ѵ�.
            else 
                break; //�̿ܿ��� ������ �����Ѵ�.
        }
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > "); //(11,11)��ġ�� ""������ �ִ´�.
        Sleep(400); //0.4�ʸ� ������Ų��.
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, "                            "); //"" ������ ���ش�.
        Sleep(400); //0.4�ʸ� ������Ų��.

    }
    reset(); // reset �Լ��� �ҷ��´�.
}

void reset(void) { //reset�Լ��� �����.
    int i; //�ݺ����� ���� i�� �����Ѵ�.
    system("cls"); //ȭ���� �����.
    draw_map(); //�� �׵θ��� �׸��� �Լ��� �ҷ��´�.
    while (_kbhit()) _getch(); //���ۿ� �ִ� Ű���� ������.

    dir = LEFT; // ������ �ʱ�ȭ  
    speed = 100; // �ӵ� �ʱ�ȭ 
    length = 5; //�� ���� �ʱ�ȭ 
    score = 0; //���� �ʱ�ȭ 
    for (i = 0; i < length; i++) { //�� ���밪 �Է� 
        x[i] = MAP_WIDTH / 2 + i; //�迭�� 15+i ���� �ִ´�.
        y[i] = MAP_HEIGHT / 2; //�迭�� 10 ���� �ִ´�.
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "��"); //(3+x�迭��, 2+y�迭��)�� �� ������ �Է��Ѵ�.
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //(3+x�迭0�� ��, 2+y�迭0�� ��)�� ��ġ�� �� �Ӹ��� �׸���.
    food(); // food�Լ��� �ҷ��´�.  
}

void draw_map(void) { //�� �׵θ� �׸��� �Լ��� �����Ѵ�.
    int i, j;
    for (i = 0; i < MAP_WIDTH; i++) { //i�� 0���� 30���� 1�� �ø���.
        gotoxy(MAP_X + i, MAP_Y, "��"); //(3+i, 2)��ġ�� �� �׵θ��� �����. (�Ʒ��� ���� ��)
    }
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { //i�� 3���� 21���� 1�� �÷�����.
        gotoxy(MAP_X, i, "��"); //(3,i) ��ġ�� �� �׵θ��� �����. (���� ���� ��)
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "��"); //(32, i)��ġ�� �� �׵θ��� �����. (������ ���� ��)
    }
    for (i = 0; i < MAP_WIDTH; i++) { //i�� 0���� 30���� 1�� �ø���.
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "��"); //(3+i, 21)�� ��ġ�� �� �׵θ��� �����.(���� ���� ��)
    }
}

void move(int dir) { //�� �̵� �Լ��� �����Ѵ�.
    int i; //�ݺ��������� ����� ���� i�� �����Ѵ�.

    if (x[0] == food_x && y[0] == food_y) { //food�� �浹���� ��� 
        score += 10; //������ �����Ѵ�.
        food(); //���ο� food�� �ִ´�.
        length++; //�� ������ ���̸� 1 ũ���Ѵ�.
        x[length - 1] = x[length - 2]; //���θ��� ���뿡 ���� �Է����ش�.
        y[length - 1] = y[length - 2]; //���θ��� ���뿡 ���� �Է����ش�.
    }
    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1) { //���� �浹���� ��� 
        game_over(); //game_over �Լ��� �ҷ��´�.
        return; //return�� ����Ͽ� �������´�.
    }
    for (i = 1; i < length; i++) { //i�� 1���� ������̱��� 1�� �÷�����.
        if (x[0] == x[i] && y[0] == y[i]) { //�ڽ��� ����� �浹�ϴ� ���
            game_over();//game_over �Լ��� �ҷ��´�.
            return; //return�� ����Ͽ� �������´�.
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //���� �������� ��ĭ���� �ٲ۴�.
    for (i = length - 1; i > 0; i--) { //i�� ����-1 ���� 0���� 1�� �ٿ�����.
        x[i] = x[i - 1]; //������ ��ǥ�� ��ĭ�� �̵��Ѵ�.
        y[i] = y[i - 1]; //������ ��ǥ�� ��ĭ�� �̵��Ѵ�.
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ģ��.
    if (dir == LEFT) --x[0]; //������ ��� x�迭 0�� ���� 1 ���δ�.
    if (dir == RIGHT) ++x[0]; //������ ��� x�迭 0�� ���� 1 �ø���.
    if (dir == UP) --y[0]; //������ ��� y�迭 0�� ���� 1 ���δ�.
    if (dir == DOWN) ++y[0]; //�Ʒ����� ��� y�迭 0�� ���� 1 �ø���.
    gotoxy(MAP_X + x[i], MAP_Y + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸���.
}

void pause(void) { // pŰ�� ������ ����� �Լ��� �����Ѵ�.
    while (1) { //��� �ݺ��Ѵ�.
        if (key == PAUSE) { //���� key���� p�� �޴´ٸ�
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "< PAUSE : PRESS ANY KEY TO RESUME > "); //(9,2)��ġ�� ""�� �ۼ��Ѵ�.
            Sleep(400); //0.4�� �������Ѵ�.
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "                                    "); //(9,2)��ġ�� ��ĭ���� �ٲ۴�.
            Sleep(400); //0.4�� �������Ѵ�.
        }
        else { //���� key���� p�� �ƴϸ�
            draw_map(); //���� �׸��� �Լ��� �ҷ��´�.
            return; //����������.
        }
        if (_kbhit()) { //Ű���尡 ���ȴ��� Ȯ���ϴ� �Լ��� �����´ٸ�
            do {
                key = _getch(); //key���� �Է��� �־��.
            } while (key == 224); //����Ű���� 224���� ��ȯ�Ѵٸ�
        }

    }
}

void game_over(void) { //�������� �Լ��� �����Ѵ�.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "+----------------------+"); //(12, 7)��ǥ�� ""�� �־��.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "|      GAME OVER..     |"); //(12,8) ��ǥ�� ""�� �־��.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "+----------------------+"); //(12,9) ��ǥ�� ""�� �־��.
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " YOUR SCORE : "); //(12, 10) ��ǥ�� ""�� �־��.
    printf("%d", last_score = score); //score�� ������������ �־� ����Ѵ�.

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, " Press any keys to restart.. "); //(11, 14)��ǥ�� ""�� �־��.

    if (score > best_score) { //���� ������ bestscore���� ������
        best_score = score; //bestscore�� �־��
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 10, "�� BEST SCORE ��"); //(14,12)��ǥ�� ""�� �־��.
    }
    Sleep(500); //0.5�� �������ض�
    while (_kbhit()) _getch(); //���� Ű�� �Է��ߴ��� Ȯ���� �ϰ�
    key = _getch(); //key���� �־��.
    title(); //title �Լ��� �ҷ��Ͷ�.
}

void food(void) { //food �Լ��� �����ض�.
    int i; //�ݺ��� ���� i�� �����Ѵ�.

    int food_crush_on = 0;//food�� �� ������ǥ�� ���� ��� on 
    int r = 0; //���� ������ ���Ǵ� ������ �����Ѵ�.
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, " YOUR SCORE: "); //(3, 22)��ǥ�� ""�� �־��. 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %3d", score, last_score, best_score); //�ִ� 3�ڸ����� ������ ǥ���ض�.

    while (1) { //��� �ݺ��Ѵ�.
        food_crush_on = 0; //food_crush_on�� off�Ѵ�.
        srand((unsigned)time(NULL) + r); //����ǥ�� �����Ѵ�.
        food_x = (rand() % (MAP_WIDTH - 2)) + 1;    //������ x��ǥ���� �־��.
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1; //������ y��ǥ���� �ִ´�.

        for (i = 0; i < length; i++) { //i�� 0���� ������̱��� 1�� �÷�����.
            if (food_x == x[i] && food_y == y[i]) { //food�� �� ����� ��ģ�ٸ�
                food_crush_on = 1; //��ġ�� food_crush_on �� on�Ѵ�.
                r++; //r�� 1 �ø���.
                break; //�����.
            }
        }

        if (food_crush_on == 1) 
            continue; //������ ��� while���� �ٽ� �����Ѵ�.

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "$"); //�Ȱ����� ��� ��ǥ���� food�� �־��.
        speed -= 3; //�ӵ��� 3��ŭ �����Ѵ�.
        break; //�����.

    }
}

void status(void) { //���� ���¸� �������� �Լ��� �����Ѵ�.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "head= "); //(34,2) ��ġ�� ""�� �ִ´�.
    printf("%2d,%2d", x[0], y[0]); //�迭 0�� ���� ǥ���ض�.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "food= "); //(34,3) ��ġ�� ""�� �ִ´�.
    printf("%2d,%2d", food_x, food_y); //food�� ��ǥ�� ǥ���ض�.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "leng= "); //(34,4) ��ġ�� ""�� �ִ´�.
    printf("%2d", length); //���� ���̸� 3�ڸ������� ǥ���ض�.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "key= "); //(34,5) ��ġ�� ""�� �ִ´�.
    printf("%3d", key); //key���� 3�ڸ������� ǥ���ض�.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "spd= "); //(34,6) ��ġ�� ""�� �ִ´�.
    printf("%3d", speed); //speed�� 3�ڸ������� ǥ���ض�.
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 6, "score= "); //(34,7) ��ġ�� ""�� �ִ´�.
    printf("%3d", score); //score�� 3�ڸ������� ǥ���ض�.
}