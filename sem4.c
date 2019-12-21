#include"lex1.h"
#include"syn1.h"
#include"sem1.h"

#define text "test.txt"

int main()
{
    char buf[maxLine];
    FILE *fp;
    int i=0;
    lexeme lex[N][N];
    arb Arbre = (struct arbre*)malloc(sizeof(struct arbre));
    //si on ne peut pas ouvrir le fichier, on ne peut rien faire.
    if((fp=fopen(text,"r"))==NULL)
    {
        printf("On ne peut pas ouvrir le fichier.");
        exit(1);
    }
    //lire le fichier ligne par ligne
    while(fgets(buf,maxLine,fp)!=NULL)
    {
        if(buf[0]=='\n' || buf[0]=='\r') continue; //si la ligne est vide, on continue
        classer(lex,i++,buf);
    }
    if(syntaxique(lex,i,Arbre))
    {
        if(semantique(Arbre))
            printf("correct semantique\n");
    }
    return 0;
}