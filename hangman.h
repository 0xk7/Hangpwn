#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 1
#define ATTEMPTS 7
#define abs "v}zsuag" 
#define change 0x34

typedef struct game {
    short attempts;
    char words[ATTEMPTS][LENGTH];
    char game_over;
    char solved;
} game;

void init_game();
void print_game(struct game *);
void char_comp(char *, char *, char *, int, int);
void enc(char *, int);
int compare_enc(char *, char *, int);