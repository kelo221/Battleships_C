#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTALSHIPCOUNT 18
#define savelenght 100
#pragma warning(disable:4996)


struct Fleet {
    int carrier_count;
    int battleship_count;
    int cruiser_count;
    int destroyer_count;
    int submarine_count;
    int ships[10][10];
    int ships_result[10][10];
    char *name;
};

void reset(struct Fleet *p) {
    p->carrier_count = 1;
    p->battleship_count = 1;
    p->cruiser_count = 1;
    p->destroyer_count = 2;
    p->submarine_count = 2;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            p->ships[i][j] = 32;
            p->ships_result[i][j] = 32;
        }
    }

}

void savetofile(char *who, char *what) {

    errno_t err;
    FILE *fp = 0;


    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);


    char str[savelenght];
    sprintf(str, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    if ((err = fopen_s(&fp, "log.bin", "ab")) != 0) {
        fprintf(stderr, "cannot open file '%s': %s\n",
                "log.bin", strerror(err));

    } else {
        fwrite(str, 8, 1, fp);
        fwrite(" ", 1, 1, fp);
        fwrite(who, sizeof(who), 1, fp);
        fwrite(what, sizeof(what), 3, fp);
        fwrite("\0", 1, 1, fp);
        fwrite("\n", 1, 1, fp);

    }

    fclose(fp);
}

int useriput() {

    int option, temp, status;
    status = scanf("%1d", &option);
    while (status != 1) {

        //clear buffer
        fseek(stdin, 0, SEEK_END);

        while ((temp = getchar()) != EOF && temp != '\n');
        printf("Incorrect input (Code G)\n");
        status = scanf("%1d", &option);
    }

    return option;
}

char numeraltoalpha(int numeral) {
    switch (numeral) {
        case 0:
            return 'A';
        case 1:
            return 'B';
        case 2:
            return 'C';
        case 3:
            return 'D';
        case 4:
            return 'E';
        case 5:
            return 'F';
        case 6:
            return 'G';
        case 7:
            return 'H';
        case 8:
            return 'I';
        case 9:
            return 'J';
        default:
            return ' ';
    }
}

int alphatonumeral(char const *numeral) {
    switch (*numeral) {
        case 'A':
            return 0;
        case 'B':
            return 1;
        case 'C':
            return 2;
        case 'D':
            return 3;
        case 'E':
            return 4;
        case 'F':
            return 5;
        case 'G':
            return 6;
        case 'H':
            return 7;
        case 'I':
            return 8;
        case 'J':
            return 9;
        default:
            return 100;
    }
}

void drawmap(int array[10][10]) {
    printf("\n    ");
    for (int i = 0; i < 10; ++i) {
        printf("[%d] ", i + 1);
    }
    for (int i = 0; i < 10; ++i) {
        printf("\n[%c] ", numeraltoalpha(i));
        for (int j = 0; j < 10; ++j) {
            printf("[%c] ", array[i][j]);
        }
    }
    printf("\n\n");
}

bool placement(struct Fleet *player, int x, int y, int size, int direction) {

    //1=up 2i=right 3= down 4=left

    //printf("x %d, y %d, size %d dir %d", x, y, size, direction);
    //TODO fix left and right detection, do addional bug testing

    //UP
    if (direction == 1) {

        int sub = y - size;
        //   printf("1\n");
        // printf("\nsubtraction : %d\n", sub);
        if (sub <= -2) {
            puts("out of bounds\n");
            return false;
        }
        for (int i = 0, j = y, tmp = size, spot; i < tmp; ++i) {
            spot = player->ships[j][x];
            j--;
            if (spot == '#') {
                printf("One of the spots was occupied\n");
                return false;
            }
        }
        while (size != 0) {

            char str[savelenght];
            sprintf(str, " placed ship at %d,%d", y, x);
            savetofile(player->name, str);

            player->ships[y][x] = '#';
            y--;
            size--;
        }
        drawmap(player->ships);
        return true;
    }
    //RIGHT
    if (direction == 2) {

        int sub = x + size;
        //    printf("1\n");
        //   printf("\nsubtraction : %d\n", sub);
        if (sub >= 11) {
            puts("out of bounds\n");
            return false;
        }
        for (int i = 0, j = x, tmp = size, spot; i < tmp; ++i) {
            spot = player->ships[y][j];
            j++;
            printf("i got this %c [%d][%d]", spot, x, j);
            if (spot == '#') {
                printf("One of the spots was occupied\n");
                return false;
            }
        }
        while (size != 0) {

            char str[savelenght];
            sprintf(str, " placed ship at %d,%d", y, x);
            savetofile(player->name, str);

            player->ships[y][x] = '#';
            x++;
            size--;
        }
        drawmap(player->ships);
        return true;
    }
    //DOWN
    if (direction == 3) {

        int add = y + size;
        //    printf("1\n");
        //   printf("\naddition : %d\n", add);
        if (add >= 11) {
            puts("out of bounds\n");
            return false;
        }
        for (int i = 0, j = y, tmp = size, spot; i < tmp; ++i) {
            spot = player->ships[j][x];
            j++;
            if (spot == '#') {
                printf("One of the spots was occupied\n");
                return false;
            }
        }
        while (size != 0) {

            char str[savelenght];
            sprintf(str, " placed ship at %d,%d", y, x);
            savetofile(player->name, str);

            player->ships[y][x] = '#';
            y++;
            size--;
        }
        drawmap(player->ships);
        return true;
    }

    //LEFT
    if (direction == 4) {

        int sub = x - size;
        //      printf("1\n");
        //       printf("\nsubtraction : %d\n", sub);
        if (sub < -1) {
            puts("out of bounds\n");
            return false;
        }
        for (int i = 0, j = x, tmp = size, spot; i < tmp; ++i) {
            spot = player->ships[y][j];
            j--;
            printf("i got this %c [%d][%d]", spot, x, j);
            if (spot == '#') {
                printf("One of the spots was occupied\n");
                return false;
            }
        }
        while (size != 0) {

            char str[savelenght];
            sprintf(str, " placed ship at %d,%d", y, x);
            savetofile(player->name, str);

            player->ships[y][x] = '#';
            x--;
            size--;
        }
        drawmap(player->ships);
        return true;
    }
    // printf("4\n");
    return true;


}

bool initships(struct Fleet *player, int howmany, char *type, int size) {

    while (1) {
        printf("Place %d %s(s), it has the size of %d units.\n", howmany, type, size);
        printf("Please enter an option:  \n1. Place ships"
               "\n2. Show ship information  \n3. Show currently placed ships\n4. Help\n5. Skip current ship\n");


        ///TEST
        //player->ships[3][3] = '#';
        ///TEST

        int option = useriput();
        if (option == 1) {
            int x = 0;
            int y = 0;

            while (1) {
                char tmp;


                //clear buffer
                fseek(stdin, 0, SEEK_END);


                while (true) {
                    //clear buffer
                    fseek(stdin, 0, SEEK_END);

                    printf("Enter coordinates EX. 1 A (space in between and a capital character) \n");
                    if (scanf("%2d %1c", &x, &tmp))
                        break;
                    else
                        printf("Incorrect input (Code Z)\n");
                }

                x--;
                y = alphatonumeral(&tmp);


                if (x < 10 && x >= 0 && y < 10 && y >= 0)
                    break;
                else
                    printf("Incorrect input (Code X)\n");
            }

            int direction = 99;

            while (1) {

                printf("Please input a direction for the ship to be placed\n"
                       "Currently selected coordinates: %d %c\n"
                       "1. Up 2. Right 3. Down 4. Left\n\n"
                       "If you wish to print the map input 5.\n"
                       "", x + 1, numeraltoalpha(y));

                direction = useriput();

                if (direction == 5)
                    drawmap(player->ships);
                else if (direction < 5 && direction > 0)
                    break;
                else
                    printf("Incorrect input. (Code V)\n");
            }

            //clear buffer
            fseek(stdin, 0, SEEK_END);


            while (1) {
                if (!placement(player, x, y, size, direction)) {
                    //puts("did not leave");
                    break;
                } else {
                    //ship placed successfully
                    printf("Ship placed\n");
                    drawmap(player->ships);
                    return true;
                }
            }
        }
        if (option == 2) {
            printf("%5s\t%-16s\t%5s\n", "Count", "Ship", "Size");
            printf("%5d\t%-16s\t%5d\n", player->carrier_count, "Aircraft Carrier", 5);
            printf("%5d\t%-16s\t%5d\n", player->battleship_count, "Battleship", 4);
            printf("%5d\t%-16s\t%5d\n", player->cruiser_count, "Cruiser", 3);
            printf("%5d\t%-16s\t%5d\n", player->destroyer_count, "Destroyer", 2);
            printf("%5d\t%-16s\t%5d\n", player->submarine_count, "Submarine", 1);
            printf("\n\n");
        } else if (option == 3) {
            drawmap(player->ships);
        } else if (option == 4) {
            printf("You will be asked coordinates where the head of the ship will be placed,\n");
            printf("and you will have to choose to which direction the rest of the ship will be facing.\n");
            printf("EX. A.1\n");
            printf("Next you will be given a choice of possible direction based on the available slots.\n\n");
        } else if (option == 5) {
            return false;
        } else
            printf("Incorrect input (Code Q)\n");

        if (player->carrier_count == 0 && player->battleship_count == 0 && player->cruiser_count == 0 &&
            player->destroyer_count == 0 && player->submarine_count == 0)
            break;
    }
    // printf("IVE BROKEN OUT OF THE MAIN LOOP\n");
    return true;
}

void init(struct Fleet *player) {
    savetofile(player->name, " started his turn.\n");

    while (1) {

        printf("Please enter an option:  \n1. Place ships (resets map)"
               "\n2. Show currently placed ships \n3. Pass your turn\n");
        int option = useriput();
        if (option == 1) {
            reset(player);
            while (1) {
                printf("\n");
                if (player->carrier_count != 0)
                    if (initships(player, player->carrier_count, "Aircraft Carrier", 5) && player->carrier_count != 0)
                        player->carrier_count--;
                if (player->battleship_count != 0)
                    if (initships(player, player->battleship_count, "Battleship", 4) && player->battleship_count != 0)
                        player->battleship_count--;
                if (player->cruiser_count != 0)
                    if (initships(player, player->cruiser_count, "Cruiser", 3) && player->cruiser_count != 0)
                        player->cruiser_count--;
                if (player->destroyer_count != 0)
                    if (initships(player, player->destroyer_count, "Destroyer", 2) && player->destroyer_count != 0)
                        player->destroyer_count--;
                if (player->destroyer_count != 0)
                    if (initships(player, player->destroyer_count, "Destroyer", 2) && player->destroyer_count != 0)
                        player->destroyer_count--;
                if (player->submarine_count != 0)
                    if (initships(player, player->submarine_count, "Submarine", 1) && player->submarine_count != 0)
                        player->submarine_count--;
                if (player->submarine_count != 0)
                    if (initships(player, player->submarine_count, "Submarine", 1) && player->submarine_count != 0)
                        player->submarine_count--;
                //reduces the count of remaining unplaced ships only when the function returns true and
                //the value is not already 0
                //if the user wants to abort the placement at any point he has to go though the loop again
                //and is only allowed to exit once all of the count of the ships is set to 0
                printf("\nRemaining ship(s):\n"
                       "Carriers: %d\nBattleships: %d\nCruisers: %d\nDestroyers: %d\nSubmarines: %d \n",
                       player->carrier_count, player->battleship_count,
                       player->cruiser_count, player->destroyer_count, player->submarine_count);
                if (player->carrier_count == 0 && player->battleship_count == 0 && player->cruiser_count == 0 &&
                    player->destroyer_count == 0 && player->submarine_count == 0)
                    break;
            }
        } else if (option == 2)
            drawmap(player->ships);
        else if (option == 3) {
            if (player->carrier_count == 0 && player->battleship_count == 0 && player->cruiser_count == 0 &&
                player->destroyer_count == 0 && player->submarine_count == 0) {
                savetofile(player->name, " ended their turn.\n\n");
                break;
            } else
                printf("\nCannot exit. Remaining ship(s):\n"
                       "Carriers: %d\nBattleships: %d\nCruisers: %d\nDestroyers: %d\nSubmarines: %d \n",
                       player->carrier_count, player->battleship_count,
                       player->cruiser_count, player->destroyer_count, player->submarine_count);
        } else
            printf("Incorrect input (Code P)\n");


    }
}

void fire(struct Fleet *attacker, struct Fleet *defender, int *defenderships) {

    int x = 0;
    int y = 0;
    while (1) {
        printf("Please enter an option:  \n1. Show your own guesses\n2. Show your own ships"
               "\n3. Take a guess\n");
        int option = useriput();

        if (option == 1) {
            drawmap(attacker->ships_result);
        }


        if (option == 2) {
            drawmap(attacker->ships);
        }


        if (option == 3) {
            while (1) {

                //clear buffer
                fseek(stdin, 0, SEEK_END);
                printf("Take a guess where the opponent has placed their ships! 'X' marks hit and '/' a miss\n");

                char tmp;

                while (true) {
                    //clear buffer
                    fseek(stdin, 0, SEEK_END);

                    printf("Enter coordinates EX. 1 A (space in between and a capital character) \n");
                    if (scanf("%2d %1c", &x, &tmp))
                        break;

                    printf("Incorrect input (Code U)\n");
                }

                x--;
                y = alphatonumeral(&tmp);


                if (x < 10 && x >= 0 && y < 10 && y >= 0)
                    break;
                else
                    printf("Incorrect input (Code E)\n");
            }

            if (defender->ships[y][x] == '#') {

                if (attacker->ships_result[y][x] == 'X') {
                    printf("You have already shot this location before.\n");

                    char str[savelenght];
                    sprintf(str, " shot at %d,%d, missed", y, x);
                    savetofile(attacker->name, str);

                    break;
                }

                char str[savelenght];
                sprintf(str, " shot at %d,%d, hit", y, x);
                savetofile(attacker->name, str);

                printf("You hit your opponents ship!\n");
                attacker->ships_result[y][x] = 'X';
                *defenderships -= 1;
                break;

            } else {

                char str[savelenght];
                sprintf(str, " shot at %d,%d, missed", y, x);
                savetofile(attacker->name, str);

                printf("You missed.\n");
                attacker->ships_result[y][x] = '/';
                break;
            }

        }

    }
    printf("\n");
}

void checklog() {

    errno_t err;
    FILE *fp;

    if ((err = fopen_s(&fp, "log.bin", "rb")) != 0) {
        printf("\nNo log file found.\n");
        fprintf(stderr, "cannot open file '%s': %s\n",
                "log.bin", strerror(err));
    } else {

        printf("Printing log...\n\n");
        int c = getc(fp);
        while (c != EOF) {
            printf("%c", c);
            c = getc(fp);
        }
    }
    fclose(fp);
}


int main() {

    //init the ship count and other values
    struct Fleet p1;
    reset(&p1);
    p1.name = "Player 1";

    printf("\n%25s\n\n", "Battleships");
    printf("Input 1 to start the game.\nInput 2 to print out the log file.\nStarting the game will clear the log file.\n");

    while (1) {
        int opt = useriput();
        if (opt == 2) {
            checklog();
            break;
        } else
            break;
    }

    printf("\n\n");

    //clearing the old file
    fclose(fopen("log.bin", "wb"));


    printf("Player 1 arrange you battleships\n\n");
    init(&p1);

    fseek(stdin, 0, SEEK_END);


    //init the ship count and other values
    struct Fleet p2;
    reset(&p2);
    p2.name = "Player 2";


    printf("Player 2 arrange you battleships\n\n");
    init(&p2);

    int p1total = TOTALSHIPCOUNT, p2total = TOTALSHIPCOUNT;

    while (1) {

        printf("Player one has %d ships remaining.\nPlayer two has %d ships remaining.\n", p1total, p2total);

        printf("Player one's turn\n");
        fire(&p1, &p2, &p2total);

        if (p2total == 0) {
            printf("Player 1 has won");
            savetofile(p1.name, " has won");
            break;
        }
        if (p1total == 0) {
            printf("Player 2 has won");
            savetofile(p2.name, " has won");
            break;
        }

        printf("Player two's turn\n");
        fire(&p2, &p1, &p1total);

        if (p2total == 0) {
            printf("Player 1 has won");
            savetofile(p1.name, " has won");
            break;
        }
        if (p1total == 0) {
            printf("Player 2 has won");
            savetofile(p2.name, " has won");
            break;
        }


    }

    return 0;


}
