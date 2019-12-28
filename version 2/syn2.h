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

//verifier si la longeur est 1
int length1(lexeme lex[][N],int i, int j)
{
    int kk=0;
    while(lex[i][j].valeur[kk]!='\0')
        kk++;
    return kk==1;
}

typedef enum lexTrans
{
    Flash,
    Par,
    Par_fe,
    Element,
} lexTrans;

typedef struct trans
{
    int index;
    lexTrans type; 
    char element;
} trouverTrans;

void trans(char *pp, arb arbre)
{
    //separer les transitions
    regex_t myregexp;
    regcomp(&myregexp, "([(]([0-9]|[1-9][0-9]+)[ ]*→[ ]*([0-9]|[1-9][0-9]+)[,][ ]*[`][a-zA-Z0-9\u4e00-\u9fa5][`]([,][ ]*([ ]*[(][)]|[ ]*[(][`][ ]*[a-zA-Z0-9\u4e00-\u9fa5][ ]*[`][,][ ]*→[)]|[ ]*[(]→[,][ ]*[`][ ]*[a-zA-Z0-9\u4e00-\u9fa5][ ]*[`][)]))*[)])+", REG_EXTENDED|REG_NEWLINE);
    regmatch_t pmatch[100];
    int res = 0, debut, fin, offset = 0;
    int i=0;
    char tran[50][100];
    do
    {
        res = regexec(&myregexp, pp, 1, pmatch, 0);
        if (res) break; // regexec s'est planté ou n'a rien filtré
        // On récupère les informations
        debut = pmatch[0].rm_so;
        fin = pmatch[0].rm_eo;
        arbre->nom_tran++;
        strncpy(tran[i++], pp+debut, fin-debut);                                     
        pp += fin;
        offset += fin;
        //printf("etat name : %s\n",arbre->etat[arbre->nom_etat-1].name);
    }
    while (res == 0);
    //printf("nom final : %d\n",arbre->nom_etat);
    regfree(&myregexp);

    //trouver les etats depart et d'arrivee, et input, element enpile et depile
    for(int j=0;j<i;j++)
    {
        regcomp(&myregexp, "([a-zA-Z]|[1-9][0-9]+|[0-9])", REG_EXTENDED|REG_NEWLINE);
        res = 0, offset = 0;
        int num=0;
        char *p;
        p=&tran[j][0];
        int from,to;
        trouverTrans ttrans[50];
        //printf("tran: %s %ld\n",tran[j],strlen(tran[j]));
        do
        {
            res = regexec(&myregexp, p, 1, pmatch, 0);
            if (res) break; // regexec s'est planté ou n'a rien filtré
            // On récupère les informations
            debut = pmatch[0].rm_so;
            fin = pmatch[0].rm_eo;                                     
            //printf("%.*s (%d, %d)\n", fin-debut, p+debut, debut+offset, fin+offset-1);
            switch(num)
            {
                case 0:
                {
                    char tmp[5];
                    strncpy(tmp, p+debut, fin-debut);
                    //printf("from: %s\n",tmp);
                    from=atoi(tmp);
                    num++;
                    break;
                }
                case 1:
                {
                    char tmp1[5];
                    strncpy(tmp1, p+debut, fin-debut);
                    to=atoi(tmp1);
                    arbre->etat[from].tran[arbre->etat[from].numero].numberP=0;
                    arbre->etat[from].tran[arbre->etat[from].numero++].to=to;
                    num++;
                    break;
                }
                case 2:
                    arbre->etat[from].tran[arbre->etat[from].numero-1].input=*(p+debut);
                    num++;
                    break;
                default:
                    ttrans[num-3].element=*(p+debut);
                    ttrans[num-3].index=debut+offset;
                    ttrans[num-3].type=Element;
                    num++;
                    break;
            }
            p += fin;
            offset += fin;
        }
        while (res == 0);
        regfree(&myregexp);
        //printf("from %d to %d\n",from,to);
        if(from>arbre->nom_max_etat) arbre->nom_max_etat=from;
        if(to>arbre->nom_max_etat) arbre->nom_max_etat=to;

        //if(num<3) continue;

        regcomp(&myregexp, "[(]", REG_EXTENDED|REG_NEWLINE);
        res = 0, offset = 1;
        int num1=num-3;
        p=&tran[j][0];
        p=p+1;
        do
        {
            res = regexec(&myregexp, p, 1, pmatch, 0);
            if (res) break; // regexec s'est planté ou n'a rien filtré
            // On récupère les informations
            debut = pmatch[0].rm_so;
            fin = pmatch[0].rm_eo; 
            //printf("par %.*s (%d, %d)\n", fin-debut, p+debut, debut+offset, fin+offset-1);                                    
            ttrans[num1].element=*(p+debut);
            ttrans[num1].index=debut+offset;
            ttrans[num1++].type=Par;
            p += fin;
            offset += fin;
        }
        while (res == 0);
        regfree(&myregexp);
        if(num1-num+3==0) continue;

        regcomp(&myregexp, "[)]", REG_EXTENDED|REG_NEWLINE);
        res = 0, offset = 0;
        p=&tran[j][0];
        do
        {
            res = regexec(&myregexp, p, 1, pmatch, 0);
            if (res) break; // regexec s'est planté ou n'a rien filtré
            // On récupère les informations
            debut = pmatch[0].rm_so;
            fin = pmatch[0].rm_eo; 
            //printf("par %.*s (%d, %d)\n", fin-debut, p+debut, debut+offset, fin+offset-1);                                    
            ttrans[num1].element=*(p+debut);
            ttrans[num1].index=debut+offset;
            ttrans[num1++].type=Par_fe;
            p += fin;
            offset += fin;
        }
        while (res == 0);
        regfree(&myregexp);


        regcomp(&myregexp, "→", REG_EXTENDED|REG_NEWLINE);
        res = 0, offset = 0;
        p=&tran[j][0];
        do
        {
            res = regexec(&myregexp, p, 1, pmatch, 0);
            if (res) break; // regexec s'est planté ou n'a rien filtré
            // On récupère les informations
            debut = pmatch[0].rm_so;
            fin = pmatch[0].rm_eo;  
            //printf("flash %.*s (%d, %d)\n", fin-debut, p+debut, debut+offset, fin+offset-1);                                   
            ttrans[num1].element='-';
            ttrans[num1].index=debut+offset;
            ttrans[num1++].type=Flash;
            p += fin;
            offset += fin;
        }
        while (res == 0);
        regfree(&myregexp);

        //sorter ttrans selon index
        for(int ii=0;ii<num1;ii++)
            for(int jj=0;jj<num1-ii-1;jj++)
                if(ttrans[jj].index>ttrans[jj+1].index)
                {
                    trouverTrans temp=ttrans[jj+1];
                    ttrans[jj+1]=ttrans[jj];
                    ttrans[jj]=temp;
                }
        int e=0;
        for(int ii=0;ii<num1;ii++)
            switch(e)
            {
                case 0:
                    switch(ttrans[ii].type)
                    {
                        case Par:
                            e=1;
                            break;
                        default:
                            break;
                    }
                    break;
                case 1:
                    switch(ttrans[ii].type)
                    {
                        case Par_fe:
                        {
                            int num_tran=arbre->etat[from].numero-1;
                            int num_pile=arbre->etat[from].tran[num_tran].numberP;
                            arbre->etat[from].tran[num_tran].ppile[num_pile].piler=0;
                            arbre->etat[from].tran[num_tran].ppile[num_pile].element='0';
                            arbre->etat[from].tran[num_tran].numberP++;
                            e=0;
                            break;
                        }
                        case Flash:
                        {
                            int num_tran=arbre->etat[from].numero-1;
                            int num_pile=arbre->etat[from].tran[num_tran].numberP;
                            arbre->etat[from].tran[num_tran].ppile[num_pile].piler=1;
                            arbre->etat[from].tran[num_tran].ppile[num_pile].element=ttrans[ii+1].element;
                            arbre->etat[from].tran[num_tran].numberP++;
                            e=0;
                            break;
                        }
                        case Element:
                        {
                            int num_tran=arbre->etat[from].numero-1;
                            int num_pile=arbre->etat[from].tran[num_tran].numberP;
                            arbre->etat[from].tran[num_tran].ppile[num_pile].piler=-1;
                            arbre->etat[from].tran[num_tran].ppile[num_pile].element=ttrans[ii].element;
                            arbre->etat[from].tran[num_tran].numberP++;
                            e=0;
                            break;
                        }
                        default:
                            printf("impossible\n");
                            exit(1);
                    }
                    break;
                default:
                    printf("impossible\n");
                    exit(1);
            }
    }
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


int syntaxique(lexeme lex[][N], int i, arb arbre);

void error(int e, char* p)
{
    printf("error syntaxique a l'etat %d, a lexeme %s\n",e,p);
}


int syntaxique(lexeme lex[][N],int high, arb arbre)
{
    int e=0;
    int i=0;
    Pile p;
    p.pop=pop;
    p.push=push;
    p.vide=vide;
    arbre->nom_max_etat=0;
    while(i<high && i<N &&lex[i][0].valeur!=NULL)
    {
        int j=0;
        while(lex[i][j].type!=Mark)
        {   //if(lex[i][j].type!=Fin)printf("%s %d %d,%d\n",lex[i][j].valeur,e,i,j);
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
                            regex_t myregexp;
                                //c'est pour verifier si mon definition de l'expression rationnel est bonne, pour debuger
                                regcomp(&myregexp, "([1-9][0-9]+|[0-9])", REG_EXTENDED|REG_NEWLINE);
                                regmatch_t pmatch[100];
                                int res = 0, debut, fin;
                                        res = regexec(&myregexp, lex[i][j].valeur, 1, pmatch, 0);
                                        if (res) break; // regexec s'est planté ou n'a rien filtré
                                        // On récupère les informations
                                        debut = pmatch[0].rm_so;
                                        fin = pmatch[0].rm_eo;
                                        char num[10];
                                        int ind=0;
                                        for(char* k=lex[i][j].valeur+debut;k<lex[i][j].valeur+fin;k++)
                                            num[ind++]=*k;
                                        arbre->nom_pile=atoi(num);
                                        //printf("num de pile : %d\n",arbre->nom_pile);
                                regfree(&myregexp);
                            e=1;
                            break;
                        case Com_ou:
                            if(!p.push(2)){error(e,lex[i][j].valeur); return 0;}
                            e=2;
                            break;
                        case Com:
                            e=3;
                            break;
                        case Fin:
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
                            arbre->nom_etat=0;
                                regex_t myregexp;
                                //c'est pour verifier si mon definition de l'expression rationnel est bonne, pour debuger
                                regcomp(&myregexp, "([a-zA-Z\u4e00-\u9fa5]+|[1-9][0-9]+|[0-9])", REG_EXTENDED|REG_NEWLINE);
                                regmatch_t pmatch[100];
                                int res = 0, debut, fin, offset = 0;
                                char*p = lex[i][j].valeur;
                                p=p+5; // pour ne pas inclue "etats" 
                                do
                                    {
                                        res = regexec(&myregexp, p, 1, pmatch, 0);
                                        if (res) break; // regexec s'est planté ou n'a rien filtré
                                        // On récupère les informations
                                        debut = pmatch[0].rm_so;
                                        fin = pmatch[0].rm_eo;
                                        arbre->etat[arbre->nom_etat].numero=0;
                                        strncpy(arbre->etat[arbre->nom_etat++].name, p+debut, fin-debut);                                     
                                        p += fin;
                                        offset += fin;
                                        //printf("etat name : %s\n",arbre->etat[arbre->nom_etat-1].name);
                                    }
                                while (res == 0);
                                //printf("nom final : %d\n",arbre->nom_etat);
                                regfree(&myregexp);
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
                            regex_t myregexp;
                                //c'est pour verifier si mon definition de l'expression rationnel est bonne, pour debuger
                                regcomp(&myregexp, "([1-9][0-9]+|[0-9])", REG_EXTENDED|REG_NEWLINE);
                                regmatch_t pmatch[100];
                                int res = 0, debut, fin;
                                        res = regexec(&myregexp, lex[i][j].valeur, 1, pmatch, 0);
                                        if (res) break; // regexec s'est planté ou n'a rien filtré
                                        // On récupère les informations
                                        debut = pmatch[0].rm_so;
                                        fin = pmatch[0].rm_eo;
                                        char num[10];
                                        int ind=0;
                                        for(char* k=lex[i][j].valeur+debut;k<lex[i][j].valeur+fin;k++)
                                            num[ind++]=*k;
                                        arbre->etat_initial=atoi(num);
                                        //printf("num de pile : %d\n",arbre->nom_pile);
                                regfree(&myregexp);
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
                            arbre->nom_final=0;
                                regex_t myregexp;
                                //c'est pour verifier si mon definition de l'expression rationnel est bonne, pour debuger
                                regcomp(&myregexp, "([1-9][0-9]+|[0-9])", REG_EXTENDED|REG_NEWLINE);
                                regmatch_t pmatch[100];
                                int res = 0, debut, fin, offset = 0;
                                char*p = lex[i][j].valeur;
                                do
                                    {
                                        res = regexec(&myregexp, p, 1, pmatch, 0);
                                        if (res) break; // regexec s'est planté ou n'a rien filtré
                                        // On récupère les informations
                                        debut = pmatch[0].rm_so;
                                        fin = pmatch[0].rm_eo;
                                        char num[10];
                                        int ind=0;
                                        for(char* k=p+debut;k<p+fin;k++)
                                            num[ind++]=*k;
                                        arbre->etat_final[arbre->nom_final++]=atoi(num);
                                        p += fin;
                                        offset += fin;
                                        //printf("etat final : %d\n",arbre->etat_final[arbre->nom_final-1]);
                                    }
                                while (res == 0);
                                //printf("nom final : %d\n",arbre->nom_final);
                                regfree(&myregexp);
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
                        {
                            e=15;
                            char *pp=lex[i][j].valeur;
                            trans(pp,arbre);
                            break;
                        }
                        case transitionsDe:
                        {
                            if(!p.push(3)){error(e,lex[i][j].valeur); return 0;}
                            e=21;
                            char *pp=lex[i][j].valeur;
                            trans(pp,arbre);
                            break;
                        }
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
                        {
                            e=21;
                            char *pp=lex[i][j].valeur;
                            trans(pp,arbre);
                            break;
                        }
                        case transitionsFin:
                        {
                            if(!p.pop(3)){error(e,lex[i][j].valeur); return 0;}
                            e=24;
                            char *pp=lex[i][j].valeur;
                            trans(pp,arbre);
                            break;
                        }
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


