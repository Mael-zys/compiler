#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <regex.h>

#define text "test.txt"
#define maxLine 1024  //nombre max de char lu par ligne
#define N 100         //nombre max de lexeme chaque ligne

//type de lexeme
typedef enum lexeme_t
{
    NOMBRE,     // nombre
    Automate,
    etats,
    initial,
    fina,       // final
    transitions,
    MOT,        // mot(les mots francais, anglais, chinois)
    Par_ou,     // (
    Par_fe,     // )
    APO,          // `
    APO_dou,      // "
    CRO_ou,          // [
    CRO_fe,          // ]
    ACC_ou,          // {
    ACC_fe,          // }
    Com_ou,             //   /*
    Com_fe,          //   */
    Com,              //   //
    Flesh,           //   ->
    Equ,             //  =
    Virgu,           //  ,
    Autre,           // les autres
    Fin,             // c'est pour marquer la fin de ligne
} lexeme_t;

// Definition du type lexeme
typedef struct lexeme
{
    lexeme_t type;  // Son type
    char valeur[10];// Sa valeur, limit¨¦e ¨¤ 20 caract¨¨res
    int index;      // l'index de cette lexeme, c'est pour sorter
    int fi;
} lexeme;

//trouver une expression rationnel et le met dans notre lexeme[N][N]
void findd(char* , char *, lexeme lex[][N],int , int *,lexeme_t );

//Trouver, classer et sorter tous les lexemes
void classer(lexeme lex[][N],int , char *);



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
    return 0;
}


void classer(lexeme lex[][N],int i, char *p)
{
    int j=0; // j est l'index de lexeme de cette ligne
    //trouver les lexemes suivantes et stocker dans lex[][N]
    findd(p,"([0-9]|[1-9][0-9]+)",lex,i,&j,NOMBRE);
    findd(p,"Automate",lex,i,&j,Automate);
    findd(p,"etats",lex,i,&j,etats);
    findd(p,"initial",lex,i,&j,initial);
    findd(p,"final",lex,i,&j,fina);
    findd(p,"transitions",lex,i,&j,transitions);
    findd(p,"→",lex,i,&j,Flesh);
    findd(p,"[\u4e00-\u9fa5\'a-zA-Z]+",lex,i,&j,MOT);
    findd(p,"[(]",lex,i,&j,Par_ou);
    findd(p,"[)]",lex,i,&j,Par_fe);
    findd(p,"[`]",lex,i,&j,APO);
    findd(p,"[\"]",lex,i,&j,APO_dou);
    findd(p,"[\[]",lex,i,&j,CRO_ou);
    findd(p,"[]]",lex,i,&j,CRO_fe);
    findd(p,"[{]",lex,i,&j,ACC_ou);
    findd(p,"[}]",lex,i,&j,ACC_fe);
    findd(p,"[/][*]",lex,i,&j,Com_ou);
    findd(p,"[*][/]",lex,i,&j,Com_fe);
    findd(p,"[/]{2}",lex,i,&j,Com);
    findd(p,"[=]",lex,i,&j,Equ);
    findd(p,"[,]",lex,i,&j,Virgu);
    findd(p,"[*]",lex,i,&j,Autre); //c'est pour le cas *, dans la fonction findd, je vais separer * et */, /*
    findd(p,"[/]",lex,i,&j,Autre); //c'est pour le cas seulment un / ou ///

    // les autres, parceque dans les commentaires, on peut mettre n'importe quoi.
    // je ne sais pourquoi je ne peux pas filter [ et ] en meme temps, si je mets les 2, il ne filte rien, donc je filtre seulment les lexemes precendantes sauf ], et je vais filtre ] dans la fonction findd
    findd(p,"[^{}\'\[0-9a-zA-Z\u4e00-\u9fa5(),`=\"*/→]",lex,i,&j,Autre);

    //sort les lexeme par son index
    for(int k=0;k<j;k++)
    {
        for(int l=0;l<j-1-k;l++)
        {
            if(lex[i][l].index>lex[i][l+1].index)
            {
                lexeme temp=lex[i][l];
                lex[i][l]=lex[i][l+1];
                lex[i][l+1]=temp;
            }
            //le cas ou on aurait trouve un lexeme comme Automate, et puis dans lexeme Mot, on le trouve aussi, donc il faut enlever un.
            else if(lex[i][l].index==lex[i][l+1].index)
            {
                for(int ii=l+2;ii<j;ii++)
                    lex[i][ii-1]=lex[i][ii];
                j--;
            }
        }
    }


    //c'est pour le cas /// ***   *//  //*  /*/  **/  /** */*
    for(int l=0;l<j;l++)
    {
        //par example, si on a //*, on trouve // dans lexeme Com, et /* dans Com_ou, mais c'est pour, il faut enlever /*
        if(lex[i][l].index+1==lex[i][l+1].index && (lex[i][l].type==Com_ou || lex[i][l].type==Com_fe || lex[i][l].type==Com )&& (lex[i][l+1].type==Com_ou || lex[i][l+1].type==Com_fe || lex[i][l+1].type==Com ) )
            {
                for(int ii=l+2;ii<j;ii++)
                    lex[i][ii-1]=lex[i][ii];
                j--;
            }
        //par example, si on a //*, on trouve // dans lexeme Com, et /  /  * dans lexeme xie et fois, donc il faut enlever les 2 premiers, en effet, le premier / est deja ete enleve par le loop de ligne 119
        if(lex[i][l].index+1==lex[i][l+1].index && (lex[i][l].type==Com_ou || lex[i][l].type==Com_fe || lex[i][l].type==Com )&& lex[i][l+1].type==Autre && (lex[i][l+1].valeur[0]=='/' || lex[i][l+1].valeur[0]=='*'))
            {
                for(int ii=l+2;ii<j;ii++)
                    lex[i][ii-1]=lex[i][ii];
                j--;
            }

    }

    //dans lexeme Autre, il compte aussi '\r', donc il faut l'enlever
    int len=strlen(lex[i][j-1].valeur);
    if(j>1 && len==1 && lex[i][j-1].valeur[0]!='}' && lex[i][j-1].valeur[0]!='*' && lex[i][j-1].valeur[0]!=']' && lex[i][j-1].valeur[0]!='/')lex[i][j-1].fi--;
    if(lex[i][j-1].fi<lex[i][j-1].index) j--;


    //imprimer pour verifer le resultat
    for(int k=0; k<j && lex[i][k].valeur[0]!='\n';k++)
        {
            printf("%s\t",lex[i][k].valeur);
        }
    printf("\n");
    //marquer la fin de cette ligne
    lex[i][j].type=Fin;
    return;
}



//trouver l'expression rationnel et le met dans notre lexeme[N][N]
void findd(char* p, char *expr, lexeme lex[][N],int i, int *j,lexeme_t typ)
{
        regex_t myregexp;
        //c'est pour verifier si mon definition de l'expression rationnel est bonne, pour debuger
        if (regcomp(&myregexp, expr, REG_EXTENDED|REG_NEWLINE))
            {
                printf("Mauvaise expression rationnelle : %s\n", expr);
                regfree(&myregexp);
                return;
            };
        regmatch_t pmatch[100];
        int res = 0, debut, fin, offset = 0;
        //trouver lexeme
        do
            {
                // On filtre à partir de p
                res = regexec(&myregexp, p, 1, pmatch, 0);
                if (res) break; // regexec s'est planté ou n'a rien filtré
                // On récupère les informations
                debut = pmatch[0].rm_so;
                fin = pmatch[0].rm_eo;
                char temp=' ';  //pour filtrer les espaces
                char temp1='\t'; //pour filtrer les \t
                char t=']';      // pour filtre ] dans le cas autre
                char fois='*';   // pour filtre /* et */ dans le cas autre
                char xie='/';    // pour filtre //, /* et */ dans le cas autre
                //set l'index
                /*
                if(typ==Flesh)
                {
                    printf("find flesh %d %d\n",i,*j);
                    printf("%.*s (%d, %d)\n", fin-debut, p+debut, debut+offset, fin+offset-1);
                }*/
                lex[i][*j].index=offset+debut;
                lex[i][*j].fi=fin+offset-1;
                //set son type
                lex[i][*j].type=typ;

                int ind=0;
                //set les valeurs
                for(char* k=p+debut  ;k<p+fin;k++)
                {
                    if(*(k)==temp || *(k)==temp1) continue; //si le char est " ", on continue
                    if(typ==Autre && *(k)==t ) continue;  // pour filtrer ] dans le cas autre
                    //set les valeurs
                    lex[i][*j].valeur[ind++]=*k;
                }
                //if(typ==Flesh) fin--;
                p += fin;
                offset += fin;
                if(ind==0) continue;  //si il n'y a rien dans cette lexeme
                /*if(typ==Flesh)
                {
                    printf("find flesh ");
                    printf("%s %d %d\n", lex[i][*j].valeur,lex[i][*j].index,lex[i][*j].fi);
                }*/
                *j=(*j)+1;

            }
        while (res == 0);
        regfree(&myregexp);
        return;
}
