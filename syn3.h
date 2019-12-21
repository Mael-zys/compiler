
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
        {//if(lex[i][j].type!=Fin)printf("%s %d %d,%d\n",lex[i][j].valeur,e,i,j);
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
                        default:
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
                        default:
                            break;
                    }break;



            }
           // printf("%d %d\n",i,j);
            j++;
        }
        i++;//放在fin里面
    }

    if(p.vide()!=1 || e!=0) printf("sytaxique error de commentaire\n");
    else
    {
        e=i=sp=0;
        while(i<high && i<N &&lex[i][0].valeur!=NULL)
        {
            int j=0;
            while(lex[i][j].type!=Mark)
            {//if(lex[i][j].type!=Fin)printf("%s %d %d,%d\n",lex[i][j].valeur,e,i,j);
                switch(e)
                {
                    case 0:
                        switch(lex[i][j].type)
                        {
                            case Automate:
                                if(!p.push(1)){error(e,lex[i][j].valeur); return;}
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
                            case etats:
                                e=2;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 2:
                        switch(lex[i][j].type)
                        {
                            case initial:
                                e=3;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 3:
                        switch(lex[i][j].type)
                        {
                            case fina:
                                e=4;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 4:
                        switch(lex[i][j].type)
                        {
                            case transitions:
                                e=5;
                                break;
                            case transitionsDe:
                                if(!p.push(2)){error(e,lex[i][j].valeur); return;}
                                e=7;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 5:
                        switch(lex[i][j].type)
                        {
                            case acc_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=6;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 6:
                        switch(lex[i][j].type)
                        {
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 7:
                        switch(lex[i][j].type)
                        {
                            case transitionsMi:
                                break;
                            case transitionsFin:
                                if(!p.pop(2)){error(e,lex[i][j].valeur); return;}
                                e=8;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                    case 8:
                        switch(lex[i][j].type)
                        {
                            case acc_fe:
                                if(!p.pop(1)){error(e,lex[i][j].valeur); return;}
                                e=6;
                                break;
                            case Fin:
                                break;
                            default:
                                error(e,lex[i][j].valeur);
                                return;
                        }break;
                }
               // printf("%d %d\n",i,j);
                j++;
            }
            i++;//放在fin里面
        }
        if(p.vide()==1 && e==6) 
		{
			printf("sytaxique correct\n");
			//printf("l'arbre sytaxique est : \n");
        }	
        else printf("sytaxique error\n");
        return;
    }

}
