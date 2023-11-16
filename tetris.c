#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int encode_state(int *state, int width, int base) {
    int encoded = 0;
    for (int i = 0; i < width; ++i) {
        encoded *= base;
        encoded += state[i];
    }
    return encoded;
}

void decode_state(int encoded, int *state, int width, int base) {
    for (int i = width - 1; i >= 0; --i) {
        state[i] = encoded % base;
        encoded /= base;
    }
}

double compute_probability(int width, int height, int lines_to_win, int *state, double *memo, int base) {
    int encoded = encode_state(state, width, base);
    if (memo[encoded] != -1) {
        return memo[encoded];
    }

    for (int i = 0; i < width; ++i) {
        if (state[i] >= height) {
            memo[encoded] = 0;
            return 0;
        }
    }

    int win = 1;
    for (int i = 0; i < width; ++i) {
        if (state[i] < lines_to_win) {
            win = 0;
            break;
        }
    }
    if (win) {
        memo[encoded] = 1;
        return 1;
    }

    double probability = 0;
    for (int i = 0; i < width; ++i) {
        state[i]++;
        probability += compute_probability(width, height, lines_to_win, state, memo, base) / width;
        state[i]--;
    }

    memo[encoded] = probability;
    return probability;
}

int main() {
    int width = 2;
    int height = 4;
    int lines_to_win = 2;
    int base = height + 1;
    int *state = calloc(width, sizeof(int));
    double *memo = malloc(pow(base, width) * sizeof(double));
    if (!state || !memo) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < pow(base, width); ++i) {
        memo[i] = -1;
    }
    
    double probability = compute_probability(width, height, lines_to_win, state, memo, base);
    printf("Probability of winning: %.7f%%\n", 100*probability);
    
    free(state);
    free(memo);
    return 0;
}