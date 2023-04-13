# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

pid_t	*pid;
int		nb;

void	handler( int signum ) {
	for (int i = 0; i < nb; ++i)
		kill(pid[i], SIGTERM);
}

int main(int argc, char **argv, char **env) {

	signal(SIGINT, handler);
	nb = 10;
	if (argc > 1)
		nb = atoi(argv[1]);
	printf("nb : %d\n", nb);
	signal(SIGINT, handler);
	pid = (pid_t *)calloc(sizeof(pid_t), nb);
	for (int i = 0; i < nb; ++i) {
		char	*ag[] = { "/usr/bin/siege", "-b", "http://79.84.128.88:8001/", NULL };
		pid[i] = fork();
		if (!pid[i]) {
			execve("/usr/bin/siege", ag, env);
			perror("ERROR execve:"); exit(EXIT_FAILURE);
		}
	}
	for (int i = 0; i < nb; ++i)
		waitpid(pid[i], 0, 0);
	free(pid);
	return (0);
}