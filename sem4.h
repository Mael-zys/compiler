
int semantique(arb arbre)
{
    //si le pile utilise est egale ou petit que nombre de pile
    for(int kk=0;kk<arbre->nom_etat;kk++)
        for(int ii=0;ii<arbre->etat[kk].numero;ii++)
            if(arbre->etat[kk].tran[ii].numberP > arbre->nom_pile) 
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
        if(arbre->nom_max_etat>=arbre->nom_etat) 
        {
            printf("erreur semantique, etat dans la transition n'est pas dans la liste d'etats\n");
            return 0;
        }
    

    // si un etat depart a 2 memes inputs 
    for(int kk=0;kk<arbre->nom_etat;kk++)
        for(int ii=0;ii<arbre->etat[kk].numero;ii++)
        {
            for(int jj=ii+1;jj<arbre->etat[kk].numero;jj++)
            {
                if(arbre->etat[kk].tran[ii].input==arbre->etat[kk].tran[jj].input)
                {
                    printf("erreur semantique, l'etat depart %d a 2 memes inputs '%c'\n",kk,arbre->etat[kk].tran[ii].input);
                    return 0;
                }
            }
        }


    //noter les caracteres enpile et depile dans une liste
    int nom_carac[arbre->nom_pile];
    char carac[arbre->nom_pile][50];
    for(int ii=0;ii<arbre->nom_pile;ii++)
        nom_carac[ii]=0;
    for(int kk=0;kk<arbre->nom_etat;kk++)
    {
        for(int ii=0;ii<arbre->etat[kk].numero;ii++)
        {
            for(int jj=0;jj<arbre->etat[kk].tran[ii].numberP;jj++)
            {
                if(arbre->etat[kk].tran[ii].ppile[jj].piler!=0) 
                {
                   int exist=0;
                   for(int ll=0;ll<nom_carac[jj];ll++)
                    {
                        if(arbre->etat[kk].tran[ii].ppile[jj].element==carac[jj][ll]) 
                        {
                            exist=1;
                            break;
                        }
                    }
                    if(exist==0) carac[jj][nom_carac[jj]++]=arbre->etat[kk].tran[ii].ppile[jj].element;
                }
            }
        }
    }

    //si un caractere a seulement etats enpiles ou depiles.
    for(int ll=0;ll<arbre->nom_pile;ll++)
        for(int jj=0;jj<nom_carac[ll];jj++)
        {
            int en=1,de=1;
            for(int kk=0;kk<arbre->nom_etat;kk++)
                for(int ii=0;ii<arbre->etat[kk].numero;ii++)
                {
                    if(arbre->etat[kk].tran[ii].ppile[ll].element==carac[ll][jj] && arbre->etat[kk].tran[ii].ppile[ll].piler==1) en=0;
                    if(arbre->etat[kk].tran[ii].ppile[ll].element==carac[ll][jj] && arbre->etat[kk].tran[ii].ppile[ll].piler==-1) de=0;
                }

            if(en!=0 || de!=0) 
            {
                printf("erreur semantique, le caractere '%c' a seulement etats enpiles ou depiles\n",carac[ll][jj]);
                return 0;
            }
        }


    //si le caractere enpile et depile est un input
    for(int ll=0;ll<arbre->nom_pile;ll++)
        for(int jj=0;jj<nom_carac[ll];jj++)
        {
            int inp=0;
            for(int kk=0;kk<arbre->nom_etat;kk++)
            {
                for(int ii=0;ii<arbre->etat[kk].numero;ii++)
                {
                    if(carac[ll][jj]==arbre->etat[kk].tran[ii].input)
                    {
                        inp=1;
                        break;
                    }
                }
                if(inp==1) break;
            }
            if(inp==0)
            {
                printf("erreur semantique, un caractere enpile et depile '%c' n'est pas un input\n",carac[ll][jj]);
                return 0;
            }
        }
        
    // si le nom d'etat est duplicated.
    for(int ii=0;ii<arbre->nom_etat;ii++)
        for(int jj=ii+1;jj<arbre->nom_etat;jj++)
            if (!strcmp(arbre->etat[ii].name,arbre->etat[jj].name))
            {
                printf("erreur semantique, nom d'etat \"%s\"est duplicated\n",arbre->etat[ii].name);
                return 0;
            }
    

    return 1;
}