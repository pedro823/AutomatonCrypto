#include <stdio.h>
#include "error_handler.h"
#define ll long long

ll fact(int x) {
    add_to_stack("error_tester -> fact");
    if(x == 0) {
        return 1;
    }
    if(x == 1) {
        kill("x is %d\n", x);
    }
    ll a = x * fact(x - 1);
    pop_stack();
    return a;
}

int main() {
    set_program_name("tester");
    printf("%lld\n", fact(15));
    return 0;
}
