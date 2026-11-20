#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024

int main() {
    char command[MAX_LINE];

    while (1) {
        // Affichage du prompt personnalisé de votre mini-shell
        printf("ocsh> ");
        fflush(stdout);

        // Lecture de la commande entrée par l'utilisateur
        if (fgets(command, MAX_LINE, stdin) == NULL) {
            break; // Sortie en cas de fin de fichier (Ctrl+D)
        }

        // Suppression du retour à la ligne (\n) à la fin de la saisie
        command[strcspn(command, "\n")] = 0;

        // Si la ligne est vide, on recommence la boucle
        if (strlen(command) == 0) {
            continue;
        }

        // Commande interne pour quitter le shell proprement
        if (strcmp(command, "quit") == 0) {
            printf("Fermeture d'ocsh...\n");
            break;
        }

        // Création d'un processus fils pour exécuter la commande externe
        pid_t pid = fork();

        if (pid < 0) {
            perror("Erreur lors du fork");
            exit(1);
        } else if (pid == 0) {
            // Processus fils : exécution de la commande
            execlp(command, command, NULL);
            
            // Si execlp échoue (commande inconnue par exemple)
            perror("ocsh: commande introuvable");
            exit(1);
        } else {
            // Processus père : attend la fin de l'exécution du fils
            wait(NULL);
        }
    }

    return 0;
}