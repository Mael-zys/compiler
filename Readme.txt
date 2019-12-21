Tous les fichiers peut directement executer.

//ce sont les fichiers qui utilisent les petits lexemes
 
lex1.c ( je choisis les lexemes tres petits comme ",", "[", "}" etc.
syn1.c (utiliser le resultat de lex1.c et les automates)
sem1.c (utiliser le resultat de syn1.c. et verifier si le texte a un sens)



//ce sont les fichiers qui utilisent les grands lexemes

// je n'ai pas fait la partie arbre syntaxique et semantique pour les codes suivants
// parce que j'ai utilise de grands lexemes, et pour l'arbre syntaxique et semantique, il revient trouver les petits lexemes et la methode est comme ce que j'ai fait dans lex1.c, je pense que ce n'est pas tres interessant de refaire ca, donc je n'ai pas continue.
lex2.c (je choisis les lexemes grands qui peut trouver les expression comme "etats=["一","2", "3",`Init`]",  "final=[0,1,2]" etc. 
syn2.c (utiliser le resultat de lex2.c et les automates pour analyser la grammaire) 
syn3.c (d'abord eliminer les commentaires, et puis utiliser les automates)
