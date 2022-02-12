//tetris!!!

#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h> //for rand()
#include <unistd.h>  /* only for usleep() */
//start rounded to the left

/*

Read input
If piece is active:
If in Ti or ACE, hold
Rotation
If this isn't a piece's first frame, shift (horizontal movement)
Gravity
Lock
Process delays
Update the screen and play sounds
*/


char I[4][4][4] = {
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    }
};
char O[4][4][4] = {
    {
        {0, 0, 0, 0},
        {0, 2, 2, 0},
        {0, 2, 2, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 2, 2, 0},
        {0, 2, 2, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 2, 2, 0},
        {0, 2, 2, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 2, 2, 0},
        {0, 2, 2, 0},
        {0, 0, 0, 0}
    }
};
char T[4][4][4] = {
    {
        {0, 0, 0, 0},
        {0, 3, 0, 0},
        {3, 3, 3, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 3, 0, 0},
        {0, 3, 3, 0},
        {0, 3, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {3, 3, 3, 0},
        {0, 3, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 3, 0, 0},
        {3, 3, 0, 0},
        {0, 3, 0, 0}
    }
};
char Z[4][4][4] = {
    {
        {0, 0, 0, 0},
        {4, 4, 0, 0},
        {0, 4, 4, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 4, 0},
        {0, 4, 4, 0},
        {0, 4, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {4, 4, 0, 0},
        {0, 4, 4, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 4, 0, 0},
        {4, 4, 0, 0},
        {4, 0, 0, 0}
    }
};
char S[4][4][4] = {
    {
        {0, 0, 0, 0},
        {0, 5, 5, 0},
        {5, 5, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 5, 0, 0},
        {0, 5, 5, 0},
        {0, 0, 5, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 5, 5, 0},
        {5, 5, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {5, 0, 0, 0},
        {5, 5, 0, 0},
        {0, 5, 0, 0}
    }
};
char J[4][4][4] = {
    {
        {0, 0, 0, 0},
        {6, 0, 0, 0},
        {6, 6, 6, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 6, 6, 0},
        {0, 6, 0, 0},
        {0, 6, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {6, 6, 6, 0},
        {0, 0, 6, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 6, 0, 0},
        {0, 6, 0, 0},
        {6, 6, 0, 0}
    }
};
char L[4][4][4] = {
    {
        {0, 0, 0, 0},
        {0, 0, 7, 0},
        {7, 7, 7, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 7, 0, 0},
        {0, 7, 0, 0},
        {0, 7, 7, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {7, 7, 7, 0},
        {7, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {7, 7, 0, 0},
        {0, 7, 0, 0},
        {0, 7, 0, 0}
    }
};

char charRef[7] = {
    '@',
    'O',
    '#',
    '$',
    'X',
    '%',
    '&'
};

//apparently i can use bit fields and then do bitwise operators
char (*ref[7])[4][4][4] = {
    &I,
    &O,
    &T,
    &Z,
    &S,
    &J,
    &L
};

int kicks[9][2] = { //x,-y
    {0,0},
    {1,0},
    {-1,0},
    {0,1},
    {1,1},
    {-1,1},
    {0,-1},
    {1,-1},
    {-1,-1}
};

/*
     1     I     @    Cyan
      2     O     O    White
     3     T     #    Magenta
      4     Z     $    Green
     5     S     X    Blue
      6     J     %    Red
     7     L     &    Yellow
*/


//int ch; //current key that was pressed, might make this a buffer later
bool stop; //wether the game has ended
char board[24][10] = {0}; //the game board, 24*10 but the top 4 lines arent visible to the player
int startx, starty, width, height;
WINDOW *win;
WINDOW *lwin; //for hold
WINDOW *rwin; //for next and score
int chBuff[5] = {0};
int x, y;
int hold; //block being held
bool hasHeld;
int score;
int rotation;
int frameNo = 0; //increments to 59 then back to 0
int tetronimoIndex = 0;
int tetronimoQueue[7] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6
};

bool checkTransform(int y, int x, int rotation, int block) {
    //for (int i = 0; i < 4; i++) {
    //    for (int j = 0; j < 4; j++) {
    //        if ((*ref[block])[rotation][i][j] != 0) {
    //            //const char *c = 3;
    //            mvwaddch(win, y+5+i, x+5+j, 3);
    //            wrefresh(win);
    //        }
    //    }
    //}
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((*ref[block])[rotation][i][j] != 0) {//(y+i < 0) ||
                if ((x+j < 0) || (x+j > 9) || (y+i > 23) || (board[y+i][x+j] != 0)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void lock() { //locks the currently active tetronimo to the board and does other things
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((*ref[tetronimoQueue[tetronimoIndex]])[rotation][i][j] != 0) {
                board[i+y][j+x] = (*ref[tetronimoQueue[tetronimoIndex]])[rotation][i][j];
            }
        }
    }
    
    //remove filled lines:


    int d = 0;
    for (int i = 0; i < 4; i++) {
        int c = 0;
        for (int j = 0; j < 10; j++) {
            if (board[i+y][j] != 0) {
                if (i+y < 4) {
                    stop = 1;
                }
                c++;
            }
        }
        if (c == 10) {//clear line
            d++;
            for (int k = (i+y); k > 2; k--) {
                for (int j = 0; j < 10; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
        }
    }
    switch (d) {
        case 1:
            score = score + 40;
            break;
        case 2:
            score = score + 100;
            break;
        case 3:
            score = score + 300;
            break;
        case 4:
            score = score + 1200;
            break;
    }





    //do reinit stuff:
    hasHeld = 0;
    rotation = 0;
    y = 0;
    x = 3;
    if (tetronimoIndex == 0) {
        for (int i = 0; i < 7; i++) {
            int t = tetronimoQueue[i];
            int r = (rand() % 7);
            tetronimoQueue[i] = tetronimoQueue[r];
            tetronimoQueue[r] = t;
        }
    }
    tetronimoIndex = (tetronimoIndex+1)%7;

    //tetronimo = rand() % 7;
    
}

void refreshboard() { //prints everything     ~~prints the currently saved board, NOT the currently active tetronimo~~
    werase(win);
    box(win, 0, 0);
    //box(rwin, 0, 0);

    //preview insta drop
    for (int e = y; e < 24; e++) {
        if (checkTransform(e, x, rotation, tetronimoQueue[tetronimoIndex])) {
            continue;
        } else {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if ((*ref[tetronimoQueue[tetronimoIndex]])[rotation][i][j] != 0) {
                        wattron(win, COLOR_PAIR(tetronimoQueue[tetronimoIndex]+1));
                        mvwaddch(win, e+i, x+j+1, '.');
                        wattroff(win, COLOR_PAIR(tetronimoQueue[tetronimoIndex]+1));
                    }
                }
            }
            break;
        }
    }
    wattron(win, COLOR_PAIR(6));
    for (int j = 0; j < 10; j++) {
        mvwaddch(win, 3, j+1, '_');
    }
    wattroff(win, COLOR_PAIR(6));
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 10; j++) {
            if (board[i][j] != 0) {
                wattron(win, COLOR_PAIR(board[i][j]));
                mvwaddch(win, i+1, j+1, charRef[board[i][j]-1]);
                wattroff(win, COLOR_PAIR(board[i][j]));
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((*ref[tetronimoQueue[tetronimoIndex]])[rotation][i][j] != 0) {
                wattron(win, COLOR_PAIR(tetronimoQueue[tetronimoIndex]+1));
                mvwaddch(win, y+1+i, x+1+j, charRef[tetronimoQueue[tetronimoIndex]]);
                wattroff(win, COLOR_PAIR(tetronimoQueue[tetronimoIndex]+1));
            }
        }
    }

    //score display
    mvwaddstr(rwin, 1, 2, "Score:");
    mvwprintw(rwin, 2, 2, "%i", score);
    //mvwprintw(rwin, 3, 2, "%i", hold);

    //hold
    //mvaddstr(starty+2, startx+width+4, "hold:");
    mvwaddstr(lwin, 1, 2, "Hold:");
    if (hold != -1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if ((*ref[hold])[0][i][j] != 0) {
                    wattron(lwin, COLOR_PAIR(hold+1));
                    mvwaddch(lwin, i+2, j+3, charRef[hold]);
                    wattroff(lwin, COLOR_PAIR(hold+1));
                } else {
                    mvwaddch(lwin, i+2, j+3, ' ');
                }
            }
        }
    }

    //next block
    mvwaddstr(rwin, 4, 2, "Next:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((*ref[tetronimoQueue[tetronimoIndex+1]])[0][i][j] != 0) {
                wattron(rwin, COLOR_PAIR(tetronimoQueue[tetronimoIndex+1]+1));
                mvwaddch(rwin, i+5, j+3, charRef[tetronimoQueue[tetronimoIndex+1]]);
                wattroff(rwin, COLOR_PAIR(tetronimoQueue[tetronimoIndex+1]+1));
            } else {
                mvwaddch(rwin, i+5, j+3, ' ');
            }
        }
    }


    //refresh();
    wrefresh(win);
    wrefresh(rwin);
    wrefresh(lwin);


}

void *mainThread() {
    y = 0;
    x = 3;
    int ch;
    rotation = 0;
    hold = -1;
    height = 24;
    width = 10;
    starty = (LINES - height) / 2;	/* Calculating for a center placement */
    startx = (COLS - width) / 2;	/* of the window		*/
    win = newwin(height+2, width+2, starty+1, startx+1);
    lwin = newwin(9, 7, starty+1, startx-9);
    rwin = newwin(15, 10, starty+1, startx+width+3);
    //tetronimo = rand() % 7;
    for (int i = 0; i < 7; i++) {
        int t = tetronimoQueue[i];
        int r = (rand() % 7);
        tetronimoQueue[i] = tetronimoQueue[r];
        tetronimoQueue[r] = t;
    }
    //refreshboard();

    while (stop == 0) { //actual game code goes here

        usleep(16667);
        frameNo = (frameNo+1)%60;
        


    
        //refreshboard();
        //wclear(win);
        //box(win, 0, 0);
        //mvwaddch(win, y+1, x+1, ' ');
        
        refreshboard();
        ch = chBuff[0];

        if (frameNo == 59) {//check if on the bottom, if so then lock
            if (checkTransform(y+1, x, rotation, tetronimoQueue[tetronimoIndex])) {
                y++;
            } else {
                lock();
            }
        }
        
        switch (ch) {
            case KEY_RIGHT: case 'd':
                if (checkTransform(y, x+1, rotation, tetronimoQueue[tetronimoIndex])) {
                    x++;
                }
                break;
            case KEY_LEFT: case 'a':
                if (checkTransform(y, x-1, rotation, tetronimoQueue[tetronimoIndex])) {
                    x--;
                }
                break;
            case KEY_UP: case 'w':
                //rotate 
                //y--;
                for (int i = 0; i < 9; i++) {
                    if (checkTransform(y+kicks[i][1], x+kicks[i][0], (rotation+1)%4, tetronimoQueue[tetronimoIndex])) {
                        rotation = (rotation+1)%4;
                        y += kicks[i][1];
                        x += kicks[i][0];
                        break;
                    }
                }
                break;
            case KEY_DOWN: case 's'://maybe add lock code here
                //move down
                if (checkTransform(y+1, x, rotation, tetronimoQueue[tetronimoIndex])) {
                    y++;
                    score++;
                } else {
                    lock();
                }
                break;
            case ' ':
                //instant drop
                for (int i = y; i < 24; i++) {
                    if (checkTransform(i, x, rotation, tetronimoQueue[tetronimoIndex])) {
                        continue;
                    } else {
                        score = score + (i-y)*2;
                        y = i-1;
                        lock();
                        break;
                    }
                }
                break;
            case 'c'://hold
                if (hasHeld == 0) {
                    hasHeld = 1;
                    y = 0;
                    x = 3;
                    rotation = 0;
                    if (hold != -1) {
                        int tempHold = hold;
                        hold = tetronimoQueue[tetronimoIndex];
                        tetronimoQueue[tetronimoIndex] = tempHold;
                    } else {
                        hold = tetronimoQueue[tetronimoIndex];
                        //do same reinit stuff as in lock:
                        hasHeld = 0;
                        if (tetronimoIndex == 0) {
                            for (int i = 0; i < 7; i++) {
                                int t = tetronimoQueue[i];
                                int r = (rand() % 7);
                                tetronimoQueue[i] = tetronimoQueue[r];
                                tetronimoQueue[r] = t;
                            }
                        }
                        tetronimoIndex = (tetronimoIndex+1)%7;
                    }
                }

        }
        
        //y = (y+height)%(height);\
        x = (x+width)%(width);
        y = y%height;

        //mvwaddch(win, y+1, x+1, 3);
        

        //remove from output buffer
        for (int i = 0; i < 4; i++) {
            chBuff[i] = chBuff[i+1];
        }
    }
    return NULL;
}
/*
{0, 0, 0, 0}  -> 0
{'a', 'b', 0, 0} -> 0 (+ 'a', 'b')
{'b', 0, 0, 0} -> 'a'
{0, 0, 0, 0} -> 'b'
{0, 0, 0, 0}  -> 0
{'a', 'b', 'c', 0}  -> 0 (+ 'a', 'b', 'c')
{'b', 'c', 'd', 'e'}  -> 'a' (+ 'd', 'e', 'f')
*/
void *inputThread() {
    while (1) {
        int ch;
        ch = getch();//simple but effective
        for (int i=0; i < 4; i++) {
            if (chBuff[i] == 0 || i == 3) {
                chBuff[i] = ch;
                break;
            }
        }
    }
    return NULL;
};

void update() {
}

int main() {
    srand(time(0));

    /*//initial test when making the ref variable; new to pointers
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                for (int l = 0; l < 4; l++) {
                    if ((*ref[i])[j][k][l] == 0) {
                        printf(" ");
                    } else {
                        printf("%c", (*ref[i])[j][k][l]);
                    }
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
    //return 0;*/

    //game init code goes here



    //ncurses setup
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    curs_set(0);
    //colours
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);

    //starting of threads
    pthread_t id1, id2;
    pthread_create(&id2, NULL, inputThread, NULL);
    pthread_create(&id1, NULL, mainThread, NULL);
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);


    //this code will only be reached once the game stops

    endwin();
    return 0;
}