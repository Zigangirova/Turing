#include "turing.h"

int process(char *alphabet, int headPos, char *tape, int *states, int statesNumber, struct COMMAND **commands,
            int flag, FILE *output) {
    printStart(output);
    printTape(headPos, tape, output);
    int step = 0;
    int currentState = 1;
    int tapeLength = strlen(tape);
    while (step != 1000) {
        step++;
        char ch = tape[headPos];
        int i = (int) (strchr(alphabet, ch) - alphabet);
        int j = 0;
        for (int k = 0; k < statesNumber; k++) {
            if (currentState == states[k]) {
                j = k;
                break;
            }
        }
        if ((commands[i][j].newSymbol == '?') && (commands[i][j].move == '?') && (commands[i][j].newState) == -1) {
            printf("The command for such combination of state and symbol was not found\n");
            exit(1);
        }
        printCommand(currentState, tape[headPos], commands[i][j], output);
        currentState = commands[i][j].newState;
        tape[headPos] = commands[i][j].newSymbol;
        if (commands[i][j].move == '>') {
            headPos++;
        }
        if (commands[i][j].move == '<') {
            headPos--;
        }
        printTape(headPos, tape, output);
        if (currentState == 0) {
            printEnd(output);
            _pclose(output);
            free(alphabet);
            free(tape);
            free(states);
            for (int l = 0; l < statesNumber; l++) {
                free(commands[l]);
            }
            free(commands);
            printf("\nProcess finished successful\n");
            exit(0);
        }
        if (headPos < 0) {
            printf("The head went beyond the left border\n");
            exit(1);
        }
        if (headPos == tapeLength - 1) {
            tapeLength++;
            tape = (char *) realloc(tape, tapeLength * sizeof(char));
            if (tape == NULL) {
                printf("Memory allocation error\n");
                exit(1);
            }
            tape[tapeLength - 1] = '_';
        }

        if (flag == 1) {
            char action[5];
            while (1) {
                scanf(" %4s", action);
                fflush(stdin);
                if (!strcmp(action, "n") || !strcmp(action, "N")) {
                    break;
                }
                if (!strcmp(action, "run") || !strcmp(action, "RUN")) {
                    flag = 0;
                    break;
                }
                if (!strcmp(action, "stop") || !strcmp(action, "STOP")) {
                    printStop(output);
                    fclose(output);
                    free(alphabet);
                    free(tape);
                    free(states);
                    for (int l = 0; l < statesNumber; l++) {
                        free(commands[l]);
                    }
                    free(commands);
                    printf("\nYou stopped the program \n");
                    exit(1);
                } else {
                    printf("Unknown action. Try this: 'N'; 'RUN'; 'STOP';\n");
                }
            }
        }
    }
    printStop(output);
    printf("RUNTIME ERROR\n");
    fclose(output);
    free(alphabet);
    free(tape);
    free(states);
    for (int l = 0; l < statesNumber; l++) {
        free(commands[l]);
    }
    free(commands);
    exit(1);
}
