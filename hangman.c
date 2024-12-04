#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ATTEMPTS 6
#define MAX_WORD_LENGTH 100
#define MAX_INCORRECT_GUESSES 26

typedef struct {
    char *wordToGuess;
    char *guessedWord;
    int attemptsLeft;
    char incorrectGuesses[MAX_INCORRECT_GUESSES];
    int incorrectGuessCount;
} HangmanGame;

void initializeGame(HangmanGame *game, const char *word) {
    game->wordToGuess = strdup(word);
    game->attemptsLeft = MAX_ATTEMPTS;
    game->incorrectGuessCount = 0;
    game->guessedWord = (char *)malloc(strlen(word) + 1);
    
    for (size_t i = 0; i < strlen(word); i++) {
        game->guessedWord[i] = '_';
    }
    game->guessedWord[strlen(word)] = '\0';
}

int isWordGuessed(HangmanGame *game) {
    return strcmp(game->wordToGuess, game->guessedWord) == 0;
}

void updateGuessedWord(HangmanGame *game, char guess) {
    for (size_t i = 0; i < strlen(game->wordToGuess); i++) {
        if (game->wordToGuess[i] == guess) {
            game->guessedWord[i] = guess;
        }
    }
}

void playGame(HangmanGame *game) {
    char guess;
    printf("Welcome to Hangman!\n");

    while (game->attemptsLeft > 0 && !isWordGuessed(game)) {
        printf("\nWord to guess: %s\n", game->guessedWord);
        printf("Incorrect guesses: ");
        for (int i = 0; i < game->incorrectGuessCount; i++) {
            printf("%c ", game->incorrectGuesses[i]);
        }
        printf("\nAttempts left: %d\n", game->attemptsLeft);
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (strchr(game->wordToGuess, guess)) {
            updateGuessedWord(game, guess);
        } else {
            int alreadyGuessed = 0;
            for (int i = 0; i < game->incorrectGuessCount; i++) {
                if (game->incorrectGuesses[i] == guess) {
                    alreadyGuessed = 1;
                    break;
                }
            }
            if (!alreadyGuessed) {
                game->incorrectGuesses[game->incorrectGuessCount++] = guess;
                game->attemptsLeft--;
            }
        }
    }

    if (isWordGuessed(game)) {
        printf("\nCongratulations! You've guessed the word: %s\n", game->wordToGuess);
    } else {
        printf("\nGame Over! The word was: %s\n", game->wordToGuess);
    }
}

char **loadWordsFromFile(const char *filename, int *wordCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error reading the words file.\n");
        *wordCount = 0;
        return NULL;
    }

    char **words = malloc(sizeof(char *) * 100);
    char line[MAX_WORD_LENGTH];
    *wordCount = 0;

    while (fgets(line, MAX_WORD_LENGTH, file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        words[*wordCount] = strdup(line);
        (*wordCount)++;
    }
    fclose(file);
    return words;
}

void freeWords(char **words, int wordCount) {
    for (int i = 0; i < wordCount; i++) {
        free(words[i]);
    }
    free(words);
}

int main() {
    int wordCount;
    char **words = loadWordsFromFile("words.txt", &wordCount);

    if (wordCount == 0) {
        printf("No words found. Please ensure 'words.txt' has words listed.\n");
        return 1;
    }

    srand(time(0));
    int randomIndex = rand() % wordCount;

    HangmanGame game;
    initializeGame(&game, words[randomIndex]);
    playGame(&game);

    free(game.wordToGuess);
    free(game.guessedWord);
    freeWords(words, wordCount);
    return 0;
}

