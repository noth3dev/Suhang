#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#define MAX_HITS 3  

int gun[8] = {0};           
int chambers = 5;           
int bullet_count = 2;       
int current_position = 0;   
int player_hits = 0;        
int dealer_hits = 0;        
int round_number = 1;       
int bullets_fired = 0;      

void initialize_game();
void reset_gun();
int fire_gun();
int player_turn();
int dealer_turn(int player_previous_choice);
void slow_print_line(const char* message);

void reset_gun() {

    if (round_number == 1) {
        chambers = 5;
        bullet_count = 2;
    } else if (round_number == 2) {
        chambers = 7;
        bullet_count = 3;
    } else if (round_number == 3) {
        chambers = 8;
        bullet_count = 4;
    }

    for (int i = 0; i < chambers; i++) {
        gun[i] = 0;  
    }

    for (int i = 0; i < bullet_count; i++) {
        int pos;
        do {
            pos = rand() % chambers;
        } while (gun[pos] == 1); 
        gun[pos] = 1;
    }

    current_position = 0;  
    bullets_fired = 0;     
    printf("\n라운드 %d: %d발 중 %d발이 실탄입니다.\n", round_number, chambers, bullet_count);
}

int fire_gun() {
    int result = gun[current_position];  
    current_position = (current_position + 1) % chambers;  
    bullets_fired++;  
    return result;
}

void slow_print_line(const char* message) {
    printf("%s\n", message);
    sleep(1);  
}

int player_turn() {
    int choice;
    int game_over = 0;

    do {
        printf("당신의 턴입니다. 자신에게 쏘려면 1, 딜러에게 쏘려면 2를 입력하세요: ");
        scanf("%d", &choice);

        if (choice == 1) {
            slow_print_line("당신은 자신에게 총을 쐈습니다...");
            if (fire_gun()) {
                slow_print_line("총알이 발사되었습니다! 당신이 총을 맞았습니다.");
                player_hits++;
                if (player_hits == MAX_HITS) {
                    slow_print_line("당신은 세 번 맞아 죽었습니다.");
                    return 1; 
                } else {
                    printf("당신은 %d번 맞았습니다. 아직 살아있습니다.\n", player_hits);
                }
            } else {
                slow_print_line("총알이 발사되지 않았습니다. 다시 시도할 수 있습니다.");
                game_over = 0;  
            }
        } else {
            slow_print_line("당신은 딜러에게 총을 쐈습니다...");
            if (fire_gun()) {
                slow_print_line("총알이 발사되었습니다! 딜러가 총을 맞았습니다.");
                dealer_hits++;
                if (dealer_hits == MAX_HITS) {
                    slow_print_line("딜러는 세 번 맞아 죽었습니다.");
                    return 1; 
                } else {
                    printf("딜러는 %d번 맞았습니다. 아직 살아있습니다.\n", dealer_hits);
                }
            } else {
                slow_print_line("총알이 발사되지 않았습니다.");
                game_over = 0;  
            }
        }

        if (bullets_fired >= chambers) {
            slow_print_line("총알이 모두 소진되었습니다. 다음 라운드로 넘어갑니다.");
            return 1;  
        }

    } while (choice == 1 && game_over == 0);  

    return 0;
}

int dealer_turn(int player_previous_choice) {
    int choice;
    int game_over = 0;

    int remaining_bullets = 0;
    for (int i = 0; i < chambers; i++) {
        if (gun[i] == 1) remaining_bullets++;
    }

    double fire_probability = (double)remaining_bullets / (chambers - current_position);

    if (fire_probability > 0.5) {

        choice = 2;  
    } else {

        choice = 1;  
    }

    do {
        if (choice == 1) {
            slow_print_line("딜러는 자신에게 총을 쐈습니다...\n");
            if (fire_gun()) {
                slow_print_line("총알이 발사되었습니다! 딜러가 총을 맞았습니다.");
                dealer_hits++;
                if (dealer_hits == MAX_HITS) {
                    slow_print_line("딜러는 세 번 맞아 죽었습니다.");
                    return 1; 
                } else {
                    printf("딜러는 %d번 맞았습니다. 아직 살아있습니다.\n", dealer_hits);
                }
            } else {
                slow_print_line("총알이 발사되지 않았습니다. 딜러가 다시 시도할 수 있습니다.");
                game_over = 0;  
            }
        } else {
            slow_print_line("딜러는 당신에게 총을 쐈습니다...\n");
            if (fire_gun()) {
                slow_print_line("총알이 발사되었습니다! 당신이 총을 맞았습니다.");
                player_hits++;
                if (player_hits == MAX_HITS) {
                    slow_print_line("당신은 세 번 맞아 죽었습니다.");
                    return 1; 
                } else {
                    printf("당신은 %d번 맞았습니다. 아직 살아있습니다.\n", player_hits);
                }
            } else {
                slow_print_line("총알이 발사되지 않았습니다.");
                game_over = 0;  
            }
        }

        if (bullets_fired >= chambers) {
            slow_print_line("총알이 모두 소진되었습니다. 다음 라운드로 넘어갑니다.");
            return 1;  
        }

        if (game_over == 0 && choice == 1) {

            choice = 1;
        }

    } while (choice == 1 && game_over == 0);  

    return 0;
}

int main() {
    srand(time(NULL));  

    while (round_number <= 3) {  
        reset_gun();

        int game_over = 0;

        while (!game_over) {
            game_over = player_turn();
            if (game_over) break;

            game_over = dealer_turn(0);
            if (game_over) break;
        }

        if (round_number < 3) {
            slow_print_line("다음 라운드로 넘어갑니다...");
            round_number++;
        } else {
            slow_print_line("게임이 종료되었습니다.");
        }
    }

    return 0;
}
