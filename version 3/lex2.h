#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <regex.h>

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

//trouver une expression rationnel et le met dans notre lexeme[N][N]
int findd(char* , char *, lexeme lex[][N],int , int *,lexeme_t );

//Trouver, classer et sorter tous les lexemes
void classer(lexeme lex[][N],int , char *);


void classer(lexeme lex[][N],int i, char *p)
{
    int j=0; // j est l'index de lexeme de cette ligne
    findd(p,"Automate[(]([0-9]|[1-9][0-9]*)[)][ ]*=[ ]*[{]",lex,i,&j,Automate);
    findd(p,"etats[ ]*=[ ]*[\[]([\"]|[`])([a-zA-Z\u4e00-\u9fa5]+|[0-9]|[1-9][0-9]+)([\"]|[`])([,][ ]*[`]([a-zA-Z\u4e00-\u9fa5]+|[0-9]|[1-9][0-9]+)[`]|[,][ ]*[\"]([a-zA-Z\u4e00-\u9fa5]+|[0-9]|[1-9][0-9]+)[\"])*[]]",lex,i,&j,etats);
    findd(p,"initial[ ]*=[ ]*([0-9]|[1-9][0-9]+)",lex,i,&j,initial);
    findd(p,"final[ ]*=[ ]*[\[]([0-9]|[1-9][0-9]+)([,][ ]*([0-9]|[1-9][0-9]+))*[]]",lex,i,&j,fina);

    int tran=findd(p,"transitions[ ]*=[ ]*[\[]([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)])([,][ ]*[(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)])*[]]",lex,i,&j,transitions);
    // si on n'a pas trouve "transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]", on essaie de trouver "transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),"
    if(tran==0)
    {
        int tranD=findd(p,"transitions[ ]*=[ ]*[\[]([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)][,])([ ]*[(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)][,])*",lex,i,&j,transitionsDe);
        // si on n'a pas trouve "transitions=[(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),", on essaie de trouver "(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]"
        if(tranD==0)
        {
            int tranF=findd(p,"([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)][,])*([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)][]])",lex,i,&j,transitionsFin);
            // si on n'a pas trouve "(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`)]", on essaie de trouver "(3 → 0,`0`),(3 → 1,`1`),(3 → 2,`2`),"
            if(tranF==0) findd(p,"([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9][`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9][ ]*[`][)]))*[)][,])+",lex,i,&j,transitionsMi);
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