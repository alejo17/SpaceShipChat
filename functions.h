#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#pragma warning( disable : 4996)

#include <ncurses.h>
#include <string>
#include <string.h>
using namespace std;

enum {der,arr,izq,aba};



typedef struct _win_border_struct {
    chtype  ls, rs, ts, bs,
            tl, tr, bl, br;

}WIN_BORDER;

typedef struct _WIN_struct {
    int startx, starty;
    int height, width;
    WIN_BORDER border;

}WIN;

void init_win_params(WIN *p_win){
    p_win->height = 8;
    p_win->width = 16;
    p_win->starty = (LINES - p_win->height)/2;
    p_win->startx = (COLS - p_win->width)/2;
    p_win->border.ls = '|';
    p_win->border.rs = '|';
    p_win->border.ts = '-';
    p_win->border.bs = '-';
    p_win->border.tl = '+';
    p_win->border.tr = '+';
    p_win->border.bl = '+';
    p_win->border.br = '+';
}

void init_bullet_params(WIN *p_win, WIN *p_player, int direction){

    int i, j;
    int x, y, w, h;

    x = p_win -> startx;
    y = p_win -> starty;
    w = p_win -> width;
    h = p_win -> height;

    switch (direction) {
      case der:
            //derecha // 0
            p_player -> height = 2;
            p_player -> width  = 2;
            p_player -> starty = y + 4;
            p_player -> startx = x + w + 2;
            break;

      case arr:
            // arrriba // 1
            p_player -> height = 2;
            p_player -> width  = 2;
            p_player -> starty = y - 2 ;
            p_player -> startx = x + w/2;
            break;

      case izq:
            // izquierda // 2
            p_player -> height = 2;
            p_player -> width  = 2;
            p_player -> starty = y + 4;
            p_player -> startx = x - 4;
            break;

      case aba:
            //abajo // 3
            p_player -> height = 2;
            p_player -> width  = 2;
            p_player -> starty = y + h + 2;
            p_player -> startx = x + w/2;
            break;
    }
}




void print_win_params(WIN *p_win){
    std::ofstream outfile ("win_params.txt");

    outfile << "LINES  = " << LINES << "\n";
    outfile << "COLS   = " << COLS << "\n";
    outfile << "width  = " << p_win -> width << "\n";
    outfile << "height = " << p_win -> height << "\n";
    outfile << "startx = " << p_win -> startx << "\n";
    outfile << "starty = " << p_win -> starty;

    outfile.close();
}

void create_box(WIN *p_win, bool flag){

    int i, j;
    int x, y, w, h;

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    if(flag == TRUE){

        move( y+0,x ); addstr("  ##        ##  ");
        move( y+1,x ); addstr("    #      #    ");
        move( y+2,x ); addstr("  ############  ");
        move( y+3,x ); addstr(" ###  ####  ### ");
        move( y+4,x ); addstr("################");
        move( y+5,x ); addstr("# ############ #");
        move( y+6,x ); addstr("# #          # #");
        move( y+7,x ); addstr("   ##      ##   ");

    }else
        for(j = y -1 ; j <= y + h; ++j)
            for(i = x; i <= x + w; ++i)
                mvaddch(j, i, ' ');

    refresh();
}

void createPlayerName(WIN *p_win, string nickName, int lives){
    int i, j;
    int x, y, w, h;

    char lv[5] = {'x','x','x','x','x'};

    lv[lives] = '\0';

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    init_pair(2, COLOR_YELLOW, COLOR_RED);
    move( y -1 ,x );
    attron(COLOR_PAIR(2));
    printw(lv);
    refresh();
    attroff(COLOR_PAIR(2));

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    move( y + 8,x );
    attron(COLOR_PAIR(1));
    printw(nickName.c_str());
    refresh();
    attroff(COLOR_PAIR(1));

}

void create_box_bullet(WIN *p_win, bool flag){

    int i, j;
    int x, y, w, h;

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    if(flag == TRUE){
        move( y , x);       addstr("**");
        move( y + 1 , x);   addstr("**");
    }else{
        move( y , x );     addstr("  ");
        move( y + 1, x);   addstr("  ");
        // for(j = y; j <= y + h; ++j)
        //     for(i = x; i <= x + w; ++i)
        //         mvaddch(j, i, ' ');
    }



    refresh();
}



#endif
