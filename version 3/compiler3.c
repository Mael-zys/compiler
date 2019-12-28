#include"lex2.h"
#include"syn3.h"
#include"sem3.h"
#include"compiler3.h"

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
    arb Arbre = (struct arbre*)malloc(sizeof(struct arbre));
    //si on ne peut pas ouvrir le fichier, on ne peut rien faire.
    if((fp=fopen(argv[1],"r"))==NULL)
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
        {
            printf("0 warning, 0 erreur\n");
            compiler(Arbre, argv[1]);
        }
    }
    return 0;
}