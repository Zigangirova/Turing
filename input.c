#include "turing.h"

int main(int argc, char *argv[]) {

    if (argc == 1) {
        printf("\nProcess finished successful \n");
        exit(0);
    }

    if ((argc != 9) && (argc != 10)) {
        printf("Wrong number of arguments in command line launch\n");
        exit(1);
    }

    FILE *input;
    FILE *alphabet;
    FILE *machine;
    FILE *output;
    int flag;

    for (int i = 1; i < 8; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            alphabet = fopen(argv[i + 1], "rt");
            if (alphabet == NULL) {
                printf("Cannot get file %s\n", argv[i + 1]);
                exit(1);
            }
        }
        if (strcmp(argv[i], "-i") == 0) {
            input = fopen(argv[i + 1], "rt");
            if (input == NULL) {
                printf("Cannot get file %s\n", argv[i + 1]);
                exit(1);
            }
        }
        if (strcmp(argv[i], "-q") == 0) {
            machine = fopen(argv[i + 1], "rt");
            if (machine == NULL) {
                printf("Cannot get file %s\n", argv[i + 1]);
                exit(1);
            }
        }
        if (strcmp(argv[i], "-o") == 0) {
            output = fopen(argv[i + 1], "wt");
            if (output == NULL) {
                printf("Cannot get file %s\n", argv[i + 1]);
                exit(1);
            }
        }
    }
    if ((alphabet == NULL) || (input == NULL) || (machine == NULL) || (output == NULL)) {
        printf("Wrong format of command line launch\n");
        exit(1);
    }

    if ((argc == 10) && (strcmp(argv[9], "-debug") == 0)) {
        flag = 1;
    } else {
        flag = 0;
    }

    start(input, alphabet, machine, output, flag);
}
