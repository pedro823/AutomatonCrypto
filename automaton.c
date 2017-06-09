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
    add_to_stack("automaton -> read_file");
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
    pop_stack();
    return buffer;
}

bool* read_bits(char* start, ull position, int amount, ull length) {
    add_to_stack("automaton -> read_bits");
    /* figure out which byte to use */
    ull byte = position / 8;
    int bit_place = position % 8;
    int i;
    bool* bits_v = emalloc(amount * sizeof(bool));

    for(i = 0; i < amount; i++) {
        bits_v[i] = (start[byte] >> bit_place) & 1;
        if(bit_place == 7) {
            bit_place = 0;
            byte++;
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
    char temp = (*start)[byte];
    if (value) {
        temp = temp | (1 << (7 - position));
    }
    else {
        char mask = ~(1 << (7 - position));
        temp &= mask;
    }
    (*start)[byte] = temp;
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

void apply_rule(const bool* rulebook, int influence, char** start, ull size) {
    add_to_stack("automaton -> create_rules");
    ull no_bits = size << 3;
    char* end = malloc(size * sizeof(char));
    char bits;
    for(ull i = 0; i < no_bits; i++) {
        bits = read_bits(*start, i, influence, size);
        debug_print(0, "\tfinal bits = %02x", bits);
        set_bit(&end, i, rulebook[bits]);
    }
    free(*start);
    *start = end;
    pop_stack();
}

void assert() {
    add_to_stack("automaton -> assert");
    char a[5] = "abcde";
    char i = read_bits(a, 0, 4, 40);
    char j = read_bits(a, 4, 4, 40);
    if(i != 0b00000110) {
        kill("i = %02x != 06!", i);
    }
    if(j != 0b00000001) {
        kill("j = %02x != 01!", j);
    }
    pop_stack();
}

int main(int argc, char** argv) {
    set_debug_priority(0);
    assert();
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
