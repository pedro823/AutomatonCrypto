#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"
#include "error_handler.h"

/*
 * AUXILIAR FUNCTIONS
 */


/* quick exponentiation of long ints. */
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
    debug_print(1, "reallocation test");
    for(ull i = 0; i < size; i++) {
        debug_print(0, "buffer[%lld] = %c", i, (*_buffer)[i]);
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
    add_to_stack("automaton -> read_file");
    ull size = 0;
    ull max = 256;
    char* buffer = emalloc(256 * sizeof(char));
    char c;
    FILE* input = fopen(stream, "r");
    if(input == NULL) {
        kill("Could not open file for reading."
        "Check if it exists and is readable.");
    }
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
    fclose(input);
    pop_stack();
    return buffer;
}

/* Asserted and checked. */
bool* read_bits(char* start, ull position, int amount, ull length) {
    add_to_stack("automaton -> read_bits");
    /* figure out which byte to use */
    ull byte = position / 8;
    /* Figure out which bit to use */
    int bit_place = position % 8;
    /* bit_place actually should be its complement */
    bit_place = 8 - amount - bit_place;
    debug_print(0, "bit_place = %d", bit_place);
    /* If bit_place is negative, walk bytes backward. */
    /* TODO: Find closed formula for this */
    while(bit_place < 0) {
        bit_place += 8;
        byte++;
    }
    // byte %= length;
    debug_print(0, "bit_place = %d", bit_place);
    int i;
    bool* bits_v = emalloc(amount * sizeof(bool));
    debug_print(0, "char = %c in hex %02x", start[byte], start[byte]);
    /* Fills the bool vector backwards. */
    for(i = amount - 1; i >= 0; i--) {
        bits_v[i] = (start[byte] >> bit_place) & 1;
        debug_print(0, "char in the moment: %c", start[byte]);
        if(bit_place >= 7) {
            bit_place = 0;
            byte--;
            /* Circular vector */
            if(byte < 0) {
                byte = length - 1;
            }
        }
        else {
            bit_place++;
        }
    }

    pop_stack();
    return bits_v;
}

void set_bit(char** start, ull position, bool value) {
    add_to_stack("automaton -> set_bit");
    debug_print(0, "~set_bit");
    ull byte = position / 8;
    int bit = position % 8;
    debug_print(0, "before reading start[byte]: byte = %lld", byte);
    char temp = (*start)[byte];
    debug_print(0, "after reading start[byte]. temp = %02x", temp);
    if (value) {
        temp = temp | (1 << (7 - position));
    }
    else {
        temp = temp & ~(1 << (7 - position));
    }
    debug_print(0, "now temp = %02x", temp);
    (*start)[byte] = temp;
    debug_print(0, "assigning successful");
    pop_stack();
}


/*
 * MAIN FUNCTIONS
 */


const bool* create_rules(ull rule_number, int influence) {
    add_to_stack("automaton -> create_rules");
    ull possible_combinations = expo(2, expo(2, influence));
    debug_print(1, "Creating rules... rule no. %d", rule_number);
    if(rule_number >= possible_combinations) {
        kill("rule_number is bigger than all possible combinations.\n"
             "rule_number = %lld, possible_combinations = %lld", rule_number, possible_combinations);
    }
    bool* rulebook = malloc(expo(2, influence) * sizeof(bool));
    for (int rule = 0; rule_number > 0; rule++) {
        rulebook[rule] = rule_number % 2;
        rule_number >>= 1;
    }
    pop_stack();
    return rulebook;
}

/* Applies the rulebook to the bits read. */
bool apply_rule(const bool* rulebook, bool* bits, int influence) {
    add_to_stack("automaton -> create_rules");
    ull expo = 1, sum = 0;

    for(int i = 0; i < influence; i++) {
        sum += expo * bits[i];
        expo <<= 1;
    }

    /* considering rulebook[6] = rulebook[0b110]... */
    bool result = rulebook[sum];

    pop_stack();
    return result;
}

void assert() {
    add_to_stack("automaton -> assert");
    char* a = strndup("abcde", 5);
    bool* i = read_bits(a, 7, 5, 5);
    bool correct[] = {1, 0, 1, 1, 0};
    for(int j = 0; j < 5; j++) {
        if(i[j] != correct[j]) {
            kill("read_bits: incorrect read at example\n"
                 "read_bits is not [1, 0, 1, 1, 0]");
        }
    }
    bool* b = read_bits(a, 0, 4, 5);
    bool* d = read_bits(a, 38, 5, 5);
    set_bit(&a, 1, 0);
    bool* c = read_bits(a, 0, 4, 5);
    bool correct_b[] = {0, 1, 1, 0};
    bool correct_c[] = {0, 0, 1, 0};
    for(int j = 0; j < 4; j++) {
        if(b[j] != correct_b[j]) {
            kill("set_bit: read_bits beforehand was incorrect.\n"
                 "read_bits is not [0, 1, 1, 0]");
        }
    }
    for(int j = 0; j < 4; j++) {
        if(c[j] != correct_c[j]) {
            kill("set_bit: did not set the bit correctly.\n"
                 "read_bits is not [0, 0, 1, 0]");
        }
    }
    free(a); free(i); free(b); free(c); free(d);
    // Readfile test.
    ull file_size;
    char* buffer = read_file("tester", &file_size);
    for(int i = 0; i < file_size; i++) {
        printf("%c", buffer[i]);
    }
    debug_print(0, "ended Readfile test.");
    free(buffer);
    pop_stack();
}

int main(int argc, char** argv) {
    set_debug_priority(0);
    add_to_stack("automaton -> assert");
    assert();
    ull file_size;
    int influence = 3;
    if(argc < 2) {
        kill("no file specified");
    }
    char* text = read_file(argv[1], &file_size);

    free(text);
    pop_stack();
    return 0;
}
