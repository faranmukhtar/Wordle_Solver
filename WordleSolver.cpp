#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <cmath>

class WordleSolver {
private:
    std::vector<std::string> board;

public:
    WordleSolver() {
        resetBoard();
    }

    std::vector<std::string> letterWords() {
        std::vector<std::string> rows;
        std::fstream file("Wordle_Solver/words.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.size() == 5)
                    rows.push_back(line);
            }
            file.close();
        } else {
            std::cerr << "Unable to open file.\n";
        }
        return rows;
    }

    std::vector<std::string> matcher(const std::vector<std::string>& bigList, const std::vector<std::string>& target) {
        std::vector<std::string> results;

        for (const std::string& word : bigList) {
            bool isMatch = true;
            for (int i = 0; i < 5; i++) {
                if (target[i] != "_" && target[i] != std::string(1, word[i])) {
                    isMatch = false;
                    break;
                }
            }
            if (isMatch) {
                results.push_back(word);
            }
        }

        return results;
    }

    std::string getInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }

    void display(const std::vector<std::string>& currentBoard) {
        std::cout << "[";
        for (size_t i = 0; i < currentBoard.size(); ++i) {
            std::cout << currentBoard[i];
            if (i < currentBoard.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }

    void resetBoard() {
        board.clear();
        for (int i = 0; i < 5; ++i) {
            board.push_back("_");
        }
    }

    int userEntry(int location) {
        std::string initialState = board[location];
        board[location] = "=";
        display(board);
        std::string userInput = getInput("> ");

        if (userInput == " " || userInput.empty()) {
            board[location] = initialState;
            return location + 1;
        } else if (userInput == "0") {
            board[location] = initialState;
            return 5; // Exit editing
        } else if (userInput == "1") {
            board[location] = initialState;
            return std::max(location - 1, 0);
        } else if (userInput.size() == 1 && std::isalpha(userInput[0])) {
            board[location] = userInput;
            return location + 1;
        } else {
            std::cout << "Invalid input. Try again.\n";
            board[location] = initialState;
            return location;
        }
    }

    void initializeBoard() {
        int i = 0;
        while (i < 5) {
            i = userEntry(i);
        }
        display(board);
        std::cout << std::endl;
    }

    std::vector<std::string> mustHave(const std::vector<std::string>& list) {
        std::string must = getInput("Type letters that must be included (in any position): ");
        std::vector<std::string> filtered;
        for (const auto& word : list) {
            bool valid = true;
            for (char ch : must) {
                if (ch != ' ' && word.find(ch) == std::string::npos) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                filtered.push_back(word);
            }
        }
        return filtered;
    }

    std::vector<std::string> removeLetters(const std::vector<std::string>& list, const std::string& badLetters) {
        std::vector<std::string> filtered;
        for (const auto& word : list) {
            bool valid = true;
            for (char ch : badLetters) {
                if (ch != ' ' && word.find(ch) != std::string::npos) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                filtered.push_back(word);
            }
        }
        return filtered;
    }

    void play() {
        resetBoard();
        initializeBoard();

        while (true) {
            std::vector<std::string> rows = letterWords();

            std::string ignore = getInput("Enter letters to exclude: ");
            if (!ignore.empty()) {
                rows = removeLetters(rows, ignore);
            }

            std::vector<std::string> matched = matcher(rows, board);
            matched = mustHave(matched);

            std::cout << "\nPossible matches:\n";
            if (matched.empty()) {
                std::cout << "[]\n";
            } else {
                std::cout << "[";
                for (size_t i = 0; i < std::min(size_t(10), matched.size()); ++i) {
                    std::cout << matched[i];
                    if (i < std::min(size_t(10), matched.size()) - 1) {
                        std::cout << ", ";
                    }
                }
                if (matched.size() > 10)
                    std::cout << ", ... (" << matched.size() << " total)";
                std::cout << "]\n";
            }

            std::string choice = getInput("\nContinue (c), reset (r), or exit (e)? ");
            if (choice == "e") break;
            if (choice == "r") {
                resetBoard();
                initializeBoard();
            }
            if (choice == "c") {
                initializeBoard();
            }
        }
    }
};

int main() {
    WordleSolver solver;
    solver.play();
    return 0;
}

