
typedef struct Symbole
{
    char nom[40];
    int addresse;
} symbole;


void compiler(arb arbre)
{
    int VM[1000];
    int nombre_etat=arbre->nom_etat;
    symbole tableSym[nombre_etat];
    for(int ii=0;ii<nombre_etat;ii++)
        strcpy(tableSym[ii].nom,arbre->name_etat[ii].name);
    int index=0;
    VM[0]=arbre->nom_pile;
    VM[1]=arbre->etat_initial - maxEtat;
    VM[2]=arbre->nom_final;
    for(int ii=0;ii<VM[2];ii++)
        VM[3+ii]=arbre->etat_final[ii] - maxEtat;
    index=3+VM[2];
    for(int ii=0;ii<nombre_etat;ii++)
    {
        
    }
}


