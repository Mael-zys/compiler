
typedef struct Symbole
{
    char nom[40];
    int addresse;
} symbole;


void compiler(arb arbre, char* name)
{
    int VM[1000];
    int nombre_etat=arbre->nom_etat;
    int nombre_final=arbre->nom_final;
    int nombre_pile=arbre->nom_pile;
    
    //remplir la table de symbole
    symbole tableSym[nombre_etat];
    for(int ii=0;ii<nombre_etat;ii++)
    {
        strcpy(tableSym[ii].nom, arbre->etat[ii].name);
        if(ii==0) tableSym[ii].addresse=3+nombre_final;
        else tableSym[ii].addresse = tableSym[ii-1].addresse + 1 + arbre->etat[ii-1].numero * ( 2+2 * nombre_pile);
    }  

    //remplir la VM
    int index=0;
    VM[0]=arbre->nom_pile;
    VM[1]=tableSym[arbre->etat_initial].addresse;
    VM[2]=arbre->nom_final;
    //etat final
    for(int ii=0;ii<VM[2];ii++)
        VM[3+ii]=tableSym[arbre->etat_final[ii]].addresse;
    index=3+VM[2];
    for(int ii=0;ii<nombre_etat;ii++)
    {
        VM[index++]=arbre->etat[ii].numero; //nombre de transition
        for(int jj=0;jj<arbre->etat[ii].numero;jj++)
        {
            VM[index++]=arbre->etat[ii].tran[jj].input;
            VM[index++]=tableSym[arbre->etat[ii].tran[jj].to].addresse;
            //operation de pile
            for(int ll=0;ll<VM[0];ll++)
            {
                if(arbre->etat[ii].tran[jj].ppile[ll].piler==0) VM[index++]=0;
                else VM[index++]=arbre->etat[ii].tran[jj].ppile[ll].element;
                VM[index++]=arbre->etat[ii].tran[jj].ppile[ll].piler;
            }
        }
    }

    //ecrire le fichier table de symbole
    FILE *fp = NULL;
    char TableName[30]="Table";
    int len=strcspn(name,".");
    fp = fopen(strncat(TableName,name,len), "w");
    fprintf(fp, "Nom Addresse\n");
    for(int ii=0;ii<nombre_etat;ii++)
    {
        fprintf(fp,"%s %d\n",tableSym[ii].nom,tableSym[ii].addresse);
    }  
    fclose(fp);

    //ecrire le fichier VM
    char VMname[30]="VM";
    fp = fopen(strncat(VMname,name,len), "w");
    for(int ii=0;ii<index;ii++)
    {
        fprintf(fp,"%d\t",VM[ii]);
    }  
    fclose(fp);
}