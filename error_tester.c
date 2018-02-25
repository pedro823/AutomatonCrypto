#include <stdio.h>
#include "error_handler.h"
#define ll long long

ll fact(int x) {
    add_to_stack("error_tester -> fact");
    if(x == 0) {
        pop_stack();
        return 1;
    }
    if(x == 1) {
        kill("x is %d\n", x);
    }
    ll a = x * fact(x - 1);
    pop_stack();
    return a;
}

void h() {
    add_to_stack("error_tester -> h");
    kill("Exception test");
}

void g() {
    add_to_stack("error_tester -> g");
    h();
}

void f() {
    add_to_stack("error_tester -> f");
    g();
}

int main() {
    set_program_name("tester");
    set_debug_priority(0);
    debug_print(0, "YOOOO");
    debug_print(1, "YOOO");
    set_debug_priority(1);
    debug_print(0, "YOOOO");
    debug_print(1, "YOOO");
    f();
    printf("%lld\n", fact(15));
    return 0;
}
