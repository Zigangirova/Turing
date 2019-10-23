#ifndef TURING_TURING_H
#define TURING_TURING_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct COMMAND {
    char newSymbol;
    char move;
    int newState;
};

int process(char *alphabet, int headPos, char *tape, int *states, int statesNumber, struct COMMAND **commands,
            int flag, FILE *output);

int start(FILE *inputTape, FILE *alphabet, FILE *machine, FILE *output, int flag);

int isNumber(char ch);


int parseState(char *str, int index);

char *parseStreamLine(FILE *file);

struct COMMAND parseCommand(char *str, int index, int line);

void printTape(int headPos, char *tape, FILE *output);

void printCommand(int currentState, char currentSymbol, struct COMMAND command, FILE *output);

void printStart(FILE *output);

void printEnd(FILE *output);

void printStop(FILE *output);

#endif //TURING_TURING_H
