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
    Mark,
} lexeme_t;

// Definition du type lexeme
typedef struct lexeme
{
    lexeme_t type;  // Son type
    char valeur[10];// Sa valeur, limit¨¦e ¨¤ 20 caract¨¨res
    int index;      // l'index de cette lexeme, c'est pour sorter
    int fi;
} lexeme;

typedef struct Pile
{
    int (*pop)(int);
    int (*push)(int);
    int (*vide)(void);
} Pile;

int pile[100];
int sp=0;

int pop(int i)
    {
        if(sp==0) {printf("Pile est deja vide, on ne peut pas pop\n"); return 0;}
        if(pile[sp-1]==i) {sp--; return 1;}
        else {printf("Mauvais pop, le chose que l'on veut pop n'est pas dans le tete de pile\n");return 0;}
    }
int push(int i)
    {
        if(sp==50) {printf("Pile est deja plein, on ne peut pas push\n");return 0;}
        pile[sp]=i;
        sp++;
        return 1;
    }
int vide()
    {
        return sp==0;
    }

void syntaxique(lexeme lex[][N], int i);

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
        printf("On ne peut pas ouvrir le fichier.");
        exit(1);
    }
    //lire le fichier ligne par ligne
    while(fgets(buf,maxLine,fp)!=NULL)
    {
        if(buf[0]=='\n' || buf[0]=='\r') continue; //si la ligne est vide, on continue
        classer(lex,i++,buf);
    }
    syntaxique(lex,i);
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
    findd(p,"[^{}\'\[0-9a-zA-Z\u4e00-\u9fa5(),`=\"*/→\n]",lex,i,&j,Autre);

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
    if(j>1 && len==1 && lex[i][j-1].valeur[0]!='}' && lex[i][j-1].valeur[0]!='*' && lex[i][j-1].valeur[0]!=']' &&lex[i][j-1].valeur[0]!='/')lex[i][j-1].fi--;
    if(lex[i][j-1].fi<lex[i][j-1].index) j--;

    //imprimer pour verifer le resultat
    for(int k=0;k<j && lex[i][k].valeur[0]!='\n';k++)
        printf("%s\t",lex[i][k].valeur);
    printf("\n");
    //marquer la fin de cette ligne
    lex[i][j].type=Fin;
    lex[i][j+1].type=Mark;
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
                lex[i][*j].index=offset+debut;
                lex[i][*j].fi=fin+offset-1;
                //set son type
                lex[i][*j].type=typ;
                int ind=0;
                //set les valeurs
                for(char* k=p+debut  ;k<p+fin;k++)
                {
                    if(*(p+debut)==temp || *(k)==temp1) continue; //si le char est " ", on continue
                    if(typ==Autre && *(k)==t ) continue;  // pour filtrer ] dans le cas autre
                    //set les valeurs
                    lex[i][*j].valeur[ind++]=*k;
                }
                p += fin;
                offset += fin;
                if(ind==0) continue;  //si il n'y a rien dans cette lexeme
                *j=*j+1;

            }
        while (res == 0);
        regfree(&myregexp);
        return;
}

void error(int e, char* p)
{
    printf("error syntaxique a l'etat %d, a lexeme %s\n",e,p);
}


void syntaxique(lexeme lex[][N],int high)
{
    int e=0;
    int i=0;
    Pile p;
    p.pop=pop;
    p.push=push;
    p.vide=vide;
    int i_com,j_com;
    while(i<high && i<N &&lex[i][0].valeur!=NULL)
    {
        int j=0;
        while(lex[i][j].type!=Mark)
        {
            switch(e)
            {
                case 0:
                    switch(lex[i][j].type)
                    {
                        case Com:
                            lex[i][j].type=Fin;
                            lex[i][j+1].type=Mark;
                            break;
                        case Com_ou:
                            i_com=i;
                            j_com=j;
                            if(!p.push(1)){error(e,lex[i][j].valeur); return;}
                            e=1;
                            break;
                    }break;
                case 1:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                            e=0;
                            if(i==i_com)
                            {
                                int k=j_com;
                                while(lex[i][j].type!=Mark)
                                {
                                    lex[i][k++]=lex[i][j+1];
                                    j++;
                                }
                                lex[i][j].type=Mark;
                                j=j_com-1;
                            }
                            else
                            {
                                int dif=i-i_com-1;
                                lex[i_com][j_com].type=Fin;
                                lex[i_com][j_com+1].type=Mark;
                                int k=0;
                                while(lex[i][j].type!=Mark)
                                {
                                    lex[i][k++]=lex[i][j+1];
                                    j++;
                                }
                                lex[i][j].type=Mark;
                                j=0;

                                while(i<high && i<N &&lex[i][0].valeur!=NULL)
                                    while(lex[i][j].type!=Mark)
                                        lex[i-dif][j]=lex[i][j];
                                i=i-dif;
                            }
                            break;
                    }break;
            }
            j++;
        }
        i++;
    }

    if(p.vide()!=1 || e!=0) printf("sytaxique error\n");
    else
    {
        e=i=sp=0;
        while(i<high && i<N &&lex[i][0].valeur!=NULL)
        {
            int j=0;
            while(lex[i][j].type!=Mark)
            {
                printf("%s , etat: %d (%d,%d)\n",lex[i][j].valeur,e,i,j);
                switch(e)
                {
                    case 0:
                        switch(lex[i][j].type)
                        {
                            case Automate:
                                e=1;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 1:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return;}
                                e=2;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 2:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=3;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 3:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=4;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 4:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=5;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 5:
                        switch(lex[i][j].type)
                        {
                            case ACC_ou:
                                if(!p.push(3)){error(e,lex[i][j].valeur); return;}
                                e=6;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 6:
                        switch(lex[i][j].type)
                        {
                            case etats:
                                e=7;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 7:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=8;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 8:
                        switch(lex[i][j].type)
                        {
                            case CRO_ou:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                                e=9;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 9:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return;}
                                e=10;
                                break;
                            case APO_dou:
                                if(!p.push(5)){error(e,lex[i][j].valeur); return;}
                                e=10;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 10:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=11;
                                break;
                            case MOT:
                                e=11;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 11:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return;}
                                e=12;
                                break;
                            case APO_dou:
                                if(!p.pop(5)){error(e,lex[i][j].valeur); return;}
                                e=12;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 12:
                        switch(lex[i][j].type)
                        {
                            case CRO_fe:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                                e=14;
                                break;
                            case Virgu:
                                e=13;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 13:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return;}
                                e=10;
                                break;
                            case APO_dou:
                                if(!p.push(5)){error(e,lex[i][j].valeur); return;}
                                e=10;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 14:
                        switch(lex[i][j].type)
                        {
                            case initial:
                                e=15;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 15:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=16;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 16:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=17;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 17:
                        switch(lex[i][j].type)
                        {
                            case fina:
                                e=18;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 18:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=19;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 19:
                        switch(lex[i][j].type)
                        {
                            case CRO_ou:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                                e=20;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 20:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=21;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 21:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=22;
                                break;
                            case CRO_fe:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                                e=23;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 22:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=21;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 23:
                        switch(lex[i][j].type)
                        {
                            case transitions:
                                e=24;
                                break;
			    case Fin:
				break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 24:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=25;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 25:
                        switch(lex[i][j].type)
                        {
                            case CRO_ou:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                                e=26;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 26:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return;}
                                e=27;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 27:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=28;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 28:
                        switch(lex[i][j].type)
                        {
                            case Flesh:
                                e=29;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 29:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=30;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 30:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=31;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 31:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return;}
                                e=32;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 32:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=33;
                                break;
			    case MOT:
				e=33;
				break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 33:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return;}
                                e=34;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 34:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=49;
                                break;
                            case Virgu:
                                e=35;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 35:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return;}
                                e=36;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 36:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return;}
                                e=37;
                                break;
                            case Flesh:
                                e=42;
                                break;
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=47;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 37:
                        switch(lex[i][j].type)
                        {
                            case MOT:
                                e=38;
                                break;
                            case NOMBRE:
                                e=38;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 38:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return;}
                                e=39;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 39:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=40;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 40:
                        switch(lex[i][j].type)
                        {
                            case Flesh:
                                e=41;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 41:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=47;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 42:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=43;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 43:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return;}
                                e=44;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 44:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=45;
                                break;
                            case MOT:
                                e=45;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 45:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return;}
                                e=46;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 46:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=47;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 47:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=49;
                                break;
                            case Virgu:
                                e=35;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 48:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return;}
                                e=27;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 49:
                        switch(lex[i][j].type)
                        {
                            case CRO_fe:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                                e=50;
                                break;
                            case Virgu:
                                e=48;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 50:
                        switch(lex[i][j].type)
                        {
                            case ACC_fe:
                                if(!p.pop(3)){error(e,lex[i][j].valeur); return;}
                                e=51;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 51:
                        switch(lex[i][j].type)
                        {
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;

                }
                j++;
            }
            i++;
        }
        if(p.vide()==1 && e==51) printf("sytaxique correct\n");
        else printf("sytaxique error\n");
        return;
    }

}
