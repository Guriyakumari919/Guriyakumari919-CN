#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081

int main()
{
    int s;
    struct sockaddr_in addr;
    char buf[1024];

    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(s, (struct sockaddr*)&addr, sizeof(addr));

    while(1)
    {
        printf("Enter request: ");
        fgets(buf, sizeof(buf), stdin);

        buf[strcspn(buf, "\n")] = '\0';

        send(s, buf, strlen(buf), 0);

        if(strcmp(buf, "bye") == 0)
            break;

        memset(buf, 0, sizeof(buf));
        read(s, buf, sizeof(buf));

        printf("Server: %s\n", buf);
    }

    close(s);

    return 0;
}
