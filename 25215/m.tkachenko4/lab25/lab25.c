
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int fd[2];
    pid_t p1, p2;
    const char text[] = "Hello woRLD!\n";
    char c;
    ssize_t n;
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
        return 1;
    }
    if (p1 == 0) {
        close(fd[0]);
        for (int i = 0; text[i] != '\0'; i++) {
            if (write(fd[1], &text[i], 1) != 1) { // переделать крч с посимвольного чтения и записи на буферный
                perror("write");
                _exit(1);
            }
        }

        close(fd[1]);
        _exit(0);
    }
    p2 = fork();
    if (p2 == -1) {
        perror("fork");
        return 1;
    }
    if (p2 == 0) {
        close(fd[1]);
        while ((n = read(fd[0], &c, 1)) > 0) {
            c = (char)toupper((unsigned char)c);

            if (write(STDOUT_FILENO, &c, 1) != 1) {
                perror("write");
                _exit(1);
            }
        }
        if (n == -1) {
            perror("read");
            _exit(1);
        }
        close(fd[0]);
        _exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}