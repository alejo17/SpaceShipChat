  #include <iostream>
  #include <string.h>

  using namespace std;

void buscarComas (string cadena, char caracter)
{
    for (int i=0;i<cadena.size();i++)
    {

          if ( cadena [i] == caracter )
          {
            cout << i << ' ' ;
          }
          // i++;

    }
}

int main(){

  buscarComas("alejo,froni,jozeya",',');

}
