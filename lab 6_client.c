#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s;
    char data[1024] = "", id[20], res[200];
    struct sockaddr_in addr;
    FILE *fp;

    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("data.txt not found\n");
        return 1;
    }

    fread(data, 1, sizeof(data) - 1, fp);
    fclose(fp);

    s = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(s, (struct sockaddr *)&addr, sizeof(addr));

    printf("Enter Student ID to search: ");
    scanf("%19s", id);

    write(s, data, strlen(data) + 1);
    write(s, id, strlen(id) + 1);

    read(s, res, sizeof(res) - 1);
    res[sizeof(res) - 1] = '\0';

    printf("Server Response: %s\n", res);
    close(s);
    return 0;
}
