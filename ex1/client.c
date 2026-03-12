#include <stdio.h>
#include <stdlib.h>

/* To be written. Do not submit this file to canvas */

extern char *processRequest (char *);

int main (int argc, char **argv) {
    char *response;

    response = processRequest("A 147.188.193.0-147.188.194.255 21-22");
    printf("%s\n", response);
    free(response);

    response = processRequest("A 1.2.3.4 80");
    printf("%s\n", response);
    free(response);

    response = processRequest("C 147.188.193.5 21");
    printf("%s\n", response);
    free(response);

    response = processRequest("C 147.188.195.1 21");
    printf("%s\n", response);
    free(response);
    response = processRequest("C 147.188.194.200 22");
    printf("%s\n", response);
    free(response);
    response = processRequest("C 1.2.3.4 80");
    printf("%s\n", response);
    free(response);
    response = processRequest("C 1.2.3.4 81");
    printf("%s\n", response);
    free(response);
    

    response = processRequest("L");
    printf("%s\n", response);
    free(response);

    response = processRequest("R");
    printf("%s\n", response);
    free(response);

    response = processRequest("D 1.2.3.4 80");
    printf("%s\n", response);
    free(response);

    response = processRequest("L");
    printf("%s\n", response);
    free(response);

    response = processRequest("F");
    printf("%s\n", response);
    free(response);

    return 0;
}
