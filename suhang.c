#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BULLETS 6
#define WIN_CONDITION 4

typedef struct {
    int bullets[MAX_BULLETS];
    int bulletIndex;
    int playerScore;
    int aiScore;
} Game;

void loadBullets(Game *game, int realBullets) {
    int blanks = MAX_BULLETS - realBullets;
    printf("Game Start! Loaded with %d live rounds and %d blanks.\n", realBullets, blanks);

    int i;
    for (i = 0; i < MAX_BULLETS; i++) {
        game->bullets[i] = i < realBullets ? 1 : 0;
    }
    for (i = MAX_BULLETS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = game->bullets[i];
        game->bullets[i] = game->bullets[j];
        game->bullets[j] = temp;
    }
    game->bulletIndex = 0;
}

int shoot(Game *game) {
    if (game->bulletIndex >= MAX_BULLETS) return -1;
    return game->bullets[game->bulletIndex++];
}

int aiDecision(Game *game) {
    int remainingBullets = MAX_BULLETS - game->bulletIndex;
    int remainingRealBullets = 0;

    for (int i = game->bulletIndex; i < MAX_BULLETS; i++) {
        if (game->bullets[i] == 1) remainingRealBullets++;
    }

    if (remainingRealBullets > remainingBullets / 2) return 0;
    else return 1;
}

void gameLoop() {
    Game game;
    srand(time(NULL));

    while (1) {
        game.playerScore = 0;
        game.aiScore = 0;

        int liveRounds = 3;
        loadBullets(&game, liveRounds);

        printf("=== Game Start ===\n");

        while (game.playerScore < WIN_CONDITION && game.aiScore < WIN_CONDITION) {
            int playerChoice;

            while (1) {
                printf("Your choice (0: Shoot opponent, 1: Shoot self): ");
                scanf("%d", &playerChoice);

                int result = shoot(&game);
                if (result == -1) {
                    printf("All bullets used. Reloading.\n");
                    loadBullets(&game, liveRounds);
                    continue;
                }

                if (playerChoice == 0) {
                    if (result == 1) {
                        game.playerScore++;
                        printf("Player: Live round! Current score - Player: %d, AI: %d\n", game.playerScore, game.aiScore);
                    } else {
                        printf("Player: Blank! No effect.\n");
                    }
                    break;
                } else {
                    if (result == 1) {
                        printf("Player: Self-hit with live round! Current score - Player: %d, AI: %d\n", game.playerScore, game.aiScore);
                    } else {
                        printf("Player: Blank! Extra turn!\n");
                        continue;
                    }
                    break;
                }
            }

            if (game.playerScore >= WIN_CONDITION) {
                printf("Player wins!\n");
                break;
            }

            while (1) {
                int aiChoice = aiDecision(&game);
                printf("AI’s choice: %s\n", aiChoice == 0 ? "Shoot opponent" : "Shoot self");

                int result = shoot(&game);
                if (result == -1) {
                    printf("All bullets used. Reloading.\n");
                    loadBullets(&game, liveRounds);
                    continue;
                }

                if (aiChoice == 0) {
                    if (result == 1) {
                        game.aiScore++;
                        printf("AI: Live round! Current score - Player: %d, AI: %d\n", game.playerScore, game.aiScore);
                    } else {
                        printf("AI: Blank! No effect.\n");
                    }
                    break;
                } else {
                    if (result == 1) {
                        printf("AI: Self-hit with live round! Current score - Player: %d, AI: %d\n", game.playerScore, game.aiScore);
                    } else {
                        printf("AI: Blank! Extra turn!\n");
                        continue;
                    }
                    break;
                }
            }

            if (game.aiScore >= WIN_CONDITION) {
                printf("AI wins!\n");
                break;
            }
        }

        printf("Play again? (1: Yes, 0: No): ");
        int restart;
        scanf("%d", &restart);
        if (!restart) break;
    }
}

int main() {
    gameLoop();
    return 0;
}
