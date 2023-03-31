#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>

int main() {
    int fd[2]; // déclare le tube

    if (pipe(fd) == -1) { // crée le tube
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) { // erreur dans fork
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // code du processus fils
        close(fd[0]); // ferme l'extrémité d'entrée du tube

        dup2(fd[1], STDOUT_FILENO); // redirige la sortie standard vers le tube
        close(fd[1]); // ferme l'extrémité de sortie du tube
        
		char 	**ag = (char **)malloc(sizeof(char *) * 2);
		ag[0] = strdup("/bin/ls");
		ag[1] = NULL;
        execve("/bin/ls", ag, NULL); // exécute la commande ls

        perror("execve"); // affiche une erreur si execve échoue
        exit(EXIT_FAILURE);
    } else { // code du processus parent
        close(fd[1]); // ferme l'extrémité de sortie du tube

        char buffer[256];
        int fd_copy = dup(fd[0]); // duplique le descripteur de fichier
        dup2(fd_copy, STDIN_FILENO); // redirige l'entrée standard vers le tube
        std::cin.getline(buffer, 256);
        std::cout << "Message reçu du processus fils : " << buffer << std::endl;

        std::cin.getline(buffer, 256);
        std::cout << "Message reçu du processus fils : " << buffer << std::endl;

        std::cin.getline(buffer, 256);
        std::cout << "Message reçu du processus fils : " << buffer << std::endl;

        close(fd_copy); // ferme la copie du descripteur de fichier
        close(fd[0]); // ferme l'extrémité d'entrée du tube

        int status;
        wait(&status); // attend la fin du processus fils
        exit(EXIT_SUCCESS);
    }

    return 0;
}