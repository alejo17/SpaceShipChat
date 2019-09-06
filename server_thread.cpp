#include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <string>
  #include <sstream>
  #include <iostream>
#include <thread>
#include <map>
#include <vector>

using namespace std;

std::map <string, int> listOfUsers;
std::map <string, int> listOfPlayers;
string login;

int currentPlayer = 0;
int current_CN;

string findNickname(int cnFD){
  for (std::map<string,int>::iterator it=listOfUsers.begin(); it!=listOfUsers.end(); ++it){
      //std::cout << it->first << " => " << it->second << '\n';
      if (it -> second == cnFD){
        return it->first;
      }
  }
  return "NULL";
}

string findPlayer(int cnFD){
  for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){
      //std::cout << it->first << " => " << it->second << '\n';
      if (it -> second == cnFD){
        return it->first;
      }
  }
  return "NULL";
}

void myRead(int cnFD){
  int n, action, sizeOfSendMsg;
  char buffer[256], len[4], sendMsg[256], header1[4], header2[1], header11[4], sendMsgTotal[256];

  do{

    /*bzero(len,4);
    n = read(cnFD, len, 4);
    len[4] = '\0';
    cout << len << "-------";
    int mySize = atoi(len);

    bzero(buffer,256);
    n = read(cnFD, buffer, mySize);
    printf("Here is the message: [%s]\n",buffer);*/

    bzero(header1, 4);
    read(cnFD, header1, 4);
    header1[4] = '\0';

    bzero(header2,1);
    read(cnFD, header2, 1);
    header2[1] = '\0';

    //login
    if (header2[0] == 'L'){
        // lectura de nickName
        int sizeOfNickname, sizeOfMsg;
        char nickName[256];

        //bzero(header1, 4);
        //read(cnFD, header1, 4);

        cout << "header1 read1: " << header1 << endl;

        //header1[4] = '\0';

        sizeOfNickname = atoi(header1);

        n = read(cnFD, nickName, sizeOfNickname);
        nickName[sizeOfNickname] = '\0';

        // Ingresar al Listado
        string login;
        login = nickName;

        listOfUsers.insert(pair<string, int>(login, cnFD));

        // mensaje de Bienvenida
        bzero(sendMsg, sizeOfMsg);
        sizeOfMsg = sprintf(sendMsg, "Bienvenido al Chat: [%s]", login.c_str());

        cout << "mensaje a enviar: " << sendMsg << endl;


        bzero(header1, 4);
        sprintf(header1, "%04d", sizeOfMsg);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "R", 1);
        write(cnFD, sendMsg, sizeOfMsg);
    }

    if (header2[0] == 'P'){
        string list;
        int sizeOfList;
        cout << "entro al print" << endl << endl;
        //read(cnFD, header1, 4);

        list =  "-------------------------------------\n";
        list += "    Lista de Usuarios Conectados     \n";
        list += "-------------------------------------\n";

        for (std::map<string,int>::iterator it=listOfUsers.begin(); it!=listOfUsers.end(); ++it){
          list += " - " + it->first + "\n";
          std::cout << it->first << " => " << it->second << '\n';
        }
        sizeOfList = list.size();

        bzero(header1, 4);
        sprintf(header1, "%04d", sizeOfList);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "R", 1);
        write(cnFD, list.c_str(), sizeOfList);
    }

    if (header2[0] == 'C'){
        cout << "entro a chatear" <<endl;
        // lectura de nickName
        int sizeOfNickname, sizeOfMensaje, sizeOfFeedback, sizeTotal;
        char nickName[256];
        char Mensaje[256];

        //bzero(header1, 4);
        //read(cnFD, header1, 4);
        //header1[4] = '\0';
        cout << "tamano del nickName: " << header1 << endl;
        sizeOfNickname = atoi(header1);
        n = read(cnFD, nickName, sizeOfNickname);
        nickName[sizeOfNickname] = '\0';  

        bzero(header11, 4);
        n = read(cnFD, header11, 4);
        header11[4] = '\0';
        cout << "tamano del mensaje: " << header11 << endl;
        sizeOfMensaje = atoi(header11);
        n = read(cnFD, Mensaje, sizeOfMensaje);
        Mensaje[sizeOfMensaje] = '\0';

        string nick = "", mensa = "";
        nick = nickName;

        bzero(sendMsg, 256);
        sizeOfFeedback = sprintf(sendMsg, "estas chateando con : [%s]", nick.c_str());
        cout << "mensaje a enviar: " << sendMsg << endl;

        mensa = Mensaje;
        cout << "mensaje q escribiste" << mensa << endl;

        bzero(header1, 4);
        sprintf(header1, "%04d", sizeOfFeedback);
        header1[4] = '\0';

        write(cnFD, header1, 4);
        write(cnFD, "T", 1);
        write(cnFD, sendMsg, sizeOfFeedback);


//------------------------ENVIAR MENSAJE AL CLIENTE-----------------
        bzero(sendMsgTotal, 256);
        sizeTotal = sprintf(sendMsgTotal, "el usuario : [%s]  te envia el mensaje [%s] ",findNickname(cnFD).c_str(), mensa.c_str());
        cout << "mensaje a enviar: " << sendMsgTotal << endl;
        int toSocket;
        toSocket = listOfUsers[nick];

        cout<< "el socket a enviar: "<<toSocket<<endl;

        bzero(header1, 4);
        sprintf(header1, "%04d", sizeTotal);
        header1[4] = '\0';

        write(toSocket, header1, 4);
        write(toSocket, "R", 1);
        write(toSocket, sendMsgTotal, sizeTotal);
//------------------------ENVIAR MENSAJE AL CLIENTE-----------------
    }

    if (header2[0] == 'F'){
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

        string tFilename, tName, tExtension;

        tFilename = bufFilename;
        tName = tFilename.substr(0,tFilename.size()-4);
        tExtension = tFilename.substr(tFilename.size()-4, tFilename.size());

        tFilename = tName + "_from_" + findNickname(cnFD) + tExtension;

        strcpy(bufNewFilename, tFilename.c_str());
        bufNewFilename[tFilename.size()] = '\0';

        cout << "---------------------------------" << endl;
        cout << "Detalles del envio de archivos:  " << endl;
        cout << "---------------------------------" << endl;
        cout << "Usuario de Origen: " << findNickname(cnFD) << endl;;
        cout << "Usuario de Destino: " << bufNickname << endl;
        cout << "Nombre del Archivo: " << bufFilename << endl;
        cout << "Nuevo nombre del Archivo: " << bufNewFilename << endl;
        cout << "Tamaño del archivo: " << v.size() << endl;
        cout << "---------------------------------" << endl;

        int toSocket;
        string nick = bufNickname;
        string msg_send;
        toSocket = listOfUsers[nick];

        sizeOfNickname = findNickname(cnFD).size(); // nickname de quien envia
        sprintf (sizeOfNick, "%02d", sizeOfNickname);
        strcpy(bufNickname, findNickname(cnFD).c_str());
        bufNickname[sizeOfNickname] = '\0';

        sizeOfFilename = tFilename.size();
        sprintf(header1, "%04d", sizeOfFilename);
        header1[4] = '\0';

        write (toSocket, header1, 4);
        write (toSocket, "D", 1);
        write (toSocket, sizeOfNick, 2);
        write (toSocket, bufNickname, sizeOfNickname);
        write (toSocket, bufNewFilename, sizeOfFilename);
        write (toSocket, bufSizeOfFile, 4);

        for (int i = 0; i < v.size(); ++i){
            msg_send = v[i];
            write (toSocket, msg_send.c_str(), 1);
        }

        sizeOfSendMsg = sprintf (sendMsg, "El usuario : [%s]  te a enviado el archivo [%s]", bufNickname, bufNewFilename);
        sendMsg[sizeOfSendMsg] = '\0';

        bzero(header1, 4);
        sprintf(header1, "%04d", sizeOfSendMsg);
        header1[4] = '\0';


        write (toSocket, header1, 4);
        write (toSocket, "R", 1);
        write (toSocket, sendMsg, sizeOfSendMsg);
    }

    if(header2[0] == 'E'){

       string firstKey;

       firstKey = findNickname(cnFD);
       listOfUsers.erase(firstKey);

       bzero(header1, 4);
       sprintf(header1, "%04d", 19);
       header1[4] = '\0';

       write (cnFD, header1, 4);
       write (cnFD, "R", 1);
       write (cnFD, "Haz salido del Chat.", 19);


       // mensaje enviado a los clientes activos en el chat. //

       sizeOfSendMsg = sprintf (sendMsg, "El usuario : [%s]  se desconectó.", firstKey.c_str());
       sendMsg[sizeOfSendMsg] = '\0';

       bzero(header1, 4);
       sprintf(header1, "%04d", sizeOfSendMsg);
       header1[4] = '\0';

      for (std::map<string,int>::iterator it=listOfUsers.begin(); it!=listOfUsers.end(); ++it){

            write (it -> second, header1, 4);
            write (it -> second, "R", 1);
            write (it -> second, sendMsg, sizeOfSendMsg);

      }

    }

    if(header2[0] == 'G'){
      string login = findNickname(cnFD);
      listOfPlayers.insert(pair<string, int>(login, ++currentPlayer));

      sprintf(header1, "%04d", currentPlayer);
      write(cnFD, header1, 4);
      write(cnFD, "Z", 1);
      cout << "Jugador = " << currentPlayer << endl;
    }

    if(header2[0] == 'l'){

      string login = findNickname(cnFD);
      int player = listOfPlayers[login];

      sprintf(sendMsg,"%04d",player);

      for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){

            write (listOfUsers[it -> first], header1, 4);
            write (listOfUsers[it -> first], "l", 1);
            write (listOfUsers[it -> first], sendMsg, 4);
      }

    }

    if(header2[0] == 'r'){

      string login = findNickname(cnFD);
      int player = listOfPlayers[login];

      sprintf(sendMsg,"%04d",player);

      for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){

            write (listOfUsers[it -> first], header1, 4);
            write (listOfUsers[it -> first], "r", 1);
            write (listOfUsers[it -> first], sendMsg, 4);
      }

    }

    if(header2[0] == 'u'){

      string login = findNickname(cnFD);
      int player = listOfPlayers[login];

      sprintf(sendMsg,"%04d",player);

      for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){

            write (listOfUsers[it -> first], header1, 4);
            write (listOfUsers[it -> first], "u", 1);
            write (listOfUsers[it -> first], sendMsg, 4);

      }

    }

    if(header2[0] == 'd'){

      string login = findNickname(cnFD);
      int player = listOfPlayers[login];

      sprintf(sendMsg,"%04d",player);

      for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){

            write (listOfUsers[it -> first], header1, 4);
            write (listOfUsers[it -> first], "d", 1);
            write (listOfUsers[it -> first], sendMsg, 4);

      }

    }

      if(header2[0] == 's'){
        string login = findNickname(cnFD);
        int player = listOfPlayers[login];

        sprintf(sendMsg,"%04d",player);

        for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){

              write (listOfUsers[it -> first], header1, 4);
              write (listOfUsers[it -> first], "s", 1);
              write (listOfUsers[it -> first], sendMsg, 4);

        }
      }

      if (header2[0] == '#'){
          // cout << "entro a colisiones" << endl;
          for (std::map<string,int>::iterator it=listOfPlayers.begin(); it!=listOfPlayers.end(); ++it){

              write (listOfUsers[it -> first], header1, 4);
              write (listOfUsers[it -> first], "#", 1);
          }

      }

  }while(1);

}

void myWrite(int cnFD){
  int n;
  char buffer[256], len[4], header1[4], header2[1];
  string cad;

  do{

    bzero(buffer,256);

    cin.getline(buffer, 255);

    cad = buffer;
    n = cad.size();

    sprintf (len, "%04d", n);
    len[4] = '\0';

    n = write(cnFD, len, 4);
    n = write(cnFD, cad.c_str(), cad.size());

  }while(1);
}

void acceptConnections(int SD){
  int n,nn;
  char buffer[256];
  string welcome;

  vector<thread> threads;

  while(1){
    int ConnectFD = accept(SD, NULL, NULL);

    if(0 > ConnectFD){
        perror("error accept failed");
        close(SD);
        exit(EXIT_FAILURE);
    }

    current_CN++;

    bzero(buffer,256);

    welcome = "Welcome!!!, Connection: " + to_string(ConnectFD);
    nn = welcome.size();

    n = write(ConnectFD,welcome.c_str(),nn);
    if (n < 0) perror("ERROR writing to socket");

    threads.push_back(thread(myRead, ConnectFD));
    //threads[current_CN].join();
    //for (auto& th : threads) th.join();

    //thread rthread(myRead, ConnectFD);
    //rthread.join();
  }
}


  int main(void){

    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;

    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))){
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    if(-1 == listen(SocketFD, 10) ){
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    /*for(;;){
      int ConnectFD = accept(SocketFD, NULL, NULL);

      if(0 > ConnectFD){
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }


      //bzero(buffer,256);

      //n = write(ConnectFD,"welcome User",18);
      //if (n < 0) perror("ERROR writing to socket");


      thread cnThread(acceptConnections);
      //thread rthread(myRead, ConnectFD);
      //thread wthread(myWrite, ConnectFD);
      //rthread.join();
      //wthread.join();

      shutdown(ConnectFD, SHUT_RDWR);

      close(ConnectFD);
    }*/

    //acceptConnections(SocketFD);
    thread cnThread(acceptConnections, SocketFD);
    cnThread.join();

    while(1){

    }

    close(SocketFD);
    return 0;
  }
