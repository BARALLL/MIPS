// Projet MIPS 2022-2023
// Auteurs: GUERIN BARAL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char **argv)
{
    bool pas = false;
    char* fileName = "";
    char* fileNameDone = "";
    char* fileNameOut = "";


    if (argc == 3 && argv[2] == "-pas")
        fileName = argv[1];
    else if (argc == 4)
    {
        fileName = argv[1];
        fileNameDone = argv[2];
        fileNameOut = argv[3];
    }
    else if (argc != 1 || (argc == 3 && argv[2] != "-pas"))
    {
        printf("MIPS Error 400: Entry argument not valid : %d && %s", argc, argv[2]);
        exit(2);
    }


    FILE* file = NULL;
    FILE* fileDone = NULL;
    FILE* fileOut = NULL;

    if (fileName != NULL) {
        file = openfile(fileName, "r");
    }
    if (fileNameDone) {
        fileDone = openfile(fileNameDone, "w+");
    }
    if (fileNameOut) {
        fileOut = openfile(fileNameOut, "w");
    }


    if (file != NULL && fileNameDone != NULL) 
    {

    }
    else if (fileName != NULL) 
    {

    }
    else 
    {
        //modeInteractif();
    }

    //fclose(file);
    //fclose(fileDone);
    //fclose(fileOut);

    /* S'il n'y a pas d'arguments (argc==1), lancer le mode interactif. */

    /* S'il y a 2 arguments (argc==3), lancer le mode automatique pas-à-pas.
       Le premier argument (argv[1]) est le nom du programme assembleur.
       Le second argument (argv[2]) est forcément "-pas". */

    /* S'il y a 3 arguments (argc==4), lancer le mode automatique.
       Le premier argument (argv[1]) est le nom du programme assembleur.
       Le second argument (argv[2]) est le nom du fichier où il faut
       enregistrer le code assemblé.
       Le troisième argument (argv[3]) est le nom du fichier où il faut
       enregistrer l'état final du programme à la fin de l'exécution. */

    /* Les rendus seront testés par le mode automatique, il est donc important
       que ce mode fasse les choses suivantes :
       - Lire le programme assembleur. Ouvrez-le avec le mode "r" de fopen().
       - Produire les deux fichiers de sortie. Ouvrez-les avec le mode "w" de
         fopen() pour qu'ils soient créés s'ils n'existent pas encore.
       Pour commencer vous n'avez pas besoin d'utiliser les fichiers, il suffit
       de les ouvrir et de les fermer immédiatement. */

    /* Le Makefile fourni contient une commande "test-cli" qui vérifie que le
       mode automatique fonctionne et crée bien les deux fichiers de sortie.
       Tapez "make test-cli" pour effectuer le test. S'il n'y a pas d'erreur,
       c'est bon. */

    /* Supprimez ces commentaires une fois que c'est fait. */

    printf("Hello émulateur MIPS!\n");

    return 0;
}
