#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"
#include "error_handler.h"

/*
 * AUXILIAR FUNCTIONS
 */


/* quick exponentiation of long ints. -- TESTED. */
ull expo(ull base, int e) {
    add_to_stack("automaton -> expo");
    ll result = 1;
    while (e > 0) {
        if (e % 2) {
           result *= base;
        }
        base *= base;
        e >>= 1;
    }
    pop_stack();
    return result;
}

/* Reallocates buffer if full */
void reallocate(char** _buffer, ull size) {
    add_to_stack("automaton -> reallocate");
    ull new_size = 2 * size;
    char* new_buffer = emalloc(new_size * sizeof(char));
    for(ull i = 0; i < size; i++) {
        new_buffer[i] = (*_buffer)[i];
    }
    free(*_buffer);
    *_buffer = new_buffer;
    pop_stack();
}

/* Reads file until empty. stores in buffer
 * writes to ull* file_size the amount of bytes read.
 */
char* read_file(char* stream, ull* file_size) {
    ull size = 0;
    ull max = 256;
    char* buffer = emalloc(256 * sizeof(char));
    char c;
    FILE* input = fopen(stream, "r");
    c = fgetc(input);
    while(c != EOF) {
        if(size == max) {
            reallocate(&buffer, max);
            max = 2 * size;
        }
        buffer[size++] = c;
        c = fgetc(input);
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

    printf("POSITION = %lld\n", position);
    char read_next = (position % 8) >= (9 - amount);
    printf("read_next = %d\n", read_next);

    char out;
    char mask = 0;

    for(int i = 0; i < amount; i++) {
        mask = (mask << 1) + 1;
    }

    if (read_next) {
        /* also reads the second byte */
        out <<= (position + amount) % 8;

        /* Treats special case: reading last byte */
        if(byte + 1 == length) {
            byte = -1;
        }
        out += start[byte + 1] & (0xff << (8 - (position + amount) % 8));
    }
    else {
        mask = (mask << (8 - amount) - (position % 8)) % 256;
        printf("mask = %02x\n", mask);
        out += start[byte] & mask;
    }
    return out;
}

void set_bit(char** start, ull position, bool value) {
    ull byte = position / 8;
    int bit = position % 8;
    char temp = (*start)[byte];
    if (value) {
        temp = temp | (1 << (7 - position));
    }
    else {
        char mask = ~(1 << (7 - position));
        temp &= mask;
    }
    (*start)[byte] = temp;
}


/*
 * MAIN FUNCTIONS
 */


const bool* create_rules(int rule_number, int influence) {
    ull possible_combinations = expo(2, expo(2, influence));
    // if(rule_number >= possible_combinations) {
    //     _error("Impossible rule. Must be a number between 0 and 2^(2^influence).")
    // }
    bool* rulebook = malloc(expo(2, influence) * sizeof(bool));
    for (int rule = 0; rule_number > 0; rule++) {
        rulebook[rule] = rule_number % 2;
        rule_number >>= 1;
    }
    return rulebook;
}

void apply_rule(const bool* rulebook, int influence, char** start, ull size) {
    ull no_bits = size << 3;
    char* end = malloc(size * sizeof(char));
    char bits;
    for(ull i = 0; i < no_bits; i++) {
        bits = read_bits(*start, i, influence, size);
        printf("\tfinal bits = %02x\n", bits);
        set_bit(&end, i, rulebook[bits]);
    }
    free(*start);
    *start = end;
}

int main(int argc, char** argv) {
    ull file_size;
    int influence = 3;
    char* buff = read_file(argv[1], &file_size);
    printf("File size: %lld\n", file_size);
    printf("%s", buff);
    const bool* rulebook = create_rules(255, influence);
    for(int i = 0; i < 8; i++) {
        printf("rule[%02x] = %d\n", i, rulebook[i]);
    }

    // for(i = 0; i < file_size * 8; i++) {
    //     printf(read_bits(buff, ));
    // }

    for(int i = 0; i < file_size; i++) {
        printf("%02x ", buff[i]);
    }
    printf("\n");
    apply_rule(rulebook, influence, &buff, file_size);
    for(int i = 0; i < file_size; i++) {
        printf("%02x ", buff[i]);
    }
    printf("\n");
    return 0;
}
