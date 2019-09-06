#pragma warning( disable : 4996)
#include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <iostream>
  #include <string.h>
  #include <thread>
  #include <vector>
  #include <fstream>
  #include <ncurses.h>
  #include "functions.h"

  using namespace std;

void menuOptions();
int getSizeOfFile(char* filename);
void Shoot(WIN*);

WIN vec_Player[3];
WIN vec_Bullet[3];
int lives[3] = {5,5,5};
int currentPlayer = -1;
string tmp_player="",pla1="",pla2="",pla3="";
vector<int> comasVec;


int buscarComas (string cadena, char caracter)
{

    for (int i=0;i<cadena.size();i++)
    {
        if ( cadena[i] == caracter )
          // cout<< i << " ";
          comasVec.push_back(i);
    }
}

//init_pair(1, COLOR_CYAN, COLOR_BLACK);

int DetectColision(WIN* p_bullet){
  for (int p = 0; p < 3; ++p){
      int x, y, w, h;
      int x_b, y_b, w_b, h_b;

      x = vec_Player[p].startx;
      y = vec_Player[p].starty;
      w = vec_Player[p].width;
      h = vec_Player[p].height;

      x_b = p_bullet -> startx;
      y_b = p_bullet -> starty;
      w_b = p_bullet -> width;
      h_b = p_bullet -> height;

      for(int jj = y_b; jj <= y_b + h_b; ++jj){
        for(int ii = x_b; ii <= x_b + w_b; ++ii){

            for(int j = y ; j <= y + h; ++j){
              for(int i = x; i <= x + w; ++i){
                if (j == jj && i == ii){
                  lives[p]--;
                  return p;
                }
              }
            }
        }
      }
  }
  return -1;
}

int Shoot(WIN *p_bullet, int direction){

    // create_box_bullet(p_bullet, TRUE);
    int i = COLS;
    int j = LINES;

    int colision = -1;

    switch (direction) {

      case 0:
        create_box_bullet(p_bullet, TRUE);

        while( p_bullet -> startx + 2 < i){
            usleep(30000);
            create_box_bullet(p_bullet, FALSE);
            p_bullet -> startx += 2;
            create_box_bullet(p_bullet, TRUE);
            colision = DetectColision(p_bullet);

            if (colision != -1){
              return colision;
            }
        }

        return colision;

      case 1:

        create_box_bullet(p_bullet, TRUE);

        while( p_bullet -> starty - 2 > 0 ){
            usleep(30000);
            create_box_bullet(p_bullet, FALSE);
            p_bullet -> starty -= 2;
            create_box_bullet(p_bullet, TRUE);

            colision = DetectColision(p_bullet);
            if (colision != -1){
              return colision;
            }
        }
        return colision;

      case 2:

        create_box_bullet(p_bullet, TRUE);

        while( p_bullet -> startx - 2 > 0){
              usleep(30000);
              create_box_bullet(p_bullet, FALSE);
              p_bullet -> startx -= 2;
              create_box_bullet(p_bullet, TRUE);

              colision = DetectColision(p_bullet);
              if (colision != -1){
                return colision;
              }
        }
        return colision;

      case 3:

        create_box_bullet(p_bullet, TRUE);

        while( p_bullet -> starty + 2 < j){
            usleep(30000);
            create_box_bullet(p_bullet, FALSE);
            p_bullet -> starty += 2;
            create_box_bullet(p_bullet, TRUE);

            colision = DetectColision(p_bullet);
            if (colision != -1){
              return colision;
            }
        }
        return colision;
    }
}


void myRead(int cnFD){
  int n, action;
  char buffer[256], len[4], header1[4], header2[1];

  do{
    bzero(header1, 4);
    read(cnFD, header1, 4);
    header1[4] = '\0';

    bzero(header2,1);
    read(cnFD, header2, 1);
    header2[1] = '\0';

    //login
    if (header2[0] == 'R'){
        // Leer msg del servidor
        int sizeOfMsg;
        char msg[256];
        string outmessage;

        sizeOfMsg = atoi(header1);

        read(cnFD, msg, sizeOfMsg);
        msg[sizeOfMsg] = '\0';
        outmessage = msg;

        cout << outmessage <<  "  " << endl << endl;
        menuOptions();
        cout << endl;
    }

    if (header2[0] == 'D'){
        int sizeOfFilename, sizeOfNickname, sizeOfFile;
        char bufNickname[256], bufFilename[256], sizeOfNick[2], bufSizeOfFile[4], bufMsg[1], bufNewFilename[256];

        //read(cnFD, header1, 4);
        //header1[4] = '\0';
        sizeOfFilename = atoi(header1);

        read(cnFD, sizeOfNick, 2);
        sizeOfNick[2] = '\0';
        sizeOfNickname = atoi(sizeOfNick);

        read (cnFD, bufNickname, sizeOfNickname);
        bufNickname[sizeOfNickname] = '\0';

        read (cnFD, bufFilename, sizeOfFilename);
        bufFilename[sizeOfFilename] = '\0';

        read (cnFD, bufSizeOfFile, 4);
        bufSizeOfFile[4] = '\0';
        sizeOfFile = atoi(bufSizeOfFile);

        vector<char> v;
        char msg;

        for (int i = 0; i < sizeOfFile; ++i){
          read(cnFD, bufMsg, 1);
          msg = bufMsg[0];
          v.push_back(msg);
        }

        std::ofstream outfile (bufFilename);
        for (int i = 0; i < v.size(); ++i){
          outfile << v[i];
        }

        outfile.close();
    }

    if (header2[0] == 'Z'){
      currentPlayer = atoi(header1);
    }


    // movements  //

    if (header2[0] == 'l'){
        int player;
        char buffplayer[256];

        read (cnFD, buffplayer, 4);
        player = atoi(buffplayer);

        switch(player){
          case 1:
            create_box(&vec_Player[player-1], FALSE);
            --vec_Player[player-1].startx;

            break;
          case 2:
            create_box(&vec_Player[player-1], FALSE);
            --vec_Player[player-1].startx;

            break;
          case 3:
            create_box(&vec_Player[player-1], FALSE);
            --vec_Player[player-1].startx;

            break;
        }

        create_box(&vec_Player[0], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[0]);

        create_box(&vec_Player[1], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[1]);

        create_box(&vec_Player[2], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[2]);
    }

    if (header2[0] == 'r'){
        int player;
        char buffplayer[256];

        read (cnFD, buffplayer, 4);
        player = atoi(buffplayer);

        switch(player){
          case 1:
            create_box(&vec_Player[player-1], FALSE);
            ++vec_Player[player-1].startx;

            //create_box(&vec_Player[player-1], TRUE);
            break;
          case 2:
            create_box(&vec_Player[player-1], FALSE);
            ++vec_Player[player-1].startx;

            //create_box(&vec_Player[player-1], TRUE);
            break;
          case 3:
            create_box(&vec_Player[player-1], FALSE);
            ++vec_Player[player-1].startx;

            //create_box(&vec_Player[player-1], TRUE);
            break;
        }

        create_box(&vec_Player[0], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[0]);

        create_box(&vec_Player[1], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[1]);

        create_box(&vec_Player[2], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[2]);

    }

    if (header2[0] == 'u'){

      int player;
        char buffplayer[256];

        read (cnFD, buffplayer, 4);
        player = atoi(buffplayer);

        switch(player){
          case 1:
            create_box(&vec_Player[player-1], FALSE);
            --vec_Player[player-1].starty;

            //create_box(&vec_Player[player-1], TRUE);
            break;
          case 2:
            create_box(&vec_Player[player-1], FALSE);
            --vec_Player[player-1].starty;

            //create_box(&vec_Player[player-1], TRUE);
            break;
          case 3:
            create_box(&vec_Player[player-1], FALSE);
            --vec_Player[player-1].starty;

            //create_box(&vec_Player[player-1], TRUE);
            break;
        }

        create_box(&vec_Player[0], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[0]);

        create_box(&vec_Player[1], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[1]);

        create_box(&vec_Player[2], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[2]);

    }

    if (header2[0] == 'd'){

      int player;
        char buffplayer[256];

        read (cnFD, buffplayer, 4);
        player = atoi(buffplayer);

        switch(player){
          case 1:
            create_box(&vec_Player[player-1], FALSE);
            ++vec_Player[player-1].starty;

            //create_box(&vec_Player[player-1], TRUE);
            break;
          case 2:
            create_box(&vec_Player[player-1], FALSE);
            ++vec_Player[player-1].starty;

            //create_box(&vec_Player[player-1], TRUE);
            break;
          case 3:
            create_box(&vec_Player[player-1], FALSE);
            ++vec_Player[player-1].starty;

            //create_box(&vec_Player[player-1], TRUE);
            break;
        }

        create_box(&vec_Player[0], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[0]);

        create_box(&vec_Player[1], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[1]);

        create_box(&vec_Player[2], TRUE);
        tmp_player="player"+to_string(player);
        createPlayerName(&vec_Player[player-1], tmp_player, lives[2]);

    }

    //Disparos

    if (header2[0] == 's'){

      int status = -1;
      int dir = atoi(header1);

      int player;
      char buffplayer[256];

        read (cnFD, buffplayer, 4);
        player = atoi(buffplayer);

        /*switch(player){
          case 1:
            init_bullet_params(&vec_Player[player-1], &vec_Bullet[player-1], dir);
            status = Shoot(&vec_Bullet[player-1], dir);
            break;
          case 2:
            init_bullet_params(&vec_Player[player-1], &vec_Bullet[player-1], dir);
            Shoot(&vec_Bullet[player-1], dir);
            break;
          case 3:
            init_bullet_params(&vec_Player[player-1], &vec_Bullet[player-1], dir);
            Shoot(&vec_Bullet[player-1], dir);
            break;
        }*/

        init_bullet_params(&vec_Player[player-1], &vec_Bullet[player-1], dir);
        status = Shoot(&vec_Bullet[player-1], dir);

        if(status != -1){
            /*init_pair(2, COLOR_YELLOW, COLOR_BLACK);
            attron(COLOR_PAIR(2));
            create_box(&vec_Player[status], FALSE);
            create_box(&vec_Player[status], TRUE);
            //refresh();
            attroff(COLOR_PAIR(2));*/
            sprintf(header1, "%04d", status);

            write(cnFD, header1, 4);
            write(cnFD, "#", 1);
        }

    }


    if (header2[0] == '#'){
        int player = atoi(header1);

        //cout << lives[player] << endl;
        //lives[player];
        //cout << lives[player] << endl;

        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        attron(COLOR_PAIR(2));
        create_box(&vec_Player[player], FALSE);
        create_box(&vec_Player[player], TRUE);

        int player_id=player+1;
        tmp_player="player"+to_string(player_id);
        createPlayerName(&vec_Player[player], tmp_player, lives[player]);

        refresh();
        attroff(COLOR_PAIR(2));
    }

  }while(1);
}


void myWrite(int cnFD){
  int n, action;
  char buffer[256], len[4], header1[4], header2[1], header11[4];
  string cad;

  do{

    cin >> action;

    switch(action){
      //login
      case 1:{
        int sizeOfNickName;
        char inNickname[256];
        string nickName;

        cout << "Ingrese su nickName: ";
        cin >> inNickname;
        //cin.getline(inNickname, 255);
        nickName = inNickname;
        sizeOfNickName = nickName.size();

        bzero(header1, 4);
        sprintf(header1, "%04d", sizeOfNickName);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "L", 1);
        write(cnFD, nickName.c_str(), sizeOfNickName);

        break;

      }case 3:{
        //cout << "obteniendo la lista..." << endl;
        bzero(header1, 4);
        sprintf(header1, "%04d", 0);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "P", 1);

        break;
      }
      case 2:{
        int sizeOfNickName,sizeOfMensaje;
        char toNickname[256];
        char toMensaje[256];
        string nickName;
        string mensaje;

        cout << "Ingrese nickName con quien hablar: ";
        cin >> toNickname;
        //cin.getline(inNickname, 255);

        nickName = toNickname;
        sizeOfNickName = nickName.size();

        bzero(header1, 4);
        sprintf(header1, "%04d", sizeOfNickName);
        header1[4] = '\0';

        cout << "Ingrese mensaje a enviar: ";
        cin >> hex >> toMensaje;
        //cin.getline(toMensaje,255);

        mensaje = toMensaje;
        sizeOfMensaje = mensaje.size();

        bzero(header11, 4);
        sprintf(header11, "%04d", sizeOfMensaje);
        header11[4] = '\0';


        cout<<"head1 "<<header1<<endl;
        write(cnFD, header1, 4);
        write(cnFD, "C", 1);
        cout<<"nick "<<nickName<<endl;
        write(cnFD, nickName.c_str(), sizeOfNickName);
        cout<<"head11 "<<header11<<endl;
        write(cnFD, header11, 4);
        cout<<"mensaje "<<mensaje<<endl;
        write(cnFD, mensaje.c_str(), sizeOfMensaje);

        break;
      }case 4:{

        char header3[256], header4[256], header5[256], header6[256];
        int sizeOfNickName, sizeOfFilename, sizeOfFile;
        string nickName, fileName;

        cout << "Ingrese NickName a quien enviar el archivo: ";
        cin >> header4;
        nickName = header4;

        sizeOfNickName = nickName.size();
        sprintf(header3, "%02d", sizeOfNickName);
        header3[2] = '\0';

        cout << "Ingrese el nombre del archivo a enviar: ";
        cin >> header5;
        fileName = header5;

        sizeOfFilename = fileName.size();
        sprintf(header1, "%04d", sizeOfFilename);
        header1[4] = '\0';

        sizeOfFile = getSizeOfFile(header5);
        sprintf(header6, "%04d", sizeOfFile);
        header6[4] = '\0';


        if (sizeOfFile != 0){
          write (cnFD, header1, 4); // filename size
          write (cnFD, "F", 1); // Action
          write (cnFD, header3, 2); // nickName size
          write (cnFD, header4, sizeOfNickName); // nickName
          write (cnFD, header5, sizeOfFilename); // filename
          write (cnFD, header6, 4); // file size

          vector<char> v;
          string msg;

          if (FILE *fp = fopen(header5, "r")){
            char buf[1024];
            while (size_t len = fread(buf, 1, sizeof(buf), fp))
              v.insert(v.end(), buf, buf + len);
            fclose(fp);
          }

          cout << "Enviando archivo ..." << endl;
          for (int i = 0; i < v.size(); ++i){
            msg = v[i];
            write (cnFD, msg.c_str(), 1);
          }
          cout << "Archivo enviado." << endl;

          //write (cnFD, header7, sizeOfFile); // file BIN
        }

        break;

      }case 0:{
        bzero(header1, 4);
        sprintf(header1, "%04d", 0);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "E", 1);

        break;
      }case 5:{
        bzero(header1, 4);
        sprintf(header1, "%04d", 0);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "G", 1);


        int ch;

        initscr();
        start_color();
        cbreak();

        keypad(stdscr, TRUE);
        noecho();

        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_RED);


        init_win_params(&vec_Player[0]);
        print_win_params(&vec_Player[0]);


        init_win_params(&vec_Player[1]);
        print_win_params(&vec_Player[1]);


        init_win_params(&vec_Player[2]);
        print_win_params(&vec_Player[2]);

        create_box(&vec_Player[0], TRUE);
        create_box(&vec_Player[1], TRUE);
        create_box(&vec_Player[2], TRUE);

        while( (ch = getch()) !=  KEY_F(1)){

          /*for (int i = 0; i < 3; ++i){
            //if (currentPlayer != i){
              create_box(&vec_Player[i], TRUE);
            //}
          }*/

          create_box(&vec_Player[0], TRUE);
          create_box(&vec_Player[1], TRUE);
          create_box(&vec_Player[2], TRUE);


          switch (ch) {
            case KEY_LEFT:
                //create_box(&vec_Player[currentPlayer-1], FALSE);
                //--vec_Player[currentPlayer-1].startx;

                write(cnFD, header1, 4);
                write(cnFD, "l", 1);

                //create_box(&vec_Player[currentPlayer-1], TRUE);
                break;

            case KEY_RIGHT:
                //create_box(&vec_Player[currentPlayer-1], FALSE);
                //++vec_Player[currentPlayer-1].startx;

                write(cnFD, header1, 4);
                write(cnFD, "r", 1);

                //create_box(&vec_Player[currentPlayer-1], TRUE);
                break;

            case KEY_UP:
                //create_box(&vec_Player[currentPlayer-1], FALSE);
                //--vec_Player[currentPlayer-1].starty;

                write(cnFD, header1, 4);
                write(cnFD, "u", 1);

                //create_box(&vec_Player[currentPlayer-1], TRUE);
                break;

            case KEY_DOWN:
                //create_box(&vec_Player[currentPlayer-1], FALSE);
                //++vec_Player[currentPlayer-1].starty;

                write(cnFD, header1, 4);
                write(cnFD, "d", 1);

                //create_box(&vec_Player[currentPlayer-1], TRUE);
                break;

            case 99: ///c // der
                //init_bullet_params(&vec_Player[currentPlayer-1], &vec_Bullet[currentPlayer-1], 0);

                sprintf(header1, "%04d", der);

                write(cnFD, header1, 4);
                write(cnFD, "s", 1);

                //write(cnFD, header1, 4);
                //write(cnFD, "c", 1);
                //write(cnFD, "s", 1);

                //Shoot(&vec_Bullet[currentPlayer-1],0);

                break;

            case 115: ///s // arrriba
                //init_bullet_params(&vec_Player[currentPlayer-1], &vec_Bullet[currentPlayer-1], 1);
                sprintf(header1, "%04d", arr);

                write(cnFD, header1, 4);
                write(cnFD, "s", 1);
                //write(cnFD, header1, 4);
                //write(cnFD, "s", 1);

                //Shoot(&vec_Bullet[currentPlayer-1],1);
                break;

            case 122:///z // izquierda
                //init_bullet_params(&vec_Player[currentPlayer-1], &vec_Bullet[currentPlayer-1], 2);

                sprintf(header1, "%04d", izq);

                write(cnFD, header1, 4);
                write(cnFD, "s", 1);
                //write(cnFD, header1, 4);
                //write(cnFD, "z", 1);
                //write(cnFD, "s", 1);

                //Shoot(&vec_Bullet[currentPlayer-1],2);
                break;

            case 120:///x  // abajo
                //init_bullet_params(&vec_Player[currentPlayer-1], &vec_Bullet[currentPlayer-1], 3);

                sprintf(header1, "%04d", aba);

                write(cnFD, header1, 4);
                write(cnFD, "s", 1);
                //write(cnFD, header1, 4);
                //write(cnFD, "x", 1);

                //Shoot(&vec_Bullet[currentPlayer-1],3);
                break;
          }
        }

        endwin();

        break;
      }
    }
  }while(1);
}

  int getSizeOfFile(char* filename){
    FILE *file = fopen(filename, "r");

    if(file == NULL)
    {
        printf("ERROR: File %s not found.\n", filename);
        //exit(1);
    }else{
      fseek(file, 0, SEEK_END);
      return ftell(file);
    }
    return 0;
  }


  void menuOptions(){

    cout << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "              BIENVENIDO AL SERVIDOR              " << endl;
    cout << "-------------------------------------------------" << endl;
    cout << endl;
    cout << " ---------------------------- " << endl;
    cout << "| Menu de Opciones:          |" << endl;
    cout << "|    [1] Ingresar al Chat.   |" << endl;
    cout << "|    [2] Chatear.            |" << endl;
    cout << "|    [3] Imprimir Usuarios.  |" << endl;
    cout << "|    [4] Enviar Archivo.     |" << endl;
    cout << "|    [5] Jugar.              |" << endl;
    cout << "|    [0] Salir del Chat.     |" << endl;
    cout << " ---------------------------- " << endl;
    cout << endl;
    cout << "Escoger Opcion : ";
  }

  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;

    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    //Res = inet_pton(AF_INET, "192.150.0.105", &stSockAddr.sin_addr);
    // Res = inet_pton(AF_INET, "192.150.0.104", &stSockAddr.sin_addr);
    //Res = inet_pton(AF_INET, "192.168.1.36", &stSockAddr.sin_addr);
    Res = inet_pton(AF_INET, "192.168.1.164", &stSockAddr.sin_addr);
    //Res = inet_pton(AF_INET, "192.168.1.23", &stSockAddr.sin_addr);
    // Res = inet_pton(AF_INET, "192.168.1.242", &stSockAddr.sin_addr);

    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }else if (0 == Res){
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))){
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    cout << "Conectado" << endl;

    char buffer[256];
    bzero(buffer,256);

    n = read(SocketFD, buffer, 255);
    if (n < 0) perror("ERROR reading from socket");
    printf("MSG: [%s]\n",buffer);

    menuOptions();

    thread rthread(myRead, SocketFD);
    thread wthread(myWrite, SocketFD);
    wthread.join();
    rthread.join();


    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);

    return 0;
  }
