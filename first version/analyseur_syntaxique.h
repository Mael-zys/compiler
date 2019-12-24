#define maxEtat 100

typedef struct PPile
{
    char element; //element enpile ou depile
    int piler;    // 1 est enpile, 0 rien, -1 est depile.
} PPile;

typedef struct transition
{
    int to;
    char input; 
    int numberP;
    PPile ppile[10];
} transition;

//definir un etat
typedef struct Etat
{
    char name[40];
    int numero; //nombre de transitions
    transition tran[50];
} Etat;

//definir arbre
typedef struct arbre
{
    int nom_pile;
    int nom_etat;
    Etat etat[maxEtat];
    int nom_max_etat;
    int etat_initial;
    int nom_final;
    int etat_final[20];
    int nom_tran;
} *arb;

//pile
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

//verifier si la longeur est 1
int length1(lexeme lex[][N],int i, int j)
{
    int kk=0;
    while(lex[i][j].valeur[kk]!='\0')
        kk++;
    return kk==1;
}

//set le nom d'etat
void setName(arb Arbre, lexeme lex[][N], int i, int j, int numeroEtat)
{
    int l=strlen(lex[i][j].valeur);
    for(int ii=0;ii<l;ii++)
    {
        Arbre->etat[numeroEtat].name[ii]=lex[i][j].valeur[ii];
    }
}

int syntaxique(lexeme lex[][N], int i, arb arbr);


void error(int e, char* p)
{
    printf("error syntaxique a l'etat %d, a lexeme %s\n",e,p);
}

//imprimer les informations d'arbre
void printArbre(arb Arbre)
{
    printf("sytaxique correct\n");  
    printf("l'arbre syntaxique est:\n");
    printf("nombre de pile est %d\n",Arbre->nom_pile);
    printf("nombre d'etats est %d\n",Arbre->nom_etat);
    printf("etat initial est: %d\n",Arbre->etat_initial);
    printf("etat final est: ");
    for(int kk=0;kk<Arbre->nom_final;kk++)
        printf("%d ",Arbre->etat_final[kk]);
    printf("\n");
    printf("nombre total de transition est %d\n",Arbre->nom_tran);
    for(int kk=0;kk<Arbre->nom_etat;kk++)
    {
        printf("Etat %d avec le nom \"%s\" a %d transitions.\n",kk,Arbre->etat[kk].name,Arbre->etat[kk].numero);
        for(int ii=0;ii<Arbre->etat[kk].numero;ii++)
        {
            printf("%d -> %d avec input '%c' utilise %d piles, ", kk,Arbre->etat[kk].tran[ii].to,Arbre->etat[kk].tran[ii].input, Arbre->etat[kk].tran[ii].numberP);
            for(int jj=0;jj<Arbre->etat[kk].tran[ii].numberP;jj++)
            {
                switch(Arbre->etat[kk].tran[ii].ppile[jj].piler)
                {
                    case 1:
                        printf("pile %d: enpiler '%c', ",jj,Arbre->etat[kk].tran[ii].ppile[jj].element);
                        break;
                    case -1:
                        printf("pile %d: depiler '%c', ",jj,Arbre->etat[kk].tran[ii].ppile[jj].element);
                        break;
                    case 0:
                        printf("pile %d: rien fait, ",jj);
                        break;
                }
            }
            printf("\n");
        }
    }
}

//la fonction principale
int syntaxique(lexeme lex[][N],int high, arb arbr)
{
    //initialiser
    int e=0; //etat
    int i=0;
    Pile p;
    p.pop=pop;
    p.push=push;
    p.vide=vide;
    int i_com,j_com; //marquer les commentaire
    int nom_pile=0, nom_etat=0, nom_final=0, nom_tran=0;
    int etat_initial;
    int etat_final[N]={0};
    int from;
    arbr->nom_max_etat=0;

    //effacer les commentaires
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
                            if(!p.push(1)){error(e,lex[i][j].valeur); return 0;}
                            e=1;
                            break;
                        default:
                            break;
                    }break;
                case 1:
                    switch(lex[i][j].type)
                    {
                        case Com_fe:
                            if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
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
                                
                                lex[i_com][j_com].type=Fin;
                                lex[i_com][j_com+1].type=Mark;
                                int k=0;
                                
                                while(lex[i][j+2].type!=Fin && lex[i][j+1].type!=Fin )
                                {
                                    lex[i][k]=lex[i][j+1];
                                    k++;
                                    j++;
                                }
                                
                                lex[i][k].type=Fin;
                                lex[i][k+1].type=Mark;
                                
                                j=-1;

                                for(int ii=i_com+1;ii<i;ii++)
                                {
                                    lex[ii][0].type=Fin;
                                    lex[ii][1].type=Mark;
                                }
                            }
                            break;
                        default:
                            break;
                    }break;
            }
            j++;
        }
        i++;
    }

    if(p.vide()!=1 || e!=0) {
        printf("sytaxique error\n");
        return 0;
    }
    else
    {
        e=i=sp=0; //inisialiser
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
                            case Automate:
                                e=1;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 1:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return 0;}
                                e=2;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 2:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=3;
                                int kk=0;
                                while(lex[i][j].valeur[kk]!='\0')
                                {
                                    nom_pile=10*nom_pile+(lex[i][j].valeur[kk]-'0');
                                    kk++;
                                }
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 3:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                                e=4;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 4:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=5;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 5:
                        switch(lex[i][j].type)
                        {
                            case ACC_ou:
                                if(!p.push(3)){error(e,lex[i][j].valeur); return 0;}
                                e=6;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
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
                                return 0;
                        }break;
                    case 7:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=8;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 8:
                        switch(lex[i][j].type)
                        {
                            case CRO_ou:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                                e=9;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 9:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return 0;}
                                e=10;
                                break;
                            case APO_dou:
                                if(!p.push(5)){error(e,lex[i][j].valeur); return 0;}
                                e=10;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 10:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=11;
                                setName(arbr,lex,i,j,nom_etat);
                                arbr->etat[nom_etat].numero=0;
                                nom_etat++;
                                break;
                            case MOT:
                                e=11;
                                setName(arbr,lex,i,j,nom_etat);
                                arbr->etat[nom_etat].numero=0;
                                nom_etat++;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 11:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return 0;}
                                e=12;
                                break;
                            case APO_dou:
                                if(!p.pop(5)){error(e,lex[i][j].valeur); return 0;}
                                e=12;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 12:
                        switch(lex[i][j].type)
                        {
                            case CRO_fe:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
                                e=14;
                                break;
                            case Virgu:
                                e=13;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 13:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return 0;}
                                e=10;
                                break;
                            case APO_dou:
                                if(!p.push(5)){error(e,lex[i][j].valeur); return 0;}
                                e=10;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
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
                                return 0;
                        }break;
                    case 15:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=16;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 16:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=17;
                                int kk=0;
                                while(lex[i][j].valeur[kk]!='\0')
                                {
                                    etat_initial=10*etat_initial+(lex[i][j].valeur[kk]-'0');
                                    kk++;
                                }
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
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
                                return 0;
                        }break;
                    case 18:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=19;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 19:
                        switch(lex[i][j].type)
                        {
                            case CRO_ou:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                                e=20;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 20:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=21;
                                int kk=0;
                                while(lex[i][j].valeur[kk]!='\0')
                                {
                                    etat_final[nom_final]=10*etat_final[nom_final]+(lex[i][j].valeur[kk]-'0');
                                    kk++;
                                }
                                nom_final++;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 21:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=22;
                                break;
                            case CRO_fe:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
                                e=23;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 22:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=21;
                                int kk=0;
                                while(lex[i][j].valeur[kk]!='\0')
                                {
                                    etat_final[nom_final]=10*etat_final[nom_final]+(lex[i][j].valeur[kk]-'0');
                                    kk++;
                                }
                                nom_final++;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
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
                                return 0;
                        }break;
                    case 24:
                        switch(lex[i][j].type)
                        {
                            case Equ:
                                e=25;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 25:
                        switch(lex[i][j].type)
                        {
                            case CRO_ou:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                                e=26;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 26:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return 0;}
                                nom_tran++;
                                //arbr->etat[from].tran[numero].numberP=0;
                                e=27;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 27:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=28;
                                int kk=0;
                                from=0;
                                while(lex[i][j].valeur[kk]!='\0')
                                {
                                    from=10*from+(lex[i][j].valeur[kk]-'0');
                                    kk++;
                                }
                                if(from>arbr->nom_max_etat) arbr->nom_max_etat=from;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 28:
                        switch(lex[i][j].type)
                        {
                            case Flesh:
                                e=29;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 29:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                e=30;
                                arbr->etat[from].tran[arbr->etat[from].numero].to=0;
                                int kk=0;
                                while(lex[i][j].valeur[kk]!='\0')
                                {
                                    arbr->etat[from].tran[arbr->etat[from].numero].to=10*arbr->etat[from].tran[arbr->etat[from].numero].to+(lex[i][j].valeur[kk]-'0');
                                    kk++;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero].numberP=0;
                                if(arbr->etat[from].tran[arbr->etat[from].numero].to>arbr->nom_max_etat) arbr->nom_max_etat=arbr->etat[from].tran[arbr->etat[from].numero].to;
                                arbr->etat[from].numero++;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 30:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=31;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 31:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return 0;}
                                e=32;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 32:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                if(!length1(lex,i,j)) 
                                {
                                    error(e,lex[i][j].valeur);
                                    return 0;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero-1].input=lex[i][j].valeur[0];
                                e=33;
                                break;
			                case MOT:
                                if(!length1(lex,i,j)) 
                                {
                                    error(e,lex[i][j].valeur);
                                    return 0;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero-1].input=lex[i][j].valeur[0];
				                e=33;
				                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 33:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return 0;}
                                e=34;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 34:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                                e=49;
                                break;
                            case Virgu:
                                e=35;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 35:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return 0;}
                                arbr->etat[from].tran[arbr->etat[from].numero-1].numberP++;
                                e=36;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 36:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return 0;}
                                e=37;
                                break;
                            case Flesh:
                                e=42;
                                break;
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                                arbr->etat[from].tran[arbr->etat[from].numero].ppile[arbr->etat[from].tran[arbr->etat[from].numero].numberP-1].piler=0;
                                e=47;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 37:
                        switch(lex[i][j].type)
                        {
                            case MOT:
                                if(!length1(lex,i,j)) 
                                {
                                    error(e,lex[i][j].valeur);
                                    return 0;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].element=lex[i][j].valeur[0];
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].piler=-1;
                                e=38;
                                break;
                            case NOMBRE:
                                if(!length1(lex,i,j)) 
                                {
                                    error(e,lex[i][j].valeur);
                                    return 0;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].element=lex[i][j].valeur[0];
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].piler=-1;
                                e=38;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 38:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return 0;}
                                e=39;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 39:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=40;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 40:
                        switch(lex[i][j].type)
                        {
                            case Flesh:
                                e=41;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 41:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                                e=47;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 42:
                        switch(lex[i][j].type)
                        {
                            case Virgu:
                                e=43;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 43:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.push(4)){error(e,lex[i][j].valeur); return 0;}
                                e=44;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 44:
                        switch(lex[i][j].type)
                        {
                            case NOMBRE:
                                if(!length1(lex,i,j)) 
                                {
                                    error(e,lex[i][j].valeur);
                                    return 0;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].element=lex[i][j].valeur[0];
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].piler=1;
                                e=45;
                                break;
                            case MOT:
                                if(!length1(lex,i,j)) 
                                {
                                    error(e,lex[i][j].valeur);
                                    return 0;
                                }
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].element=lex[i][j].valeur[0];
                                arbr->etat[from].tran[arbr->etat[from].numero-1].ppile[arbr->etat[from].tran[arbr->etat[from].numero-1].numberP-1].piler=1;
                                e=45;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 45:
                        switch(lex[i][j].type)
                        {
                            case APO:
                                if(!p.pop(4)){error(e,lex[i][j].valeur); return 0;}
                                e=46;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 46:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                                e=47;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 47:
                        switch(lex[i][j].type)
                        {
                            case Par_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return 0;}
                                e=49;
                                break;
                            case Virgu:
                                e=35;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 48:
                        switch(lex[i][j].type)
                        {
                            case Par_ou:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return 0;}
                                e=27;
                                nom_tran++;
                                //arbr->etat[from].tran[nom_tran-1].nomP=0;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 49:
                        switch(lex[i][j].type)
                        {
                            case CRO_fe:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return 0;}
                                e=50;
                                break;
                            case Virgu:
                                e=48;
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 50:
                        switch(lex[i][j].type)
                        {
                            case ACC_fe:
                                if(!p.pop(3)){error(e,lex[i][j].valeur); return 0;}
                                e=51;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;
                    case 51:
                        switch(lex[i][j].type)
                        {
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return 0;
                        }break;

                }
                j++;
            }
            i++;
        }
        if(p.vide()==1 && e==51)
        {     
            arbr->nom_pile=nom_pile;
            arbr->nom_etat=nom_etat;
            arbr->etat_initial=etat_initial;
            arbr->nom_final=nom_final;
            for(int kk=0;kk<nom_final;kk++)
                arbr->etat_final[kk]=etat_final[kk];
            arbr->nom_tran=nom_tran;
            return 1;
        } 
        else printf("sytaxique error\n");
        return 0;
    }
}
