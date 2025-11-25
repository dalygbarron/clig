#include <clig.h>
#include <stdio.h>

static void demo_prompt() {
    printf("Demo>");
}

static clig_input_result gamer_func(char const *input) {
    printf("Woweeee, I just got input saying %s\n", input);
    return clig_OK;
}

int main(int argc, char **argv) {
    clig_repl_set_info(
        "clig demo",
        "0.1.0",
        "Yeah it does nothing sorry.",
        demo_prompt
    );
    clig_repl_add_input_handler("gamer", gamer_func);
    return clig_repl_main(argc, argv);
}