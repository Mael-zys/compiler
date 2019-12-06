#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <regex.h>


#define text "test.txt"
#define maxLine 1024  //nombre max de char lu par ligne
#define N 50         //nombre max de lexeme chaque ligne

//type de lexeme
typedef enum lexeme_t
{
    Automate,   //  Automate(0)={
    etats,       //  etats=["一","2", "3",`Init`]
    initial,     // initial=3
    fina,       // final=[0,1,2]
    transitions,  // transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]
    transitionsDe, // transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),
    transitionsMi,  //(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),
    transitionsFin,  // (3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]
    acc_fe,            // }
    Com_ou,          // /*
    Com_fe,           //  */
    Com,             //   //
    Autre,           // les autres
    Fin,             // c'est pour marquer la fin de ligne
    Mark,
} lexeme_t;

// Definition du type lexeme
typedef struct lexeme
{
    lexeme_t type;  // Son type
    char valeur[200];// Sa valeur, limit¨¦e ¨¤ 20 caract¨¨res
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

//trouver une expression rationnel et le met dans notre lexeme[N][N]
int findd(char* , char *, lexeme lex[][N],int , int *,lexeme_t );

//Trouver, classer et sorter tous les lexemes
void classer(lexeme lex[][N],int , char *);

void syntaxique(lexeme lex[][N], int i);

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
        if(buf[0]=='\n'|| buf[0]=='\r') continue; //si la ligne est vide, on continue
        else {  classer(lex,i++,buf);}
    }
    syntaxique(lex,i);
    return 0;
}


void classer(lexeme lex[][N],int i, char *p)
{
    int j=0; // j est l'index de lexeme de cette ligne
    findd(p,"Automate[(]([0-9]|[1-9][0-9]*)[)][ ]*=[ ]*[{]",lex,i,&j,Automate);
    findd(p,"etats[ ]*=[ ]*[\[][\"]([a-zA-Z0-9\u4e00-\u9fa5])+[\"]([,][ ]*[`]([a-zA-Z0-9\u4e00-\u9fa5])+[`]|[,][ ]*[\"]([a-zA-Z0-9\u4e00-\u9fa5])+[\"])*[]]",lex,i,&j,etats);
    findd(p,"initial[ ]*=[ ]*([0-9]|[1-9][0-9]+)",lex,i,&j,initial);
    findd(p,"final[ ]*=[ ]*[\[]([0-9]|[1-9][0-9]+)([,][ ]*([0-9]|[1-9][0-9]+))*[]]",lex,i,&j,fina);

    int tran=findd(p,"transitions[ ]*=[ ]*[\[]([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)])([,][ ]*[(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)])*[]]",lex,i,&j,transitions);
    // si on n'a pas trouve "transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]", on essaie de trouver "transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),"
    if(tran==0)
    {
        int tranD=findd(p,"transitions[ ]*=[ ]*[\[]([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)][,])([ ]*[(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)][,])*",lex,i,&j,transitionsDe);
        // si on n'a pas trouve "transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),", on essaie de trouver "(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]"
        if(tranD==0)
        {
            int tranF=findd(p,"([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)][,])*([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)][]])",lex,i,&j,transitionsFin);
            // si on n'a pas trouve "(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]", on essaie de trouver "(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),"
            if(tranF==0) findd(p,"([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][a-zA-Z0-9\u4e00-\u9fa5]+[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5]+[`][)]))*[)][,])+",lex,i,&j,transitionsMi);
        }
    }

    findd(p,"[}]",lex,i,&j,acc_fe);
    findd(p,"[/][*]",lex,i,&j,Com_ou);
    findd(p,"[*][/]",lex,i,&j,Com_fe);
    findd(p,"[/]{2}",lex,i,&j,Com);
    findd(p,"[*]",lex,i,&j,Autre); //c'est pour le cas *, dans la fonction findd, je vais separer * et */, /*
    findd(p,"[/]",lex,i,&j,Autre); //c'est pour le cas seulment un / ou ///

    // les autres, parceque dans les commentaires, on peut mettre n'importe quoi.
    findd(p,"[^*/} \t\n]+",lex,i,&j,Autre);
    //sort les lexeme par son index
    if(j>1){
        for(int k=0;k<j;k++)
        {
            for(int l=0;l<j-1-k;l++)
            {
                //sort les lexemes par son index
                if(lex[i][l].index>lex[i][l+1].index)
                {
                    lexeme temp=lex[i][l];
                    lex[i][l]=lex[i][l+1];
                    lex[i][l+1]=temp;
                }
                //le cas ou on aurait trouve un lexeme comme Automate, et puis dans lexeme Mot, on le trouve aussi, donc il faut enlever un.
                else if(lex[i][l].index==lex[i][l+1].index )
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


        for(int l=0;l<j-1;l++)
        {
            //si on trouve "(2 → 0,`0`),(2 → 1,`1`)]" dans lexeme transitionFin, mais on le trouve aussi dans lexeme Autre, donc il faut enlever le dernier
            while(lex[i][l].fi>=lex[i][l+1].fi && l<j-1)
                {
                    for(int ii=l+2;ii<j;ii++)
                        lex[i][ii-1]=lex[i][ii];
                    j--;
                }
            if(lex[i][l].fi>=lex[i][l+1].index && lex[i][l+1].type==Autre)
                {
                    int diff=lex[i][l+1].fi-lex[i][l].fi;
                    int len=strlen(lex[i][l+1].valeur);
                    for(int kk=0;kk<diff;kk++) lex[i][l+1].valeur[kk]=lex[i][l+1].valeur[len-diff+kk];
                    lex[i][l+1].valeur[diff]='\0';
                    lex[i][l+1].index=lex[i][l].fi+1;
                }
        }
    }

    //dans lexeme Autre, il compte aussi '\r', donc il faut l'enlever
    int len=strlen(lex[i][j-1].valeur);
    if(j>1 && len==1 && lex[i][j-1].valeur[0]!='}' && lex[i][j-1].valeur[0]!='*' && lex[i][j-1].valeur[0]!='/')lex[i][j-1].fi--;
    if(lex[i][j-1].fi<lex[i][j-1].index) j--;
    //imprimer pour verifer le resultat
    for(int k=0;k<j;k++)
{
    printf("%s\t",lex[i][k].valeur);
}
    printf("\n");
    //marquer la fin de cette ligne
    lex[i][j].type=Fin;
    lex[i][j+1].type=Mark;
    return;
}



//trouver l'expression rationnel et le met dans notre lexeme[N][N]
int findd(char* p, char *expr, lexeme lex[][N],int i, int *j,lexeme_t typ)
{
        regex_t myregexp;
        //c'est pour verifier si mon definition de l'expression rationnel est bonne, pour debuger
        if (regcomp(&myregexp, expr, REG_EXTENDED|REG_NEWLINE))
            {
                printf("Mauvaise expression rationnelle : %s\n", expr);
                regfree(&myregexp);
                return 0;
            };
        regmatch_t pmatch[100];
        int res = 0, debut, fin, offset = 0, jj=*j;
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
                char temp2='\r';
                char temp3='\n';
                //char t=']';      // pour filtre ] dans le cas autre
                //char fois='*';   // pour filtre /* et */ dans le cas autre
                //char xie='/';    // pour filtre //, /* et */ dans le cas autre
                //set l'index
                lex[i][*j].index=offset+debut;
                lex[i][*j].fi=fin+offset-1;
                //set son type
                lex[i][*j].type=typ;
                int ind=0;
                //set les valeurs
                for(char* k=p+debut  ;k<p+fin;k++)
                {
                    if(*(k)==temp || *(k)==temp1 ||*(k)==temp2 || *(k)==temp3 ){ continue;} //si le char est " ", on continue
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
        if(jj==*j) return 0;
        return 1;
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
    while(i<high && i<N &&lex[i][0].valeur!=NULL)
    {
        int j=0;
        while(lex[i][j].type!=Mark)
        {//if(lex[i][j].type!=Fin)printf("%s %d %d,%d\n",lex[i][j].valeur,e,i,j);
            switch(e)
            {
                case 0 :
                    //printf("%d\n",e);
                    switch(lex[i][j].type)
                    {
                        case Automate:
                            //printf("%d Auto\n",e);
                            if(!p.push(1))
                                {error(e,lex[i][j].valeur); return;}
                            e=1;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=2;
                            break;
                        case Com:
                            e=3;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 1:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=1;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=5;
                            break;
                        case Com:
                            e=4;
                            break;
                        case etats:
                            e=6;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 2:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=0;
                            break;
                        default:
                            e=2;
                            break;
                    }
                    break;
                case 3:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=0;
                            break;
                        default:
                            e=3;
                            break;
                    }
                    break;
                case 4:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=1;
                            break;
                        default:
                            e=4;
                            break;
                    }
                    break;
                case 5:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=1;
                            break;
                        default:
                            e=5;
                            break;
                    }
                    break;
                case 6:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=6;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=7;
                            break;
                        case Com:
                            e=8;
                            break;
                        case initial:
                            e=9;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 7:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=6;
                            break;
                        default:
                            e=7;
                            break;
                    }
                    break;
                case 8:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=6;
                            break;
                        default:
                            e=8;
                            break;
                    }
                    break;
                case 9:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=9;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=10;
                            break;
                        case Com:
                            e=11;
                            break;
                        case fina:
                            e=12;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 10:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=9;
                            break;
                        default:
                            e=10;
                            break;
                    }
                    break;
                case 11:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=9;
                            break;
                        default:
                            e=11;
                            break;
                    }
                    break;
                case 12:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=12;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=13;
                            break;
                        case Com:
                            e=14;
                            break;
                        case transitions:
                            e=15;
                            break;
                        case transitionsDe:
                            if(!p.push(3)){error(e,lex[i][j].valeur); return;}
                            e=21;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 13:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=12;
                            break;
                        default:
                            e=13;
                            break;
                    }
                    break;
                case 14:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=12;
                            break;
                        default:
                            e=14;
                            break;
                    }
                    break;
                case 15:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=15;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=16;
                            break;
                        case Com:
                            e=17;
                            break;
                        case acc_fe:
                            if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                            e=18;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 16:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=15;
                            break;
                        default:
                            e=16;
                            break;
                    }
                    break;
                case 17:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=15;
                            break;
                        default:
                            e=17;
                            break;
                    }
                    break;
                case 18:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=18;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=19;
                            break;
                        case Com:
                            e=20;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 19:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=18;
                            break;
                        default:
                            e=19;
                            break;
                    }
                    break;
                case 20:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=18;
                            break;
                        default:
                            e=20;
                            break;
                    }
                    break;
                case 21:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=21;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=22;
                            break;
                        case Com:
                            e=23;
                            break;
                        case transitionsMi:
                            e=21;
                            break;
                        case transitionsFin:
                            if(!p.pop(3)){error(e,lex[i][j].valeur); return;}
                            e=24;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 22:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=21;
                            break;
                        default:
                            e=22;
                            break;
                    }
                    break;
                case 23:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=21;
                            break;
                        default:
                            e=23;
                            break;
                    }
                    break;
                case 24:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=24;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=25;
                            break;
                        case Com:
                            e=26;
                            break;
                        case acc_fe:
                            if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                            e=27;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 25:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=24;
                            break;
                        default:
                            e=25;
                            break;
                    }
                    break;
                case 26:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=24;
                            break;
                        default:
                            e=26;
                            break;
                    }
                    break;
                case 27:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=27;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                            e=28;
                            break;
                        case Com:
                            e=29;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return;
                    }
                    break;
                case 28:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                            e=27;
                            break;
                        default:
                            e=28;
                            break;
                    }
                    break;
                case 29:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=27;
                            break;
                        default:
                            e=29;
                            break;
                    }
                    break;
            }
           // printf("%d %d\n",i,j);
            j++;
        }
        i++;//放在fin里面
    }

    if(p.vide()==1 && (e==18 || e==27)) printf("sytaxique correct\n");
    else printf("error\n");
    return;
}
