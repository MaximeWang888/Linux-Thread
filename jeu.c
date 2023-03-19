#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
    int joueur1[2], joueur2[2];
    pid_t pid;

    // création des pipes
    if(pipe(joueur1) == -1 || pipe(joueur2) == -1) {
        perror("Erreur lors de la création des pipes");
        exit(EXIT_FAILURE);
    }

    // création du processus fils
    pid = fork();

    if(pid == -1) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) { // code du processus fils (Joueur 2)
        close(joueur1[1]); // ferme le descripteur d'écriture du pipe
        close(joueur2[0]); // ferme le descripteur de lecture du pipe
        int nombreMystere;
        read(joueur1[0], &nombreMystere, sizeof(int));
        printf("Joueur 1 a choisi le nombre ****\n\n");

        int proposition;
        int victoire = 0;
        do {
            printf("Proposition de Joueur 2 : ");
            scanf("%d", &proposition);
            write(joueur2[1], &proposition, sizeof(int));
            read(joueur1[0], &victoire, sizeof(int));
        } while(proposition != nombreMystere && !victoire);
        
        if(victoire) printf("Joueur 2 a gagné !\n");

        close(joueur1[0]); // ferme le descripteur de lecture du pipe
        close(joueur2[1]); // ferme le descripteur d'écriture du pipe
        exit(EXIT_SUCCESS); // termine le processus en cours avec succès
    }
    else { // code du processus père (Joueur 1)
        close(joueur1[0]); // ferme le descripteur de lecture du pipe
        close(joueur2[1]); // ferme le descripteur d'écriture du pipe
        srand(time(NULL));
        int nombreMystere = rand() % 10 + 1;
        printf("Joueur 1, choisissez un nombre entre 1 et 10 : ****\n");
        write(joueur1[1], &nombreMystere, sizeof(int));

        const int NBESSAIMAX = 5;
        int proposition;
        int victoire = 0;
        int nbEssai = 0;
        do {
            // lire dans le descripteur de fichier du pipe du joueur 2, 
            // et un pointeur vers un tampon de mémoire où les données lues seront stockées. 
            read(joueur2[0], &proposition, sizeof(int));

            if(proposition < nombreMystere) {
                printf("Trop petit\n");
                ++nbEssai;
            }
            else if(proposition > nombreMystere) {
                printf("Trop grand\n");
                ++nbEssai;
            }
            else {
                victoire = 1;
            }

            // écrire dans le descripteur de fichier du pipe du joueur 1,
            // et un pointeur vers un tampon de mémoire contenant les données que l'on veut écrire 
            write(joueur1[1], &victoire, sizeof(int));
            
            // si le nb essai max est atteint alors le joueur 1 gagne
            if(nbEssai == NBESSAIMAX) {
                printf("\n\nJoueur 1 a gagné !\n");
                close(joueur1[1]); // ferme le descripteur d'écriture du pipe
                close(joueur2[0]); // ferme le descripteur de lecture du pipe
                exit(EXIT_SUCCESS); // termine le processus en cours avec succès
            } else { 
                printf("\nIl ne te reste plus que %d chances.\n\n\n", NBESSAIMAX - nbEssai);
            }
        } while(proposition != nombreMystere && !victoire);

        if(!victoire) printf("Joueur 2 a gagné !\n");

        close(joueur1[1]); // ferme le descripteur d'écriture du pipe
        close(joueur2[0]); // ferme le descripteur de lecture du pipe
        exit(EXIT_SUCCESS); // termine le processus en cours avec succès
    }

    return 0;
}
