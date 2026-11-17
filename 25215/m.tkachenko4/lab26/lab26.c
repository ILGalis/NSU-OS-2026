
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>

#define SIZE 1024

int main(void)
{
    FILE *pipe_stream;
    char buffer[SIZE];
    size_t n;
    int status;
    pipe_stream = popen("printf 'Hello woRLD!\\n'", "r");

    if (pipe_stream == NULL) {
        perror("popen");
        return 1;
    }

    while ((n = fread(buffer, 1, sizeof(buffer), pipe_stream)) > 0) {
        for (size_t i = 0; i < n; i++) {
            buffer[i] = (char)toupper((unsigned char)buffer[i]);
        }
        if (fwrite(buffer, 1, n, stdout) != n) {
            perror("fwrite");
            pclose(pipe_stream);
            return 1;
        }
    }

    if (ferror(pipe_stream)) {
        perror("fread");
        pclose(pipe_stream);
        return 1;
    }

    status = pclose(pipe_stream);

    if (status == -1) {
        perror("pclose");
        return 1;
    }

    if (fflush(stdout) == EOF) {
        perror("fflush");
        return 1;
    }

    return 0;
}
