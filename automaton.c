#include <stdio.h>
#include <stdlib.h>
#include "arg_parser.h"

/*
 * AUXILIAR FUNCTIONS
 */


/* quick exponentiation of long ints. */
ull expo(ull base, int e) {
    ll result = 1;
    while (e > 0) {
        if (e % 2) {
           result *= base;
        }
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Reallocates buffer if full */
void reallocate(char** _buffer, ull size) {
    ull new_size = 2 * size;
    char* new_buffer = malloc(new_size * sizeof(char));
    for(ull i = 0; i < size; i++) {
        new_buffer[i] = *_buffer[i];
    }
    free(*_buffer);
    *_buffer = new_buffer;
}

/* Reads file until empty. stores in buffer
 * writes to ull* file_size the amount of bytes read.
 */
char* read_file(FILE* stream, ull* file_size) {
    ull size = 0;
    ull max = 256;
    char* buffer = malloc(256 * sizeof(char));
    char c;
    fscanf(stream, "%c", &c);
    while(c != EOF) {
        if(size == max) {
            reallocate(&buffer, max);
            max = 2 * size;
        }
        buffer[size++] = c;
        fscanf(stream, "%c", &c);
    }
    *file_size = size;
    return buffer;
}

/*
* Returns the bits in position, as if start was a big strip of bits, instead of bytes.
* length is the amount of bytes total, amount is how many bits to read.
* Amount must be <= 8, since return is char.
*/
char read_bits(char* start, ull position, int amount, ull length) {

    /* Considering 8-bit byte... */
    ull byte = position / 8;
    char read_next = (position % 8) >= (9 - amount);
    char out;

    /* Reads start on first byte */
    out += start[byte] & (0xff >> (position % 8));

    if (read_next) {
        /* also reads the second byte */
        out <<= (position + amount) % 8;

        /* Treats special case: reading last byte */
        if(byte + 1 == length) {
            byte = -1;
        }
        out += start[byte + 1] & (0xff << (8 - (position + amount) % 8));
    }
    return out;
}

void set_bit(char* start, ull position, bool value) {
    ull byte = position / 8;
    int bit = position % 8;
    char temp = start[byte];
    if (value) {
        temp = temp | (1 << (7 - position));
    }
    else {
        char mask = ((0xff << (8 - bit)) + (0xff >> (1 + bit))) % 256;
        temp &= mask;
    }
    start[byte] = temp;
}


/*
 * MAIN FUNCTIONS
 */


bool* createRules(int rule_number, int influence) {
    ull possible_combinations = expo(2, expo(2, influence));
    bool* rulebook = malloc(possible_combinations * sizeof(bool));
    for (int rule = 0; rule_number > 0; rule++) {
        rulebook[rule] = rule_number % 2;
        rule_number >>= 1;
    }
    return rulebook;
}

void applyRule(bool* rulebook, int influence, char** start, ull size) {
    ull no_bits = size << 3;
    char* end = malloc(size * sizeof(char));
    char bits;
    for(ull i = 0; i < no_bits; i++) {
        bits = read_bits(*start, i, influence, size);
        set_bit(*start, i, rulebook[bits]);
    }
    free(*start);
    *start = end;
}

int main(int argc, char** argv) {
    int a, b;
    scanf("%d %d", &a, &b);

    printf("%lld\n", expo(a, b));


}
