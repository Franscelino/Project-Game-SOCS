#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TRIES 8
#define MAX_WORDS 25
#define MAX_WORD_LEN 50

char words[MAX_WORDS][MAX_WORD_LEN];
int wordCount = 11;

int getRandom(int max) {
    return rand() % max;
}

void printHangman(int tries) {
    printf("   ______\n");
    printf("  |     |\n");
    if (tries >= 1) printf("  |     O\n");
    if (tries == 2) printf("  |     |\n");
    if (tries == 3) printf("  |    /|\n");
    if (tries >= 4) printf("  |    /|\\\n");
    if (tries >= 5) printf("  |     |\n");
    if (tries == 6) printf("  |    / \n");
    if (tries >= 7) printf("  |    / \\\n");
    printf("  |\n");
    printf("__|__\n");
}

void initializeWords() {
    strcpy(words[0], "hangman|The game you are playing right now");
    strcpy(words[1], "computer|An electronic device for processing data");
    strcpy(words[2], "keyboard|An input device for computer");
    strcpy(words[3], "programming|Writing instructions for computers");
    strcpy(words[4], "language|A system of communication");
    strcpy(words[5], "software|Computer programs");
    strcpy(words[6], "developer|Creates software applications");
    strcpy(words[7], "algorithm|A set of rules for problem-solving");
    strcpy(words[8], "variable|A storage location with a symbolic name");
    strcpy(words[9], "function|A block of code that performs a specific task");
    strcpy(words[10], "binus|World Class University");

    wordCount = 11;
}

void displayWords() {
    printf("Words List:\n");
    for (int i = 0; i < wordCount; i++) {
        printf("%d. %s\n", i + 1, words[i]);
    }
}

void addNewWord() {
    if (wordCount >= MAX_WORDS) {
        printf("Word list is full. Cannot add a new word.\n");
        return;
    }

    char newWord[MAX_WORD_LEN];
    char clue[MAX_WORD_LEN];

    printf("Enter the new word: ");
    fgets(newWord, sizeof(newWord), stdin);
    newWord[strcspn(newWord, "\n")] = '\0';

    printf("Enter the clue for the new word: ");
    fgets(clue, sizeof(clue), stdin);
    clue[strcspn(clue, "\n")] = '\0';

    for (int i = 0; i < wordCount; i++) {
        char temp[MAX_WORD_LEN];
        strcpy(temp, words[i]);

        char *token = strtok(temp, "|");
        if (token != NULL && strcmp(token, newWord) == 0) {
            printf("Word '%s' already exists! Cannot add duplicate word.\n", newWord);
            return;
        }
    }

    snprintf(words[wordCount], sizeof(words[wordCount]), "%s|%s", newWord, clue);
    printf("New word added successfully!\n");
    wordCount++;
}

int selectRandomWord() {
    return getRandom(wordCount);
}

void displayWord(char *word, bool *guessed, int len) {
    for (int i = 0; i < len; i++) {
        if (guessed[i])
            printf("%c ", word[i]);
        else if (word[i] == ' ')
            printf("  ");
        else
            printf("_ ");
    }
    printf("\n");
}

bool checkWin(bool *guessed, int len) {
    for (int i = 0; i < len; i++) {
        if (!guessed[i])
            return false;
    }
    return true;
}

void setNewWord(char *word, char *clue) {
    int randomIndex = selectRandomWord();
    char *temp = strtok(words[randomIndex], "|");
    strcpy(word, temp);
    strcpy(clue, strtok(NULL, "|"));
}

int wins = 0;
int losses = 0;

int main() {
    srand(time(NULL));
    char word[MAX_WORD_LEN];
    char clue[MAX_WORD_LEN];

    initializeWords();

    int choice;
    printf("Welcome to Hangman!\n");

    do {
        printf("\nMenu:\n");
        printf("1. Play\n");
        printf("2. Add New Words\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1: {
                printf("\nStarting new game...\n");
                setNewWord(word, clue);
                int len = strlen(word);
                bool guessed[MAX_WORD_LEN] = {false};

                int tries = 0;
                bool gameWon = false;
                char guess;

                printf("\nClue: %s\n", clue);

                while (tries < MAX_TRIES && !gameWon) {
                    printf("\nWord: ");
                    int i;
                    for (i = 0; i < len; i++) {
                        if (guessed[i])
                            printf("%c ", word[i]);
                        else
                            printf("_ ");
                    }

                    printf("\n");
                    printHangman(tries);

                    printf("Enter a letter: ");
                    scanf(" %c", &guess);

                    bool found = false;
                    for (i = 0; i < len; i++) {
                        if (!guessed[i] && tolower(guess) == tolower(word[i])) {
                            guessed[i] = true;
                            found = true;
                        }
                    }

                    if (!found) {
                        tries++;
                        printf("Incorrect guess. Tries left: %d\n", MAX_TRIES - tries);
                    } else {
                        printf("Good guess!\n");
                    }

                    gameWon = checkWin(guessed, len);
                }

                printf("\nWord: ");
                displayWord(word, guessed, len);

                if (gameWon) {
                    printf("\nCongratulations! You guessed the word: %s\n", word);
                    wins ++;
                } else {
                    printf("\nSorry, you didn't guess the word: %s\n", word);
                    losses ++;
                }
                printf("Win : %d\nLose : %d\n", wins, losses);
                break;
            }

            case 2:
                addNewWord();
                break;

            case 3:
                printf("\nExiting the game...");
                printf("\nThank you for playing!\n");
                return 0;

            default:
                printf("\nInvalid choice! Please select a valid option.\n");
                break;
        }
    } while (choice != 3);

    return 0;
}
