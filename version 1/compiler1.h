
typedef struct Symbole
{
    char nom[40];
    int addresse;
} symbole;


void compiler(arb arbre,char* name)
{
    int VM[1000]={0};
    int nombre_etat=arbre->nom_etat;
    symbole tableSym[nombre_etat];
    for(int ii=0;ii<nombre_etat;ii++)
        strcpy(tableSym[ii].nom,arbre->name_etat[ii].name);
    int index=0;
    VM[0]=arbre->nom_pile;
    VM[1]=arbre->etat_initial - maxEtat-1000;
    VM[2]=arbre->nom_final;
    for(int ii=0;ii<VM[2];ii++)
        VM[3+ii]=arbre->etat_final[ii] - maxEtat-1000;
    index=3+VM[2];
    int transi=0;
    for(int ii=0;ii<nombre_etat;ii++)
    {
        int nom_tran=0;
        int index_etat=index++;
        tableSym[ii].addresse=index_etat;
        while(arbre->tran[transi].from==ii)
        {
            nom_tran++;
            VM[index++]=arbre->tran[transi].input;
            VM[index++]=arbre->tran[transi].to - maxEtat-1000;
            for(int jj=0;jj<VM[0];jj++)
            {
                if(jj>=arbre->tran[transi].nomP)
                {
                    VM[index++]=0;
                    VM[index++]=0;
                }
                else 
                {
                    VM[index++]=arbre->tran[transi].ppile[jj].element;
                    VM[index++]=arbre->tran[transi].ppile[jj].piler;

                }
            }
            transi++;
        }
        VM[index_etat]=nom_tran;
    }

    for(int ii=0;ii<index;ii++)
    {
        if(VM[ii]<-1000)
        {
            VM[ii]=tableSym[VM[ii]+maxEtat+1000].addresse;
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


