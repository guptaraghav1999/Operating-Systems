#pragma once
#include "util/config.h"
#include "util/debug.h"

struct shellstate_t{
    char* char_to_int;
    char* keys;
    char** options;
    char* num_of_keys_pressed;
    char get_input[81]="";

    int offset;
    int option_offset;
    int cursor_at;

    int key_scancode;
    char key_pressed;
    int total_key_pressed;
    bool is_int;
    bool is_char;

    bool is_option_selected;
    int selected_option;
    int total_options;
    bool execute_function;
    bool has_executed;
    int input_len;
    int input_int;
    int answer;

    bool isPrime;
    int f_a;
    int f_i;
    int f_j;
    int f_enter;



};

struct renderstate_t{
    char* char_to_int;
    char* keys;
    char** options;
    char* num_of_keys_pressed;
    char get_input[81]="";

    int offset;
    int option_offset;
    int cursor_at;

    int key_scancode;
    char key_pressed;
    int total_key_pressed;
    bool is_int;
    bool is_char;

    bool is_option_selected;
    int selected_option;
    int total_options;
    bool execute_function;
    bool has_executed;
    int input_len;
    int input_int;
    int answer;

    bool isPrime;
    int f_a;
    int f_i;
    int f_j;
    int f_enter;

};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

