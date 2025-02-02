#include <stdio.h>

#define RESET "\033[0m"
#define MAX_COLORS 256

void print_color(int code, const char* text) {
    printf("\033[38;5;%dm%-6s\033[0m", code, text); // Foreground color
}

void print_background_color(int code, char* text) {
    printf("\033[48;5;%dm %s \033[0m", code, text); // Background color
}
void print_color_matrix() {
    printf("\nColor matrix (Foreground):\n");
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            int code = i * 16 + j;
            print_background_color(code, "");
            if (code >= 15 && (code - 15) % 6 == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
}




int main() {
    printf("ANSI Colors and their codes:\n");

    for (int i = 0; i < 8; i++) {
        print_color(i, "Code");
        printf("\n");
    }

    printf("\n256 ANSI colors (Foreground colors):\n");
    for (int i = 0; i < 256; i++) {
        print_color(i, "Code");
        if (i % 16 == 15) {
            printf("\n");
        }
    }

    print_color_matrix();

    return 0;
}

