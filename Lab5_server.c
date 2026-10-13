#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>

#define PORT 8081

int main() {
    int s, c, n, w, v;
    char b[1024], r[1024], *p, *t;
    struct sockaddr_in a;

    s = socket(AF_INET, SOCK_STREAM, 0);
    a.sin_family = AF_INET;
    a.sin_port = htons(PORT);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&a, sizeof(a));
    listen(s, 1);
    c = accept(s, NULL, NULL);

    while ((n = read(c, b, 1023)) > 0) {
        b[n] = '\0';
        b[strcspn(b, "\r\n")] = 0;
        if (!strcmp(b, "bye")) break;

        p = strchr(b, '|');
        if (!p) {
            send(c, "Invalid Command", 15, 0);
            continue;
        }

        *p = '\0';
        t = p + 1;
        while (isspace((unsigned char)*t)) t++;

        if (strcmp(b, "ANALYZE ") == 0 ||
            strcmp(b, "ANALYZE") == 0) {
            n = strlen(t);
            w = v = 0;
            int in = 0;

            for (int i = 0; t[i]; i++) {
                if (!isspace((unsigned char)t[i]) && !in)
                    w++, in = 1;
                if (isspace((unsigned char)t[i])) in = 0;
                if (strchr("aeiouAEIOU", t[i])) v++;
            }

            sprintf(r, "Chars=%d, Words=%d, Vowels=%d\n",
                    n, w, v);
            send(c, r, strlen(r), 0);
        } else {
            send(c, "Invalid Command\n", 16, 0);
        }
    }

    close(c);
    close(s);
    return 0;
}
