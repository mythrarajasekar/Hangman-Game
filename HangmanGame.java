import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class HangmanGame {
    private static final int MAX_ATTEMPTS = 6;
    private String wordToGuess;
    private char[] guessedWord;
    private int attemptsLeft;
    private ArrayList<Character> incorrectGuesses;

    public HangmanGame(List<String> words) {
        Random random = new Random();
        wordToGuess = words.get(random.nextInt(words.size())).toLowerCase();
        guessedWord = new char[wordToGuess.length()];
        attemptsLeft = MAX_ATTEMPTS;
        incorrectGuesses = new ArrayList<>();

        for (int i = 0; i < guessedWord.length; i++) {
            guessedWord[i] = '_';
        }
    }

    public void play() {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("Welcome to Hangman!");
        
        while (attemptsLeft > 0 && !isWordGuessed()) {
            System.out.println("\nWord to guess: " + String.valueOf(guessedWord));
            System.out.println("Incorrect guesses: " + incorrectGuesses);
            System.out.println("Attempts left: " + attemptsLeft);
            System.out.print("Enter a letter: ");
            char guess = scanner.nextLine().toLowerCase().charAt(0);

            if (wordToGuess.indexOf(guess) >= 0) {
                updateGuessedWord(guess);
            } else {
                if (!incorrectGuesses.contains(guess)) {
                    incorrectGuesses.add(guess);
                    attemptsLeft--;
                }
            }
        }
        
        if (isWordGuessed()) {
            System.out.println("\nCongratulations! You've guessed the word: " + wordToGuess);
        } else {
            System.out.println("\nGame Over! The word was: " + wordToGuess);
        }
        
        scanner.close();
    }

    private boolean isWordGuessed() {
        for (char c : guessedWord) {
            if (c == '_') {
                return false;
            }
        }
        return true;
    }

    private void updateGuessedWord(char guess) {
        for (int i = 0; i < wordToGuess.length(); i++) {
            if (wordToGuess.charAt(i) == guess) {
                guessedWord[i] = guess;
            }
        }
    }

    public static List<String> loadWordsFromFile(String fileName) {
        try {
            return Files.readAllLines(Paths.get(fileName));
        } catch (IOException e) {
            System.out.println("Error reading the words file.");
            return new ArrayList<>(); // Return an empty list if there's an error
        }
    }

    public static void main(String[] args) {
        List<String> words = loadWordsFromFile("words.txt");
        if (words.isEmpty()) {
            System.out.println("No words found. Please ensure 'words.txt' has words listed.");
            return;
        }
        
        HangmanGame game = new HangmanGame(words);
        game.play();
    }
}
