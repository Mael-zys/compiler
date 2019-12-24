#include"lex1.h"

#define text "Test.txt"

int main()
{
    char buf[maxLine];
    FILE *fp;
    int i=0;
    lexeme lex[N][N];
    //si on ne peut pas ouvrir le fichier, on ne peut rien faire.
    if((fp=fopen(text,"r"))==NULL)
    {
        printf("On ne peut pas ouvrir le fichier.\n");
        exit(1);
    }
    //lire le fichier ligne par ligne
    while(fgets(buf,maxLine,fp)!=NULL)
    {
        if(buf[0]=='\n') continue; //si la ligne est vide, on continue
        classer(lex,i++,buf);
    }
    int high=i;
    i=0;
    while(i<high && i<N &&lex[i][0].valeur!=NULL)
    {
        int j=0;
        while(lex[i][j].type!=Fin)
        {
            printf("%s\t",lex[i][j].valeur);
            j++;
        }
        printf("\n");
        i++;
    }
    return 0;
}