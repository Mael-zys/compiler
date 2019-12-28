typedef struct Pile
{
    int sp;  //stack pointer
    int pile[100]; //pile
} Pile;

typedef struct listPile
{
    Pile p[100]; //tous les pile
} *listPile;

//table de symbole
typedef struct Symbole
{
    char nom[40];
    int addresse;
} symbole;

//depiler la pile
int pop(listPile lp, int l, int i, int mode, char mot[500])
{
        if(lp->p[l].sp==0) 
        {
            if(mode==1) printf("Erreur : Pile %d vide!\n",l);
            printf("Le mot %s est refuse!\n",mot);
            exit(1);
        }
        if(lp->p[l].pile[lp->p[l].sp-1]==i) {lp->p[l].sp--; return 1;}
        else 
        {
            if(mode==1) printf("Erreur : Pile %d: le char que l'on veut pop n'est pas dans la tete de pile!\n",l);
            printf("Le mot %s est refuse!\n",mot);
            exit(1);
        }
}

//enpiler la pile
int push(listPile lp, int l, int i, int mode, char mot[500])
{
        if(lp->p[l].sp==50) 
        {
            if(mode==1) printf("Erreur : Pile %d pleine!\n",l);
            printf("Le mot %s est refuse!\n",mot);
            exit(1);
        }
        lp->p[l].pile[lp->p[l].sp]=i;
        lp->p[l].sp++;
        return 1;
}

//si la pile est vide
int vide(listPile lp, int l)
{
        return lp->p[l].sp==0;
}

//les operation de pile
int opPile(listPile lp, int l, int i, int piler, int mode, char mot[500])
{
    switch(piler)
    {
        case 0: 
            return 1;
        case 1:
            if(push(lp,l,i,mode,mot)) return 1;
            else return 0;
        case -1:
            if(pop(lp,l,i,mode,mot)) return 1;
            else return 0;
        default:
            return 0;      
    }
}

//trouver le nom d'etat
int findName(int addresse, int length, symbole Symbole[100])
{
    //printf("%d %d\n",addresse,length);
    for(int i=0;i<length;i++)
        if(Symbole[i].addresse==addresse) return i;
    printf("error\n");
    exit(1);
}

//la fonction principale
void executer(char *input, int mode)
{
    //lire VM
    FILE *fp = NULL;
    char buff[10];
    if((fp=fopen(input,"r"))==NULL)
    {
        printf("On ne peut pas ouvrir le fichier VM: '%s'.\n",input);
        exit(1);
    }
    int VM[500];
    int i=0;
    while((fscanf(fp, "%s", buff))!=EOF)
        VM[i++]=atoi(buff);
    fclose(fp);

    int nom_etat;
    symbole Sym[100];
    
    //si dans la mode debug, lire d'abord le fichier Table
    if(mode==1)
    {
        char tableName[30]="Table";
        strcat(tableName, input+2);
        if((fp=fopen(tableName,"r"))==NULL)
        {
            printf("On ne peut pas ouvrir le fichier de table des symboles: '%s'.\n",tableName);
            exit(1);
        }

        int i=0;
        char buff[50];
        char buff1[10];
        int addresse;
        fscanf(fp, "%s %s\n", buff, buff1);
        while((fscanf(fp, "%s %d\n", buff, &addresse))!=EOF)
        {
                strcpy(Sym[i].nom,buff);
                Sym[i].addresse=addresse; 
                //printf("nom:%s length:%ld addresse:%d\n",Sym[i].nom,strlen(Sym[i].nom),Sym[i].addresse);
                i++;
        } 
        fclose(fp);
        nom_etat=i;
    }


    listPile lp=(struct listPile*)malloc(sizeof(struct listPile));; //nombre de pile
    //initialiser tous les stack pointer
    for(int ii=0;ii<VM[0];ii++)
        lp->p[ii].sp=0;
    
    char mot[500];
    //obetenir le mot tester
    printf("Donner le mot entree: ");
    scanf("%s",mot);

    int position=VM[1];
    if(mode==1)
    {
        printf("  -> etat: %s\t\t",Sym[findName(position,nom_etat,Sym)].nom);
        for(int i=0;i<VM[0];i++)
            printf("Pile %d : Vide\t\t",i);
        printf("\n");
    }

    //tester le mot
    for(int i=0;i<strlen(mot);i++)
    {
        int in=0;
        if(mode==1) printf("%c -> ",mot[i]);
        for(int j=0;j<VM[position];j++)
        {
            int tmp=position+1+(2+2*VM[0])*j;
            if(mot[i]==VM[tmp])
            {
                in=1;
                position=VM[tmp+1];
                
                //operation de piles
                for(int l=0;l<VM[0];l++)
                {
                    char carc=VM[tmp+2+2*l];
                    int piler=VM[tmp+3+2*l];
                    opPile(lp,l,carc,piler,mode,mot);
                }
                if(mode==1) printf("etat: %s\t\t",Sym[findName(position,nom_etat,Sym)].nom);
                for(int l=0;l<VM[0];l++)
                {
                    if(mode==1 && lp->p[l].sp==0) printf("Pile %d : Vide\t\t",l);
                    if(mode==1 && lp->p[l].sp>0) 
                    {
                        printf("Pile %d : ",l);
                        for(int iii=0;iii<lp->p[l].sp;iii++)
                            printf("%c  ",lp->p[l].pile[iii]);
                        printf("\t\t");
                    }
                }
                if(mode==1) printf("\n");
            }
        }
        //si un char n'est pas trouve 
        if(in==0)
        {
            if(mode==1) printf("Erreur : le char '%c'est interdit!\n",mot[i]);
            printf("Le mot %s est refuse.\n",mot);
            return;
        }
    }

    //verifier si dans l'etat accepte
    int accept=0;
    for(int i=0;i<VM[2];i++)
    {
        if(position==VM[3+i])
        {
            accept=1;
            break;
        }
    }
    if(accept!=1)
    {
        printf("Le mot %s est refuse. ",mot);
        if(mode==1) printf("Il n'est pas dans l'etat accepte!\n");
        return;
    }

    //verifier si la pile est vide
    for(int i=0;i<VM[0];i++)
        if(!vide(lp,i)) 
        {
            printf("Le mot %s est refuse. ",mot);
            if(mode==1) printf("Pile %d n'est pas vide\n",i);
            return;
        }

    printf("Le mot %s est accepte\n",mot);
    return ;
}