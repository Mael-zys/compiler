typedef struct pileTransition
{
    char element; //element enpile ou depile
    int piler;    // 1 est enpile, 0 rien, -1 est depile.
} pileTransition;

typedef struct transition
{
    int from;
    int to;
    char input; 
    int nomP;  //nombre de pile utilise par ce input
    pileTransition ppile[10];
} transition;

typedef struct arbre
{
    int nom_pile;
    int nom_etat;
    int etat_initial;
    int nom_final;
    int etat_final[20];
    int nom_tran;
    transition tran[100];
} *arb;

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


int syntaxique(lexeme lex[][N], int i, arb arbr);


void error(int e, char* p)
{
    printf("error syntaxique a l'etat %d, a lexeme %s\n",e,p);
}

int length1(lexeme lex[][N],int i, int j)
{
    int kk=0;
    while(lex[i][j].valeur[kk]!='\0')
        kk++;
    return kk==1;
}

void printArbre(arb Arbre)
{
    printf("sytaxique correct\n");
}


int syntaxique(lexeme lex[][N],int high, arb arbr)
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
                                {error(e,lex[i][j].valeur); return 0;}
                            e=1;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=2;
                            break;
                        case Com:
                            e=3;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 1:
                    switch(lex[i][j].type)
                    {
                        case Fin:
                            e=1;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
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
                            return 0;
                    }
                    break;
                case 2:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
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
                            return 0;
                    }
                    break;
                case 7:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
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
                            return 0;
                    }
                    break;
                case 10:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=13;
                            break;
                        case Com:
                            e=14;
                            break;
                        case transitions:
                            e=15;
                            break;
                        case transitionsDe:
                            if(!p.push(3)){error(e,lex[i][j].valeur); return 0;}
                            e=21;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 13:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=16;
                            break;
                        case Com:
                            e=17;
                            break;
                        case acc_fe:
                            if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                            e=18;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 16:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=19;
                            break;
                        case Com:
                            e=20;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 19:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=22;
                            break;
                        case Com:
                            e=23;
                            break;
                        case transitionsMi:
                            e=21;
                            break;
                        case transitionsFin:
                            if(!p.pop(3)){error(e,lex[i][j].valeur); return 0;}
                            e=24;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 22:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=25;
                            break;
                        case Com:
                            e=26;
                            break;
                        case acc_fe:
                            if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                            e=27;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 25:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=28;
                            break;
                        case Com:
                            e=29;
                            break;
                        default:
                            error(e,lex[i][j].valeur);
                            return 0;
                    }
                    break;
                case 28:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
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

    if(p.vide()==1 && (e==18 || e==27)) 
    {
        return 1;
    }
    else printf("error\n");
    return 0;
}


