//#include <iostream>
#include<sstream>
//#include <string.h>
using namespace std;
int main()
{
  system("cls");
  //cout << "Enter a string : ";
  char string[30];
  //getline(string);     //input the string
   istringstream iss(string);     //object 'iss' created
   while (iss)
   {
     char S[15];
     iss >> S;       
     //cout << S << endl;     //The token is displayed
   }
   return 0;
}