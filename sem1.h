
int semantique(arb arbre)
{
    //si le pile utilise est egale ou petit que nombre de pile
    for(int kk=0;kk<arbre->nom_tran;kk++)
        if(arbre->tran[kk].nomP>arbre->nom_pile) 
        {
            printf("erreur semantique, le nombre de pile utilise dans la transition est grand que le nombre de pile\n");
            return 0;
        }
    

    //si etat initial et etat final est dans la liste d'etats
    if(arbre->etat_initial>=arbre->nom_etat)
    {
        printf("erreur semantique, etat initial n'est pas dans la liste d'etats\n");
        return 0;
    }
    for(int kk=0;kk<arbre->nom_final;kk++)
        if(arbre->etat_final[kk]>=arbre->nom_etat) 
        {
            printf("erreur semantique, etat final %d n'est pas dans la liste d'etats\n",arbre->etat_final[kk]);
            return 0;
        }
    

    // si etat dans la transition est dans la liste d'etats
    for(int kk=0;kk<arbre->nom_tran;kk++)
        if(arbre->tran[kk].from>=arbre->nom_etat || arbre->tran[kk].from>=arbre->nom_etat) 
        {
            printf("erreur semantique, etat dans la transition n'est pas dans la liste d'etats\n");
            return 0;
        }
    

    // sorter les transitions.
    for(int kk=0;kk<arbre->nom_tran;kk++)
        for(int ii=0;ii<arbre->nom_tran-kk-1;ii++)
            if(arbre->tran[ii].from>arbre->tran[ii+1].from) 
            {
                transition temp=arbre->tran[ii];
                arbre->tran[ii]=arbre->tran[ii+1];
                arbre->tran[ii+1]=temp;
            }
    
    for(int kk=0;kk<arbre->nom_tran;kk++)
        for(int ii=0;ii<arbre->nom_tran-kk-1;ii++)
            if(arbre->tran[ii].to>arbre->tran[ii+1].to) 
            {
                transition temp=arbre->tran[ii];
                arbre->tran[ii]=arbre->tran[ii+1];
                arbre->tran[ii+1]=temp;
            }
    

    // si un etat depart a 2 memes inputs 
    for(int kk=0;kk<arbre->nom_tran;kk++)
        for(int ii=kk+1;ii<arbre->nom_tran;ii++)
        {
            if(arbre->tran[kk].from<arbre->tran[ii].from) break;
            if(arbre->tran[kk].from==arbre->tran[ii].from && arbre->tran[kk].input==arbre->tran[ii].input)
            {
                printf("erreur semantique, l'etat depart %d a 2 memes inputs '%c'\n",arbre->tran[kk].from,arbre->tran[kk].input);
                return 0;
            }
        }


    //noter les caracteres enpile et depile dans une liste
    int nom_carac=0;
    char carac[50];
    for(int kk=0;kk<arbre->nom_tran;kk++)
        for(int ii=0;ii<arbre->tran[kk].nomP;ii++)
        {
            int exist=0;
            for(int jj=0;jj<nom_carac;jj++)
            {
                if(arbre->tran[kk].ppile[ii].piler!=0 && arbre->tran[kk].ppile[ii].element==carac[jj]) 
                {
                    exist=1;
                    break;
                }
            }
            if(exist==0 && arbre->tran[kk].ppile[ii].piler!=0)carac[nom_carac++]=arbre->tran[kk].ppile[ii].element;
        }
            
    //si un caractere a seulement etats enpiles ou depiles.
    for(int jj=0;jj<nom_carac;jj++)
    {
        int en=1,de=1;
        for(int kk=0;kk<arbre->nom_tran;kk++)
            for(int ii=0;ii<arbre->tran[kk].nomP;ii++)
            {
                if(arbre->tran[kk].ppile[ii].element==carac[jj])
                {
                    if(arbre->tran[kk].ppile[ii].piler==1) en=0;
                    if(arbre->tran[kk].ppile[ii].piler==-1) de=0;
                }
                    

            }
        if(en!=0 || de!=0) 
        {
            printf("erreur semantique, le caractere '%c' a seulement etats enpiles ou depiles\n",carac[jj]);
            return 0;
        }
    }


    //si le caractere enpile et depile est un input
    for(int jj=0;jj<nom_carac;jj++)
    {
        int inp=0;
        for(int kk=0;kk<arbre->nom_tran;kk++)
            if(carac[jj]==arbre->tran[kk].input) 
            {
                inp=1;
                break;
            }
        if(inp==0)
        {
            printf("erreur semantique, un caractere enpile et depile n'est pas un input\n");
            return 0;
        }
    }
        
    // si le nom d'etat est duplicated.
    for(int ii=0;ii<arbre->nom_etat;ii++)
        for(int jj=ii+1;jj<arbre->nom_etat;jj++)
            if (!strcmp(arbre->name_etat[ii].name,arbre->name_etat[jj].name))
            {
                printf("erreur semantique, nom d'etat \"%s\"est duplicated\n",arbre->name_etat[ii].name);
                return 0;
            }
    

    return 1;
}