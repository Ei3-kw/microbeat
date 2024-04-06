#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DELAY 30000
#define WIDTH 230
#define HEIGHT 120
#define FONT_PX 1
#define LETTER "1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm"
#define NUM_LETTERS 44

int main() {
    // Initialise ncurses
    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);
    srand(time(NULL));

    // Create an array to store the screen
    chtype screen[HEIGHT / FONT_PX][WIDTH / FONT_PX];
    memset(screen, ' ', sizeof(screen));

    // Create an array of characters to display
    chtype letters[NUM_LETTERS];
    for (int i = 0; i < NUM_LETTERS; i++) {
        letters[i] = LETTER[i] | COLOR_PAIR(1);
    }

    // Initialise drop positions
    int drops[WIDTH / FONT_PX];
    for (int i = 0; i < WIDTH / FONT_PX; i++) {
        drops[i] = 0;
    }
    int count = 0;

    while (1) {
        count++;
        // Check for quit event
        int ch = getch();
        if (ch == ' ') {
            break;
        }

        // Clear the screen
        if (count % 4 == 0) {
            memset(screen, ' ', sizeof(screen));
        }

        // Display the rain effect
        for (int i = 0; i < WIDTH / FONT_PX; i++) {
            screen[drops[i]][i] = letters[rand() % NUM_LETTERS];
            drops[i] += 1;
            if (drops[i] >= HEIGHT / FONT_PX || rand() > 0.95 * RAND_MAX) {
                drops[i] = 0;
            }
        }

        // Refresh the screen
        for (int y = 0; y < HEIGHT / FONT_PX; y++) {
            for (int x = 0; x < WIDTH / FONT_PX; x++) {
                mvaddch(y * FONT_PX, x * FONT_PX, screen[y][x]);
            }
        }
        refresh();
        usleep(DELAY);
    }

    endwin();
    return 0;
}


