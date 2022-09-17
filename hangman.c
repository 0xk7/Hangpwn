#include "hangman.h"

void main() {
    init_game();

    game state = {};
    char buffer[16] = {};
    char guess_buffer[16] = {};

    char guessed[ATTEMPTS] = {};

    while (!(state.game_over || state.solved)) {
        print_game(&state);
        
        printf("Enter letter: ");
        if (!fgets(buffer, sizeof(buffer), stdin)){
            return;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) != LENGTH) {
            printf("\n\tINVALID INPUT\n");
            continue;
        }

        char_comp(buffer, abs, guessed, (int)state.attempts, LENGTH);

        if (state.attempts + 1 == ATTEMPTS) {
            state.game_over = 1;
        }

        printf("FINAL GUESS\nEnter word: ");
        if (!fgets(guess_buffer, sizeof(guess_buffer), stdin)){
            return;
        }

        if (compare_enc(guess_buffer, abs, ATTEMPTS)){
            state.solved = 1;
        }

        strcpy(state.words[state.attempts], buffer);
        enc(state.words[state.attempts], LENGTH);
        state.attempts++;
    }

    print_game(&state);
}