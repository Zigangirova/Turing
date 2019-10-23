#include "turing.h"

void printTape(int headPos, char *tape, FILE *output) {
    int length = strlen(tape);

    for (int i = 0; i < length; i++) {
        if (i == headPos) {
                  fprintf(output, "%c", 'V');
        } else {
                  fprintf(output, "%c", '.');
        }
    }
        fprintf(output, "\n");


      fprintf(output, "%s\n", tape);

    fprintf(output, "\n\n");
}

void printCommand(int currentState, char currentSymbol, struct COMMAND command, FILE *output) {
       fprintf(output, "q%d: %c %c %c q%d\n", currentState, currentSymbol, command.move, command.newSymbol, command.newState);
}

void printStart(FILE *output) {
      fprintf(output, "START\n\n");
}

void printEnd(FILE *output) {
       fprintf(output, "END\n");
}
void printStop(FILE *output) {
        fprintf(output, "FORCED STOP\n");
}
