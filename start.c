#include "turing.h"
#include <math.h>
#define DEFAULT_SIZE 15

int start(FILE *input, FILE *alphabet, FILE *machine, FILE *output, int flag) {

    // Alphabet
    char *alphabetString = parseStreamLine(alphabet); // Stores alphabet characters
    int alphabetLength = strlen(alphabetString);
    for (int i = 0; i < alphabetLength; i++) {
        if (strchr(alphabetString, alphabetString[i]) != strrchr(alphabetString, alphabetString[i])) {
            printf("Symbols in alphabet input file cannot have duplicate\n");
            exit(1);
        }
    }
    if (strchr(alphabetString, '_') == NULL) {
        alphabetString = strcat(alphabetString, "_");
        alphabetLength = alphabetLength + 1;
    }
    fclose(alphabet);

    // Head
    char *headString = parseStreamLine(input);
    headString[strlen(headString) - 1] = '\0'; // Deleting '\n' from the end of the string
    int headPos = -1; // Stores position of the head, counts from 0
    for (int i = 0; i < strlen(headString); i++) {
        if ((headString[i] != '.') && (headString[i] != 'V')) {
            printf("The problem of determining the position of the head\n");
            exit(1);
        } else if ((headString[i] == 'V') && (headPos > 0)) {
            printf("Tape must contain exactly one head\n");
            exit(1);
        } else if (headString[i] == 'V') {
            headPos = i;
        }
    }
    if (headPos < 0) {
        printf("Tape must contain exactly one head\n");
        exit(1);
    }

    // Initial tape
    char *tape = parseStreamLine(input);
    for (int i = 0; i < strlen(tape); i++) {
        if (strchr(alphabetString, tape[i]) == NULL) {
            printf("Tape must contain only alphabet characters\n");
            exit(1);
        }
    }
    fclose(input);

    // State-machine
    int numberOfStates = 0;
    int maxStatesNumber = DEFAULT_SIZE;
    int *stateNumbers;
    int *stateIndexes; // Start line index of each state column
    struct COMMAND **commands;

    // Memory allocation
    commands = (struct COMMAND **) calloc(DEFAULT_SIZE, sizeof(struct COMMAND *));
    if (commands == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    if (alphabetLength > DEFAULT_SIZE) {
        commands = (struct COMMAND **) realloc(commands, alphabetLength * sizeof(struct COMMAND *));
        if (commands == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }
        for (int i = 0; i < alphabetLength; i++) {
            commands[i] = (struct COMMAND *) calloc(DEFAULT_SIZE, sizeof(struct COMMAND));
            if (commands[i] == NULL) {
                printf("Memory allocation error\n");
                exit(1);
            }
        }
    }
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        commands[i] = (struct COMMAND *) calloc(DEFAULT_SIZE, sizeof(struct COMMAND));
        if (commands[i] == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }
    }
    stateNumbers = (int *) calloc(DEFAULT_SIZE, sizeof(int));
    if (stateNumbers == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stateIndexes = (int *) calloc(DEFAULT_SIZE, sizeof(int));
    if (stateIndexes == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    // Parsing first line of state-machine input data file
    char *firstMachineLine = parseStreamLine(machine);
    firstMachineLine[strlen(firstMachineLine) - 1] = '\0';
    if ((firstMachineLine[0] != '.') && (firstMachineLine[1] != ' ')) {
        printf("Wrong state-machine input data format\n");
        exit(1);
    }
    for (int i = 2; i < strlen(firstMachineLine) - 1; i++) {
        if (((isNumber(firstMachineLine[i]) == -1) && (firstMachineLine[i] != 'q') && (firstMachineLine[i] != '.') &&
             (firstMachineLine[i] != ' ')) ||
            ((firstMachineLine[i] == '.') && (firstMachineLine[i + 1] != ' ') && (firstMachineLine[i + 1] != 'q') &&
             (firstMachineLine[i + 1] != '\0')) ||
            ((firstMachineLine[i] == 'q') && (isNumber(firstMachineLine[i + 1]) == -1)) ||
            ((isNumber(firstMachineLine[i] != -1)) && (isNumber(firstMachineLine[i + 1] == -1)) &&
             (firstMachineLine[i + 1] != '.')) ||
            ((firstMachineLine[i] == ' ') && (firstMachineLine[i + 1] != ' ') && (firstMachineLine[i + 1] != '.') &&
             (firstMachineLine[i + 1] != '\0'))) {
            printf("Wrong state-machine input data format\n");
            exit(1);
        } else if ((firstMachineLine[i] == '.') && (firstMachineLine[i + 1] == 'q')) {
            stateNumbers[numberOfStates] = parseState(firstMachineLine, i + 2);
            stateIndexes[numberOfStates] = i;
            numberOfStates++;
        }
        if (i == maxStatesNumber) {
            maxStatesNumber = maxStatesNumber * 2;
            stateNumbers = (int *) realloc(stateNumbers, maxStatesNumber * sizeof(int));
            if (stateNumbers == NULL) {
                printf("Memory allocation error\n");
                exit(1);
            }
            stateIndexes = (int *) realloc(stateIndexes, maxStatesNumber * sizeof(int));
            if (stateIndexes == NULL) {
                printf("Memory allocation error\n");
                exit(3);
            }
        }
    }

    // Parsing state-machine commands
    for (int i = 0; i < alphabetLength; i++) {
        char *line = parseStreamLine(machine);
        if (line[0] == '\0') {
            printf("State-machine must contain all alphabet symbols in the first column\n");
            exit(1);
        }
        if (i != alphabetLength - 1) {
            line[strlen(line) - 1] = '\0';
        }
        if ((i == alphabetLength - 1) && (line[strlen(line) - 1]) == '\n') {
            printf("State-machine input file cannot have additional line at the end\n");
            exit(12);
        }
        if (line[0] != alphabetString[i]) {
            if (strlen(line) == 0) {
                printf("Empty line in state-machine file if forbidden\n");
            }
            printf("First symbols of lines in state-machine file must have the same sequence "
                   "as the sequence of characters in the alphabet\n");
            exit(1);
        }

        struct COMMAND *lineCommands;
        lineCommands = (struct COMMAND *) calloc(DEFAULT_SIZE, sizeof(struct COMMAND));
        if (numberOfStates > DEFAULT_SIZE) {
            lineCommands = (struct COMMAND *) realloc(lineCommands, numberOfStates * sizeof(struct COMMAND));
        }
        if (lineCommands == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }

        int prev = 1;
        for (int j = 0; j < numberOfStates; j++) {
            int n = stateIndexes[j];
            if ((line[n] == (char) NULL) || (line[n] == '\0')) {
                struct COMMAND command;
                command.newSymbol = '?';
                command.move = '?';
                command.newState = -1;
                lineCommands[j] = command;
                continue;
            }
            for (int k = prev + 1; k < n; k++) {
                if (line[k] != ' ') {
                    printf("The state columns in the input file must have strong structure separated by spaces\n");
                    exit(1);
                }
            }
            lineCommands[j] = parseCommand(line, n, i + 2);
            if ((lineCommands[j].newSymbol == '?') && (lineCommands[j].move == '?') &&
                (lineCommands[j].newState == -1)) {
                continue;
            }
            if (strchr(alphabetString, lineCommands[j].newSymbol) == NULL) {
                printf("New tape symbol must be from alphabet\n");
                exit(1);
            }
            if ((lineCommands[j].move != '.') && (lineCommands[j].move != '>') && (lineCommands[j].move != '<')) {
                printf("Unknown move command\n");
                exit(1);
            }
            int exist = 0;
            for (int k = 0; k < numberOfStates; k++) {
                if (stateNumbers[k] == lineCommands[j].newState) {
                    exist = 1;
                    break;
                }
            }
            if ((lineCommands[j].newState != 0) && (exist == 0)) {
                printf("New state in commands must be present in the first line of state-machine table (it must exist)\n");
                exit(1);
            }
            char *stateNumStr;
            stateNumStr = (char *) calloc(DEFAULT_SIZE, sizeof(char));
            itoa(lineCommands[j].newState, stateNumStr, 10);
            prev = n + 2 + strlen(stateNumStr);
        }
        commands[i] = lineCommands;
    }
    fclose(machine);

    free(headString);
    free(stateIndexes);
    free(firstMachineLine);

    process(alphabetString, headPos, tape, stateNumbers, numberOfStates, commands, flag, output);
    return 0;
}

// Determination of state number
int parseState(char *str, int index) {
    int stateNumber = 0;
    int i = index;
    int rank = 0;
    while (isNumber(str[i]) != -1) {
        rank++;
        i++;
    }
    i = index;
    while (rank > 0) {
        char *string;
        string = (char *) calloc(2, sizeof(char));
        if (string == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }
        string[0] = str[i];
        stateNumber += atoi(string) * pow(10, rank - 1);
        rank--;
        i++;
    }
    if (stateNumber > 0) {
        return stateNumber;
    } else {
        printf("Wrong state number\n");
        exit(1);
    }
}

// Making command from string
struct COMMAND parseCommand(char *str, int index, int line) {
    struct COMMAND command;
    if ((str[index] == ' ') && (str[index + 1] == ' ') && (str[index + 2] == ' ') && (str[index + 3] == ' ')) {
        command.newSymbol = '?';
        command.move = '?';
        command.newState = -1;
        return command;
    }
    command.newSymbol = str[index];
    command.move = str[index + 1];
    if (str[index + 2] != 'q') {
        printf("Wrong state-machine command format\n");
        exit(1);
    }
    int i = index + 3;
    int rank = 0;
    int num = 0;
    if (isNumber(str[i]) == -1) {
        printf("Wrong state-machine command format\n");
        exit(1);
    }
    while ((str[i] != ' ') && (str[i] != '\0')) {
        if (isNumber(str[i]) != -1) {
            rank++;
            i++;
        } else {
            printf("Wrong state number\n");
            exit(1);
        }
    }
    i = index + 3;
    while (rank > 0) {
        char *string;
        string = (char *) calloc(2, sizeof(char));
        if (string == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }
        string[0] = str[i];
        num += atoi(string) * pow(10, rank - 1);
        rank--;
        i++;
    }
    command.newState = num;
    return command;
}
int isNumber(char ch) {
    char num[] = "0123456789";
    if (strchr(num, ch) != NULL) {
        char *string;
        string = (char *) calloc(2, sizeof(char));
        string[0] = ch;
        return atoi(string);
    } else {
        return -1;
    }
}

// Parsing line from stream to string
char *parseStreamLine(FILE *file) {
    char *string;
    int newNumber = DEFAULT_SIZE;
    int total = DEFAULT_SIZE;
    string = (char *) calloc(DEFAULT_SIZE, sizeof(char));
    if (string == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    fgets(string, DEFAULT_SIZE, file);
    char *firstNullSymbol = strchr(string, '\0');
    while (((firstNullSymbol - string + 1) == total) && (string[strlen(string) - 1] != '\n')) {
        newNumber = newNumber * 2;
        char *string1;
        string1 = (char *) calloc(DEFAULT_SIZE, sizeof(char));
        if (string1 == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }
        string1 = (char *) realloc(string1, newNumber * sizeof(char));
        if (string1 == NULL) {
            printf("Memory allocation error\n");
            exit(1);
        }
        char *ret = fgets(string1, newNumber, file);
        if (ret != NULL) {
            string = strcat(string, string1);
        }
        firstNullSymbol = strchr(string, '\0');
        total += (newNumber - 1);
    }
    return string;
}

