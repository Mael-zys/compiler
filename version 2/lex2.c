#include"lex2.h"

int main(int argc, char* argv[])
{
    if(argc!=2) 
    {
        printf("erreur, manque ou trop d'arguments\n");
        return 0;
    }
    char buf[maxLine];
    FILE *fp;
    int i=0;
    lexeme lex[N][N];
    //si on ne peut pas ouvrir le fichier, on ne peut rien faire.
    if((fp=fopen(argv[1],"r"))==NULL)
    {
        printf("On ne peut pas ouvrir le fichier.");
        exit(1);
    }
    //lire le fichier ligne par ligne
    while(fgets(buf,maxLine,fp)!=NULL)
    {
        if(buf[0]=='\n'|| buf[0]=='\r') continue; //si la ligne est vide, on continue
        else {  classer(lex,i++,buf);}
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