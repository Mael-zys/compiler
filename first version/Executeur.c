#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Executeur.h"

int main(int argc, char* argv[])
{
    if(argc==2) executer(argv[1],0);
    else if(argc==3 && !strcmp("-debug",argv[1])) executer(argv[2],1);
    else printf("erreur, manque ou trop d'arguments\n");
    return 0;
}
