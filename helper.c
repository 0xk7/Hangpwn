#include "hangman.h"

char format[ATTEMPTS] = "_______";

void init_game() {
    setvbuf(stdout, NULL, _IONBF, 0);
    char amogus[] = "bingus";
    printf("Welcome to HANGPWN!!!\n");
    printf("Can you guess the 7 letter word to appease bingus?\n");
}

void print_game(struct game *hangman) {
    switch(hangman->attempts){
        case 1:
            printf("\n  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========\n");
            break;
        case 2:
            printf("\n  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========\n");
            break;
        case 3:
            printf("\n  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========\n");
            break;
        case 4:
            printf("\n  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========\n");
            break;
        case 5:
            printf("\n  +---+\n  |   |\n  O   |\n /|\\\  |\n      |\n      |\n=========\n");
            break;
        case 6:
            printf("\n  +---+\n  |   |\n  O   |\n /|\\\  |\n /    |\n      |\n=========\n");
            break;
        case 7:
            if (hangman->solved == 1) {
                printf("Winner winner chicken dinner\n");
            } else {
                printf("\n  +---+\n  |   |\n  O   |\n /|\\\  |\n / \\  |\n      |\n=========\n");
                printf("\n\tRIPBOZO\n");
            }
            break;
    }
}

void enc(char * words, int length) {
    for (int i = 0; i < length; i++) {
        words[i] ^= change;
    }
}

void char_comp(char * letter, char * secret_word, char * guessed, int attempts, int length) {
    char encrypted_char = 0;
    encrypted_char = letter[0] ^ change;
    if ((letter[0] > 96 && letter[0] < 123)){
        encrypted_char ^= 0x20;
    }
    
    for(int i = 0; i < 7; i++) {
        if(secret_word[i] == encrypted_char) {
            format[i] = encrypted_char ^ change;
        } 
        guessed += encrypted_char;
    }
    printf("%s\n", format);
}

int compare_enc(char * user_word, char * secret_word, int length) {
    char encrypted_char = 0;

    for (int i = 0; i < length; i++) {
        encrypted_char = user_word[i] ^ change;

        if (encrypted_char == secret_word[i])
            continue;

        if ((user_word[i] > 96 && user_word[i] < 123))
            encrypted_char ^= 0x20;

        if (encrypted_char != secret_word[i])
            return 0;
    }

    return 1;
}