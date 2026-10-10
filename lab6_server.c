#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

int main() {
    int s, c;
    struct sockaddr_in addr;
    char data[1024], id[20], line[100], res[200];

    s = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    while (1) {
        c = accept(s, NULL, NULL);

        if (fork() == 0) {
            close(s);
            read(c, data, sizeof(data) - 1);
            read(c, id, sizeof(id) - 1);

            data[1023] = '\0';
            id[19] = '\0';

            strcpy(res, "Record Not Found");

            char *p = strtok(data, "\n");
            while (p != NULL) {
                char sid[20], name[50], dept[20];

                if (sscanf(p, "%19s %49s %19s",
                           sid, name, dept) == 3 &&
                    strcmp(sid, id) == 0) {
                    snprintf(res, sizeof(res),
                             "Record Found : %s", p);
                    break;
                }
                p = strtok(NULL, "\n");
            }

            write(c, res, strlen(res) + 1);
            close(c);
            exit(0);
        }
        close(c);
    }
}
