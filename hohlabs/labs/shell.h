#pragma once
#include "util/config.h"
#include "util/debug.h"

struct shellstate_t{
    int num_keys;
    int cur_line;
    int cur_coln;
    char* commands;
    char*  result;
    char cstart;
    char fstart;
    int carg;
    int fret;
    bool exec_f;
    bool fibo;
    bool fact;
    bool echo;
    bool ltask;
    bool ctask;
    bool cdone;
    bool ftask;
    int farg;
    bool fdone;
    
    int buffer_pos;
    int GArg;
    int HArg;
    bool G;
    bool H;
    int G_invoke;
    int H_invoke;
    int array_arg[5];
    bool runnable[5];
    int sret[5];
    bool sdone[5];
    char* g_h; 
};

struct renderstate_t{
    int num_keys;
    int cur_line;
    int cur_coln;
    char* commands;
    char* result;
    char cstart;
    int carg;
    char fstart;
    int fret;
    int farg;
    bool exec_f;
    bool fibo;
    bool fact; 
    bool echo;
    bool ltask;
    bool ctask;
    bool cdone;
    bool ftask;
    bool fdone;
    
    int buffer_pos;
    int GArg;
    int HArg;
    bool G;
    bool H;
    int G_invoke;
    int H_invoke;
    int array_arg[5];
    bool runnable[5];
    int sret[5];
    bool sdone[5];
    char* g_h;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);
