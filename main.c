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

int normKicks[4][5][2] = {//index is the initial state of rotation before rotating 90° clockwise (also given as x, -y)
    {
        {0,0},
        {-1,0},
        {-1,-1},
        {0,2},
        {-1,2}
    },
    {
        {0,0},
        {1,0},
        {1,1},
        {0,-2},
        {1,-2}
    },
    {
        {0,0},
        {1,0},
        {1,-1},
        {0,2},
        {1,2}
    },
    {
        {0,0},
        {-1,0},
        {-1,1},
        {0,-2},
        {-1,-2}
    }
};
int iKicks[4][5][2] = {
    {
        {0,0},
        {-2,0},
        {1,0},
        {-2,1},
        {1,-2}
    },
    {
        {0,0},
        {-1,0},
        {2,0},
        {-1,-2},
        {2,1}
    },
    {
        {0,0},
        {2,0},
        {-1,0},
        {2,-1},
        {-1,2}
    },
    {
        {0,0},
        {1,0},
        {-2,0},
        {1,2},
        {-2,-1}
    }
};

int (*kickTable[7])[4][5][2] = {
    &iKicks,
    &normKicks,
    &normKicks,
    &normKicks,
    &normKicks,
    &normKicks,
    &normKicks
};

//int kicks[9][2] = { //x,-y
//    {0,0},
//    {1,0},
//    {-1,0},
//    {0,1},
//    {1,1},
//    {-1,1},
//    {0,-1},
//    {1,-1},
//    {-1,-1}
//};

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
WINDOW *nwin; //for next
WINDOW *swin; //for score and level
int chBuff[5] = {0};
int x, y;
int level;
int grav = 60;
int hold; //block being held
bool hasHeld;
int score = 0;
int rotation;
int lockDelay;
int lockMoves;
int paused;
int rot; //1 if last move was a rotation, 2 if there was also a kick. for t spin recognition.
int frameNo = 0; //increments to 59 then back to 0
//bool altQueue = 0;//wether we are reading the first or second half of the tetromino queue
int tIndex = 0;
int numLines = 0; //using variable goal levelling, also the var name is kinda misleading; this refers to the point goal for levelling up
int tQueue[7] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6
};
int tQueueNext[7] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6
};

int levelGravity[30] = {
    60,
    53,
    49,
    45,
    41,
    37,
    33,
    28,
    22,
    17,
    11,
    10,
    9,
    8,
    7,
    6,
    6,
    5,
    5,
    4,
    4,
    3,
    3,
    3,
    2,
    2,
    2,
    1,
    1,
    1
};


void shuffle(int *array, int size) {
    for (int i = 0; i < size-1; ++i) {
        int j = rand() % (size-i) + i;
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

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
            if ((*ref[tQueue[tIndex]])[rotation][i][j] != 0) {
                board[i+y][j+x] = (*ref[tQueue[tIndex]])[rotation][i][j];
            }
        }
    }
    
    //remove filled lines:


    //int numLines = 0;
    int d = 0;
    for (int i = 0; i < 4; i++) {
        int c = 0;
        for (int j = 0; j < 10; j++) {
            if (board[i+y][j] != 0) {
                if (i+y < 3) {
                    stop = 1;
                    return;
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
    } //numlines doesnt roll over when levelling up
    switch (d) {
        case 1:
            numLines += 1;
            break;
        case 2:
            numLines += 3;
            break;
        case 3:
            numLines += 5;
            break;
        case 4:
            numLines += 8;
            break;
    }

    if (numLines >= ((level+1) * 5)) {
        numLines = 0;
        level++;
    }

        //{0, 0, 0, 0},
        //{0, 3, 0, 0},
        //{0, 3, 3, 0},
        //{0, 3, 0, 0}
    //int t_multiplier = 0;
    //check if a t-spin has happened
    if (tQueue[tIndex] == 2 && rot != 0) {//check if block is a t and the last action was a rotation
        if (((board[y+1][x] != 0) + (board[y+1][x+2] != 0) + (board[y+3][x] != 0) + (board[y+3][x+2] != 0)) >= 3) {
            //now award points
            if (rot == 1) {//no kick
                score += 200*(level+1);
            } else {//with kick
                score += 400*(level+1);
            }
        }
    }

    //level = (numLines/10);

    if (level > 29) {
        grav = 1;
    } else {
        grav = levelGravity[level];
    }

    switch (d) {//yet to add perfect clears
        case 1:
            score += 100*(level+1);
            break;
        case 2:
            score += 300*(level+1);
            break;
        case 3:
            score += 500*(level+1);
            break;
        case 4:
            score += 800*(level+1); //'difficult'
            break;
    }



    //do reinit stuff:
    hasHeld = 0;
    frameNo = 0;
    lockDelay = 0;
    lockMoves = 0;
    rotation = 0;
    rot = 0;
    y = 0;
    x = 3;
    if (tIndex == 6) {
        for (int i = 0; i < 7; i++) {
            tQueue[i] = tQueueNext[i];
        }
        shuffle(tQueueNext, 7);
    }
    tIndex = (tIndex+1)%7;

    //tetronimo = rand() % 7;
    
}

void refreshboard() { //prints everything     ~~prints the currently saved board, NOT the currently active tetronimo~~
    werase(win);
    box(win, 0, 0);
    box(nwin, 0, 0);
    box(lwin, 0, 0);
    //box(swin, 0, 0);


    //preview insta drop
    if (paused == 0) {
        for (int e = y; e < 24; e++) {
            if (checkTransform(e, x, rotation, tQueue[tIndex])) {
                continue;
            } else {
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if ((*ref[tQueue[tIndex]])[rotation][i][j] != 0) {
                            //wattron(win, COLOR_PAIR(tQueue[tIndex]+1));
                            mvwaddch(win, e+i, x+j+1, '.');
                            //wattroff(win, COLOR_PAIR(tQueue[tIndex]+1));
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
                    //if (paused == 1) {
                    //    mvwaddch(win, i+1, j+1, '#');
                    //} else {
                    wattron(win, COLOR_PAIR(board[i][j]));
                    mvwaddch(win, i+1, j+1, charRef[board[i][j]-1]);
                    wattroff(win, COLOR_PAIR(board[i][j]));
                    //}
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if ((*ref[tQueue[tIndex]])[rotation][i][j] != 0) {
                    if (lockDelay != 0) {
                        wattron(win, A_BOLD);
                    }
                    wattron(win, COLOR_PAIR(tQueue[tIndex]+1));
                    mvwaddch(win, y+1+i, x+1+j, charRef[tQueue[tIndex]]);
                    wattroff(win, COLOR_PAIR(tQueue[tIndex]+1));
                    wattroff(win, A_BOLD);
                }
            }
        }
    }

    
    //mvwaddstr(swin, 3, 1, "Score:");
   // mvwprintw(swin, 4, 1, "%i", score);
    //mvwprintw(nwin, 3, 2, "%i", hold);
    
    //level
    mvwaddstr(swin, 1, 1, "LEVEL:");
    mvwprintw(swin, 2, 1, "%i", level+1);//level+1);

    //score display
    mvwaddstr(swin, 4, 1, "SCORE:");
    mvwprintw(swin, 5, 1, "%i", score);

    
    //hold
    //mvaddstr(starty+2, startx+width+4, "hold:");
    mvwaddstr(lwin, 0, 1, "HOLD");
    if (hold != -1) {
        int r = 1;
        if (hold == 1 || hold == 0) {
            r = 0;
        }
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if ((*ref[hold])[r][(i-1+r)%4][j] != 0 && paused == 0) {
                    wattron(lwin, COLOR_PAIR(hold+1));
                    mvwaddch(lwin, i, j+1, charRef[hold]);
                    wattroff(lwin, COLOR_PAIR(hold+1));
                } else {
                    mvwaddch(lwin, i, j+1, ' ');
                }
            }
        }
    }

    //next block
    mvwaddstr(nwin, 0, 1, "NEXT"); //display next 3 blocks
    for (int k = 0; k < 3; k++) {
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int nextTetramino;
                int r = 1;
                //printf("%i\n", tIndex);
                if (tIndex+k >= 6) {
                    nextTetramino = tQueueNext[(tIndex+k)%6];
                } else {
                    nextTetramino = tQueue[(tIndex+k)+1];
                }
                if (nextTetramino == 1 || nextTetramino == 0) {
                    r = 0;
                }
                if ((*ref[nextTetramino])[r][(i-1+r)%4][j] != 0 && paused == 0) {
                    wattron(nwin, COLOR_PAIR(nextTetramino+1));
                    mvwaddch(nwin, i+4*k, j+1, charRef[nextTetramino]);
                    wattroff(nwin, COLOR_PAIR(nextTetramino+1));
                } else {
                    mvwaddch(nwin, i+4*k, j+1, ' ');
                }
            }
        }
    }

    if (paused == 1) {
        mvwaddstr(win, 12, 2, "<PAUSED>");
    }

    //refresh();
    wrefresh(win);
    wrefresh(nwin);
    wrefresh(lwin);
    wrefresh(swin);


}

void *mainThread() {
    y = 0;
    x = 3;
    int ch;
    rotation = 0;
    level = 0;
    hold = -1;
    //score = 1234567890;
    height = 24;
    width = 10;
    starty = (LINES - height) / 2 - 1 > 0 ? (LINES - height) / 2 - 1 : 0;	/* Calculating for a center placement */
    startx = (COLS - width) / 2 - 1 > 0 ? (COLS - width) / 2 - 1 : 0;//(COLS - width) / 2;	/* of the window		*/
    win = newwin(height+2, width+2, starty, startx);
    lwin = newwin(5, 6, starty, startx-6);
    nwin = newwin(13, 6, starty, startx+width+2);
    swin = newwin(6, 7, starty+12, startx+width+1);
    //tetronimo = rand() % 7;

    shuffle(tQueue, 7);
    shuffle(tQueueNext, 7);
    shuffle(tQueue, 7); //second time to remove the predictibility of RNG in first few frames of gameplay, otherwise the game would always start with the same piece usually
    //endwin();
    //for (int i = 0; i < 7; i++) {
    //    printf("%i\n", tQueue[i]);
    //}
    //printf("\n\n");
    //for (int i = 0; i < 7; i++) {
    //    printf("%i\n", tQueueNext[i]);
    //}
//
    //return 0;

    //refreshboard();

    while (stop == 0) { //actual game code goes here

        usleep(16667);

        ch = chBuff[0];

        //remove from output buffer
        for (int i = 0; i < 4; i++) {
            chBuff[i] = chBuff[i+1];
        }
            //wclear(win);
            //werase(win);
            //wclear(stdscr);
            //werase(stdscr);
        refreshboard();

        //i know its clunky but eh it still works\
        (as in putting the check for r to recentre outside the switch)
        if (ch == 'r') {
            //refreshboard();
            //height = 24;
            //width = 10;
            //starty = (0);
            starty = (LINES - height) / 2 - 1 > 0 ? (LINES - height) / 2 - 1 : 0;
            startx = (COLS - width) / 2 - 1 > 0 ? (COLS - width) / 2 - 1 : 0;
            mvwin(win, starty, startx);
            wresize(win, height+2, width+2);
            mvwin(lwin, starty, startx-6);
            mvwin(nwin, starty, startx+width+2);
            mvwin(swin, starty+12, startx+width+1);
            erase();
            refreshboard();
               //i was getting desperate to fix a visual bug at one point -\v
            //redrawwin(win);
            //redrawwin(lwin);
            //redrawwin(nwin);
            //wclear(win);
            //wclear(lwin);
            //wclear(nwin);
            //clear();
            //werase(win);
            //werase(lwin);
            //werase(nwin);
            //erase();
            //wrefresh(win);
            //wrefresh(lwin);
            //wrefresh(nwin);
            //refresh();
            //wclear(win);
            //wclear(lwin);
            //wclear(nwin);
            //clear();
            //werase(win);
            //werase(lwin);
            //werase(nwin);
            //erase();
            //clear();
            //refresh();
        }

        if (ch == 'p') {
            //mvwin(win, 0, (COLS - width) / 2);
            if (paused == 1) {
                paused = 0;
                //3
                mvwaddstr(win, 12, 2, "   3..  ");
                wrefresh(win);
                usleep(400000);
                mvwaddstr(win, 12, 5, "2..");
                wrefresh(win);
                //2
                usleep(400000);
                mvwaddstr(win, 12, 5, "1..");
                wrefresh(win);
                //1
                usleep(400000);
                mvwaddstr(win, 12, 5, "GO!");
                wrefresh(win);
                //go!
                usleep(300000);
                mvwaddstr(win, 12, 5, "   ");
                wrefresh(win);
                //<blank>
            } else {
                paused = 1;
            }
            //paused = (paused+1)%2;
        }

        if (paused) {
            continue;
        }
        frameNo = (frameNo+1)%grav;
        //score+=1000000;
        //score = COLS;
        //startx = (COLS - width) / 2;
        //printf("%i\n", mvwin(win, 0, (COLS - width) / 2));
        //mvwin(win, 0, (COLS - width) / 2);

    
        //wclear(win);
        //box(win, 0, 0);
        //mvwaddch(win, y+1, x+1, ' ');
        
        //refreshboard();

        if (frameNo == grav-1) {//check if on the bottom, if so then lock
            if (checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                y++;
                lockDelay = 0;
                lockMoves = 0;
                if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                    lockDelay = 1;
                }
            }
            // else if (lockDelay == 0) {
            //    lockDelay = 1;
            //}
        }

        if (lockDelay >= 30) {
            lock();
        } else if (lockDelay != 0) {
            lockDelay++;
        }
        
        switch (ch) {
            int delta[2] = {0};
            //case 'x'://recentre everything
            //    clear();
            //    height = 24;
            //    width = 10;
            //    starty = (0);
            //    startx = (COLS - width) / 2;
            //    mvwin(win, starty, startx);
            //    mvwin(lwin, starty, startx-9);
            //    mvwin(nwin, starty, startx+width+1);
            //    break;
            case KEY_RIGHT: case 'd':
                if (checkTransform(y, x+1, rotation, tQueue[tIndex])) {
                    x++;
                    rot = 0;
                    lockDelay = 0;
                    lockMoves++;

                    if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                        if (lockMoves >= 15) {
                            lock();
                        } else {
                            //x++;
                            //frameNo = 0;
                            //lockMoves++;
                            frameNo = 0;
                            lockDelay = 1;
                        }
                    }
                }
                break;
            case KEY_LEFT: case 'a':
                if (checkTransform(y, x-1, rotation, tQueue[tIndex])) {
                    x--;
                    rot = 0;
                    lockDelay = 0;
                    lockMoves++;

                    if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                        if (lockMoves >= 15) {
                            lock();
                        } else {
                            lockDelay = 1;
                            frameNo = 0;
                        }
                    }
                }
                //if (checkTransform(y, x-1, rotation, tQueue[tIndex])) {
                //    if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                //        if (lockMoves >= 15) {
                //            lock();
                //        } else {
                //            x--;
                //            lockMoves++;
                //            lockDelay = 1;
                //            frameNo = 0;
                //        }
                //    } else {
                //        x--;
                //        lockDelay = 0;
                //        lockMoves++;
                //    }
                //}
                //if (checkTransform(y, x-1, rotation, tQueue[tIndex])) {
                //    if (lockMoves >= 15 && !checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                //        lock();
                //        lockMoves = 0;
                //    } else {
                //        x--;
                //        lockMoves++;
                //    }
                //    lockDelay = 0;
                //}
                break;
            case KEY_UP: case 'w':
                //rotate 
                //y--;
                
                for (int i = 0; i < 5; i++) {
                    //(*kicks[block])[rotation][i]
                    delta[0] = (*kickTable[tQueue[tIndex]])[rotation][i][0];
                    delta[1] = (*kickTable[tQueue[tIndex]])[rotation][i][1];
                    if (checkTransform(y+delta[1], x+delta[0], (rotation+1)%4, tQueue[tIndex])) {
                        rotation = (rotation+1)%4;
                        y += delta[1];
                        x += delta[0];
                        
                        if (delta[1] != 0 || delta[0] != 0) {
                            rot = 2; //now eligible for a t-spin
                        } else {
                            rot = 1;
                        }
                        lockDelay = 0;
                        lockMoves++;

                        if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                            if (lockMoves >= 15) {
                                lock();
                            } else {
                                lockDelay = 1;
                                frameNo = 0;
                            }
                        }
                        //if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                        //    if (lockMoves >= 15) {
                        //        lock();
                        //    } else {
                        //        rotation = (rotation+1)%4;
                        //        y += kicks[i][1];
                        //        x += kicks[i][0];
                        //        lockMoves++;
                        //        lockDelay = 1;
                        //        frameNo = 0;
                        //    }
                        //} else {
                        //    rotation = (rotation+1)%4;
                        //    y += kicks[i][1];
                        //    x += kicks[i][0];
                        //    lockDelay = 0;
                        //    lockMoves++;
                        //}
                        break;
                    }
                }
                break;
            case KEY_DOWN: case 's'://maybe add lock code here
                //move down
                if (checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                    y++;
                    if (!checkTransform(y+1, x, rotation, tQueue[tIndex])) {
                        lockDelay = 1;
                    } else {
                        lockDelay = 0;
                    }
                    frameNo = 0;
                    lockMoves = 0;
                    score++;
                } else {
                    lock();
                }
                break;
            case ' ':
                //instant drop
                for (int i = y; i < 24; i++) {
                    if (checkTransform(i, x, rotation, tQueue[tIndex])) {
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
                        hold = tQueue[tIndex];
                        tQueue[tIndex] = tempHold;
                    } else {
                        hold = tQueue[tIndex];
                        //do same reinit stuff as in lock:
                        //hasHeld = 0;
                        if (tIndex == 6) {
                            for (int i = 0; i < 7; i++) {
                                tQueue[i] = tQueueNext[i];
                            }shuffle(tQueueNext, 7);
                        }
                        tIndex = (tIndex+1)%7;
                    }
                }
                break;

        }
        
        //y = (y+height)%(height);\
        x = (x+width)%(width);
        y = y%height;

        //mvwaddch(win, y+1, x+1, 3);
        

    }

    //code after it ends
    refreshboard();
    sleep(2);
    endwin();


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
    while (stop == 0) {
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

int main() {
    srand(time(NULL));

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
    usleep(500000);
    printf("\n\n\n\n\n\n      Your score was %i.\n\n\nPress any key to continue...", score);
    usleep(500000);
    getch();
    return 0;
}
