#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *a = malloc(100);
    getcwd(a, 100);
    printf("%s\n", a);

    char *pwd = getenv("PWD");
    printf("%s\n", pwd);

    chdir("/home/mrubal-c/Cursus/try");


    getcwd(a, 100);
    printf("%s\n", a);

    pwd = getenv("PWD");
    printf("%s\n", pwd);

    return 0;
}