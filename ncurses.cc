/*
 * ncurses-ruby is a ruby module for accessing the FSF's ncurses library
 * (C) 2002 Tobias Peters <t-peters@berlios.de>
 * 
 *  This module is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This module is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this module; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 *
 * This file was adapted from the original ncurses header file which
 * has the following copyright statements:
 */

/****************************************************************************
 * Copyright (c) 1998 Free Software Foundation, Inc.                        *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Zeyd M. Ben-Halim <zmbenhal@netcom.com> 1992,1995               *
 *     and: Eric S. Raymond <esr@snark.thyrsus.com>                         *
 ****************************************************************************/

/*
  NOT IMPLEMENTED:
  - *scanw functions
  - terminfo, termcap-functions
  - rippoffline
  - v*printw functions (but normal printw functions are supported!)
*/

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
extern "C" int dup(int);
extern "C" int close(int);
#endif
#include <vector>
#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#ifdef HAVE_NCURSES_CURSES_H
#include <ncurses/curses.h>
#else
#include <curses.h>
#endif
#endif
#include <ruby.h>

static VALUE mNcurses;
static VALUE cWINDOW;
static VALUE cSCREEN;

static
void
init_constants_1(void)
{
    rb_define_const(mNcurses, "CURSES",      INT2NUM(int(CURSES)));
    rb_define_const(mNcurses, "CURSES_H",    INT2NUM(int(CURSES_H)));
    rb_define_const(mNcurses, "NCURSES_VERSION_MAJOR",
                    INT2NUM(int(NCURSES_VERSION_MAJOR)));
    rb_define_const(mNcurses, "NCURSES_VERSION_MINOR",
                    INT2NUM(int(NCURSES_VERSION_MINOR)));
    rb_define_const(mNcurses, "NCURSES_VERSION_PATCH",
                    INT2NUM(int(NCURSES_VERSION_PATCH)));
    rb_define_const(mNcurses, "NCURSES_VERSION",
                    rb_str_new2(NCURSES_VERSION));

    // attributes
    rb_define_const(mNcurses, "WA_ATTRIBUTES", INT2NUM(WA_ATTRIBUTES));
    rb_define_const(mNcurses, "WA_NORMAL",     INT2NUM(WA_NORMAL));
    rb_define_const(mNcurses, "WA_STANDOUT",   INT2NUM(WA_STANDOUT));
    rb_define_const(mNcurses, "WA_UNDERLINE",  INT2NUM(WA_UNDERLINE));
    rb_define_const(mNcurses, "WA_REVERSE",    INT2NUM(WA_REVERSE));
    rb_define_const(mNcurses, "WA_BLINK",      INT2NUM(WA_BLINK));
    rb_define_const(mNcurses, "WA_DIM",        INT2NUM(WA_DIM));
    rb_define_const(mNcurses, "WA_BOLD",       INT2NUM(WA_BOLD));
    rb_define_const(mNcurses, "WA_ALTCHARSET", INT2NUM(WA_ALTCHARSET));
    rb_define_const(mNcurses, "WA_INVIS",      INT2NUM(WA_INVIS));
    rb_define_const(mNcurses, "WA_PROTECT",    INT2NUM(WA_PROTECT));
    rb_define_const(mNcurses, "WA_HORIZONTAL", INT2NUM(WA_HORIZONTAL));
    rb_define_const(mNcurses, "WA_LEFT",       INT2NUM(WA_LEFT));
    rb_define_const(mNcurses, "WA_LOW",        INT2NUM(WA_LOW));
    rb_define_const(mNcurses, "WA_RIGHT",      INT2NUM(WA_RIGHT));
    rb_define_const(mNcurses, "WA_TOP",        INT2NUM(WA_TOP));
    rb_define_const(mNcurses, "WA_VERTICAL",   INT2NUM(WA_VERTICAL));
}

static VALUE rb_COLORS(...)
{return INT2NUM(COLORS);}
static VALUE rb_COLOR_PAIRS(...)
{return INT2NUM(COLOR_PAIRS);}

static
void
init_globals_1(void)
{
    // colors
    rb_define_module_function(mNcurses, "COLORS", &rb_COLORS, 0);
    rb_define_module_function(mNcurses, "COLOR_PAIRS", &rb_COLOR_PAIRS, 0);
}
static
void
init_constants_2(void)
{
    rb_define_const(mNcurses, "COLOR_BLACK",   INT2NUM(COLOR_BLACK));
    rb_define_const(mNcurses, "COLOR_RED",     INT2NUM(COLOR_RED));
    rb_define_const(mNcurses, "COLOR_GREEN",   INT2NUM(COLOR_GREEN));
    rb_define_const(mNcurses, "COLOR_YELLOW",  INT2NUM(COLOR_YELLOW));
    rb_define_const(mNcurses, "COLOR_BLUE",    INT2NUM(COLOR_BLUE));
    rb_define_const(mNcurses, "COLOR_MAGENTA", INT2NUM(COLOR_MAGENTA));
    rb_define_const(mNcurses, "COLOR_CYAN",    INT2NUM(COLOR_CYAN));
    rb_define_const(mNcurses, "COLOR_WHITE",   INT2NUM(COLOR_WHITE));

    rb_define_const(mNcurses, "ERR",           INT2NUM(ERR));
    rb_define_const(mNcurses, "OK",            INT2NUM(OK));

    /* values for the _flags member */
    rb_define_const(mNcurses, "SUBWIN",       INT2NUM(_SUBWIN));
    rb_define_const(mNcurses, "ENDLINE",      INT2NUM(_ENDLINE));
    rb_define_const(mNcurses, "FULLWIN",      INT2NUM(_FULLWIN));
    rb_define_const(mNcurses, "SCROLLWIN",    INT2NUM(_SCROLLWIN));
    rb_define_const(mNcurses, "ISPAD",        INT2NUM(_ISPAD));
    rb_define_const(mNcurses, "HASMOVED",     INT2NUM(_HASMOVED));
    rb_define_const(mNcurses, "WRAPPED",      INT2NUM(_WRAPPED));

    /*
     * this value is used in the firstchar and lastchar fields to mark
     * unchanged lines
     */
    rb_define_const(mNcurses, "NOCHANGE",     INT2NUM(_NOCHANGE));

    /*
     * this value is used in the oldindex field to mark lines created by
     * insertions and scrolls.
     */
    rb_define_const(mNcurses, "NEWINDEX",     INT2NUM(_NEWINDEX));

#ifdef CCHARW_MAX
    rb_define_const(mNcurses, "CCHARW_MAX",   INT2NUM(CCHARW_MAX));
#endif
}

static VALUE   wrap_window(WINDOW* window)
{
    if (window == 0) return Qnil;
    VALUE windows_hash   = rb_iv_get(mNcurses, "@windows_hash");
    VALUE window_adress  = INT2NUM(reinterpret_cast<long>(window));
    VALUE rb_window      = rb_hash_aref(windows_hash, window_adress);
    if (rb_window == Qnil) {
        rb_window = Data_Wrap_Struct(cWINDOW, 0, 0, window);
        rb_iv_set(rb_window, "@destroyed", Qfalse);
        rb_hash_aset(windows_hash, window_adress, rb_window);
    }
    return rb_window;
}
static WINDOW* get_window(VALUE rb_window)
{
    if (rb_window == Qnil) return 0;
    if (rb_iv_get(rb_window, "@destroyed") == Qtrue) {
        rb_raise(rb_eRuntimeError, "Attempt to access a destroyed window");
        return 0;
    }
    WINDOW* window;
    Data_Get_Struct(rb_window, WINDOW, window);
    return window;
}
static VALUE rb_delwin(VALUE, VALUE arg1) {
    VALUE windows_hash   = rb_iv_get(mNcurses, "@windows_hash");
    WINDOW* window       = get_window(arg1);
    VALUE window_adress  = INT2NUM(reinterpret_cast<long>(window));
    rb_funcall(windows_hash, rb_intern("delete"), 1, window_adress);
    rb_iv_set(arg1, "@destroyed", Qtrue);
    return INT2NUM(delwin(window));
}

static VALUE   wrap_screen(SCREEN* screen)
{
    if (screen == 0) return Qnil;
    VALUE screens_hash   = rb_iv_get(mNcurses, "@screens_hash");
    VALUE screen_adress  = INT2NUM(reinterpret_cast<long>(screen));
    VALUE rb_screen      = rb_hash_aref(screens_hash, screen_adress);
    if (rb_screen == Qnil) {
        rb_screen = Data_Wrap_Struct(cSCREEN, 0, 0, screen);
        rb_iv_set(rb_screen, "@destroyed", Qfalse);
        rb_hash_aset(screens_hash, screen_adress, rb_screen);
    }
    return rb_screen;
}
static SCREEN* get_screen(VALUE rb_screen)
{
    if (rb_screen == Qnil) return 0;
    if (rb_iv_get(rb_screen, "@destroyed") == Qtrue) {
        rb_raise(rb_eRuntimeError, "Attempt to access a destroyed screen");
        return 0;
    }
    SCREEN* screen;
    Data_Get_Struct(rb_screen, SCREEN, screen);
    return screen;
}
static VALUE rb_delscreen(VALUE, VALUE arg1) {
    VALUE screens_hash   = rb_iv_get(mNcurses, "@screens_hash");
    SCREEN* screen       = get_screen(arg1);
    VALUE screen_adress  = INT2NUM(reinterpret_cast<long>(screen));
    rb_funcall(screens_hash, rb_intern("delete"), 1, screen_adress);
    rb_iv_set(arg1, "@destroyed", Qtrue);
    delscreen(screen);
    return Qnil;
}
static VALUE rb_winchnstr(VALUE, VALUE rb_win, VALUE rb_str, VALUE rb_n)
{
    if (rb_obj_is_instance_of(rb_str, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError, "2nd argument must be an empty Array");
        return Qnil;
    }
    
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    std::vector<chtype> str (n + 1);
    int return_value;
    return_value = winchnstr(window, &str.front(), n);
    if (return_value != ERR) {
        for (int i = 0; i < return_value; ++i) {
            rb_ary_push(rb_str, INT2NUM(str[i]));
        }
    }
    return INT2NUM(return_value);
}
static VALUE rb_wgetnstr(VALUE, VALUE rb_win, VALUE rb_chstr, VALUE rb_n)
{
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    std::vector<char> str (n + 1);
    int return_value;
    return_value = wgetnstr(window, &str.front(), n);
    if (return_value != ERR) {
        rb_str_cat(rb_chstr, &str.front(), return_value);
    }
    return INT2NUM(return_value);
}
static VALUE rb_winnstr(VALUE, VALUE rb_win, VALUE rb_chstr, VALUE rb_n)
{
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    std::vector<char> str (n + 1);
    int return_value;
    return_value = winnstr(window, &str.front(), n);
    if (return_value != ERR) {
        rb_str_cat(rb_chstr, &str.front(), return_value);
    }
    return INT2NUM(return_value);
}

#ifdef HAVE_PANEL_H
#include "panel.hh" // needs access to mNcurses, wrap_window, get_window
#endif

static
void
init_functions_0(void)
{
    rb_define_module_function(mNcurses, "delscreen",
                              reinterpret_cast<VALUE(*)(...)>(&rb_delscreen),
                              1);
    rb_define_module_function(mNcurses, "delwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_delwin), 1);
    rb_define_module_function(mNcurses, "winchnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winchnstr),
                              3);
    rb_define_module_function(mNcurses, "winnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winnstr),
                              3);
    rb_define_module_function(mNcurses, "wgetnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wgetnstr),
                              3);
}

static VALUE get_stdscr(...)
{
    VALUE rb_stdscr = rb_iv_get(mNcurses, "@stdscr");
    if (rb_stdscr == Qnil) {
        rb_stdscr = wrap_window(stdscr);
        rb_iv_set(mNcurses, "@stdscr", rb_stdscr);
    }
    return rb_stdscr;
}
static VALUE get_curscr(...)
{
    VALUE rb_curscr = rb_iv_get(mNcurses, "@curscr");
    if (rb_curscr == Qnil) {
        rb_curscr = wrap_window(curscr);
        rb_iv_set(mNcurses, "@curscr", rb_curscr);
    }
    return rb_curscr;
}
static VALUE get_newscr(...)
{
    VALUE rb_newscr = rb_iv_get(mNcurses, "@newscr");
    if (rb_newscr == Qnil) {
        rb_newscr = wrap_window(newscr);
        rb_iv_set(mNcurses, "@newscr", rb_newscr);
    }
    return rb_newscr;
}
static VALUE get_LINES(...)   {return INT2NUM(LINES);}
static VALUE get_COLS(...)    {return INT2NUM(COLS);}
static VALUE get_TABSIZE(...) {return INT2NUM(TABSIZE);}
// This global was an undocumented feature under AIX curses.
// ESC expire time in milliseconds
static VALUE get_ESCDELAY(...){return INT2NUM(ESCDELAY);}

static
void
init_globals_2(void)
{
    rb_iv_set(mNcurses, "@stdscr", Qnil);
    rb_iv_set(mNcurses, "@curscr", Qnil);
    rb_iv_set(mNcurses, "@newscr", Qnil);
    rb_define_module_function(mNcurses, "stdscr", &get_stdscr, 0);
    rb_define_module_function(mNcurses, "curscr", &get_curscr, 0);
    rb_define_module_function(mNcurses, "newscr", &get_newscr, 0);
    rb_define_module_function(mNcurses, "LINES",  &get_LINES,  0);
    rb_define_module_function(mNcurses, "COLS",   &get_COLS,   0);
    rb_define_module_function(mNcurses, "TABSIZE",&get_TABSIZE,0);
    rb_define_module_function(mNcurses, "ESCDELAY",&get_ESCDELAY,0);
}

static VALUE rb_keybound(VALUE, VALUE keycode, VALUE count)
{
    char * str = keybound(NUM2INT(keycode), NUM2INT(count));
    VALUE rb_str = Qnil;
    if (str) {
        rb_str = rb_str_new2(str);
        free(str);
    }
    return rb_str;
}
static VALUE rb_curses_version(...){return rb_str_new2(curses_version());}
static VALUE rb_define_key(VALUE, VALUE definition, VALUE keycode)
{
    return INT2NUM(define_key((definition != Qnil)
                              ? STR2CSTR(definition)
                              : static_cast<char*>(0),
                              NUM2INT(keycode)));
}

static VALUE rb_keyok(VALUE, VALUE keycode, VALUE enable)
{
    return INT2NUM(keyok(NUM2INT(keycode), RTEST(enable)));
}
static VALUE rb_resizeterm(VALUE, VALUE lines, VALUE columns)
{
    return INT2NUM(resizeterm(NUM2INT(lines), NUM2INT(columns)));
}
static VALUE rb_use_default_colors(...)
{return INT2NUM(use_default_colors());}
static VALUE rb_use_extended_names(VALUE, VALUE boolean)
{return INT2NUM(use_extended_names(RTEST(boolean)));}
static VALUE rb_wresize(VALUE, VALUE win, VALUE lines, VALUE columns)
{
    return INT2NUM(wresize(get_window(win), NUM2INT(lines), NUM2INT(columns)));
}
static
void
init_functions_1(void)
{
    rb_define_module_function(mNcurses, "keybound",
                              reinterpret_cast<VALUE(*)(...)>(&rb_keybound),
                              2);
    rb_define_module_function(mNcurses, "curses_version",
                              &rb_curses_version, 0);
    rb_define_module_function(mNcurses, "define_key",
                              reinterpret_cast<VALUE(*)(...)>(&rb_define_key),
                              2);
    rb_define_module_function(mNcurses, "keyok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_keyok), 2);
    rb_define_module_function(mNcurses, "resizeterm",
                              reinterpret_cast<VALUE(*)(...)>(&rb_resizeterm),
                              2);
    rb_define_module_function(mNcurses, "use_default_colors",
                              &rb_use_default_colors, 0);
    rb_define_module_function(mNcurses, "use_extended_names",
                              reinterpret_cast<VALUE(*)(...)>(&rb_use_extended_names), 1);
    rb_define_module_function(mNcurses, "wresize",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wresize), 3);
}
// FIXME: what's this?
// extern char ttytype[];		/* needed for backward compatibility */


// copy a chstr from ruby to c, using c++'s std::vector
static std::vector<chtype> RB2CHSTR(VALUE array)
{
    if (rb_obj_is_instance_of(array, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "chtype string argument must be an empty Array");
        return std::vector<chtype>();
    }
    size_t string_length = NUM2ULONG(rb_funcall(array, rb_intern("size"), 0));
    size_t vector_length = string_length + 1;        // for terminating 0
    std::vector<chtype> chstr(vector_length);        // all initialized to 0
    for (unsigned long i = 0; i < string_length; ++i) {
        chstr[i] = NUM2ULONG(rb_ary_entry(array, i));
    }
    return chstr;
}

static VALUE rb_addch(VALUE, VALUE arg1) {
    return INT2NUM(addch(NUM2ULONG(arg1)));
}
static VALUE rb_addchnstr(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(addchnstr(RB2CHSTR(arg1).begin(),  NUM2INT(arg2)));
}
static VALUE rb_addchstr(VALUE, VALUE arg1) {
    return INT2NUM(addchstr(RB2CHSTR(arg1).begin()));
}
static VALUE rb_addnstr(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(addnstr(STR2CSTR(arg1),  NUM2INT(arg2)));
}
static VALUE rb_addstr(VALUE, VALUE arg1) {
    return INT2NUM(addstr(STR2CSTR(arg1)));
}
static VALUE rb_attroff(VALUE, VALUE arg1) {
    return INT2NUM(attroff(NUM2ULONG(arg1)));
}
static VALUE rb_attron(VALUE, VALUE arg1) {
    return INT2NUM(attron(NUM2ULONG(arg1)));
}
static VALUE rb_attrset(VALUE, VALUE arg1) {
    return INT2NUM(attrset(NUM2ULONG(arg1)));
}
static VALUE rb_attr_off(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(attr_off(NUM2ULONG(arg1),  ((arg2),static_cast<void*>(0))));
}
static VALUE rb_attr_on(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(attr_on(NUM2ULONG(arg1),  ((arg2),static_cast<void*>(0))));
}
static VALUE rb_attr_set(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(attr_set(NUM2ULONG(arg1),  NUM2INT(arg2),  ((arg3),static_cast<void*>(0))));
}
static VALUE rb_baudrate(VALUE) {
    return INT2NUM(baudrate());
}
static VALUE rb_beep(VALUE) {
    return INT2NUM(beep());
}
static VALUE rb_bkgd(VALUE, VALUE arg1) {
    return INT2NUM(bkgd(NUM2ULONG(arg1)));
}
static VALUE rb_bkgdset(VALUE, VALUE arg1) {
    return ((bkgdset(NUM2ULONG(arg1))),Qnil);
}
static VALUE rb_border(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8) {
    return INT2NUM(border(NUM2ULONG(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3),  NUM2ULONG(arg4),  NUM2ULONG(arg5),  NUM2ULONG(arg6),  NUM2ULONG(arg7),  NUM2ULONG(arg8)));
}
static VALUE rb_box(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(box(get_window(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3)));
}
static VALUE rb_can_change_color(VALUE) {
    return (can_change_color()) ? Qtrue : Qfalse;
}
static VALUE rb_cbreak(VALUE) {
    return INT2NUM(cbreak());
}
static VALUE rb_chgat(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(chgat(NUM2INT(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3),  ((arg4),static_cast<void*>(0))));
}
static VALUE rb_clear(VALUE) {
    return INT2NUM(clear());
}
static VALUE rb_clearok(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(clearok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_clrtobot(VALUE) {
    return INT2NUM(clrtobot());
}
static VALUE rb_clrtoeol(VALUE) {
    return INT2NUM(clrtoeol());
}
static VALUE rb_color_set(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(color_set(NUM2INT(arg1),  ((arg2),static_cast<void*>(0))));
}
static VALUE rb_COLOR_PAIR(VALUE, VALUE arg1) {
    return INT2NUM(COLOR_PAIR(NUM2INT(arg1)));
}
static VALUE rb_copywin(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9) {
    return INT2NUM(copywin(get_window(arg1),  get_window(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7),  NUM2INT(arg8),  NUM2INT(arg9)));
}
static VALUE rb_curs_set(VALUE, VALUE arg1) {
    return INT2NUM(curs_set(NUM2INT(arg1)));
}
static VALUE rb_def_prog_mode(VALUE) {
    return INT2NUM(def_prog_mode());
}
static VALUE rb_def_shell_mode(VALUE) {
    return INT2NUM(def_shell_mode());
}
static VALUE rb_delay_output(VALUE, VALUE arg1) {
    return INT2NUM(delay_output(NUM2INT(arg1)));
}
static VALUE rb_delch(VALUE) {
    return INT2NUM(delch());
}
static VALUE rb_deleteln(VALUE) {
    return INT2NUM(deleteln());
}
static VALUE rb_derwin(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(derwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rb_doupdate(VALUE) {
    return INT2NUM(doupdate());
}
static VALUE rb_dupwin(VALUE, VALUE arg1) {
    return wrap_window(dupwin(get_window(arg1)));
}
static VALUE rb_echo(VALUE) {
    return INT2NUM(echo());
}
static VALUE rb_echochar(VALUE, VALUE arg1) {
    return INT2NUM(echochar(NUM2ULONG(arg1)));
}
static VALUE rb_endwin(VALUE) {
    return INT2NUM(endwin());
}
static VALUE rb_erasechar(VALUE) {
    return INT2NUM(erasechar());
}
static VALUE rb_filter(VALUE) {
    return ((filter()),Qnil);
}
static VALUE rb_flash(VALUE) {
    return INT2NUM(flash());
}
static VALUE rb_flushinp(VALUE) {
    return INT2NUM(flushinp());
}
static VALUE rb_getbkgd(VALUE, VALUE arg1) {
    return INT2NUM(getbkgd(get_window(arg1)));
}
static VALUE rb_getch(VALUE) {
    return INT2NUM(getch());
}
static VALUE rb_halfdelay(VALUE, VALUE arg1) {
    return INT2NUM(halfdelay(NUM2INT(arg1)));
}
static VALUE rb_has_colors(VALUE) {
    return (has_colors()) ? Qtrue : Qfalse;
}
static VALUE rb_has_ic(VALUE) {
    return (has_ic()) ? Qtrue : Qfalse;
}
static VALUE rb_has_il(VALUE) {
    return (has_il()) ? Qtrue : Qfalse;
}
static VALUE rb_hline(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(hline(NUM2ULONG(arg1),  NUM2INT(arg2)));
}
static VALUE rb_idcok(VALUE, VALUE arg1, VALUE arg2) {
    return ((idcok(get_window(arg1),  RTEST(arg2))),Qnil);
}
static VALUE rb_idlok(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(idlok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_immedok(VALUE, VALUE arg1, VALUE arg2) {
    return ((immedok(get_window(arg1),  RTEST(arg2))),Qnil);
}
static VALUE rb_inch(VALUE) {
    return INT2NUM(inch());
}
static VALUE rb_initscr(VALUE) {
    VALUE v = wrap_window(initscr());
    
    // These constants are not defined before the call to initscr.
    
    /* line graphics */

    /* VT100 symbols begin here */

    rb_define_const(mNcurses, "ACS_ULCORNER",  INT2NUM(ACS_ULCORNER));
    rb_define_const(mNcurses, "ACS_LLCORNER",  INT2NUM(ACS_LLCORNER));
    rb_define_const(mNcurses, "ACS_URCORNER",  INT2NUM(ACS_URCORNER));
    rb_define_const(mNcurses, "ACS_LRCORNER",  INT2NUM(ACS_LRCORNER));
    rb_define_const(mNcurses, "ACS_LTEE",      INT2NUM(ACS_LTEE));
    rb_define_const(mNcurses, "ACS_RTEE",      INT2NUM(ACS_RTEE));
    rb_define_const(mNcurses, "ACS_BTEE",      INT2NUM(ACS_BTEE));
    rb_define_const(mNcurses, "ACS_TTEE",      INT2NUM(ACS_TTEE));
    rb_define_const(mNcurses, "ACS_HLINE",     INT2NUM(ACS_HLINE));
    rb_define_const(mNcurses, "ACS_VLINE",     INT2NUM(ACS_VLINE));
    rb_define_const(mNcurses, "ACS_PLUS",      INT2NUM(ACS_PLUS));
    rb_define_const(mNcurses, "ACS_S1",        INT2NUM(ACS_S1));
    rb_define_const(mNcurses, "ACS_S9",        INT2NUM(ACS_S9));
    rb_define_const(mNcurses, "ACS_DIAMOND",   INT2NUM(ACS_DIAMOND));
    rb_define_const(mNcurses, "ACS_CKBOARD",   INT2NUM(ACS_CKBOARD));
    rb_define_const(mNcurses, "ACS_DEGREE",    INT2NUM(ACS_DEGREE));
    rb_define_const(mNcurses, "ACS_PLMINUS",   INT2NUM(ACS_PLMINUS));
    rb_define_const(mNcurses, "ACS_BULLET",    INT2NUM(ACS_BULLET));
    /* Teletype 5410v1 symbols begin here */
    rb_define_const(mNcurses, "ACS_LARROW",    INT2NUM(ACS_LARROW));
    rb_define_const(mNcurses, "ACS_RARROW",    INT2NUM(ACS_RARROW));
    rb_define_const(mNcurses, "ACS_DARROW",    INT2NUM(ACS_DARROW));
    rb_define_const(mNcurses, "ACS_UARROW",    INT2NUM(ACS_UARROW));
    rb_define_const(mNcurses, "ACS_BOARD",     INT2NUM(ACS_BOARD));
    rb_define_const(mNcurses, "ACS_LANTERN",   INT2NUM(ACS_LANTERN));
    rb_define_const(mNcurses, "ACS_BLOCK",     INT2NUM(ACS_BLOCK));
    /*
     * These aren't documented, but a lot of System Vs have them anyway
     * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
     * The ACS_names may not match AT&T's, our source didn't know them.
     */
    rb_define_const(mNcurses, "ACS_S3",        INT2NUM(ACS_S3));
    rb_define_const(mNcurses, "ACS_S7",        INT2NUM(ACS_S7));
    rb_define_const(mNcurses, "ACS_LEQUAL",    INT2NUM(ACS_LEQUAL));
    rb_define_const(mNcurses, "ACS_GEQUAL",    INT2NUM(ACS_GEQUAL));
    rb_define_const(mNcurses, "ACS_PI",        INT2NUM(ACS_PI));
    rb_define_const(mNcurses, "ACS_NEQUAL",    INT2NUM(ACS_NEQUAL));
    rb_define_const(mNcurses, "ACS_STERLING",  INT2NUM(ACS_STERLING));
    /*
     * Line drawing ACS names are of the form ACS_trbl, where t is the top, r
     * is the right, b is the bottom, and l is the left.  t, r, b, and l might
     * be B (blank), S (single), D (double), or T (thick).  The subset defined
     * here only uses B and S.
     */
    rb_define_const(mNcurses, "ACS_BSSB",      INT2NUM(ACS_BSSB));
    rb_define_const(mNcurses, "ACS_SSBB",      INT2NUM(ACS_SSBB));
    rb_define_const(mNcurses, "ACS_BBSS",      INT2NUM(ACS_BBSS));
    rb_define_const(mNcurses, "ACS_SBBS",      INT2NUM(ACS_SBBS));
    rb_define_const(mNcurses, "ACS_SBSS",      INT2NUM(ACS_SBSS));
    rb_define_const(mNcurses, "ACS_SSSB",      INT2NUM(ACS_SSSB));
    rb_define_const(mNcurses, "ACS_SSBS",      INT2NUM(ACS_SSBS));
    rb_define_const(mNcurses, "ACS_BSSS",      INT2NUM(ACS_BSSS));
    rb_define_const(mNcurses, "ACS_BSBS",      INT2NUM(ACS_BSBS));
    rb_define_const(mNcurses, "ACS_SBSB",      INT2NUM(ACS_SBSB));
    rb_define_const(mNcurses, "ACS_SSSS",      INT2NUM(ACS_SSSS));

    return v;
}
static VALUE rb_init_color(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(init_color(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_init_pair(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(init_pair(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_insch(VALUE, VALUE arg1) {
    return INT2NUM(insch(NUM2ULONG(arg1)));
}
static VALUE rb_insdelln(VALUE, VALUE arg1) {
    return INT2NUM(insdelln(NUM2INT(arg1)));
}
static VALUE rb_insertln(VALUE) {
    return INT2NUM(insertln());
}
static VALUE rb_insnstr(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(insnstr(STR2CSTR(arg1),  NUM2INT(arg2)));
}
static VALUE rb_insstr(VALUE, VALUE arg1) {
    return INT2NUM(insstr(STR2CSTR(arg1)));
}
static VALUE rb_intrflush(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(intrflush(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_isendwin(VALUE) {
    return (isendwin()) ? Qtrue : Qfalse;
}
static VALUE rb_is_linetouched(VALUE, VALUE arg1, VALUE arg2) {
    return (is_linetouched(get_window(arg1),  NUM2INT(arg2))) ? Qtrue : Qfalse;
}
static VALUE rb_is_wintouched(VALUE, VALUE arg1) {
    return (is_wintouched(get_window(arg1))) ? Qtrue : Qfalse;
}
static VALUE rb_keyname(VALUE, VALUE arg1) {
    return rb_str_new2(keyname(NUM2INT(arg1)));
}
static VALUE rb_keypad(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(keypad(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_killchar(VALUE) {
    return INT2NUM(killchar());
}
static VALUE rb_leaveok(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(leaveok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_longname(VALUE) {
    return rb_str_new2(longname());
}
static VALUE rb_meta(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(meta(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_move(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(move(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvaddch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddch(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3)));
}
static VALUE rb_mvaddchnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvaddchnstr(NUM2INT(arg1),  NUM2INT(arg2),  RB2CHSTR(arg3).begin(),  NUM2INT(arg4)));
}
static VALUE rb_mvaddchstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddchstr(NUM2INT(arg1),  NUM2INT(arg2),  RB2CHSTR(arg3).begin()));
}
static VALUE rb_mvaddnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvaddnstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvaddstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3)));
}
static VALUE rb_mvchgat(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6) {
    return INT2NUM(mvchgat(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5),  ((arg6),static_cast<void*>(0))));
}
static VALUE rb_mvcur(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvcur(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvdelch(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvdelch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvderwin(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvderwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvgetch(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvgetch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvhline(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvhline(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvinch(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvinch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvinsch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvinsch(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3)));
}
static VALUE rb_mvinsnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvinsnstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvinsstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvinsstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3)));
}
static VALUE rb_mvvline(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvvline(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvwaddch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4)));
}
static VALUE rb_mvwaddchnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwaddchnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  RB2CHSTR(arg4).begin(),  NUM2INT(arg5)));
}
static VALUE rb_mvwaddchstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddchstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  RB2CHSTR(arg4).begin()));
}
static VALUE rb_mvwaddnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwaddnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4),  NUM2INT(arg5)));
}
static VALUE rb_mvwaddstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4)));
}
static VALUE rb_mvwchgat(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(mvwchgat(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2ULONG(arg5),  NUM2INT(arg6),  ((arg7),static_cast<void*>(0))));
}
static VALUE rb_mvwdelch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwdelch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwgetch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwgetch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwhline(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwhline(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5)));
}
static VALUE rb_mvwin(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwinch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwinch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwinsch(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwinsch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4)));
}
static VALUE rb_mvwinsnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwinsnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4),  NUM2INT(arg5)));
}
static VALUE rb_mvwinsstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwinsstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4)));
}
static VALUE rb_mvwvline(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwvline(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5)));
}
static VALUE rb_napms(VALUE, VALUE arg1) {
    return INT2NUM(napms(NUM2INT(arg1)));
}
static VALUE rb_newpad(VALUE, VALUE arg1, VALUE arg2) {
    return wrap_window(newpad(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_newwin(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return wrap_window(newwin(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_nl(VALUE) {
    return INT2NUM(nl());
}
static VALUE rb_nocbreak(VALUE) {
    return INT2NUM(nocbreak());
}
static VALUE rb_nodelay(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(nodelay(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_noecho(VALUE) {
    return INT2NUM(noecho());
}
static VALUE rb_nonl(VALUE) {
    return INT2NUM(nonl());
}
static VALUE rb_noqiflush(VALUE) {
    return ((noqiflush()),Qnil);
}
static VALUE rb_noraw(VALUE) {
    return INT2NUM(noraw());
}
static VALUE rb_notimeout(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(notimeout(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_overlay(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(overlay(get_window(arg1),  get_window(arg2)));
}
static VALUE rb_overwrite(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(overwrite(get_window(arg1),  get_window(arg2)));
}
static VALUE rb_PAIR_NUMBER(VALUE, VALUE arg1) {
    return INT2NUM(PAIR_NUMBER(NUM2INT(arg1)));
}
static VALUE rb_pechochar(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(pechochar(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_pnoutrefresh(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(pnoutrefresh(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7)));
}
static VALUE rb_prefresh(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(prefresh(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7)));
}
static VALUE rb_putp(VALUE, VALUE arg1) {
    return INT2NUM(putp(STR2CSTR(arg1)));
}
static VALUE rb_qiflush(VALUE) {
    return ((qiflush()),Qnil);
}
static VALUE rb_raw(VALUE) {
    return INT2NUM(raw());
}
static VALUE rb_redrawwin(VALUE, VALUE arg1) {
    return INT2NUM(redrawwin(get_window(arg1)));
}
static VALUE rb_refresh(VALUE) {
    return INT2NUM(refresh());
}
static VALUE rb_resetty(VALUE) {
    return INT2NUM(resetty());
}
static VALUE rb_reset_prog_mode(VALUE) {
    return INT2NUM(reset_prog_mode());
}
static VALUE rb_reset_shell_mode(VALUE) {
    return INT2NUM(reset_shell_mode());
}
static VALUE rb_savetty(VALUE) {
    return INT2NUM(savetty());
}
static VALUE rb_scr_dump(VALUE, VALUE arg1) {
    return INT2NUM(scr_dump(STR2CSTR(arg1)));
}
static VALUE rb_scr_init(VALUE, VALUE arg1) {
    return INT2NUM(scr_init(STR2CSTR(arg1)));
}
static VALUE rb_scrl(VALUE, VALUE arg1) {
    return INT2NUM(scrl(NUM2INT(arg1)));
}
static VALUE rb_scroll(VALUE, VALUE arg1) {
    return INT2NUM(scroll(get_window(arg1)));
}
static VALUE rb_scrollok(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(scrollok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_scr_restore(VALUE, VALUE arg1) {
    return INT2NUM(scr_restore(STR2CSTR(arg1)));
}
static VALUE rb_scr_set(VALUE, VALUE arg1) {
    return INT2NUM(scr_set(STR2CSTR(arg1)));
}
static VALUE rb_setscrreg(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(setscrreg(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_set_term(VALUE, VALUE arg1) {
    return wrap_screen(set_term(get_screen(arg1)));
}
static VALUE rb_slk_attroff(VALUE, VALUE arg1) {
    return INT2NUM(slk_attroff(NUM2ULONG(arg1)));
}
static VALUE rb_slk_attr_off(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(slk_attr_off(NUM2ULONG(arg1),  ((arg2),static_cast<void*>(0))));
}
static VALUE rb_slk_attron(VALUE, VALUE arg1) {
    return INT2NUM(slk_attron(NUM2ULONG(arg1)));
}
static VALUE rb_slk_attr_on(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(slk_attr_on(NUM2ULONG(arg1),  ((arg2),static_cast<void*>(0))));
}
static VALUE rb_slk_attrset(VALUE, VALUE arg1) {
    return INT2NUM(slk_attrset(NUM2ULONG(arg1)));
}
static VALUE rb_slk_attr(VALUE) {
    return INT2NUM(slk_attr());
}
static VALUE rb_slk_attr_set(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(slk_attr_set(NUM2ULONG(arg1),  NUM2INT(arg2),  ((arg3),static_cast<void*>(0))));
}
static VALUE rb_slk_clear(VALUE) {
    return INT2NUM(slk_clear());
}
static VALUE rb_slk_color(VALUE, VALUE arg1) {
    return INT2NUM(slk_color(NUM2INT(arg1)));
}
static VALUE rb_slk_init(VALUE, VALUE arg1) {
    return INT2NUM(slk_init(NUM2INT(arg1)));
}
static VALUE rb_slk_label(VALUE, VALUE arg1) {
    return rb_str_new2(slk_label(NUM2INT(arg1)));
}
static VALUE rb_slk_noutrefresh(VALUE) {
    return INT2NUM(slk_noutrefresh());
}
static VALUE rb_slk_refresh(VALUE) {
    return INT2NUM(slk_refresh());
}
static VALUE rb_slk_restore(VALUE) {
    return INT2NUM(slk_restore());
}
static VALUE rb_slk_set(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(slk_set(NUM2INT(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rb_slk_touch(VALUE) {
    return INT2NUM(slk_touch());
}
static VALUE rb_standout(VALUE) {
    return INT2NUM(standout());
}
static VALUE rb_standend(VALUE) {
    return INT2NUM(standend());
}
static VALUE rb_start_color(VALUE) {
    return INT2NUM(start_color());
}
static VALUE rb_subpad(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(subpad(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rb_subwin(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(subwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rb_syncok(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(syncok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_termattrs(VALUE) {
    return INT2NUM(termattrs());
}
static VALUE rb_termname(VALUE) {
    return rb_str_new2(termname());
}
static VALUE rb_tigetflag(VALUE, VALUE arg1) {
    return INT2NUM(tigetflag(STR2CSTR(arg1)));
}
static VALUE rb_tigetnum(VALUE, VALUE arg1) {
    return INT2NUM(tigetnum(STR2CSTR(arg1)));
}
static VALUE rb_tigetstr(VALUE, VALUE arg1) {
    return rb_str_new2(tigetstr(STR2CSTR(arg1)));
}
static VALUE rb_timeout(VALUE, VALUE arg1) {
    return ((timeout(NUM2INT(arg1))),Qnil);
}
static VALUE rb_typeahead(VALUE, VALUE arg1) {
    return INT2NUM(typeahead(NUM2INT(arg1)));
}
static VALUE rb_ungetch(VALUE, VALUE arg1) {
    return INT2NUM(ungetch(NUM2INT(arg1)));
}
static VALUE rb_untouchwin(VALUE, VALUE arg1) {
    return INT2NUM(untouchwin(get_window(arg1)));
}
static VALUE rb_use_env(VALUE, VALUE arg1) {
    return ((use_env(RTEST(arg1))),Qnil);
}
static VALUE rb_vidattr(VALUE, VALUE arg1) {
    return INT2NUM(vidattr(NUM2ULONG(arg1)));
}
static VALUE rb_vid_attr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(vid_attr(NUM2ULONG(arg1),  NUM2INT(arg2),  ((arg3),static_cast<void*>(0))));
}
static VALUE rb_vline(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(vline(NUM2ULONG(arg1),  NUM2INT(arg2)));
}
static VALUE rb_waddch(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddch(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_waddchnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(waddchnstr(get_window(arg1),  RB2CHSTR(arg2).begin(),  NUM2INT(arg3)));
}
static VALUE rb_waddchstr(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddchstr(get_window(arg1),  RB2CHSTR(arg2).begin()));
}
static VALUE rb_waddnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(waddnstr(get_window(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rb_waddstr(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddstr(get_window(arg1),  STR2CSTR(arg2)));
}
static VALUE rb_wattron(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattron(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wattroff(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattroff(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wattrset(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattrset(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wattr_on(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wattr_on(get_window(arg1),  NUM2ULONG(arg2),  ((arg3),static_cast<void*>(0))));
}
static VALUE rb_wattr_off(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wattr_off(get_window(arg1),  NUM2ULONG(arg2),  ((arg3),static_cast<void*>(0))));
}
static VALUE rb_wattr_set(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(wattr_set(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3),  ((arg4),static_cast<void*>(0))));
}
static VALUE rb_wbkgd(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(wbkgd(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_wbkgdset(VALUE, VALUE arg1, VALUE arg2) {
    return ((wbkgdset(get_window(arg1),  NUM2ULONG(arg2))),Qnil);
}
static VALUE rb_wborder(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9) {
    return INT2NUM(wborder(get_window(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3),  NUM2ULONG(arg4),  NUM2ULONG(arg5),  NUM2ULONG(arg6),  NUM2ULONG(arg7),  NUM2ULONG(arg8),  NUM2ULONG(arg9)));
}
static VALUE rb_wchgat(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(wchgat(get_window(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4),  ((arg5),static_cast<void*>(0))));
}
static VALUE rb_wclear(VALUE, VALUE arg1) {
    return INT2NUM(wclear(get_window(arg1)));
}
static VALUE rb_wclrtobot(VALUE, VALUE arg1) {
    return INT2NUM(wclrtobot(get_window(arg1)));
}
static VALUE rb_wclrtoeol(VALUE, VALUE arg1) {
    return INT2NUM(wclrtoeol(get_window(arg1)));
}
static VALUE rb_wcolor_set(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wcolor_set(get_window(arg1),  NUM2INT(arg2),  ((arg3),static_cast<void*>(0))));
}
static VALUE rb_wcursyncup(VALUE, VALUE arg1) {
    return ((wcursyncup(get_window(arg1))),Qnil);
}
static VALUE rb_wdelch(VALUE, VALUE arg1) {
    return INT2NUM(wdelch(get_window(arg1)));
}
static VALUE rb_wdeleteln(VALUE, VALUE arg1) {
    return INT2NUM(wdeleteln(get_window(arg1)));
}
static VALUE rb_wechochar(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(wechochar(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_werase(VALUE, VALUE arg1) {
    return INT2NUM(werase(get_window(arg1)));
}
static VALUE rb_wgetch(VALUE, VALUE arg1) {
    return INT2NUM(wgetch(get_window(arg1)));
}
static VALUE rb_whline(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(whline(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3)));
}
static VALUE rb_winch(VALUE, VALUE arg1) {
    return INT2NUM(winch(get_window(arg1)));
}
static VALUE rb_winsch(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsch(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_winsdelln(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsdelln(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_winsertln(VALUE, VALUE arg1) {
    return INT2NUM(winsertln(get_window(arg1)));
}
static VALUE rb_winsnstr(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(winsnstr(get_window(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rb_winsstr(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsstr(get_window(arg1),  STR2CSTR(arg2)));
}
static VALUE rb_wmove(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wmove(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_wnoutrefresh(VALUE, VALUE arg1) {
    return INT2NUM(wnoutrefresh(get_window(arg1)));
}
static VALUE rb_wredrawln(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wredrawln(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_wrefresh(VALUE, VALUE arg1) {
    return INT2NUM(wrefresh(get_window(arg1)));
}
static VALUE rb_wscrl(VALUE, VALUE arg1, VALUE arg2) {
    return INT2NUM(wscrl(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wsetscrreg(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wsetscrreg(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_wstandout(VALUE, VALUE arg1) {
    return INT2NUM(wstandout(get_window(arg1)));
}
static VALUE rb_wstandend(VALUE, VALUE arg1) {
    return INT2NUM(wstandend(get_window(arg1)));
}
static VALUE rb_wsyncdown(VALUE, VALUE arg1) {
    return ((wsyncdown(get_window(arg1))),Qnil);
}
static VALUE rb_wsyncup(VALUE, VALUE arg1) {
    return ((wsyncup(get_window(arg1))),Qnil);
}
static VALUE rb_wtimeout(VALUE, VALUE arg1, VALUE arg2) {
    return ((wtimeout(get_window(arg1),  NUM2INT(arg2))),Qnil);
}
static VALUE rb_wtouchln(VALUE, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(wtouchln(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_wvline(VALUE, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wvline(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3)));
}
static VALUE rb_color_content(VALUE, VALUE color, VALUE r, VALUE g, VALUE b) {
    if (rb_obj_is_instance_of(r, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(g, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(b, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "r,g and b (2nd to 4th argument) must be an empty Arrays");
        return Qnil;
    }
    short cv[3] = {0,0,0};
    int return_value = color_content(NUM2INT(color), &cv[0], &cv[1], &cv[2]);
    rb_ary_push(r, INT2NUM(cv[0])); rb_ary_push(g, INT2NUM(cv[1]));
    rb_ary_push(b, INT2NUM(cv[2])); return INT2NUM(return_value);
}
static VALUE rb_pair_content(VALUE, VALUE pair, VALUE fg, VALUE bg) {
    if (rb_obj_is_instance_of(fg, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(bg, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "fg and bg (2nd and 3rd argument) must be an empty Arrays");
        return Qnil;
    }
    short cn[2] = {0,0};
    int return_value = pair_content(NUM2INT(pair), &cn[0], &cn[1]);
    rb_ary_push(fg, INT2NUM(cn[0])); rb_ary_push(bg, INT2NUM(cn[1]));
    return INT2NUM(return_value);
}
static VALUE rb_getwin(VALUE, VALUE io)
{
    int fd = dup(NUM2INT(rb_funcall(io, rb_intern("to_i"), 0)));
    FILE * f = fdopen(fd, "r");
    WINDOW * win = getwin(f);
    fclose(f);
    close(fd);
    VALUE return_value = Qnil;
    if (win) return_value = wrap_window(win);
    return return_value;
}
static VALUE rb_putwin(VALUE, VALUE rb_win, VALUE io)
{
    int fd = dup(NUM2INT(rb_funcall(io, rb_intern("to_i"), 0)));
    FILE * f = fdopen(fd, "w");
    WINDOW * win = get_window(rb_win);
    int return_value = putwin(win, f);
    fclose(f);
    close(fd);
    return INT2NUM(return_value);
}
static VALUE rb_unctrl(VALUE, VALUE ch)
{ return rb_str_new2(unctrl(NUM2ULONG(ch))); }
static VALUE rb_newterm(VALUE, VALUE rb_type, VALUE rb_outfd, VALUE rb_infd)
{
    char * type = (rb_type == Qnil) ? (char*)0 : STR2CSTR(rb_type);
    int outfd = NUM2INT(rb_funcall(rb_outfd, rb_intern("to_i"), 0));
    int infd  = NUM2INT(rb_funcall(rb_infd, rb_intern("to_i"), 0));
    return wrap_screen(newterm(type, fdopen(outfd, "w"), fdopen(infd, "r")));
}
    

static void init_functions_2(void) {
    rb_define_module_function(mNcurses, "addch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_addch),
                              1);
    rb_define_module_function(mNcurses, "addchnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_addchnstr),
                              2);
    rb_define_module_function(mNcurses, "addchstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_addchstr),
                              1);
    rb_define_module_function(mNcurses, "addnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_addnstr),
                              2);
    rb_define_module_function(mNcurses, "addstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_addstr),
                              1);
    rb_define_module_function(mNcurses, "attroff",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attroff),
                              1);
    rb_define_module_function(mNcurses, "attron",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attron),
                              1);
    rb_define_module_function(mNcurses, "attrset",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attrset),
                              1);
    rb_define_module_function(mNcurses, "attr_off",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attr_off),
                              2);
    rb_define_module_function(mNcurses, "attr_on",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attr_on),
                              2);
    rb_define_module_function(mNcurses, "attr_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attr_set),
                              3);
    rb_define_module_function(mNcurses, "baudrate",
                              reinterpret_cast<VALUE(*)(...)>(&rb_baudrate),
                              0);
    rb_define_module_function(mNcurses, "beep",
                              reinterpret_cast<VALUE(*)(...)>(&rb_beep),
                              0);
    rb_define_module_function(mNcurses, "bkgd",
                              reinterpret_cast<VALUE(*)(...)>(&rb_bkgd),
                              1);
    rb_define_module_function(mNcurses, "bkgdset",
                              reinterpret_cast<VALUE(*)(...)>(&rb_bkgdset),
                              1);
    rb_define_module_function(mNcurses, "border",
                              reinterpret_cast<VALUE(*)(...)>(&rb_border),
                              8);
    rb_define_module_function(mNcurses, "box",
                              reinterpret_cast<VALUE(*)(...)>(&rb_box),
                              3);
    rb_define_module_function(mNcurses, "can_change_color?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_can_change_color),
                              0);
    rb_define_module_function(mNcurses, "cbreak",
                              reinterpret_cast<VALUE(*)(...)>(&rb_cbreak),
                              0);
    rb_define_module_function(mNcurses, "chgat",
                              reinterpret_cast<VALUE(*)(...)>(&rb_chgat),
                              4);
    rb_define_module_function(mNcurses, "clear",
                              reinterpret_cast<VALUE(*)(...)>(&rb_clear),
                              0);
    rb_define_module_function(mNcurses, "clearok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_clearok),
                              2);
    rb_define_module_function(mNcurses, "clrtobot",
                              reinterpret_cast<VALUE(*)(...)>(&rb_clrtobot),
                              0);
    rb_define_module_function(mNcurses, "clrtoeol",
                              reinterpret_cast<VALUE(*)(...)>(&rb_clrtoeol),
                              0);
    rb_define_module_function(mNcurses, "color_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_color_set),
                              2);
    rb_define_module_function(mNcurses, "COLOR_PAIR",
                              reinterpret_cast<VALUE(*)(...)>(&rb_COLOR_PAIR),
                              1);
    rb_define_module_function(mNcurses, "copywin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_copywin),
                              9);
    rb_define_module_function(mNcurses, "curs_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_curs_set),
                              1);
    rb_define_module_function(mNcurses, "def_prog_mode",
                              reinterpret_cast<VALUE(*)(...)>(&rb_def_prog_mode),
                              0);
    rb_define_module_function(mNcurses, "def_shell_mode",
                              reinterpret_cast<VALUE(*)(...)>(&rb_def_shell_mode),
                              0);
    rb_define_module_function(mNcurses, "delay_output",
                              reinterpret_cast<VALUE(*)(...)>(&rb_delay_output),
                              1);
    rb_define_module_function(mNcurses, "delch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_delch),
                              0);
    rb_define_module_function(mNcurses, "deleteln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_deleteln),
                              0);
    rb_define_module_function(mNcurses, "derwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_derwin),
                              5);
    rb_define_module_function(mNcurses, "doupdate",
                              reinterpret_cast<VALUE(*)(...)>(&rb_doupdate),
                              0);
    rb_define_module_function(mNcurses, "dupwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_dupwin),
                              1);
    rb_define_module_function(mNcurses, "echo",
                              reinterpret_cast<VALUE(*)(...)>(&rb_echo),
                              0);
    rb_define_module_function(mNcurses, "echochar",
                              reinterpret_cast<VALUE(*)(...)>(&rb_echochar),
                              1);
    rb_define_module_function(mNcurses, "endwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_endwin),
                              0);
    rb_define_module_function(mNcurses, "erasechar",
                              reinterpret_cast<VALUE(*)(...)>(&rb_erasechar),
                              0);
    rb_define_module_function(mNcurses, "filter",
                              reinterpret_cast<VALUE(*)(...)>(&rb_filter),
                              0);
    rb_define_module_function(mNcurses, "flash",
                              reinterpret_cast<VALUE(*)(...)>(&rb_flash),
                              0);
    rb_define_module_function(mNcurses, "flushinp",
                              reinterpret_cast<VALUE(*)(...)>(&rb_flushinp),
                              0);
    rb_define_module_function(mNcurses, "getbkgd",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getbkgd),
                              1);
    rb_define_module_function(mNcurses, "getch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getch),
                              0);
    rb_define_module_function(mNcurses, "halfdelay",
                              reinterpret_cast<VALUE(*)(...)>(&rb_halfdelay),
                              1);
    rb_define_module_function(mNcurses, "has_colors?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_has_colors),
                              0);
    rb_define_module_function(mNcurses, "has_ic?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_has_ic),
                              0);
    rb_define_module_function(mNcurses, "has_il?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_has_il),
                              0);
    rb_define_module_function(mNcurses, "hline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_hline),
                              2);
    rb_define_module_function(mNcurses, "idcok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_idcok),
                              2);
    rb_define_module_function(mNcurses, "idlok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_idlok),
                              2);
    rb_define_module_function(mNcurses, "immedok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_immedok),
                              2);
    rb_define_module_function(mNcurses, "inch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_inch),
                              0);
    rb_define_module_function(mNcurses, "initscr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_initscr),
                              0);
    rb_define_module_function(mNcurses, "init_color",
                              reinterpret_cast<VALUE(*)(...)>(&rb_init_color),
                              4);
    rb_define_module_function(mNcurses, "init_pair",
                              reinterpret_cast<VALUE(*)(...)>(&rb_init_pair),
                              3);
    rb_define_module_function(mNcurses, "insch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_insch),
                              1);
    rb_define_module_function(mNcurses, "insdelln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_insdelln),
                              1);
    rb_define_module_function(mNcurses, "insertln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_insertln),
                              0);
    rb_define_module_function(mNcurses, "insnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_insnstr),
                              2);
    rb_define_module_function(mNcurses, "insstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_insstr),
                              1);
    rb_define_module_function(mNcurses, "intrflush",
                              reinterpret_cast<VALUE(*)(...)>(&rb_intrflush),
                              2);
    rb_define_module_function(mNcurses, "isendwin?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_isendwin),
                              0);
    rb_define_module_function(mNcurses, "is_linetouched?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_is_linetouched),
                              2);
    rb_define_module_function(mNcurses, "is_wintouched?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_is_wintouched),
                              1);
    rb_define_module_function(mNcurses, "keyname",
                              reinterpret_cast<VALUE(*)(...)>(&rb_keyname),
                              1);
    rb_define_module_function(mNcurses, "keypad",
                              reinterpret_cast<VALUE(*)(...)>(&rb_keypad),
                              2);
    rb_define_module_function(mNcurses, "killchar",
                              reinterpret_cast<VALUE(*)(...)>(&rb_killchar),
                              0);
    rb_define_module_function(mNcurses, "leaveok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_leaveok),
                              2);
    rb_define_module_function(mNcurses, "longname",
                              reinterpret_cast<VALUE(*)(...)>(&rb_longname),
                              0);
    rb_define_module_function(mNcurses, "meta",
                              reinterpret_cast<VALUE(*)(...)>(&rb_meta),
                              2);
    rb_define_module_function(mNcurses, "move",
                              reinterpret_cast<VALUE(*)(...)>(&rb_move),
                              2);
    rb_define_module_function(mNcurses, "mvaddch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvaddch),
                              3);
    rb_define_module_function(mNcurses, "mvaddchnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvaddchnstr),
                              4);
    rb_define_module_function(mNcurses, "mvaddchstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvaddchstr),
                              3);
    rb_define_module_function(mNcurses, "mvaddnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvaddnstr),
                              4);
    rb_define_module_function(mNcurses, "mvaddstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvaddstr),
                              3);
    rb_define_module_function(mNcurses, "mvchgat",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvchgat),
                              6);
    rb_define_module_function(mNcurses, "mvcur",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvcur),
                              4);
    rb_define_module_function(mNcurses, "mvdelch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvdelch),
                              2);
    rb_define_module_function(mNcurses, "mvderwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvderwin),
                              3);
    rb_define_module_function(mNcurses, "mvgetch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvgetch),
                              2);
    rb_define_module_function(mNcurses, "mvhline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvhline),
                              4);
    rb_define_module_function(mNcurses, "mvinch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvinch),
                              2);
    rb_define_module_function(mNcurses, "mvinsch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvinsch),
                              3);
    rb_define_module_function(mNcurses, "mvinsnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvinsnstr),
                              4);
    rb_define_module_function(mNcurses, "mvinsstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvinsstr),
                              3);
    rb_define_module_function(mNcurses, "mvvline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvvline),
                              4);
    rb_define_module_function(mNcurses, "mvwaddch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwaddch),
                              4);
    rb_define_module_function(mNcurses, "mvwaddchnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwaddchnstr),
                              5);
    rb_define_module_function(mNcurses, "mvwaddchstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwaddchstr),
                              4);
    rb_define_module_function(mNcurses, "mvwaddnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwaddnstr),
                              5);
    rb_define_module_function(mNcurses, "mvwaddstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwaddstr),
                              4);
    rb_define_module_function(mNcurses, "mvwchgat",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwchgat),
                              7);
    rb_define_module_function(mNcurses, "mvwdelch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwdelch),
                              3);
    rb_define_module_function(mNcurses, "mvwgetch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwgetch),
                              3);
    rb_define_module_function(mNcurses, "mvwhline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwhline),
                              5);
    rb_define_module_function(mNcurses, "mvwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwin),
                              3);
    rb_define_module_function(mNcurses, "mvwinch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwinch),
                              3);
    rb_define_module_function(mNcurses, "mvwinsch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwinsch),
                              4);
    rb_define_module_function(mNcurses, "mvwinsnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwinsnstr),
                              5);
    rb_define_module_function(mNcurses, "mvwinsstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwinsstr),
                              4);
    rb_define_module_function(mNcurses, "mvwvline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mvwvline),
                              5);
    rb_define_module_function(mNcurses, "napms",
                              reinterpret_cast<VALUE(*)(...)>(&rb_napms),
                              1);
    rb_define_module_function(mNcurses, "newpad",
                              reinterpret_cast<VALUE(*)(...)>(&rb_newpad),
                              2);
    rb_define_module_function(mNcurses, "newwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_newwin),
                              4);
    rb_define_module_function(mNcurses, "nl",
                              reinterpret_cast<VALUE(*)(...)>(&rb_nl),
                              0);
    rb_define_module_function(mNcurses, "nocbreak",
                              reinterpret_cast<VALUE(*)(...)>(&rb_nocbreak),
                              0);
    rb_define_module_function(mNcurses, "nodelay",
                              reinterpret_cast<VALUE(*)(...)>(&rb_nodelay),
                              2);
    rb_define_module_function(mNcurses, "noecho",
                              reinterpret_cast<VALUE(*)(...)>(&rb_noecho),
                              0);
    rb_define_module_function(mNcurses, "nonl",
                              reinterpret_cast<VALUE(*)(...)>(&rb_nonl),
                              0);
    rb_define_module_function(mNcurses, "noqiflush",
                              reinterpret_cast<VALUE(*)(...)>(&rb_noqiflush),
                              0);
    rb_define_module_function(mNcurses, "noraw",
                              reinterpret_cast<VALUE(*)(...)>(&rb_noraw),
                              0);
    rb_define_module_function(mNcurses, "notimeout",
                              reinterpret_cast<VALUE(*)(...)>(&rb_notimeout),
                              2);
    rb_define_module_function(mNcurses, "overlay",
                              reinterpret_cast<VALUE(*)(...)>(&rb_overlay),
                              2);
    rb_define_module_function(mNcurses, "overwrite",
                              reinterpret_cast<VALUE(*)(...)>(&rb_overwrite),
                              2);
    rb_define_module_function(mNcurses, "PAIR_NUMBER",
                              reinterpret_cast<VALUE(*)(...)>(&rb_PAIR_NUMBER),
                              1);
    rb_define_module_function(mNcurses, "pechochar",
                              reinterpret_cast<VALUE(*)(...)>(&rb_pechochar),
                              2);
    rb_define_module_function(mNcurses, "pnoutrefresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_pnoutrefresh),
                              7);
    rb_define_module_function(mNcurses, "prefresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_prefresh),
                              7);
    rb_define_module_function(mNcurses, "putp",
                              reinterpret_cast<VALUE(*)(...)>(&rb_putp),
                              1);
    rb_define_module_function(mNcurses, "qiflush",
                              reinterpret_cast<VALUE(*)(...)>(&rb_qiflush),
                              0);
    rb_define_module_function(mNcurses, "raw",
                              reinterpret_cast<VALUE(*)(...)>(&rb_raw),
                              0);
    rb_define_module_function(mNcurses, "redrawwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_redrawwin),
                              1);
    rb_define_module_function(mNcurses, "refresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_refresh),
                              0);
    rb_define_module_function(mNcurses, "resetty",
                              reinterpret_cast<VALUE(*)(...)>(&rb_resetty),
                              0);
    rb_define_module_function(mNcurses, "reset_prog_mode",
                              reinterpret_cast<VALUE(*)(...)>(&rb_reset_prog_mode),
                              0);
    rb_define_module_function(mNcurses, "reset_shell_mode",
                              reinterpret_cast<VALUE(*)(...)>(&rb_reset_shell_mode),
                              0);
    rb_define_module_function(mNcurses, "savetty",
                              reinterpret_cast<VALUE(*)(...)>(&rb_savetty),
                              0);
    rb_define_module_function(mNcurses, "scr_dump",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scr_dump),
                              1);
    rb_define_module_function(mNcurses, "scr_init",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scr_init),
                              1);
    rb_define_module_function(mNcurses, "scrl",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scrl),
                              1);
    rb_define_module_function(mNcurses, "scroll",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scroll),
                              1);
    rb_define_module_function(mNcurses, "scrollok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scrollok),
                              2);
    rb_define_module_function(mNcurses, "scr_restore",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scr_restore),
                              1);
    rb_define_module_function(mNcurses, "scr_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_scr_set),
                              1);
    rb_define_module_function(mNcurses, "setscrreg",
                              reinterpret_cast<VALUE(*)(...)>(&rb_setscrreg),
                              2);
    rb_define_module_function(mNcurses, "set_term",
                              reinterpret_cast<VALUE(*)(...)>(&rb_set_term),
                              1);
    rb_define_module_function(mNcurses, "slk_attroff",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attroff),
                              1);
    rb_define_module_function(mNcurses, "slk_attr_off",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attr_off),
                              2);
    rb_define_module_function(mNcurses, "slk_attron",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attron),
                              1);
    rb_define_module_function(mNcurses, "slk_attr_on",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attr_on),
                              2);
    rb_define_module_function(mNcurses, "slk_attrset",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attrset),
                              1);
    rb_define_module_function(mNcurses, "slk_attr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attr),
                              0);
    rb_define_module_function(mNcurses, "slk_attr_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_attr_set),
                              3);
    rb_define_module_function(mNcurses, "slk_clear",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_clear),
                              0);
    rb_define_module_function(mNcurses, "slk_color",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_color),
                              1);
    rb_define_module_function(mNcurses, "slk_init",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_init),
                              1);
    rb_define_module_function(mNcurses, "slk_label",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_label),
                              1);
    rb_define_module_function(mNcurses, "slk_noutrefresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_noutrefresh),
                              0);
    rb_define_module_function(mNcurses, "slk_refresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_refresh),
                              0);
    rb_define_module_function(mNcurses, "slk_restore",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_restore),
                              0);
    rb_define_module_function(mNcurses, "slk_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_set),
                              3);
    rb_define_module_function(mNcurses, "slk_touch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_slk_touch),
                              0);
    rb_define_module_function(mNcurses, "standout",
                              reinterpret_cast<VALUE(*)(...)>(&rb_standout),
                              0);
    rb_define_module_function(mNcurses, "standend",
                              reinterpret_cast<VALUE(*)(...)>(&rb_standend),
                              0);
    rb_define_module_function(mNcurses, "start_color",
                              reinterpret_cast<VALUE(*)(...)>(&rb_start_color),
                              0);
    rb_define_module_function(mNcurses, "subpad",
                              reinterpret_cast<VALUE(*)(...)>(&rb_subpad),
                              5);
    rb_define_module_function(mNcurses, "subwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_subwin),
                              5);
    rb_define_module_function(mNcurses, "syncok",
                              reinterpret_cast<VALUE(*)(...)>(&rb_syncok),
                              2);
    rb_define_module_function(mNcurses, "termattrs",
                              reinterpret_cast<VALUE(*)(...)>(&rb_termattrs),
                              0);
    rb_define_module_function(mNcurses, "termname",
                              reinterpret_cast<VALUE(*)(...)>(&rb_termname),
                              0);
    rb_define_module_function(mNcurses, "tigetflag",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tigetflag),
                              1);
    rb_define_module_function(mNcurses, "tigetnum",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tigetnum),
                              1);
    rb_define_module_function(mNcurses, "tigetstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tigetstr),
                              1);
    rb_define_module_function(mNcurses, "timeout",
                              reinterpret_cast<VALUE(*)(...)>(&rb_timeout),
                              1);
    rb_define_module_function(mNcurses, "typeahead",
                              reinterpret_cast<VALUE(*)(...)>(&rb_typeahead),
                              1);
    rb_define_module_function(mNcurses, "ungetch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_ungetch),
                              1);
    rb_define_module_function(mNcurses, "untouchwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_untouchwin),
                              1);
    rb_define_module_function(mNcurses, "use_env",
                              reinterpret_cast<VALUE(*)(...)>(&rb_use_env),
                              1);
    rb_define_module_function(mNcurses, "vidattr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_vidattr),
                              1);
    rb_define_module_function(mNcurses, "vid_attr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_vid_attr),
                              3);
    rb_define_module_function(mNcurses, "vline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_vline),
                              2);
    rb_define_module_function(mNcurses, "waddch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_waddch),
                              2);
    rb_define_module_function(mNcurses, "waddchnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_waddchnstr),
                              3);
    rb_define_module_function(mNcurses, "waddchstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_waddchstr),
                              2);
    rb_define_module_function(mNcurses, "waddnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_waddnstr),
                              3);
    rb_define_module_function(mNcurses, "waddstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_waddstr),
                              2);
    rb_define_module_function(mNcurses, "wattron",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattron),
                              2);
    rb_define_module_function(mNcurses, "wattroff",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattroff),
                              2);
    rb_define_module_function(mNcurses, "wattrset",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattrset),
                              2);
    rb_define_module_function(mNcurses, "wattr_on",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattr_on),
                              3);
    rb_define_module_function(mNcurses, "wattr_off",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattr_off),
                              3);
    rb_define_module_function(mNcurses, "wattr_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattr_set),
                              4);
    rb_define_module_function(mNcurses, "wbkgd",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wbkgd),
                              2);
    rb_define_module_function(mNcurses, "wbkgdset",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wbkgdset),
                              2);
    rb_define_module_function(mNcurses, "wborder",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wborder),
                              9);
    rb_define_module_function(mNcurses, "wchgat",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wchgat),
                              5);
    rb_define_module_function(mNcurses, "wclear",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wclear),
                              1);
    rb_define_module_function(mNcurses, "wclrtobot",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wclrtobot),
                              1);
    rb_define_module_function(mNcurses, "wclrtoeol",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wclrtoeol),
                              1);
    rb_define_module_function(mNcurses, "wcolor_set",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wcolor_set),
                              3);
    rb_define_module_function(mNcurses, "wcursyncup",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wcursyncup),
                              1);
    rb_define_module_function(mNcurses, "wdelch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wdelch),
                              1);
    rb_define_module_function(mNcurses, "wdeleteln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wdeleteln),
                              1);
    rb_define_module_function(mNcurses, "wechochar",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wechochar),
                              2);
    rb_define_module_function(mNcurses, "werase",
                              reinterpret_cast<VALUE(*)(...)>(&rb_werase),
                              1);
    rb_define_module_function(mNcurses, "wgetch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wgetch),
                              1);
    rb_define_module_function(mNcurses, "whline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_whline),
                              3);
    rb_define_module_function(mNcurses, "winch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winch),
                              1);
    rb_define_module_function(mNcurses, "winsch",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winsch),
                              2);
    rb_define_module_function(mNcurses, "winsdelln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winsdelln),
                              2);
    rb_define_module_function(mNcurses, "winsertln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winsertln),
                              1);
    rb_define_module_function(mNcurses, "winsnstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winsnstr),
                              3);
    rb_define_module_function(mNcurses, "winsstr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_winsstr),
                              2);
    rb_define_module_function(mNcurses, "wmove",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wmove),
                              3);
    rb_define_module_function(mNcurses, "wnoutrefresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wnoutrefresh),
                              1);
    rb_define_module_function(mNcurses, "wredrawln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wredrawln),
                              3);
    rb_define_module_function(mNcurses, "wrefresh",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wrefresh),
                              1);
    rb_define_module_function(mNcurses, "wscrl",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wscrl),
                              2);
    rb_define_module_function(mNcurses, "wsetscrreg",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wsetscrreg),
                              3);
    rb_define_module_function(mNcurses, "wstandout",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wstandout),
                              1);
    rb_define_module_function(mNcurses, "wstandend",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wstandend),
                              1);
    rb_define_module_function(mNcurses, "wsyncdown",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wsyncdown),
                              1);
    rb_define_module_function(mNcurses, "wsyncup",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wsyncup),
                              1);
    rb_define_module_function(mNcurses, "wtimeout",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wtimeout),
                              2);
    rb_define_module_function(mNcurses, "wtouchln",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wtouchln),
                              4);
    rb_define_module_function(mNcurses, "wvline",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wvline),
                              3);
    rb_define_module_function(mNcurses, "color_content",
                              reinterpret_cast<VALUE(*)(...)>(&rb_color_content), 4);
    rb_define_module_function(mNcurses, "pair_content",
                              reinterpret_cast<VALUE(*)(...)>(&rb_pair_content), 3);
    rb_define_module_function(mNcurses, "pair_content",
                              reinterpret_cast<VALUE(*)(...)>(&rb_pair_content), 3);
    rb_define_module_function(mNcurses, "getwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getwin), 1);
    rb_define_module_function(mNcurses, "putwin",
                              reinterpret_cast<VALUE(*)(...)>(&rb_putwin), 2);
    rb_define_module_function(mNcurses, "unctrl",
                              reinterpret_cast<VALUE(*)(...)>(&rb_unctrl), 1);
    rb_define_module_function(mNcurses, "newterm",
                              reinterpret_cast<VALUE(*)(...)>(&rb_newterm), 3);
}


static void init_constants_3(void) {
    // #define NCURSES_BITS(mask,shift) ((mask) << ((shift) + 8))

    /* attributes */

    rb_define_const(mNcurses, "A_NORMAL", INT2NUM(A_NORMAL));
    rb_define_const(mNcurses, "A_ATTRIBUTES", INT2NUM(A_ATTRIBUTES));
    rb_define_const(mNcurses, "A_CHARTEXT", INT2NUM(A_CHARTEXT));
    rb_define_const(mNcurses, "A_COLOR", INT2NUM(A_COLOR));
    rb_define_const(mNcurses, "A_STANDOUT", INT2NUM(A_STANDOUT));
    rb_define_const(mNcurses, "A_UNDERLINE", INT2NUM(A_UNDERLINE));
    rb_define_const(mNcurses, "A_REVERSE", INT2NUM(A_REVERSE));
    rb_define_const(mNcurses, "A_BLINK", INT2NUM(A_BLINK));
    rb_define_const(mNcurses, "A_DIM", INT2NUM(A_DIM));
    rb_define_const(mNcurses, "A_BOLD", INT2NUM(A_BOLD));
    rb_define_const(mNcurses, "A_ALTCHARSET", INT2NUM(A_ALTCHARSET));
    rb_define_const(mNcurses, "A_INVIS", INT2NUM(A_INVIS));

// /* Tradeoff on 32-bit machines ('protect' vs widec).  The others (e.g., left
//  * highlight are not implemented in any terminal descriptions, anyway.
//  */
    rb_define_const(mNcurses, "A_PROTECT", INT2NUM(A_PROTECT));
    rb_define_const(mNcurses, "A_HORIZONTAL", INT2NUM(A_HORIZONTAL));
    rb_define_const(mNcurses, "A_LEFT", INT2NUM(A_LEFT));
    rb_define_const(mNcurses, "A_LOW", INT2NUM(A_LOW));
    rb_define_const(mNcurses, "A_RIGHT", INT2NUM(A_RIGHT));
    rb_define_const(mNcurses, "A_TOP", INT2NUM(A_TOP));
    rb_define_const(mNcurses, "A_VERTICAL", INT2NUM(A_VERTICAL));

    //* Pseudo-character tokens outside ASCII range.  The curses wgetch()
    //* function will return any given one of these only if the corresponding
    //* k- capability is defined in your terminal's terminfo entry.
    rb_define_const(mNcurses, "KEY_CODE_YES", INT2NUM(KEY_CODE_YES));
    rb_define_const(mNcurses, "KEY_MIN", INT2NUM(KEY_MIN));
    rb_define_const(mNcurses, "KEY_BREAK", INT2NUM(KEY_BREAK));
    rb_define_const(mNcurses, "KEY_DOWN", INT2NUM(KEY_DOWN));
    rb_define_const(mNcurses, "KEY_UP", INT2NUM(KEY_UP));
    rb_define_const(mNcurses, "KEY_LEFT", INT2NUM(KEY_LEFT));
    rb_define_const(mNcurses, "KEY_RIGHT", INT2NUM(KEY_RIGHT));
    rb_define_const(mNcurses, "KEY_HOME", INT2NUM(KEY_HOME));
    rb_define_const(mNcurses, "KEY_BACKSPACE", INT2NUM(KEY_BACKSPACE));
    rb_define_const(mNcurses, "KEY_F0",  INT2NUM(KEY_F(0)));
    rb_define_const(mNcurses, "KEY_F1",  INT2NUM(KEY_F(1)));
    rb_define_const(mNcurses, "KEY_F2",  INT2NUM(KEY_F(2)));
    rb_define_const(mNcurses, "KEY_F3",  INT2NUM(KEY_F(3)));
    rb_define_const(mNcurses, "KEY_F4",  INT2NUM(KEY_F(4)));
    rb_define_const(mNcurses, "KEY_F5",  INT2NUM(KEY_F(5)));
    rb_define_const(mNcurses, "KEY_F6",  INT2NUM(KEY_F(6)));
    rb_define_const(mNcurses, "KEY_F7",  INT2NUM(KEY_F(7)));
    rb_define_const(mNcurses, "KEY_F8",  INT2NUM(KEY_F(8)));
    rb_define_const(mNcurses, "KEY_F9",  INT2NUM(KEY_F(9)));
    rb_define_const(mNcurses, "KEY_F10", INT2NUM(KEY_F(10)));
    rb_define_const(mNcurses, "KEY_F11", INT2NUM(KEY_F(11)));
    rb_define_const(mNcurses, "KEY_F12", INT2NUM(KEY_F(12)));
    rb_define_const(mNcurses, "KEY_F13", INT2NUM(KEY_F(13)));
    rb_define_const(mNcurses, "KEY_F14", INT2NUM(KEY_F(14)));
    rb_define_const(mNcurses, "KEY_F15", INT2NUM(KEY_F(15)));
    rb_define_const(mNcurses, "KEY_F16", INT2NUM(KEY_F(16)));
    rb_define_const(mNcurses, "KEY_F17", INT2NUM(KEY_F(17)));
    rb_define_const(mNcurses, "KEY_F18", INT2NUM(KEY_F(18)));
    rb_define_const(mNcurses, "KEY_F19", INT2NUM(KEY_F(19)));
    rb_define_const(mNcurses, "KEY_F20", INT2NUM(KEY_F(20)));
    rb_define_const(mNcurses, "KEY_F21", INT2NUM(KEY_F(21)));
    rb_define_const(mNcurses, "KEY_F22", INT2NUM(KEY_F(22)));
    rb_define_const(mNcurses, "KEY_F23", INT2NUM(KEY_F(23)));
    rb_define_const(mNcurses, "KEY_F24", INT2NUM(KEY_F(24)));
    rb_define_const(mNcurses, "KEY_F25", INT2NUM(KEY_F(25)));
    rb_define_const(mNcurses, "KEY_F26", INT2NUM(KEY_F(26)));
    rb_define_const(mNcurses, "KEY_F27", INT2NUM(KEY_F(27)));
    rb_define_const(mNcurses, "KEY_F28", INT2NUM(KEY_F(28)));
    rb_define_const(mNcurses, "KEY_F29", INT2NUM(KEY_F(29)));
    rb_define_const(mNcurses, "KEY_F30", INT2NUM(KEY_F(30)));
    rb_define_const(mNcurses, "KEY_DL", INT2NUM(KEY_DL));
    rb_define_const(mNcurses, "KEY_IL", INT2NUM(KEY_IL));
    rb_define_const(mNcurses, "KEY_DC", INT2NUM(KEY_DC));
    rb_define_const(mNcurses, "KEY_IC", INT2NUM(KEY_IC));
    rb_define_const(mNcurses, "KEY_EIC", INT2NUM(KEY_EIC));
    rb_define_const(mNcurses, "KEY_CLEAR", INT2NUM(KEY_CLEAR));
    rb_define_const(mNcurses, "KEY_EOS", INT2NUM(KEY_EOS));
    rb_define_const(mNcurses, "KEY_EOL", INT2NUM(KEY_EOL));
    rb_define_const(mNcurses, "KEY_SF", INT2NUM(KEY_SF));
    rb_define_const(mNcurses, "KEY_SR", INT2NUM(KEY_SR));
    rb_define_const(mNcurses, "KEY_NPAGE", INT2NUM(KEY_NPAGE));
    rb_define_const(mNcurses, "KEY_PPAGE", INT2NUM(KEY_PPAGE));
    rb_define_const(mNcurses, "KEY_STAB", INT2NUM(KEY_STAB));
    rb_define_const(mNcurses, "KEY_CTAB", INT2NUM(KEY_CTAB));
    rb_define_const(mNcurses, "KEY_CATAB", INT2NUM(KEY_CATAB));
    rb_define_const(mNcurses, "KEY_ENTER", INT2NUM(KEY_ENTER));
    rb_define_const(mNcurses, "KEY_SRESET", INT2NUM(KEY_SRESET));
    rb_define_const(mNcurses, "KEY_RESET", INT2NUM(KEY_RESET));
    rb_define_const(mNcurses, "KEY_PRINT", INT2NUM(KEY_PRINT));
    rb_define_const(mNcurses, "KEY_LL", INT2NUM(KEY_LL));

// /* The keypad is arranged like this: */
// /* a1    up    a3   */
// /* left   b2  right  */
// /* c1   down   c3   */

    rb_define_const(mNcurses, "KEY_A1", INT2NUM(KEY_A1));
    rb_define_const(mNcurses, "KEY_A3", INT2NUM(KEY_A3));
    rb_define_const(mNcurses, "KEY_B2", INT2NUM(KEY_B2));
    rb_define_const(mNcurses, "KEY_C1", INT2NUM(KEY_C1));
    rb_define_const(mNcurses, "KEY_C3", INT2NUM(KEY_C3));
    rb_define_const(mNcurses, "KEY_BTAB", INT2NUM(KEY_BTAB));
    rb_define_const(mNcurses, "KEY_BEG", INT2NUM(KEY_BEG));
    rb_define_const(mNcurses, "KEY_CANCEL", INT2NUM(KEY_CANCEL));
    rb_define_const(mNcurses, "KEY_CLOSE", INT2NUM(KEY_CLOSE));
    rb_define_const(mNcurses, "KEY_COMMAND", INT2NUM(KEY_COMMAND));
    rb_define_const(mNcurses, "KEY_COPY", INT2NUM(KEY_COPY));
    rb_define_const(mNcurses, "KEY_CREATE", INT2NUM(KEY_CREATE));
    rb_define_const(mNcurses, "KEY_END", INT2NUM(KEY_END));
    rb_define_const(mNcurses, "KEY_EXIT", INT2NUM(KEY_EXIT));
    rb_define_const(mNcurses, "KEY_FIND", INT2NUM(KEY_FIND));
    rb_define_const(mNcurses, "KEY_HELP", INT2NUM(KEY_HELP));
    rb_define_const(mNcurses, "KEY_MARK", INT2NUM(KEY_MARK));
    rb_define_const(mNcurses, "KEY_MESSAGE", INT2NUM(KEY_MESSAGE));
    rb_define_const(mNcurses, "KEY_MOVE", INT2NUM(KEY_MOVE));
    rb_define_const(mNcurses, "KEY_NEXT", INT2NUM(KEY_NEXT));
    rb_define_const(mNcurses, "KEY_OPEN", INT2NUM(KEY_OPEN));
    rb_define_const(mNcurses, "KEY_OPTIONS", INT2NUM(KEY_OPTIONS));
    rb_define_const(mNcurses, "KEY_PREVIOUS", INT2NUM(KEY_PREVIOUS));
    rb_define_const(mNcurses, "KEY_REDO", INT2NUM(KEY_REDO));
    rb_define_const(mNcurses, "KEY_REFERENCE", INT2NUM(KEY_REFERENCE));
    rb_define_const(mNcurses, "KEY_REFRESH", INT2NUM(KEY_REFRESH));
    rb_define_const(mNcurses, "KEY_REPLACE", INT2NUM(KEY_REPLACE));
    rb_define_const(mNcurses, "KEY_RESTART", INT2NUM(KEY_RESTART));
    rb_define_const(mNcurses, "KEY_RESUME", INT2NUM(KEY_RESUME));
    rb_define_const(mNcurses, "KEY_SAVE", INT2NUM(KEY_SAVE));
    rb_define_const(mNcurses, "KEY_SBEG", INT2NUM(KEY_SBEG));
    rb_define_const(mNcurses, "KEY_SCANCEL", INT2NUM(KEY_SCANCEL));
    rb_define_const(mNcurses, "KEY_SCOMMAND", INT2NUM(KEY_SCOMMAND));
    rb_define_const(mNcurses, "KEY_SCOPY", INT2NUM(KEY_SCOPY));
    rb_define_const(mNcurses, "KEY_SCREATE", INT2NUM(KEY_SCREATE));
    rb_define_const(mNcurses, "KEY_SDC", INT2NUM(KEY_SDC));
    rb_define_const(mNcurses, "KEY_SDL", INT2NUM(KEY_SDL));
    rb_define_const(mNcurses, "KEY_SELECT", INT2NUM(KEY_SELECT));
    rb_define_const(mNcurses, "KEY_SEND", INT2NUM(KEY_SEND));
    rb_define_const(mNcurses, "KEY_SEOL", INT2NUM(KEY_SEOL));
    rb_define_const(mNcurses, "KEY_SEXIT", INT2NUM(KEY_SEXIT));
    rb_define_const(mNcurses, "KEY_SFIND", INT2NUM(KEY_SFIND));
    rb_define_const(mNcurses, "KEY_SHELP", INT2NUM(KEY_SHELP));
    rb_define_const(mNcurses, "KEY_SHOME", INT2NUM(KEY_SHOME));
    rb_define_const(mNcurses, "KEY_SIC", INT2NUM(KEY_SIC));
    rb_define_const(mNcurses, "KEY_SLEFT", INT2NUM(KEY_SLEFT));
    rb_define_const(mNcurses, "KEY_SMESSAGE", INT2NUM(KEY_SMESSAGE));
    rb_define_const(mNcurses, "KEY_SMOVE", INT2NUM(KEY_SMOVE));
    rb_define_const(mNcurses, "KEY_SNEXT", INT2NUM(KEY_SNEXT));
    rb_define_const(mNcurses, "KEY_SOPTIONS", INT2NUM(KEY_SOPTIONS));
    rb_define_const(mNcurses, "KEY_SPREVIOUS", INT2NUM(KEY_SPREVIOUS));
    rb_define_const(mNcurses, "KEY_SPRINT", INT2NUM(KEY_SPRINT));
    rb_define_const(mNcurses, "KEY_SREDO", INT2NUM(KEY_SREDO));
    rb_define_const(mNcurses, "KEY_SREPLACE", INT2NUM(KEY_SREPLACE));
    rb_define_const(mNcurses, "KEY_SRIGHT", INT2NUM(KEY_SRIGHT));
    rb_define_const(mNcurses, "KEY_SRSUME", INT2NUM(KEY_SRSUME));
    rb_define_const(mNcurses, "KEY_SSAVE", INT2NUM(KEY_SSAVE));
    rb_define_const(mNcurses, "KEY_SSUSPEND", INT2NUM(KEY_SSUSPEND));
    rb_define_const(mNcurses, "KEY_SUNDO", INT2NUM(KEY_SUNDO));
    rb_define_const(mNcurses, "KEY_SUSPEND", INT2NUM(KEY_SUSPEND));
    rb_define_const(mNcurses, "KEY_UNDO", INT2NUM(KEY_UNDO));
    rb_define_const(mNcurses, "KEY_MOUSE", INT2NUM(KEY_MOUSE));
    rb_define_const(mNcurses, "KEY_RESIZE", INT2NUM(KEY_RESIZE));
    rb_define_const(mNcurses, "KEY_MAX", INT2NUM(KEY_MAX));

// /* mouse interface */
// #define NCURSES_MOUSE_VERSION	1

// /* event masks */
    rb_define_const(mNcurses, "BUTTON1_RELEASED", INT2NUM(BUTTON1_RELEASED));
    rb_define_const(mNcurses, "BUTTON1_PRESSED", INT2NUM(BUTTON1_PRESSED));
    rb_define_const(mNcurses, "BUTTON1_CLICKED", INT2NUM(BUTTON1_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_DOUBLE_CLICKED", INT2NUM(BUTTON1_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_TRIPLE_CLICKED", INT2NUM(BUTTON1_TRIPLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_RESERVED_EVENT", INT2NUM(BUTTON1_RESERVED_EVENT));
    rb_define_const(mNcurses, "BUTTON2_RELEASED", INT2NUM(BUTTON2_RELEASED));
    rb_define_const(mNcurses, "BUTTON2_PRESSED", INT2NUM(BUTTON2_PRESSED));
    rb_define_const(mNcurses, "BUTTON2_CLICKED", INT2NUM(BUTTON2_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_DOUBLE_CLICKED", INT2NUM(BUTTON1_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_TRIPLE_CLICKED", INT2NUM(BUTTON1_TRIPLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_RESERVED_EVENT", INT2NUM(BUTTON1_RESERVED_EVENT));
    rb_define_const(mNcurses, "BUTTON3_RELEASED", INT2NUM(BUTTON3_RELEASED));
    rb_define_const(mNcurses, "BUTTON3_PRESSED", INT2NUM(BUTTON3_PRESSED));
    rb_define_const(mNcurses, "BUTTON3_CLICKED", INT2NUM(BUTTON3_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_DOUBLE_CLICKED", INT2NUM(BUTTON1_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_TRIPLE_CLICKED", INT2NUM(BUTTON1_TRIPLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_RESERVED_EVENT", INT2NUM(BUTTON1_RESERVED_EVENT));
    rb_define_const(mNcurses, "BUTTON4_RELEASED", INT2NUM(BUTTON4_RELEASED));
    rb_define_const(mNcurses, "BUTTON4_PRESSED", INT2NUM(BUTTON4_PRESSED));
    rb_define_const(mNcurses, "BUTTON4_CLICKED", INT2NUM(BUTTON4_CLICKED));
    rb_define_const(mNcurses, "BUTTON4_DOUBLE_CLICKED", INT2NUM(BUTTON1_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON4_TRIPLE_CLICKED", INT2NUM(BUTTON1_TRIPLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON4_RESERVED_EVENT", INT2NUM(BUTTON1_RESERVED_EVENT));
    rb_define_const(mNcurses, "BUTTON_CTRL", INT2NUM(BUTTON_CTRL));
    rb_define_const(mNcurses, "BUTTON_SHIFT", INT2NUM(BUTTON_SHIFT));
    rb_define_const(mNcurses, "BUTTON_ALT", INT2NUM(BUTTON_ALT));
    rb_define_const(mNcurses, "ALL_MOUSE_EVENTS", INT2NUM(ALL_MOUSE_EVENTS));
    rb_define_const(mNcurses, "REPORT_MOUSE_POSITION", INT2NUM(REPORT_MOUSE_POSITION));
}
// typedef struct
// {
//     short id;		/* ID to distinguish multiple devices */
//     int x, y, z;	/* event coordinates (character-cell) */
//     mmask_t bstate;	/* button state bits */
// }
// MEVENT;

static VALUE rb_getmouse(VALUE, VALUE rb_m)
{
    MEVENT m;
    int return_value = getmouse(&m);
    if (return_value != ERR) {
        rb_iv_set(rb_m, "@id", INT2NUM(m.id));
        rb_iv_set(rb_m, "@x", INT2NUM(m.x));
        rb_iv_set(rb_m, "@y", INT2NUM(m.y));
        rb_iv_set(rb_m, "@z", INT2NUM(m.z));
        rb_iv_set(rb_m, "@bstate", INT2NUM(m.bstate));
    }
    return INT2NUM(return_value);
}
static VALUE rb_ungetmouse(VALUE, VALUE rb_m)
{
    MEVENT m;
    m.id = NUM2INT(rb_iv_get(rb_m, "@id"));
    m.x  = NUM2INT(rb_iv_get(rb_m, "@x"));
    m.y  = NUM2INT(rb_iv_get(rb_m, "@y"));
    m.z  = NUM2INT(rb_iv_get(rb_m, "@z"));
    m.bstate  = NUM2ULONG(rb_iv_get(rb_m, "@bstate"));
    return INT2NUM(ungetmouse(&m));
}
static VALUE rb_mousemask(VALUE, VALUE rb_newmask, VALUE rb_oldmask)
{
    if (rb_obj_is_instance_of(rb_oldmask, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "oldmask (2nd argument) must be an empty Array");
        return Qnil;
    }
    mmask_t oldmask, return_value;
    return_value = mousemask(NUM2ULONG(rb_newmask), &oldmask);
    rb_ary_push(rb_oldmask, INT2NUM(oldmask));
    return INT2NUM(return_value);
}
static VALUE rb_wenclose(VALUE, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    return wenclose(get_window(rb_win), NUM2INT(rb_y), NUM2INT(rb_y))
        ? Qtrue : Qfalse;
}
static VALUE rb_mouseinterval(VALUE, VALUE rb_interval)
{ return INT2NUM(mouseinterval(NUM2INT(rb_interval))); }
static VALUE rb_wmouse_trafo(VALUE, VALUE rb_win, VALUE rb_pY, VALUE rb_pX,
                             VALUE rb_to_screen)
{
    if ((rb_obj_is_instance_of(rb_pY, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pY, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "pY and pX arguments must be Arrays, containing exactly one "
                 "Integer");
        return Qnil;
    }
    int X =                             NUM2INT(rb_ary_pop(rb_pX));
    int Y = NUM2INT(rb_ary_pop(rb_pY));
    bool return_value =
        wmouse_trafo(get_window(rb_win), &Y, &X, RTEST(rb_to_screen));
    rb_ary_push(rb_pY, INT2NUM(Y)); rb_ary_push(rb_pX, INT2NUM(X));
    return return_value ? Qtrue : Qfalse;
}
static VALUE rb_mcprint(VALUE, VALUE data, VALUE len)
{
    return INT2NUM(mcprint(STR2CSTR(data), NUM2INT(len)));
}
static VALUE rb_has_key(VALUE, VALUE ch)
{return INT2NUM(has_key(NUM2INT(ch)));}
static VALUE rb_getyx(VALUE, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    int y,x;
    getyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getattrs(VALUE, VALUE rb_win)
{return INT2NUM(getattrs(get_window(rb_win)));}
static VALUE rb_getbegyx(VALUE, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    int y,x;
    getbegyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getmaxyx(VALUE, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    int y,x;
    getmaxyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getparyx(VALUE, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    int y,x;
    getparyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getsyx(VALUE, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    int y,x;
    getsyx(y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_setsyx(VALUE, VALUE rb_y, VALUE rb_x)
{
    int y = NUM2INT(rb_y), x = NUM2INT(rb_x);
    setsyx(y,x);
    return Qnil;
}

static VALUE rb_wprintw(int argc, VALUE * argv, VALUE)
{
    if (argc < 2) {
        rb_raise(rb_eArgError, "function needs at least 2 arguments: a WINDOW"
                 " and a String");
        return Qnil;
    }
    wprintw(get_window(argv[0]), "%s",
            STR2CSTR(rb_funcall3(rb_mKernel, rb_intern("sprintf"), argc-1,
                              argv + 1)));
    return Qnil;
}

// /* Debugging : use with libncurses_g.a */
#ifdef HAVE__TRACEF
static VALUE rb_tracef(int argc, VALUE * argv, VALUE)
{
    if (argc < 1) {
        rb_raise(rb_eArgError, "function needs at least 1 argument");
        return Qnil;
    }
    _tracef("%s",
            STR2CSTR(funcall3(rb_mKernel, rb_intern("sprintf"), argc, argv)));
    return Qnil;
}
#endif // HAVE__TRACEF
#ifdef HAVE__TRACEDUMP
static VALUE rb_tracedump(VALUE, VALUE rb_label, label rb_win)
{
    _tracedump(STR2CSTR(rb_label), get_window(rb_win));
}
#endif // HAVE__TRACEDUMP
#ifdef HAVE__TRACEATTR
static VALUE rb_traceattr(VALUE, VALUE attr)
{ return rb_str_new2(_traceattr(NUM2ULONG(attr))); }
#endif // HAVE__TRACEATTR
#ifdef HAVE__TRACEATTR2
static VALUE rb_traceattr2(VALUE, VALUE buffer, VALUE ch)
{ return rb_str_new2(_traceattr2(NUM2INT(buffer),NUM2ULONG(ch))); }
#endif // HAVE__TRACEATTR2
#ifdef HAVE__TRACEBITS
static VALUE rb_tracebits(VALUE)
{ return rb_str_new2(_tracebits()); }
#endif // HAVE__TRACEBITS
#ifdef HAVE__TRACECHAR
static VALUE rb_tracechar(VALUE, VALUE ch)
{ return rb_str_new2(_tracechar(NUM2ULONG(ch))); }
#endif // HAVE__TRACECHAR
#ifdef HAVE__TRACECHTYPE
static VALUE rb_tracechtype(VALUE, VALUE ch)
{ return rb_str_new2(_tracechtype(NUM2ULONG(ch))); }
#endif // HAVE__TRACECHTYPE
#ifdef HAVE__TRACECHTYPE2
static VALUE rb_tracechtype2(VALUE, VALUE buffer, VALUE ch)
{ return rb_str_new2(_tracechtype2(NUM2INT(buffer),NUM2ULONG(ch))); }
#endif // HAVE__TRACECHTYPE2
#ifdef HAVE__TRACEMOUSE
static VALUE rb_tracemouse(VALUE, VALUE rb_m)
{
    MEVENT m;
    m.id = NUM2INT(rb_iv_get(rb_m, "@id"));
    m.x  = NUM2INT(rb_iv_get(rb_m, "@x"));
    m.y  = NUM2INT(rb_iv_get(rb_m, "@y"));
    m.z  = NUM2INT(rb_iv_get(rb_m, "@z"));
    m.bstate  = NUM2ULONG(rb_iv_get(rb_m, "@bstate"));
    return rb_str_new2(_tracemouse(&m));
}
#endif // HAVE__TRACEMOUSE
#ifdef HAVE_TRACE
static VALUE rb_trace(VALUE, VALUE param)
{ trace(NUM2ULONG(param)); return Qnil; }
#endif // HAVE_TRACE
#ifdef HAVE__NC_TRACEBITS
static VALUE rb_nc_tracebits(...)
{ return rb_str_new2(_nc_tracebits()); }
#endif // HAVE__NC_TRACEBITS

#ifdef HAVE_ASSUME_DEFAULT_COLORS
static VALUE rb_assume_default_colors(VALUE, VALUE fg, VALUE bg)
{ return INT2NUM(assume_default_colors(NUM2INT(fg),NUM2INT(bg))); }
#endif  // HAVE_ASSUME_DEFAULT_COLORS
#ifdef HAVE_ATTR_GET
static VALUE rb_attr_get(VALUE, VALUE rb_attrs, VALUE rb_pair, VALUE)
{
    if ((rb_obj_is_instance_of(rb_attrs, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pair, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "attrs and pair arguments must be empty Arrays");
        return Qnil;
    }
    attr_t attrs = 0;
    short  pair  = 0;
    int return_value = attr_get(&attrs, &pair, 0);
    rb_ary_push(rb_attrs, INT2NUM(attrs));
    rb_ary_push(rb_pair, INT2NUM(pair));
    return INT2NUM(return_value);
}
static VALUE rb_wattr_get(VALUE,VALUE win, VALUE rb_attrs, VALUE rb_pair,VALUE)
{
    if ((rb_obj_is_instance_of(rb_attrs, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pair, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "attrs and pair arguments must be empty Arrays");
        return Qnil;
    }
    attr_t attrs = 0;
    short  pair  = 0;
    int return_value = wattr_get(get_window(win), &attrs, &pair, 0);
    rb_ary_push(rb_attrs, INT2NUM(attrs));
    rb_ary_push(rb_pair, INT2NUM(pair));
    return INT2NUM(return_value);
}
#endif // HAVE_ATTR_GET

static void init_functions_3(void)
{
    rb_define_module_function(mNcurses, "getmouse",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getmouse),
                              1);
    rb_define_module_function(mNcurses, "ungetmouse",
                              reinterpret_cast<VALUE(*)(...)>(&rb_ungetmouse),
                              1);
    rb_define_module_function(mNcurses, "mousemask",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mousemask),
                              1);
    rb_define_module_function(mNcurses, "wenclose?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wenclose),
                              1);
    rb_define_module_function(mNcurses, "mouseinterval",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mouseinterval), 1);
    rb_define_module_function(mNcurses, "wmouse_trafo",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wmouse_trafo), 4);
    rb_define_module_function(mNcurses, "mcprint",
                              reinterpret_cast<VALUE(*)(...)>(&rb_mcprint),
                              2);
    rb_define_module_function(mNcurses, "has_key?",
                              reinterpret_cast<VALUE(*)(...)>(&rb_has_key),
                              2);
    rb_define_module_function(mNcurses, "getyx",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getyx),
                              3);
    rb_define_module_function(mNcurses, "getbegyx",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getbegyx),
                              3);
    rb_define_module_function(mNcurses, "getmaxyx",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getmaxyx),
                              3);
    rb_define_module_function(mNcurses, "getparyx",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getparyx),
                              3);
    rb_define_module_function(mNcurses, "getsyx",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getsyx),
                              2);
    rb_define_module_function(mNcurses, "setsyx",
                              reinterpret_cast<VALUE(*)(...)>(&rb_setsyx),
                              2);
    rb_define_module_function(mNcurses, "getattrs",
                              reinterpret_cast<VALUE(*)(...)>(&rb_getattrs),
                              1);

#ifdef HAVE__TRACEF
    rb_define_module_function(mNcurses, "_tracef",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracef), -1);
#endif // HAVE__TRACEF
#ifdef HAVE__TRACEDUMP
    rb_define_module_function(mNcurses, "_tracedump",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracedump),
                              2);
#endif // HAVE__TRACEDUMP
#ifdef HAVE__TRACEATTR
    rb_define_module_function(mNcurses, "_traceattr",
                              reinterpret_cast<VALUE(*)(...)>(&rb_traceattr),
                              1);
#endif // HAVE__TRACEATTR
#ifdef HAVE__TRACEATTR2
    rb_define_module_function(mNcurses, "_traceattr2",
                              reinterpret_cast<VALUE(*)(...)>(&rb_traceattr2),
                              2);
#endif // HAVE__TRACEATTR2
#ifdef HAVE__TRACEBITS
    rb_define_module_function(mNcurses, "_tracebits",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracebits),
                              0);
#endif // HAVE__TRACEBITS
#ifdef HAVE__TRACECHAR
    rb_define_module_function(mNcurses, "_tracechar",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracechar),
                              1);
#endif // HAVE__TRACECHAR
#ifdef HAVE__TRACECHTYPE
    rb_define_module_function(mNcurses, "_tracechtype",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracechtype),
                              1);
#endif // HAVE__TRACECHTYPE
#ifdef HAVE__TRACECHTYPE2
    rb_define_module_function(mNcurses, "_tracechtype2",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracechtype2), 2);
#endif // HAVE__TRACECHTYPE2
#ifdef HAVE__TRACEMOUSE
    rb_define_module_function(mNcurses, "_tracechmouse",
                              reinterpret_cast<VALUE(*)(...)>(&rb_tracemouse),
                              1);
#endif // HAVE__TRACEMOUSE
#ifdef HAVE_TRACE
    rb_define_module_function(mNcurses, "trace",
                              reinterpret_cast<VALUE(*)(...)>(&rb_trace),
                              1);
#endif // HAVE_TRACE
#ifdef HAVE__NC_TRACEBITS
    rb_define_module_function(mNcurses, "_nc_tracebits", &rb_nc_tracebits, 0);
#endif // HAVE__NC_TRACEBITS
#ifdef HAVE_ASSUME_DEFAULT_COLORS
    rb_define_module_function(mNcurses, "assume_default_colors",
                              reinterpret_cast<VALUE(*)(...)>(&rb_assume_default_colors), 2);
#endif  // HAVE_ASSUME_DEFAULT_COLORS
#ifdef HAVE_ATTR_GET
    rb_define_module_function(mNcurses, "attr_get",
                              reinterpret_cast<VALUE(*)(...)>(&rb_attr_get),
                              3);
    rb_define_module_function(mNcurses, "wattr_get",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wattr_get),
                              4);
#endif // HAVE_ATTR_GET
    rb_define_module_function(mNcurses, "wprintw",
                              reinterpret_cast<VALUE(*)(...)>(&rb_wprintw),
                              -1);
}

static void init_constants_4(void)
{
    // /* trace masks */
    rb_define_const(mNcurses, "TRACE_DISABLE", INT2NUM(TRACE_DISABLE));
    rb_define_const(mNcurses, "TRACE_TIMES", INT2NUM(TRACE_TIMES));
    rb_define_const(mNcurses, "TRACE_TPUTS", INT2NUM(TRACE_TPUTS));
    rb_define_const(mNcurses, "TRACE_UPDATE", INT2NUM(TRACE_UPDATE));
    rb_define_const(mNcurses, "TRACE_MOVE", INT2NUM(TRACE_MOVE));
    rb_define_const(mNcurses, "TRACE_CHARPUT", INT2NUM(TRACE_CHARPUT));
    rb_define_const(mNcurses, "TRACE_ORDINARY", INT2NUM(TRACE_ORDINARY));
    rb_define_const(mNcurses, "TRACE_CALLS", INT2NUM(TRACE_CALLS));
    rb_define_const(mNcurses, "TRACE_VIRTPUT", INT2NUM(TRACE_VIRTPUT));
    rb_define_const(mNcurses, "TRACE_IEVENT", INT2NUM(TRACE_IEVENT));
    rb_define_const(mNcurses, "TRACE_BITS", INT2NUM(TRACE_BITS));
    rb_define_const(mNcurses, "TRACE_ICALLS", INT2NUM(TRACE_ICALLS));
    rb_define_const(mNcurses, "TRACE_CCALLS", INT2NUM(TRACE_CCALLS));
    rb_define_const(mNcurses, "TRACE_MAXIMUM", INT2NUM(TRACE_MAXIMUM));
}


// #if defined(TRACE) || defined(NCURSES_TEST)
// extern int _nc_optimize_enable;		/* enable optimizations */
// extern const char *_nc_visbuf(const char *);
// #define OPTIMIZE_MVCUR		0x01	/* cursor movement optimization */
// #define OPTIMIZE_HASHMAP	0x02	/* diff hashing to detect scrolls */
// #define OPTIMIZE_SCROLL		0x04	/* scroll optimization */
// #define OPTIMIZE_ALL		0xff	/* enable all optimizations (dflt) */
// #endif

// #ifdef __cplusplus
// }
// #endif

// #endif /* __NCURSES_H */


// Wrap ACS_* constants (additionally) as methods of SCREEN:
#define rb_ACS(ACS)                               \
VALUE rb_## ACS (VALUE rb_screen)                 \
{                                                 \
    SCREEN * c_screen = get_screen(rb_screen);    \
    SCREEN * current_screen = set_term(c_screen); \
    VALUE rb_ACS_CONST = INT2NUM(ACS);            \
    set_term(current_screen);                     \
    return  rb_ACS_CONST;                         \
}             
#define wrap_ACS(ACS)                                          \
rb_define_method(cSCREEN, #ACS,                                \
                 reinterpret_cast<VALUE(*)(...)>(&rb_ ## ACS), \
                 0)
rb_ACS(ACS_ULCORNER)
    rb_ACS(ACS_LLCORNER)
    rb_ACS(ACS_URCORNER)
    rb_ACS(ACS_LRCORNER)
    rb_ACS(ACS_LTEE)
    rb_ACS(ACS_RTEE)
    rb_ACS(ACS_BTEE)
    rb_ACS(ACS_TTEE)
    rb_ACS(ACS_HLINE)
    rb_ACS(ACS_VLINE)
    rb_ACS(ACS_PLUS)
    rb_ACS(ACS_S1)
    rb_ACS(ACS_S9)
    rb_ACS(ACS_DIAMOND)
    rb_ACS(ACS_CKBOARD)
    rb_ACS(ACS_DEGREE)
    rb_ACS(ACS_PLMINUS)
    rb_ACS(ACS_BULLET)
    rb_ACS(ACS_LARROW)
    rb_ACS(ACS_RARROW)
    rb_ACS(ACS_DARROW)
    rb_ACS(ACS_UARROW)
    rb_ACS(ACS_BOARD)
    rb_ACS(ACS_LANTERN)
    rb_ACS(ACS_BLOCK)
    rb_ACS(ACS_S3)
    rb_ACS(ACS_S7)
    rb_ACS(ACS_LEQUAL)
    rb_ACS(ACS_GEQUAL)
    rb_ACS(ACS_PI)
    rb_ACS(ACS_NEQUAL)
    rb_ACS(ACS_STERLING)

void init_SCREEN_methods(void)
{
    wrap_ACS(ACS_ULCORNER);
    wrap_ACS(ACS_LLCORNER);
    wrap_ACS(ACS_URCORNER);
    wrap_ACS(ACS_LRCORNER);
    wrap_ACS(ACS_LTEE);
    wrap_ACS(ACS_RTEE);
    wrap_ACS(ACS_BTEE);
    wrap_ACS(ACS_TTEE);
    wrap_ACS(ACS_HLINE);
    wrap_ACS(ACS_VLINE);
    wrap_ACS(ACS_PLUS);
    wrap_ACS(ACS_S1);
    wrap_ACS(ACS_S9);
    wrap_ACS(ACS_DIAMOND);
    wrap_ACS(ACS_CKBOARD);
    wrap_ACS(ACS_DEGREE);
    wrap_ACS(ACS_PLMINUS);
    wrap_ACS(ACS_BULLET);
    wrap_ACS(ACS_LARROW);
    wrap_ACS(ACS_RARROW);
    wrap_ACS(ACS_DARROW);
    wrap_ACS(ACS_UARROW);
    wrap_ACS(ACS_BOARD);
    wrap_ACS(ACS_LANTERN);
    wrap_ACS(ACS_BLOCK);
    wrap_ACS(ACS_S3);
    wrap_ACS(ACS_S7);
    wrap_ACS(ACS_LEQUAL);
    wrap_ACS(ACS_GEQUAL);
    wrap_ACS(ACS_PI);
    wrap_ACS(ACS_NEQUAL);
    wrap_ACS(ACS_STERLING);
}


extern "C" void Init_ncurses(void)
{
    mNcurses = rb_define_module("Ncurses");
    rb_iv_set(mNcurses, "@windows_hash", rb_hash_new());
    rb_iv_set(mNcurses, "@screens_hash", rb_hash_new());
    cWINDOW  = rb_define_class_under(mNcurses, "WINDOW", rb_cObject);
    cSCREEN  = rb_define_class_under(mNcurses, "SCREEN", rb_cObject);

    // printf("c1\n");
    init_constants_1();
    // printf("c2\n");
    init_constants_2();
    // printf("c3\n");
    init_constants_3();
    // printf("c4\n");
    init_constants_4();
    // printf("g1\n");
    init_globals_1();
    // printf("g2\n");
    init_globals_2();
    // printf("f0\n");
    init_functions_0();
    // printf("f1\n");
    init_functions_1();
    // printf("f2\n");
    init_functions_2();
    // printf("f3\n");
    init_functions_3();

    init_SCREEN_methods();
#ifdef HAVE_PANEL_H
    init_panel();
#endif
}
