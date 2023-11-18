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

//used to compare integers for qsort
int compare_int (const void * a, const void * b) {
   return (*(int*)a - *(int*)b);
}

//used to compute the factorial to account for permutations
int factorial(int n){
    int factorial = 1;
    for(int i = 2; i <= n; i++){
        factorial *= i;
    }
    return factorial;
}

//memoized algorithm
double compute_probability(int width, int height, int lines_to_win, int *state, double *memo, int base) {
    int encoded = encode_state(state, width, base);
    //if this state already has a probability stored, just use that probability
    if (memo[encoded] != -1) {
        return memo[encoded];
    }

    //if any column has h blocks in it, it is a loss state and probability is 0
    for (int i = 0; i < width; ++i) {
        if (state[i] >= height) {
            memo[encoded] = 0;
            return 0;
        }
    }

    //checking if the number lines required are filled, signifying a win state and setting probability to 1
    int win = 1;
    for (int i = 0; i < width; ++i) {
        if (state[i] < lines_to_win) {
            win = 0;
            break;
        }
    }
    if (win) {
        memo[encoded] = 1 * factorial(width); //multiply by w! to account for all permutations
        return 1;
    }

    //if state isn't a win or loss, all possible next state probabilities must be added and divided by width
    double probability = 0;
    //for loop adds a block to each column separately
    for (int i = 0; i < width; ++i) {
        //making sure that only sorted permutations of states are checked
        int *next_state = calloc(width, sizeof(int));
        for(int i = 0; i < width; i++) next_state[i] = state[i];
        next_state[i]++;
        qsort(next_state, width, sizeof(int), compare_int);

        probability += compute_probability(width, height, lines_to_win, next_state, memo, base);
    }
    probability /= width;

    memo[encoded] = probability;
    return probability;
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    if (!inputFile || !outputFile) {
        fprintf(stderr, "File opening failed\n");
        return 1;
    }

    int width, height, lines_to_win, num_problems;
    fscanf(inputFile, "%d", &num_problems);
    for(int i = 0; i < num_problems; i++){
        fscanf(inputFile, "%d %d %d", &width, &height, &lines_to_win);

        //lines 95-106 are essentially the wrapper function, where we create and initialize our data structure for each problem
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
        fprintf(outputFile, "%g\n", probability);

        free(state);
        free(memo);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}