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
 * $Id: ncurses_wrap.c,v 1.1 2002/11/16 21:57:20 t-peters Exp $
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

#if defined(HAVE_GETWIN) || defined(HAVE_PUTWIN)
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
int dup(int);
int close(int);
#endif
#endif

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

#define RB_F_TYPE VALUE(*)()

static VALUE mNcurses;
static VALUE cWINDOW;
static VALUE cSCREEN;

static
void
init_constants_1(void)
{
#ifdef CURSES
    rb_define_const(mNcurses, "CURSES",      INT2NUM((int)(CURSES)));
#endif
#ifdef CURSES_H
    rb_define_const(mNcurses, "CURSES_H",    INT2NUM((int)(CURSES_H)));
#endif
#ifdef NCURSES_VERSION_MAJOR
    rb_define_const(mNcurses, "NCURSES_VERSION_MAJOR",
                    INT2NUM((int)(NCURSES_VERSION_MAJOR)));
#endif
#ifdef NCURSES_VERSION_MINOR
    rb_define_const(mNcurses, "NCURSES_VERSION_MINOR",
                    INT2NUM((int)(NCURSES_VERSION_MINOR)));
#endif
#ifdef NCURSES_VERSION_PATCH
    rb_define_const(mNcurses, "NCURSES_VERSION_PATCH",
                    INT2NUM((int)(NCURSES_VERSION_PATCH)));
#endif
#ifdef NCURSES_VERSION
    rb_define_const(mNcurses, "NCURSES_VERSION",
                    rb_str_new2(NCURSES_VERSION));
#endif

    /* attributes */
#ifdef WA_ATTRIBUTES
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
#endif
}

static VALUE rb_COLORS()
{return INT2NUM(COLORS);}
static VALUE rb_COLOR_PAIRS()
{return INT2NUM(COLOR_PAIRS);}

static
void
init_globals_1(void)
{
    /* colors */
    rb_define_module_function(mNcurses, "COLORS",
			      (&rb_COLORS), 0);
    rb_define_module_function(mNcurses, "COLOR_PAIRS",
			      (&rb_COLOR_PAIRS), 0);
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
#ifdef _SUBWIN
    rb_define_const(mNcurses, "SUBWIN",       INT2NUM(_SUBWIN));
#endif
#ifdef _ENDLINE
    rb_define_const(mNcurses, "ENDLINE",      INT2NUM(_ENDLINE));
#endif
#ifdef _FULLWIN
    rb_define_const(mNcurses, "FULLWIN",      INT2NUM(_FULLWIN));
#endif
#ifdef _SCROLLWIN
    rb_define_const(mNcurses, "SCROLLWIN",    INT2NUM(_SCROLLWIN));
#endif
#ifdef _ISPAD
    rb_define_const(mNcurses, "ISPAD",        INT2NUM(_ISPAD));
#endif
#ifdef _HASMOVED
    rb_define_const(mNcurses, "HASMOVED",     INT2NUM(_HASMOVED));
#endif
#ifdef _WRAPPED
    rb_define_const(mNcurses, "WRAPPED",      INT2NUM(_WRAPPED));
#endif

#ifdef _NOCHANGE
    /*
     * this value is used in the firstchar and lastchar fields to mark
     * unchanged lines
     */
    rb_define_const(mNcurses, "NOCHANGE",     INT2NUM(_NOCHANGE));
#endif
#ifdef _NEWINDEX
    /*
     * this value is used in the oldindex field to mark lines created by
     * insertions and scrolls.
     */
    rb_define_const(mNcurses, "NEWINDEX",     INT2NUM(_NEWINDEX));
#endif
#ifdef CCHARW_MAX
    rb_define_const(mNcurses, "CCHARW_MAX",   INT2NUM(CCHARW_MAX));
#endif
}

static VALUE   wrap_window(WINDOW* window)
{
    if (window == 0) return Qnil;
    {
        VALUE windows_hash   = rb_iv_get(mNcurses, "@windows_hash");
        VALUE window_adress  = INT2NUM((long)(window));
        VALUE rb_window      = rb_hash_aref(windows_hash, window_adress);
        if (rb_window == Qnil) {
            rb_window = Data_Wrap_Struct(cWINDOW, 0, 0, window);
            rb_iv_set(rb_window, "@destroyed", Qfalse);
            rb_hash_aset(windows_hash, window_adress, rb_window);
        }
        return rb_window;
    }
}
static WINDOW* get_window(VALUE rb_window)
{
    WINDOW* window;
    if (rb_window == Qnil) return 0;
    if (rb_iv_get(rb_window, "@destroyed") == Qtrue) {
        rb_raise(rb_eRuntimeError, "Attempt to access a destroyed window");
        return 0;
    }
    Data_Get_Struct(rb_window, WINDOW, window);
    return window;
}
static VALUE rb_delwin(VALUE dummy, VALUE arg1) {
    VALUE windows_hash   = rb_iv_get(mNcurses, "@windows_hash");
    WINDOW* window       = get_window(arg1);
    VALUE window_adress  = INT2NUM((long)(window));
    rb_funcall(windows_hash, rb_intern("delete"), 1, window_adress);
    rb_iv_set(arg1, "@destroyed", Qtrue);
    return INT2NUM(delwin(window));
}

static VALUE   wrap_screen(SCREEN* screen)
{
    if (screen == 0) return Qnil;
    {
        VALUE screens_hash   = rb_iv_get(mNcurses, "@screens_hash");
        VALUE screen_adress  = INT2NUM((long)(screen));
        VALUE rb_screen      = rb_hash_aref(screens_hash, screen_adress);
        if (rb_screen == Qnil) {
            rb_screen = Data_Wrap_Struct(cSCREEN, 0, 0, screen);
            rb_iv_set(rb_screen, "@destroyed", Qfalse);
            rb_hash_aset(screens_hash, screen_adress, rb_screen);
        }
        return rb_screen;
    }
}
static SCREEN* get_screen(VALUE rb_screen)
{
    SCREEN* screen;
    if (rb_screen == Qnil) return 0;
    if (rb_iv_get(rb_screen, "@destroyed") == Qtrue) {
        rb_raise(rb_eRuntimeError, "Attempt to access a destroyed screen");
        return 0;
    }
    Data_Get_Struct(rb_screen, SCREEN, screen);
    return screen;
}
#ifdef HAVE_DELSCREEN
static VALUE rb_delscreen(VALUE dummy, VALUE arg1) {
    VALUE screens_hash   = rb_iv_get(mNcurses, "@screens_hash");
    SCREEN* screen       = get_screen(arg1);
    VALUE screen_adress  = INT2NUM((long)(screen));
    rb_funcall(screens_hash, rb_intern("delete"), 1, screen_adress);
    rb_iv_set(arg1, "@destroyed", Qtrue);
    delscreen(screen);
    return Qnil;
}
#endif
static VALUE rb_winchnstr(VALUE dummy, VALUE rb_win, VALUE rb_str, VALUE rb_n)
{
    if (rb_obj_is_instance_of(rb_str, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError, "2nd argument must be an empty Array");
        return Qnil;
    }

    {
        WINDOW * window = get_window(rb_win);
        int      n      = NUM2INT(rb_n);
        chtype * str    = ALLOC_N(chtype, n + 1);
        int return_value;
        return_value = winchnstr(window, str, n);
        if (return_value != ERR) {
            int i;
            for (i = 0; i < return_value; ++i) {
                rb_ary_push(rb_str, INT2NUM(str[i]));
            }
        }
        xfree(str);
        return INT2NUM(return_value);
    }
}
static VALUE rb_wgetnstr(VALUE dummy, VALUE rb_win, VALUE rb_chstr, VALUE rb_n)
{
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    char * str      = ALLOC_N(char, n + 1);
    int return_value;
    return_value = wgetnstr(window, str, n);
    if (return_value != ERR) {
        rb_str_cat(rb_chstr, str, return_value);
    }
    xfree(str);
    return INT2NUM(return_value);
}
static VALUE rb_winnstr(VALUE dummy, VALUE rb_win, VALUE rb_chstr, VALUE rb_n)
{
    WINDOW * window = get_window(rb_win);
    int      n      = NUM2INT(rb_n);
    char* str       = ALLOC_N(char, n + 1);
    int return_value;
    return_value = winnstr(window, str, n);
    if (return_value != ERR) {
        rb_str_cat(rb_chstr, str, return_value);
    }
    xfree(str);
    return INT2NUM(return_value);
}

#ifdef HAVE_PANEL_H
#include "panel_wrap.h" /* needs access to mNcurses, wrap_window, get_window */
#endif

static
void
init_functions_0(void)
{
#ifdef HAVE_DELSCREEN
    rb_define_module_function(mNcurses, "delscreen",
                              (&rb_delscreen),
                              1);
#endif
    rb_define_module_function(mNcurses, "delwin",
                              (&rb_delwin), 1);
    rb_define_module_function(mNcurses, "winchnstr",
                              (&rb_winchnstr),
                              3);
    rb_define_module_function(mNcurses, "winnstr",
                              (&rb_winnstr),
                              3);
    rb_define_module_function(mNcurses, "wgetnstr",
                              (&rb_wgetnstr),
                              3);
}

static VALUE get_stdscr()
{
    VALUE rb_stdscr = rb_iv_get(mNcurses, "@stdscr");
    if (rb_stdscr == Qnil) {
        rb_stdscr = wrap_window(stdscr);
        rb_iv_set(mNcurses, "@stdscr", rb_stdscr);
    }
    return rb_stdscr;
}
static VALUE get_curscr()
{
    VALUE rb_curscr = rb_iv_get(mNcurses, "@curscr");
    if (rb_curscr == Qnil) {
        rb_curscr = wrap_window(curscr);
        rb_iv_set(mNcurses, "@curscr", rb_curscr);
    }
    return rb_curscr;
}
#ifdef HAVE_NEWSCR
static VALUE get_newscr()
{
    VALUE rb_newscr = rb_iv_get(mNcurses, "@newscr");
    if (rb_newscr == Qnil) {
        rb_newscr = wrap_window(newscr);
        rb_iv_set(mNcurses, "@newscr", rb_newscr);
    }
    return rb_newscr;
}
#endif
static VALUE get_LINES()   {return INT2NUM(LINES);}
static VALUE get_COLS()    {return INT2NUM(COLS);}
#ifdef HAVE_TABSIZE
static VALUE get_TABSIZE() {return INT2NUM(TABSIZE);}
#endif
#ifdef HAVE_ESCDELAY
/* This global was an undocumented feature under AIX curses. */
/* ESC expire time in milliseconds                           */
static VALUE get_ESCDELAY(){return INT2NUM(ESCDELAY);}
static VALUE set_ESCDELAY(VALUE dummy, VALUE new_delay)
{
    ESCDELAY=NUM2INT(new_delay);
    return INT2NUM(ESCDELAY);
}
#endif
static
void
init_globals_2(void)
{
    rb_iv_set(mNcurses, "@stdscr", Qnil);
    rb_iv_set(mNcurses, "@curscr", Qnil);
    rb_iv_set(mNcurses, "@newscr", Qnil);
    rb_define_module_function(mNcurses, "stdscr", 
                              (&get_stdscr), 0);
    rb_define_module_function(mNcurses, "curscr", 
                              (&get_curscr), 0);
#ifdef HAVE_NEWSCR
    rb_define_module_function(mNcurses, "newscr", 
                              (&get_newscr), 0);
#endif
    rb_define_module_function(mNcurses, "LINES",  
                              (&get_LINES),  0);
    rb_define_module_function(mNcurses, "COLS",   
                              (&get_COLS),   0);
#ifdef HAVE_TABSIZE
    rb_define_module_function(mNcurses, "TABSIZE",
                              (&get_TABSIZE),0);
#endif
#ifdef HAVE_ESCDELAY
    rb_define_module_function(mNcurses, "ESCDELAY",
                              (&get_ESCDELAY),0);
    rb_define_module_function(mNcurses, "ESCDELAY=",
                              (&set_ESCDELAY),1);
#endif
}
#ifdef HAVE_KEYBOUND
static VALUE rb_keybound(VALUE dummy, VALUE keycode, VALUE count)
{
    char * str = keybound(NUM2INT(keycode), NUM2INT(count));
    VALUE rb_str = Qnil;
    if (str) {
        rb_str = rb_str_new2(str);
        free(str);
    }
    return rb_str;
}
#endif
#ifdef HAVE_CURSES_VERSION
static VALUE rb_curses_version(){return rb_str_new2(curses_version());}
#endif
#ifdef HAVE_DEFINE_KEY
static VALUE rb_define_key(VALUE dummy, VALUE definition, VALUE keycode)
{
    return INT2NUM(define_key((definition != Qnil)
                              ? STR2CSTR(definition)
                              : (char*)(NULL),
                              NUM2INT(keycode)));
}
#endif
#ifdef HAVE_KEYOK
static VALUE rb_keyok(VALUE dummy, VALUE keycode, VALUE enable)
{
    return INT2NUM(keyok(NUM2INT(keycode), RTEST(enable)));
}
#endif
#ifdef HAVE_RESIZETERM
static VALUE rb_resizeterm(VALUE dummy, VALUE lines, VALUE columns)
{
    return INT2NUM(resizeterm(NUM2INT(lines), NUM2INT(columns)));
}
#endif
#ifdef HAVE_USE_DEFAULT_COLORS
static VALUE rb_use_default_colors()
{return INT2NUM(use_default_colors());}
#endif
#ifdef HAVE_USE_EXTENDED_NAMES
static VALUE rb_use_extended_names(VALUE dummy, VALUE boolean)
{return INT2NUM(use_extended_names(RTEST(boolean)));}
#endif
#ifdef HAVE_WRESIZE
static VALUE rb_wresize(VALUE dummy, VALUE win, VALUE lines, VALUE columns)
{
    return INT2NUM(wresize(get_window(win), NUM2INT(lines), NUM2INT(columns)));
}
#endif
static
void
init_functions_1(void)
{
#ifdef HAVE_KEYBOUND
    rb_define_module_function(mNcurses, "keybound",
                              (&rb_keybound),
                              2);
#endif
#ifdef HAVE_CURSES_VERSION
    rb_define_module_function(mNcurses, "curses_version",
                              (&rb_curses_version),
			      0);
#endif
#ifdef HAVE_DEFINE_KEY
    rb_define_module_function(mNcurses, "define_key",
                              (&rb_define_key),
                              2);
#endif
#ifdef HAVE_KEYOK
    rb_define_module_function(mNcurses, "keyok",
                              (&rb_keyok), 2);
#endif
#ifdef HAVE_RESIZETERM
    rb_define_module_function(mNcurses, "resizeterm",
                              (&rb_resizeterm),
                              2);
#endif
#ifdef HAVE_USE_DEFAULT_COLORS
    rb_define_module_function(mNcurses, "use_default_colors",
                              (&rb_use_default_colors), 0);
#endif
#ifdef HAVE_USE_EXTENDED_NAMES
    rb_define_module_function(mNcurses, "use_extended_names",
                              (&rb_use_extended_names), 1);
#endif
#ifdef HAVE_WRESIZE
    rb_define_module_function(mNcurses, "wresize",
                              (&rb_wresize), 3);
#endif
}
/* FIXME: what's this? */
/* extern char ttytype[]; */		/* needed for backward compatibility */


/* copy a chstr from ruby to c */
static chtype * RB2CHSTR(VALUE array)
{
    if (rb_obj_is_instance_of(array, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "chtype string argument must be an empty Array");
        return NULL;
    }
    {
        size_t string_length =
            NUM2ULONG(rb_funcall(array, rb_intern("size"), 0));
        size_t vector_length =
            string_length + 1;        /* for terminating 0 */
        chtype * chstr = ALLOC_N(chtype, vector_length);
        unsigned long i;
        for (i = 0; i < string_length; ++i) {
            chstr[i] = NUM2ULONG(rb_ary_entry(array, i));
        }
        chstr[string_length] = 0;
        return chstr;
    }
}

static VALUE rb_addch(VALUE dummy, VALUE arg1) {
    return INT2NUM(addch(NUM2ULONG(arg1)));
}
static VALUE rb_addchnstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    chtype * chstr = RB2CHSTR(arg1);
    VALUE return_value = INT2NUM(addchnstr(chstr,  NUM2INT(arg2)));
    xfree(chstr);
    return return_value;
}
static VALUE rb_addchstr(VALUE dummy, VALUE arg1) {
    chtype * chstr = RB2CHSTR(arg1);
    VALUE return_value = INT2NUM(addchstr(chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rb_addnstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(addnstr(STR2CSTR(arg1),  NUM2INT(arg2)));
}
static VALUE rb_addstr(VALUE dummy, VALUE arg1) {
    return INT2NUM(addstr(STR2CSTR(arg1)));
}
static VALUE rb_attroff(VALUE dummy, VALUE arg1) {
    return INT2NUM(attroff(NUM2ULONG(arg1)));
}
static VALUE rb_attron(VALUE dummy, VALUE arg1) {
    return INT2NUM(attron(NUM2ULONG(arg1)));
}
static VALUE rb_attrset(VALUE dummy, VALUE arg1) {
    return INT2NUM(attrset(NUM2ULONG(arg1)));
}
#ifdef HAVE_ATTR_OFF
static VALUE rb_attr_off(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(attr_off(NUM2ULONG(arg1),  ((arg2),NULL)));
}
#endif
#ifdef HAVE_ATTR_ON
static VALUE rb_attr_on(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(attr_on(NUM2ULONG(arg1),  ((arg2),NULL)));
}
#endif
#ifdef HAVE_ATTR_SET
static VALUE rb_attr_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(attr_set(NUM2ULONG(arg1),  NUM2INT(arg2),  ((arg3),NULL)));
}
#endif
static VALUE rb_baudrate(VALUE dummy) {
    return INT2NUM(baudrate());
}
static VALUE rb_beep(VALUE dummy) {
    return INT2NUM(beep());
}
static VALUE rb_bkgd(VALUE dummy, VALUE arg1) {
    return INT2NUM(bkgd(NUM2ULONG(arg1)));
}
static VALUE rb_bkgdset(VALUE dummy, VALUE arg1) {
    return ((bkgdset(NUM2ULONG(arg1))),Qnil);
}
static VALUE rb_border(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8) {
    return INT2NUM(border(NUM2ULONG(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3),  NUM2ULONG(arg4),  NUM2ULONG(arg5),  NUM2ULONG(arg6),  NUM2ULONG(arg7),  NUM2ULONG(arg8)));
}
static VALUE rb_box(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(box(get_window(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3)));
}
static VALUE rb_can_change_color(VALUE dummy) {
    return (can_change_color()) ? Qtrue : Qfalse;
}
static VALUE rb_cbreak(VALUE dummy) {
    return INT2NUM(cbreak());
}
#ifdef HAVE_CHGAT
static VALUE rb_chgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(chgat(NUM2INT(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3),
                         ((arg4),NULL)));
}
#endif
static VALUE rb_clear(VALUE dummy) {
    return INT2NUM(clear());
}
static VALUE rb_clearok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(clearok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_clrtobot(VALUE dummy) {
    return INT2NUM(clrtobot());
}
static VALUE rb_clrtoeol(VALUE dummy) {
    return INT2NUM(clrtoeol());
}
#ifdef HAVE_COLOR_SET
static VALUE rb_color_set(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(color_set(NUM2INT(arg1),  ((arg2),NULL)));
}
#endif
static VALUE rb_COLOR_PAIR(VALUE dummy, VALUE arg1) {
    return INT2NUM(COLOR_PAIR(NUM2INT(arg1)));
}
static VALUE rb_copywin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9) {
    return INT2NUM(copywin(get_window(arg1),  get_window(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7),  NUM2INT(arg8),  NUM2INT(arg9)));
}
static VALUE rb_curs_set(VALUE dummy, VALUE arg1) {
    return INT2NUM(curs_set(NUM2INT(arg1)));
}
static VALUE rb_def_prog_mode(VALUE dummy) {
    return INT2NUM(def_prog_mode());
}
static VALUE rb_def_shell_mode(VALUE dummy) {
    return INT2NUM(def_shell_mode());
}
static VALUE rb_delay_output(VALUE dummy, VALUE arg1) {
    return INT2NUM(delay_output(NUM2INT(arg1)));
}
static VALUE rb_delch(VALUE dummy) {
    return INT2NUM(delch());
}
static VALUE rb_deleteln(VALUE dummy) {
    return INT2NUM(deleteln());
}
static VALUE rb_derwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(derwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rb_doupdate(VALUE dummy) {
    return INT2NUM(doupdate());
}
static VALUE rb_dupwin(VALUE dummy, VALUE arg1) {
    return wrap_window(dupwin(get_window(arg1)));
}
static VALUE rb_echo(VALUE dummy) {
    return INT2NUM(echo());
}
static VALUE rb_echochar(VALUE dummy, VALUE arg1) {
    return INT2NUM(echochar(NUM2ULONG(arg1)));
}
static VALUE rb_endwin(VALUE dummy) {
    return INT2NUM(endwin());
}
static VALUE rb_erasechar(VALUE dummy) {
    return INT2NUM(erasechar());
}
#ifdef HAVE_FILTER
static VALUE rb_filter(VALUE dummy) {
    return ((filter()),Qnil);
}
#endif
static VALUE rb_flash(VALUE dummy) {
    return INT2NUM(flash());
}
static VALUE rb_flushinp(VALUE dummy) {
    return INT2NUM(flushinp());
}
static VALUE rb_getbkgd(VALUE dummy, VALUE arg1) {
    return INT2NUM(getbkgd(get_window(arg1)));
}
static VALUE rb_getch(VALUE dummy) {
    return INT2NUM(getch());
}
static VALUE rb_halfdelay(VALUE dummy, VALUE arg1) {
    return INT2NUM(halfdelay(NUM2INT(arg1)));
}
static VALUE rb_has_colors(VALUE dummy) {
    return (has_colors()) ? Qtrue : Qfalse;
}
static VALUE rb_has_ic(VALUE dummy) {
    return (has_ic()) ? Qtrue : Qfalse;
}
static VALUE rb_has_il(VALUE dummy) {
    return (has_il()) ? Qtrue : Qfalse;
}
static VALUE rb_hline(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(hline(NUM2ULONG(arg1),  NUM2INT(arg2)));
}
static VALUE rb_idcok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((idcok(get_window(arg1),  RTEST(arg2))),Qnil);
}
static VALUE rb_idlok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(idlok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_immedok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((immedok(get_window(arg1),  RTEST(arg2))),Qnil);
}
static VALUE rb_inch(VALUE dummy) {
    return INT2NUM(inch());
}
static VALUE rb_initscr(VALUE dummy) {
    VALUE v = wrap_window(initscr());
    
    /* These constants are not defined before the call to initscr. */
    
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
#ifdef ACS_S3
    rb_define_const(mNcurses, "ACS_S3",        INT2NUM(ACS_S3));
#endif
#ifdef ACS_S7
    rb_define_const(mNcurses, "ACS_S7",        INT2NUM(ACS_S7));
#endif
#ifdef ACS_LEQUAL
    rb_define_const(mNcurses, "ACS_LEQUAL",    INT2NUM(ACS_LEQUAL));
#endif
#ifdef ACS_LEQUAL
    rb_define_const(mNcurses, "ACS_GEQUAL",    INT2NUM(ACS_GEQUAL));
#endif
#ifdef ACS_PI
    rb_define_const(mNcurses, "ACS_PI",        INT2NUM(ACS_PI));
#endif
#ifdef ACS_NEQUAL
    rb_define_const(mNcurses, "ACS_NEQUAL",    INT2NUM(ACS_NEQUAL));
#endif
#ifdef ACS_STERLING
    rb_define_const(mNcurses, "ACS_STERLING",  INT2NUM(ACS_STERLING));
#endif
    /*
     * Line drawing ACS names are of the form ACS_trbl, where t is the top, r
     * is the right, b is the bottom, and l is the left.  t, r, b, and l might
     * be B (blank), S (single), D (double), or T (thick).  The subset defined
     * here only uses B and S.
     */
#ifdef ACS_BSSB
    rb_define_const(mNcurses, "ACS_BSSB",      INT2NUM(ACS_BSSB));
#endif
#ifdef ACS_SSBB
    rb_define_const(mNcurses, "ACS_SSBB",      INT2NUM(ACS_SSBB));
#endif
#ifdef ACS_BBSS
    rb_define_const(mNcurses, "ACS_BBSS",      INT2NUM(ACS_BBSS));
#endif
#ifdef ACS_SBBS
    rb_define_const(mNcurses, "ACS_SBBS",      INT2NUM(ACS_SBBS));
#endif
#ifdef ACS_SBSS
    rb_define_const(mNcurses, "ACS_SBSS",      INT2NUM(ACS_SBSS));
#endif
#ifdef ACS_SSSB
    rb_define_const(mNcurses, "ACS_SSSB",      INT2NUM(ACS_SSSB));
#endif
#ifdef ACS_SSBS
    rb_define_const(mNcurses, "ACS_SSBS",      INT2NUM(ACS_SSBS));
#endif
#ifdef ACS_BSSS
    rb_define_const(mNcurses, "ACS_BSSS",      INT2NUM(ACS_BSSS));
#endif
#ifdef ACS_BSBS
    rb_define_const(mNcurses, "ACS_BSBS",      INT2NUM(ACS_BSBS));
#endif
#ifdef ACS_SBSB
    rb_define_const(mNcurses, "ACS_SBSB",      INT2NUM(ACS_SBSB));
#endif
#ifdef ACS_SSSS
    rb_define_const(mNcurses, "ACS_SSSS",      INT2NUM(ACS_SSSS));
#endif
    return v;
}
static VALUE rb_init_color(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(init_color(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_init_pair(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(init_pair(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_insch(VALUE dummy, VALUE arg1) {
    return INT2NUM(insch(NUM2ULONG(arg1)));
}
static VALUE rb_insdelln(VALUE dummy, VALUE arg1) {
    return INT2NUM(insdelln(NUM2INT(arg1)));
}
static VALUE rb_insertln(VALUE dummy) {
    return INT2NUM(insertln());
}
static VALUE rb_insnstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(insnstr(STR2CSTR(arg1),  NUM2INT(arg2)));
}
static VALUE rb_insstr(VALUE dummy, VALUE arg1) {
    return INT2NUM(insstr(STR2CSTR(arg1)));
}
#ifdef HAVE_INTRFLUSH
static VALUE rb_intrflush(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(intrflush(get_window(arg1),  RTEST(arg2)));
}
#endif
static VALUE rb_isendwin(VALUE dummy) {
    return (isendwin()) ? Qtrue : Qfalse;
}
static VALUE rb_is_linetouched(VALUE dummy, VALUE arg1, VALUE arg2) {
    return (is_linetouched(get_window(arg1),  NUM2INT(arg2))) ? Qtrue : Qfalse;
}
static VALUE rb_is_wintouched(VALUE dummy, VALUE arg1) {
    return (is_wintouched(get_window(arg1))) ? Qtrue : Qfalse;
}
static VALUE rb_keyname(VALUE dummy, VALUE arg1) {
    return rb_str_new2(keyname(NUM2INT(arg1)));
}
static VALUE rb_keypad(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(keypad(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_killchar(VALUE dummy) {
    return INT2NUM(killchar());
}
static VALUE rb_leaveok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(leaveok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_longname(VALUE dummy) {
    return rb_str_new2(longname());
}
static VALUE rb_meta(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(meta(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_move(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(move(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvaddch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddch(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3)));
}
static VALUE rb_mvaddchnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3,
                            VALUE arg4) {
    chtype * chstr = RB2CHSTR(arg3);
    VALUE return_value = INT2NUM(mvaddchnstr(NUM2INT(arg1), NUM2INT(arg2),
                                             chstr, NUM2INT(arg4)));
    xfree(chstr);
    return return_value;
}
static VALUE rb_mvaddchstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    chtype * chstr = RB2CHSTR(arg3);
    VALUE return_value = INT2NUM(mvaddchstr(NUM2INT(arg1), NUM2INT(arg2),
                                            chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rb_mvaddnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvaddnstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvaddstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvaddstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3)));
}
#ifdef HAVE_MVCHGAT
static VALUE rb_mvchgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6) {
    return INT2NUM(mvchgat(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5),  ((arg6),NULL)));
}
#endif
static VALUE rb_mvcur(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvcur(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvdelch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvdelch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvderwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvderwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvgetch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvgetch(NUM2INT(arg1),  NUM2INT(arg2)));
}
#ifdef HAVE_MVHLINE
static VALUE rb_mvhline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvhline(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4)));
}
#endif
static VALUE rb_mvinch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(mvinch(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_mvinsch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvinsch(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3)));
}
static VALUE rb_mvinsnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvinsnstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3),  NUM2INT(arg4)));
}
static VALUE rb_mvinsstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvinsstr(NUM2INT(arg1),  NUM2INT(arg2),  STR2CSTR(arg3)));
}
#ifdef HAVE_MVVLINE
static VALUE rb_mvvline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvvline(NUM2INT(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4)));
}
#endif
static VALUE rb_mvwaddch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4)));
}
static VALUE rb_mvwaddchnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3,
                             VALUE arg4, VALUE arg5) {
    chtype * chstr = RB2CHSTR(arg4);
    VALUE return_value = INT2NUM(mvwaddchnstr(get_window(arg1), NUM2INT(arg2),
                                              NUM2INT(arg3), chstr,
                                              NUM2INT(arg5)));
    xfree(chstr);
    return return_value;
}
static VALUE rb_mvwaddchstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3,
                            VALUE arg4) {
    chtype * chstr = RB2CHSTR(arg4);
    VALUE return_value = INT2NUM(mvwaddchstr(get_window(arg1), NUM2INT(arg2),
                                             NUM2INT(arg3), chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rb_mvwaddnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwaddnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4),  NUM2INT(arg5)));
}
static VALUE rb_mvwaddstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwaddstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4)));
}
#ifdef HAVE_MVWCHGAT
static VALUE rb_mvwchgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(mvwchgat(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2ULONG(arg5),  NUM2INT(arg6),  ((arg7),NULL)));
}
#endif
static VALUE rb_mvwdelch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwdelch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwgetch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwgetch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
#ifdef HAVE_MVWHLINE
static VALUE rb_mvwhline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwhline(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5)));
}
#endif
static VALUE rb_mvwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwinch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(mvwinch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_mvwinsch(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwinsch(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4)));
}
static VALUE rb_mvwinsnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwinsnstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4),  NUM2INT(arg5)));
}
static VALUE rb_mvwinsstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(mvwinsstr(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  STR2CSTR(arg4)));
}
#ifdef HAVE_MVWVLINE
static VALUE rb_mvwvline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(mvwvline(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2ULONG(arg4),  NUM2INT(arg5)));
}
#endif
static VALUE rb_napms(VALUE dummy, VALUE arg1) {
    return INT2NUM(napms(NUM2INT(arg1)));
}
static VALUE rb_newpad(VALUE dummy, VALUE arg1, VALUE arg2) {
    return wrap_window(newpad(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_newwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return wrap_window(newwin(NUM2INT(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_nl(VALUE dummy) {
    return INT2NUM(nl());
}
static VALUE rb_nocbreak(VALUE dummy) {
    return INT2NUM(nocbreak());
}
static VALUE rb_nodelay(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(nodelay(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_noecho(VALUE dummy) {
    return INT2NUM(noecho());
}
static VALUE rb_nonl(VALUE dummy) {
    return INT2NUM(nonl());
}
#ifdef HAVE_NOQIFLUSH
static VALUE rb_noqiflush(VALUE dummy) {
    return ((noqiflush()),Qnil);
}
#endif
static VALUE rb_noraw(VALUE dummy) {
    return INT2NUM(noraw());
}
static VALUE rb_notimeout(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(notimeout(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_overlay(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(overlay(get_window(arg1),  get_window(arg2)));
}
static VALUE rb_overwrite(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(overwrite(get_window(arg1),  get_window(arg2)));
}
static VALUE rb_PAIR_NUMBER(VALUE dummy, VALUE arg1) {
    return INT2NUM(PAIR_NUMBER(NUM2INT(arg1)));
}
#ifndef __PDCURSES__ /* pdcurses "pechochar" macro won't compile*/
static VALUE rb_pechochar(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(pechochar(get_window(arg1),  NUM2ULONG(arg2)));
}
#endif
static VALUE rb_pnoutrefresh(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(pnoutrefresh(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7)));
}
static VALUE rb_prefresh(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7) {
    return INT2NUM(prefresh(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5),  NUM2INT(arg6),  NUM2INT(arg7)));
}
#ifdef HAVE_PUTP
static VALUE rb_putp(VALUE dummy, VALUE arg1) {
    return INT2NUM(putp(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_QIFLUSH
static VALUE rb_qiflush(VALUE dummy) {
    return ((qiflush()),Qnil);
}
#endif
static VALUE rb_raw(VALUE dummy) {
    return INT2NUM(raw());
}
#ifndef __PDCURSES__ /* __PDCURSES__ redrawwin macro is buggy */
static VALUE rb_redrawwin(VALUE dummy, VALUE arg1) {
    return INT2NUM(redrawwin(get_window(arg1)));
}
#endif
static VALUE rb_refresh(VALUE dummy) {
    return INT2NUM(refresh());
}
static VALUE rb_resetty(VALUE dummy) {
    return INT2NUM(resetty());
}
static VALUE rb_reset_prog_mode(VALUE dummy) {
    return INT2NUM(reset_prog_mode());
}
static VALUE rb_reset_shell_mode(VALUE dummy) {
    return INT2NUM(reset_shell_mode());
}
static VALUE rb_savetty(VALUE dummy) {
    return INT2NUM(savetty());
}
#ifdef HAVE_SCR_DUMP
static VALUE rb_scr_dump(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_dump(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_SCR_INIT
static VALUE rb_scr_init(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_init(STR2CSTR(arg1)));
}
#endif
static VALUE rb_scrl(VALUE dummy, VALUE arg1) {
    return INT2NUM(scrl(NUM2INT(arg1)));
}
static VALUE rb_scroll(VALUE dummy, VALUE arg1) {
    return INT2NUM(scroll(get_window(arg1)));
}
static VALUE rb_scrollok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(scrollok(get_window(arg1),  RTEST(arg2)));
}
#ifdef HAVE_SCR_RESTORE
static VALUE rb_scr_restore(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_restore(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_SCR_SET
static VALUE rb_scr_set(VALUE dummy, VALUE arg1) {
    return INT2NUM(scr_set(STR2CSTR(arg1)));
}
#endif
static VALUE rb_setscrreg(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(setscrreg(NUM2INT(arg1),  NUM2INT(arg2)));
}
static VALUE rb_set_term(VALUE dummy, VALUE arg1) {
    return wrap_screen(set_term(get_screen(arg1)));
}
static VALUE rb_slk_attroff(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_attroff(NUM2ULONG(arg1)));
}
#if defined(HAVE_SLK_ATTR_OFF) || defined(slk_attr_off)
static VALUE rb_slk_attr_off(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(slk_attr_off(NUM2ULONG(arg1),  ((arg2),NULL)));
}
#endif
static VALUE rb_slk_attron(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_attron(NUM2ULONG(arg1)));
}
#if defined(HAVE_SLK_ATTR_ON) || defined(slk_attr_on)
static VALUE rb_slk_attr_on(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(slk_attr_on(NUM2ULONG(arg1),  ((arg2),NULL)));
}
#endif
static VALUE rb_slk_attrset(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_attrset(NUM2ULONG(arg1)));
}
#ifdef HAVE_SLK_ATTR
static VALUE rb_slk_attr(VALUE dummy) {
    return INT2NUM(slk_attr());
}
#endif
#ifdef HAVE_SLK_ATTR_SET
static VALUE rb_slk_attr_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(slk_attr_set(NUM2ULONG(arg1),  NUM2INT(arg2),  ((arg3),NULL)));
}
#endif
static VALUE rb_slk_clear(VALUE dummy) {
    return INT2NUM(slk_clear());
}
#ifdef HAVE_SLK_COLOR
static VALUE rb_slk_color(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_color(NUM2INT(arg1)));
}
#endif
static VALUE rb_slk_init(VALUE dummy, VALUE arg1) {
    return INT2NUM(slk_init(NUM2INT(arg1)));
}
static VALUE rb_slk_label(VALUE dummy, VALUE arg1) {
    return rb_str_new2(slk_label(NUM2INT(arg1)));
}
static VALUE rb_slk_noutrefresh(VALUE dummy) {
    return INT2NUM(slk_noutrefresh());
}
static VALUE rb_slk_refresh(VALUE dummy) {
    return INT2NUM(slk_refresh());
}
static VALUE rb_slk_restore(VALUE dummy) {
    return INT2NUM(slk_restore());
}
static VALUE rb_slk_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(slk_set(NUM2INT(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rb_slk_touch(VALUE dummy) {
    return INT2NUM(slk_touch());
}
static VALUE rb_standout(VALUE dummy) {
    return INT2NUM(standout());
}
static VALUE rb_standend(VALUE dummy) {
    return INT2NUM(standend());
}
static VALUE rb_start_color(VALUE dummy) {
    return INT2NUM(start_color());
}
static VALUE rb_subpad(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(subpad(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rb_subwin(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return wrap_window(subwin(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4),  NUM2INT(arg5)));
}
static VALUE rb_syncok(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(syncok(get_window(arg1),  RTEST(arg2)));
}
static VALUE rb_termattrs(VALUE dummy) {
    return INT2NUM(termattrs());
}
static VALUE rb_termname(VALUE dummy) {
    return rb_str_new2(termname());
}
#ifdef HAVE_TIGETFLAG
static VALUE rb_tigetflag(VALUE dummy, VALUE arg1) {
    return INT2NUM(tigetflag(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_TIGETNUM
static VALUE rb_tigetnum(VALUE dummy, VALUE arg1) {
    return INT2NUM(tigetnum(STR2CSTR(arg1)));
}
#endif
#ifdef HAVE_TIGETSTR
static VALUE rb_tigetstr(VALUE dummy, VALUE arg1) {
    return rb_str_new2(tigetstr(STR2CSTR(arg1)));
}
#endif
static VALUE rb_timeout(VALUE dummy, VALUE arg1) {
    return ((timeout(NUM2INT(arg1))),Qnil);
}
static VALUE rb_typeahead(VALUE dummy, VALUE arg1) {
    return INT2NUM(typeahead(NUM2INT(arg1)));
}
static VALUE rb_ungetch(VALUE dummy, VALUE arg1) {
    return INT2NUM(ungetch(NUM2INT(arg1)));
}
static VALUE rb_untouchwin(VALUE dummy, VALUE arg1) {
    return INT2NUM(untouchwin(get_window(arg1)));
}
#ifdef HAVE_USE_ENV
static VALUE rb_use_env(VALUE dummy, VALUE arg1) {
    return ((use_env(RTEST(arg1))),Qnil);
}
#endif
#ifdef HAVE_VIDATTR
static VALUE rb_vidattr(VALUE dummy, VALUE arg1) {
    return INT2NUM(vidattr(NUM2ULONG(arg1)));
}
#endif
#if defined(HAVE_VID_ATTR) || defined(vid_attr)
static VALUE rb_vid_attr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(vid_attr(NUM2ULONG(arg1),  NUM2INT(arg2),  ((arg3),NULL)));
}
#endif
static VALUE rb_vline(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(vline(NUM2ULONG(arg1),  NUM2INT(arg2)));
}
static VALUE rb_waddch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddch(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_waddchnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    chtype * chstr = RB2CHSTR(arg2);
    VALUE return_value = INT2NUM(waddchnstr(get_window(arg1), chstr,
                                            NUM2INT(arg3)));
    xfree(chstr);
    return return_value;
}
static VALUE rb_waddchstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    chtype * chstr = RB2CHSTR(arg2);
    VALUE return_value = INT2NUM(waddchstr(get_window(arg1), chstr));
    xfree(chstr);
    return return_value;
}
static VALUE rb_waddnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(waddnstr(get_window(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rb_waddstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(waddstr(get_window(arg1),  STR2CSTR(arg2)));
}
static VALUE rb_wattron(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattron(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wattroff(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattroff(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wattrset(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wattrset(get_window(arg1),  NUM2INT(arg2)));
}
#ifdef HAVE_WATTR_ON
static VALUE rb_wattr_on(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wattr_on(get_window(arg1),  NUM2ULONG(arg2),  ((arg3),NULL)));
}
#endif
#ifdef HAVE_WATTR_OFF
static VALUE rb_wattr_off(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wattr_off(get_window(arg1),  NUM2ULONG(arg2),  ((arg3),NULL)));
}
#endif
#ifdef HAVE_WATTR_SET
static VALUE rb_wattr_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(wattr_set(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3),  ((arg4),NULL)));
}
#endif
static VALUE rb_wbkgd(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wbkgd(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_wbkgdset(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((wbkgdset(get_window(arg1),  NUM2ULONG(arg2))),Qnil);
}
static VALUE rb_wborder(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5, VALUE arg6, VALUE arg7, VALUE arg8, VALUE arg9) {
    return INT2NUM(wborder(get_window(arg1),  NUM2ULONG(arg2),  NUM2ULONG(arg3),  NUM2ULONG(arg4),  NUM2ULONG(arg5),  NUM2ULONG(arg6),  NUM2ULONG(arg7),  NUM2ULONG(arg8),  NUM2ULONG(arg9)));
}
#ifdef HAVE_WCHGAT
static VALUE rb_wchgat(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4, VALUE arg5) {
    return INT2NUM(wchgat(get_window(arg1),  NUM2INT(arg2),  NUM2ULONG(arg3),  NUM2INT(arg4),  ((arg5),NULL)));
}
#endif
static VALUE rb_wclear(VALUE dummy, VALUE arg1) {
    return INT2NUM(wclear(get_window(arg1)));
}
static VALUE rb_wclrtobot(VALUE dummy, VALUE arg1) {
    return INT2NUM(wclrtobot(get_window(arg1)));
}
static VALUE rb_wclrtoeol(VALUE dummy, VALUE arg1) {
    return INT2NUM(wclrtoeol(get_window(arg1)));
}
#ifdef HAVE_WCOLOR_SET
static VALUE rb_wcolor_set(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wcolor_set(get_window(arg1),  NUM2INT(arg2),  ((arg3),NULL)));
}
#endif
static VALUE rb_wcursyncup(VALUE dummy, VALUE arg1) {
    return ((wcursyncup(get_window(arg1))),Qnil);
}
static VALUE rb_wdelch(VALUE dummy, VALUE arg1) {
    return INT2NUM(wdelch(get_window(arg1)));
}
static VALUE rb_wdeleteln(VALUE dummy, VALUE arg1) {
    return INT2NUM(wdeleteln(get_window(arg1)));
}
static VALUE rb_wechochar(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wechochar(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_werase(VALUE dummy, VALUE arg1) {
    return INT2NUM(werase(get_window(arg1)));
}
static VALUE rb_wgetch(VALUE dummy, VALUE arg1) {
    return INT2NUM(wgetch(get_window(arg1)));
}
static VALUE rb_whline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(whline(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3)));
}
static VALUE rb_winch(VALUE dummy, VALUE arg1) {
    return INT2NUM(winch(get_window(arg1)));
}
static VALUE rb_winsch(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsch(get_window(arg1),  NUM2ULONG(arg2)));
}
static VALUE rb_winsdelln(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsdelln(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_winsertln(VALUE dummy, VALUE arg1) {
    return INT2NUM(winsertln(get_window(arg1)));
}
static VALUE rb_winsnstr(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(winsnstr(get_window(arg1),  STR2CSTR(arg2),  NUM2INT(arg3)));
}
static VALUE rb_winsstr(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(winsstr(get_window(arg1),  STR2CSTR(arg2)));
}
static VALUE rb_wmove(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wmove(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_wnoutrefresh(VALUE dummy, VALUE arg1) {
    return INT2NUM(wnoutrefresh(get_window(arg1)));
}
static VALUE rb_wredrawln(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wredrawln(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_wrefresh(VALUE dummy, VALUE arg1) {
    return INT2NUM(wrefresh(get_window(arg1)));
}
static VALUE rb_wscrl(VALUE dummy, VALUE arg1, VALUE arg2) {
    return INT2NUM(wscrl(get_window(arg1),  NUM2INT(arg2)));
}
static VALUE rb_wsetscrreg(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wsetscrreg(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3)));
}
static VALUE rb_wstandout(VALUE dummy, VALUE arg1) {
    return INT2NUM(wstandout(get_window(arg1)));
}
static VALUE rb_wstandend(VALUE dummy, VALUE arg1) {
    return INT2NUM(wstandend(get_window(arg1)));
}
static VALUE rb_wsyncdown(VALUE dummy, VALUE arg1) {
    return ((wsyncdown(get_window(arg1))),Qnil);
}
static VALUE rb_wsyncup(VALUE dummy, VALUE arg1) {
    return ((wsyncup(get_window(arg1))),Qnil);
}
static VALUE rb_wtimeout(VALUE dummy, VALUE arg1, VALUE arg2) {
    return ((wtimeout(get_window(arg1),  NUM2INT(arg2))),Qnil);
}
static VALUE rb_wtouchln(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3, VALUE arg4) {
    return INT2NUM(wtouchln(get_window(arg1),  NUM2INT(arg2),  NUM2INT(arg3),  NUM2INT(arg4)));
}
static VALUE rb_wvline(VALUE dummy, VALUE arg1, VALUE arg2, VALUE arg3) {
    return INT2NUM(wvline(get_window(arg1),  NUM2ULONG(arg2),  NUM2INT(arg3)));
}
static VALUE rb_color_content(VALUE dummy, VALUE color, VALUE r, VALUE g, VALUE b) {
    if (rb_obj_is_instance_of(r, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(g, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(b, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "r,g and b (2nd to 4th argument) must be an empty Arrays");
        return Qnil;
    }
    {
        short cv[3] = {0,0,0};
        int return_value = color_content(NUM2INT(color), &cv[0], &cv[1],
                                         &cv[2]);
        rb_ary_push(r, INT2NUM(cv[0])); rb_ary_push(g, INT2NUM(cv[1]));
        rb_ary_push(b, INT2NUM(cv[2])); return INT2NUM(return_value);
    }
}
static VALUE rb_pair_content(VALUE dummy, VALUE pair, VALUE fg, VALUE bg) {
    if (rb_obj_is_instance_of(fg, rb_cArray) != Qtrue
        || rb_obj_is_instance_of(bg, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "fg and bg (2nd and 3rd argument) must be an empty Arrays");
        return Qnil;
    }
    {
        short cn[2] = {0,0};
        int return_value = pair_content(NUM2INT(pair), &cn[0], &cn[1]);
        rb_ary_push(fg, INT2NUM(cn[0])); rb_ary_push(bg, INT2NUM(cn[1]));
        return INT2NUM(return_value);
    }
}
#ifdef HAVE_GETWIN
static VALUE rb_getwin(VALUE dummy, VALUE io)
{
    int fd = dup(NUM2INT(rb_funcall(io, rb_intern("to_i"), 0)));
    FILE * f = fdopen(fd, "r");
    WINDOW * win = getwin(f);
    fclose(f);
    close(fd);
    {
        VALUE return_value = Qnil;
        if (win) return_value = wrap_window(win);
        return return_value;
    }
}
#endif
#ifdef HAVE_PUTWIN
static VALUE rb_putwin(VALUE dummy, VALUE rb_win, VALUE io)
{
    int fd = dup(NUM2INT(rb_funcall(io, rb_intern("to_i"), 0)));
    FILE * f = fdopen(fd, "w");
    WINDOW * win = get_window(rb_win);
    int return_value = putwin(win, f);
    fclose(f);
    close(fd);
    return INT2NUM(return_value);
}
#endif
static VALUE rb_unctrl(VALUE dummy, VALUE ch)
{ return rb_str_new2(unctrl(NUM2ULONG(ch))); }
static VALUE rb_newterm(VALUE dummy, VALUE rb_type, VALUE rb_outfd, VALUE rb_infd)
{
    char * type = (rb_type == Qnil) ? (char*)0 : STR2CSTR(rb_type);
    int outfd = NUM2INT(rb_funcall(rb_outfd, rb_intern("to_i"), 0));
    int infd  = NUM2INT(rb_funcall(rb_infd, rb_intern("to_i"), 0));
    return wrap_screen(newterm(type, fdopen(outfd, "w"), fdopen(infd, "r")));
}
    

static void init_functions_2(void) {
    rb_define_module_function(mNcurses, "addch",
                              (&rb_addch),
                              1);
    rb_define_module_function(mNcurses, "addchnstr",
                              (&rb_addchnstr),
                              2);
    rb_define_module_function(mNcurses, "addchstr",
                              (&rb_addchstr),
                              1);
    rb_define_module_function(mNcurses, "addnstr",
                              (&rb_addnstr),
                              2);
    rb_define_module_function(mNcurses, "addstr",
                              (&rb_addstr),
                              1);
    rb_define_module_function(mNcurses, "attroff",
                              (&rb_attroff),
                              1);
    rb_define_module_function(mNcurses, "attron",
                              (&rb_attron),
                              1);
    rb_define_module_function(mNcurses, "attrset",
                              (&rb_attrset),
                              1);
#ifdef HAVE_ATTR_OFF
    rb_define_module_function(mNcurses, "attr_off",
                              (&rb_attr_off),
                              2);
#endif
#ifdef HAVE_ATTR_ON
    rb_define_module_function(mNcurses, "attr_on",
                              (&rb_attr_on),
                              2);
#endif
#ifdef HAVE_ATTR_SET
    rb_define_module_function(mNcurses, "attr_set",
                              (&rb_attr_set),
                              3);
#endif
    rb_define_module_function(mNcurses, "baudrate",
                              (&rb_baudrate),
                              0);
    rb_define_module_function(mNcurses, "beep",
                              (&rb_beep),
                              0);
    rb_define_module_function(mNcurses, "bkgd",
                              (&rb_bkgd),
                              1);
    rb_define_module_function(mNcurses, "bkgdset",
                              (&rb_bkgdset),
                              1);
    rb_define_module_function(mNcurses, "border",
                              (&rb_border),
                              8);
    rb_define_module_function(mNcurses, "box",
                              (&rb_box),
                              3);
    rb_define_module_function(mNcurses, "can_change_color?",
                              (&rb_can_change_color),
                              0);
    rb_define_module_function(mNcurses, "cbreak",
                              (&rb_cbreak),
                              0);
#ifdef HAVE_CHGAT
    rb_define_module_function(mNcurses, "chgat",
                              (&rb_chgat),
                              4);
#endif
    rb_define_module_function(mNcurses, "clear",
                              (&rb_clear),
                              0);
    rb_define_module_function(mNcurses, "clearok",
                              (&rb_clearok),
                              2);
    rb_define_module_function(mNcurses, "clrtobot",
                              (&rb_clrtobot),
                              0);
    rb_define_module_function(mNcurses, "clrtoeol",
                              (&rb_clrtoeol),
                              0);
#ifdef HAVE_COLOR_SET
    rb_define_module_function(mNcurses, "color_set",
                              (&rb_color_set),
                              2);
#endif
    rb_define_module_function(mNcurses, "COLOR_PAIR",
                              (&rb_COLOR_PAIR),
                              1);
    rb_define_module_function(mNcurses, "copywin",
                              (&rb_copywin),
                              9);
    rb_define_module_function(mNcurses, "curs_set",
                              (&rb_curs_set),
                              1);
    rb_define_module_function(mNcurses, "def_prog_mode",
                              (&rb_def_prog_mode),
                              0);
    rb_define_module_function(mNcurses, "def_shell_mode",
                              (&rb_def_shell_mode),
                              0);
    rb_define_module_function(mNcurses, "delay_output",
                              (&rb_delay_output),
                              1);
    rb_define_module_function(mNcurses, "delch",
                              (&rb_delch),
                              0);
    rb_define_module_function(mNcurses, "deleteln",
                              (&rb_deleteln),
                              0);
    rb_define_module_function(mNcurses, "derwin",
                              (&rb_derwin),
                              5);
    rb_define_module_function(mNcurses, "doupdate",
                              (&rb_doupdate),
                              0);
    rb_define_module_function(mNcurses, "dupwin",
                              (&rb_dupwin),
                              1);
    rb_define_module_function(mNcurses, "echo",
                              (&rb_echo),
                              0);
    rb_define_module_function(mNcurses, "echochar",
                              (&rb_echochar),
                              1);
    rb_define_module_function(mNcurses, "endwin",
                              (&rb_endwin),
                              0);
    rb_define_module_function(mNcurses, "erasechar",
                              (&rb_erasechar),
                              0);
#ifdef HAVE_FILTER
    rb_define_module_function(mNcurses, "filter",
                              (&rb_filter),
                              0);
#endif
    rb_define_module_function(mNcurses, "flash",
                              (&rb_flash),
                              0);
    rb_define_module_function(mNcurses, "flushinp",
                              (&rb_flushinp),
                              0);
    rb_define_module_function(mNcurses, "getbkgd",
                              (&rb_getbkgd),
                              1);
    rb_define_module_function(mNcurses, "getch",
                              (&rb_getch),
                              0);
    rb_define_module_function(mNcurses, "halfdelay",
                              (&rb_halfdelay),
                              1);
    rb_define_module_function(mNcurses, "has_colors?",
                              (&rb_has_colors),
                              0);
    rb_define_module_function(mNcurses, "has_ic?",
                              (&rb_has_ic),
                              0);
    rb_define_module_function(mNcurses, "has_il?",
                              (&rb_has_il),
                              0);
    rb_define_module_function(mNcurses, "hline",
                              (&rb_hline),
                              2);
    rb_define_module_function(mNcurses, "idcok",
                              (&rb_idcok),
                              2);
    rb_define_module_function(mNcurses, "idlok",
                              (&rb_idlok),
                              2);
    rb_define_module_function(mNcurses, "immedok",
                              (&rb_immedok),
                              2);
    rb_define_module_function(mNcurses, "inch",
                              (&rb_inch),
                              0);
    rb_define_module_function(mNcurses, "initscr",
                              (&rb_initscr),
                              0);
    rb_define_module_function(mNcurses, "init_color",
                              (&rb_init_color),
                              4);
    rb_define_module_function(mNcurses, "init_pair",
                              (&rb_init_pair),
                              3);
    rb_define_module_function(mNcurses, "insch",
                              (&rb_insch),
                              1);
    rb_define_module_function(mNcurses, "insdelln",
                              (&rb_insdelln),
                              1);
    rb_define_module_function(mNcurses, "insertln",
                              (&rb_insertln),
                              0);
    rb_define_module_function(mNcurses, "insnstr",
                              (&rb_insnstr),
                              2);
    rb_define_module_function(mNcurses, "insstr",
                              (&rb_insstr),
                              1);
#ifdef HAVE_INTRFLUSH
    rb_define_module_function(mNcurses, "intrflush",
                              (&rb_intrflush),
                              2);
#endif
    rb_define_module_function(mNcurses, "isendwin?",
                              (&rb_isendwin),
                              0);
    rb_define_module_function(mNcurses, "is_linetouched?",
                              (&rb_is_linetouched),
                              2);
    rb_define_module_function(mNcurses, "is_wintouched?",
                              (&rb_is_wintouched),
                              1);
    rb_define_module_function(mNcurses, "keyname",
                              (&rb_keyname),
                              1);
    rb_define_module_function(mNcurses, "keypad",
                              (&rb_keypad),
                              2);
    rb_define_module_function(mNcurses, "killchar",
                              (&rb_killchar),
                              0);
    rb_define_module_function(mNcurses, "leaveok",
                              (&rb_leaveok),
                              2);
    rb_define_module_function(mNcurses, "longname",
                              (&rb_longname),
                              0);
    rb_define_module_function(mNcurses, "meta",
                              (&rb_meta),
                              2);
    rb_define_module_function(mNcurses, "move",
                              (&rb_move),
                              2);
    rb_define_module_function(mNcurses, "mvaddch",
                              (&rb_mvaddch),
                              3);
    rb_define_module_function(mNcurses, "mvaddchnstr",
                              (&rb_mvaddchnstr),
                              4);
    rb_define_module_function(mNcurses, "mvaddchstr",
                              (&rb_mvaddchstr),
                              3);
    rb_define_module_function(mNcurses, "mvaddnstr",
                              (&rb_mvaddnstr),
                              4);
    rb_define_module_function(mNcurses, "mvaddstr",
                              (&rb_mvaddstr),
                              3);
#ifdef HAVE_MVCHGAT
    rb_define_module_function(mNcurses, "mvchgat",
                              (&rb_mvchgat),
                              6);
#endif
    rb_define_module_function(mNcurses, "mvcur",
                              (&rb_mvcur),
                              4);
    rb_define_module_function(mNcurses, "mvdelch",
                              (&rb_mvdelch),
                              2);
    rb_define_module_function(mNcurses, "mvderwin",
                              (&rb_mvderwin),
                              3);
    rb_define_module_function(mNcurses, "mvgetch",
                              (&rb_mvgetch),
                              2);
#ifdef HAVE_MVHLINE
    rb_define_module_function(mNcurses, "mvhline",
                              (&rb_mvhline),
                              4);
#endif
    rb_define_module_function(mNcurses, "mvinch",
                              (&rb_mvinch),
                              2);
    rb_define_module_function(mNcurses, "mvinsch",
                              (&rb_mvinsch),
                              3);
    rb_define_module_function(mNcurses, "mvinsnstr",
                              (&rb_mvinsnstr),
                              4);
    rb_define_module_function(mNcurses, "mvinsstr",
                              (&rb_mvinsstr),
                              3);
#ifdef HAVE_MVVLINE
    rb_define_module_function(mNcurses, "mvvline",
                              (&rb_mvvline),
                              4);
#endif
    rb_define_module_function(mNcurses, "mvwaddch",
                              (&rb_mvwaddch),
                              4);
    rb_define_module_function(mNcurses, "mvwaddchnstr",
                              (&rb_mvwaddchnstr),
                              5);
    rb_define_module_function(mNcurses, "mvwaddchstr",
                              (&rb_mvwaddchstr),
                              4);
    rb_define_module_function(mNcurses, "mvwaddnstr",
                              (&rb_mvwaddnstr),
                              5);
    rb_define_module_function(mNcurses, "mvwaddstr",
                              (&rb_mvwaddstr),
                              4);
#ifdef HAVE_MVWCHGAT
    rb_define_module_function(mNcurses, "mvwchgat",
                              (&rb_mvwchgat),
                              7);
#endif
    rb_define_module_function(mNcurses, "mvwdelch",
                              (&rb_mvwdelch),
                              3);
    rb_define_module_function(mNcurses, "mvwgetch",
                              (&rb_mvwgetch),
                              3);
#ifdef HAVE_MVWHLINE
    rb_define_module_function(mNcurses, "mvwhline",
                              (&rb_mvwhline),
                              5);
#endif
    rb_define_module_function(mNcurses, "mvwin",
                              (&rb_mvwin),
                              3);
    rb_define_module_function(mNcurses, "mvwinch",
                              (&rb_mvwinch),
                              3);
    rb_define_module_function(mNcurses, "mvwinsch",
                              (&rb_mvwinsch),
                              4);
    rb_define_module_function(mNcurses, "mvwinsnstr",
                              (&rb_mvwinsnstr),
                              5);
    rb_define_module_function(mNcurses, "mvwinsstr",
                              (&rb_mvwinsstr),
                              4);
#ifdef HAVE_MVWVLINE
    rb_define_module_function(mNcurses, "mvwvline",
                              (&rb_mvwvline),
                              5);
#endif
    rb_define_module_function(mNcurses, "napms",
                              (&rb_napms),
                              1);
    rb_define_module_function(mNcurses, "newpad",
                              (&rb_newpad),
                              2);
    rb_define_module_function(mNcurses, "newwin",
                              (&rb_newwin),
                              4);
    rb_define_module_function(mNcurses, "nl",
                              (&rb_nl),
                              0);
    rb_define_module_function(mNcurses, "nocbreak",
                              (&rb_nocbreak),
                              0);
    rb_define_module_function(mNcurses, "nodelay",
                              (&rb_nodelay),
                              2);
    rb_define_module_function(mNcurses, "noecho",
                              (&rb_noecho),
                              0);
    rb_define_module_function(mNcurses, "nonl",
                              (&rb_nonl),
                              0);
#ifdef HAVE_NOQIFLUSH
    rb_define_module_function(mNcurses, "noqiflush",
                              (&rb_noqiflush),
                              0);
#endif
    rb_define_module_function(mNcurses, "noraw",
                              (&rb_noraw),
                              0);
    rb_define_module_function(mNcurses, "notimeout",
                              (&rb_notimeout),
                              2);
    rb_define_module_function(mNcurses, "overlay",
                              (&rb_overlay),
                              2);
    rb_define_module_function(mNcurses, "overwrite",
                              (&rb_overwrite),
                              2);
    rb_define_module_function(mNcurses, "PAIR_NUMBER",
                              (&rb_PAIR_NUMBER),
                              1);
#ifdef HAVE_PECHOCHAR
    rb_define_module_function(mNcurses, "pechochar",
                              (&rb_pechochar),
                              2);
#endif
    rb_define_module_function(mNcurses, "pnoutrefresh",
                              (&rb_pnoutrefresh),
                              7);
    rb_define_module_function(mNcurses, "prefresh",
                              (&rb_prefresh),
                              7);
#ifdef HAVE_PUTP
    rb_define_module_function(mNcurses, "putp",
                              (&rb_putp),
                              1);
#endif
#ifdef HAVE_QIFLUSH
    rb_define_module_function(mNcurses, "qiflush",
                              (&rb_qiflush),
                              0);
#endif
    rb_define_module_function(mNcurses, "raw",
                              (&rb_raw),
                              0);
#ifdef HAVE_REDRAWWIN
    rb_define_module_function(mNcurses, "redrawwin",
                              (&rb_redrawwin),
                              1);
#endif
    rb_define_module_function(mNcurses, "refresh",
                              (&rb_refresh),
                              0);
    rb_define_module_function(mNcurses, "resetty",
                              (&rb_resetty),
                              0);
    rb_define_module_function(mNcurses, "reset_prog_mode",
                              (&rb_reset_prog_mode),
                              0);
    rb_define_module_function(mNcurses, "reset_shell_mode",
                              (&rb_reset_shell_mode),
                              0);
    rb_define_module_function(mNcurses, "savetty",
                              (&rb_savetty),
                              0);
#ifdef HAVE_SCR_DUMP
    rb_define_module_function(mNcurses, "scr_dump",
                              (&rb_scr_dump),
                              1);
#endif
#ifdef HAVE_SCR_INIT
    rb_define_module_function(mNcurses, "scr_init",
                              (&rb_scr_init),
                              1);
#endif
    rb_define_module_function(mNcurses, "scrl",
                              (&rb_scrl),
                              1);
    rb_define_module_function(mNcurses, "scroll",
                              (&rb_scroll),
                              1);
    rb_define_module_function(mNcurses, "scrollok",
                              (&rb_scrollok),
                              2);
#ifdef HAVE_SCR_RESTORE
    rb_define_module_function(mNcurses, "scr_restore",
                              (&rb_scr_restore),
                              1);
#endif
#ifdef HAVE_SCR_SET
    rb_define_module_function(mNcurses, "scr_set",
                              (&rb_scr_set),
                              1);
#endif
    rb_define_module_function(mNcurses, "setscrreg",
                              (&rb_setscrreg),
                              2);
    rb_define_module_function(mNcurses, "set_term",
                              (&rb_set_term),
                              1);
    rb_define_module_function(mNcurses, "slk_attroff",
                              (&rb_slk_attroff),
                              1);
#if defined(HAVE_SLK_ATTR_OFF) || defined(slk_attr_off)
    rb_define_module_function(mNcurses, "slk_attr_off",
                              (&rb_slk_attr_off),
                              2);
#endif
    rb_define_module_function(mNcurses, "slk_attron",
                              (&rb_slk_attron),
                              1);
#if defined(HAVE_SLK_ATTR_ON) || defined(slk_attr_on)
    rb_define_module_function(mNcurses, "slk_attr_on",
                              (&rb_slk_attr_on),
                              2);
#endif
    rb_define_module_function(mNcurses, "slk_attrset",
                              (&rb_slk_attrset),
                              1);
#ifdef HAVE_SLK_ATTR
    rb_define_module_function(mNcurses, "slk_attr",
                              (&rb_slk_attr),
                              0);
#endif
#ifdef HAVE_SLK_ATTR_SET
    rb_define_module_function(mNcurses, "slk_attr_set",
                              (&rb_slk_attr_set),
                              3);
#endif
    rb_define_module_function(mNcurses, "slk_clear",
                              (&rb_slk_clear),
                              0);
#ifdef HAVE_SLK_COLOR
    rb_define_module_function(mNcurses, "slk_color",
                              (&rb_slk_color),
                              1);
#endif
    rb_define_module_function(mNcurses, "slk_init",
                              (&rb_slk_init),
                              1);
    rb_define_module_function(mNcurses, "slk_label",
                              (&rb_slk_label),
                              1);
    rb_define_module_function(mNcurses, "slk_noutrefresh",
                              (&rb_slk_noutrefresh),
                              0);
    rb_define_module_function(mNcurses, "slk_refresh",
                              (&rb_slk_refresh),
                              0);
    rb_define_module_function(mNcurses, "slk_restore",
                              (&rb_slk_restore),
                              0);
    rb_define_module_function(mNcurses, "slk_set",
                              (&rb_slk_set),
                              3);
    rb_define_module_function(mNcurses, "slk_touch",
                              (&rb_slk_touch),
                              0);
    rb_define_module_function(mNcurses, "standout",
                              (&rb_standout),
                              0);
    rb_define_module_function(mNcurses, "standend",
                              (&rb_standend),
                              0);
    rb_define_module_function(mNcurses, "start_color",
                              (&rb_start_color),
                              0);
    rb_define_module_function(mNcurses, "subpad",
                              (&rb_subpad),
                              5);
    rb_define_module_function(mNcurses, "subwin",
                              (&rb_subwin),
                              5);
    rb_define_module_function(mNcurses, "syncok",
                              (&rb_syncok),
                              2);
    rb_define_module_function(mNcurses, "termattrs",
                              (&rb_termattrs),
                              0);
    rb_define_module_function(mNcurses, "termname",
                              (&rb_termname),
                              0);
#ifdef HAVE_TIGETFLAG
    rb_define_module_function(mNcurses, "tigetflag",
                              (&rb_tigetflag),
                              1);
#endif
#ifdef HAVE_TIGETNUM
    rb_define_module_function(mNcurses, "tigetnum",
                              (&rb_tigetnum),
                              1);
#endif
#ifdef HAVE_TIGETSTR
    rb_define_module_function(mNcurses, "tigetstr",
                              (&rb_tigetstr),
                              1);
#endif
    rb_define_module_function(mNcurses, "timeout",
                              (&rb_timeout),
                              1);
    rb_define_module_function(mNcurses, "typeahead",
                              (&rb_typeahead),
                              1);
    rb_define_module_function(mNcurses, "ungetch",
                              (&rb_ungetch),
                              1);
    rb_define_module_function(mNcurses, "untouchwin",
                              (&rb_untouchwin),
                              1);
#ifdef HAVE_USE_ENV
    rb_define_module_function(mNcurses, "use_env",
                              (&rb_use_env),
                              1);
#endif
#ifdef HAVE_VIDATTR
    rb_define_module_function(mNcurses, "vidattr",
                              (&rb_vidattr),
                              1);
#endif
#if defined (HAVE_VID_ATTR) || defined(vid_attr)
    rb_define_module_function(mNcurses, "vid_attr",
                              (&rb_vid_attr),
                              3);
#endif
    rb_define_module_function(mNcurses, "vline",
                              (&rb_vline),
                              2);
    rb_define_module_function(mNcurses, "waddch",
                              (&rb_waddch),
                              2);
    rb_define_module_function(mNcurses, "waddchnstr",
                              (&rb_waddchnstr),
                              3);
    rb_define_module_function(mNcurses, "waddchstr",
                              (&rb_waddchstr),
                              2);
    rb_define_module_function(mNcurses, "waddnstr",
                              (&rb_waddnstr),
                              3);
    rb_define_module_function(mNcurses, "waddstr",
                              (&rb_waddstr),
                              2);
    rb_define_module_function(mNcurses, "wattron",
                              (&rb_wattron),
                              2);
    rb_define_module_function(mNcurses, "wattroff",
                              (&rb_wattroff),
                              2);
    rb_define_module_function(mNcurses, "wattrset",
                              (&rb_wattrset),
                              2);
 #ifdef HAVE_WATTR_ON
   rb_define_module_function(mNcurses, "wattr_on",
                              (&rb_wattr_on),
                              3);
#endif
#ifdef HAVE_WATTR_OFF
    rb_define_module_function(mNcurses, "wattr_off",
                              (&rb_wattr_off),
                              3);
#endif
#ifdef HAVE_WATTR_SET
    rb_define_module_function(mNcurses, "wattr_set",
                              (&rb_wattr_set),
                              4);
#endif
    rb_define_module_function(mNcurses, "wbkgd",
                              (&rb_wbkgd),
                              2);
    rb_define_module_function(mNcurses, "wbkgdset",
                              (&rb_wbkgdset),
                              2);
    rb_define_module_function(mNcurses, "wborder",
                              (&rb_wborder),
                              9);
#ifdef HAVE_WCHGAT
    rb_define_module_function(mNcurses, "wchgat",
                              (&rb_wchgat),
                              5);
#endif
    rb_define_module_function(mNcurses, "wclear",
                              (&rb_wclear),
                              1);
    rb_define_module_function(mNcurses, "wclrtobot",
                              (&rb_wclrtobot),
                              1);
    rb_define_module_function(mNcurses, "wclrtoeol",
                              (&rb_wclrtoeol),
                              1);
#ifdef HAVE_WCOLOR_SET
    rb_define_module_function(mNcurses, "wcolor_set",
                              (&rb_wcolor_set),
                              3);
#endif
    rb_define_module_function(mNcurses, "wcursyncup",
                              (&rb_wcursyncup),
                              1);
    rb_define_module_function(mNcurses, "wdelch",
                              (&rb_wdelch),
                              1);
    rb_define_module_function(mNcurses, "wdeleteln",
                              (&rb_wdeleteln),
                              1);
    rb_define_module_function(mNcurses, "wechochar",
                              (&rb_wechochar),
                              2);
    rb_define_module_function(mNcurses, "werase",
                              (&rb_werase),
                              1);
    rb_define_module_function(mNcurses, "wgetch",
                              (&rb_wgetch),
                              1);
    rb_define_module_function(mNcurses, "whline",
                              (&rb_whline),
                              3);
    rb_define_module_function(mNcurses, "winch",
                              (&rb_winch),
                              1);
    rb_define_module_function(mNcurses, "winsch",
                              (&rb_winsch),
                              2);
    rb_define_module_function(mNcurses, "winsdelln",
                              (&rb_winsdelln),
                              2);
    rb_define_module_function(mNcurses, "winsertln",
                              (&rb_winsertln),
                              1);
    rb_define_module_function(mNcurses, "winsnstr",
                              (&rb_winsnstr),
                              3);
    rb_define_module_function(mNcurses, "winsstr",
                              (&rb_winsstr),
                              2);
    rb_define_module_function(mNcurses, "wmove",
                              (&rb_wmove),
                              3);
    rb_define_module_function(mNcurses, "wnoutrefresh",
                              (&rb_wnoutrefresh),
                              1);
    rb_define_module_function(mNcurses, "wredrawln",
                              (&rb_wredrawln),
                              3);
    rb_define_module_function(mNcurses, "wrefresh",
                              (&rb_wrefresh),
                              1);
    rb_define_module_function(mNcurses, "wscrl",
                              (&rb_wscrl),
                              2);
    rb_define_module_function(mNcurses, "wsetscrreg",
                              (&rb_wsetscrreg),
                              3);
    rb_define_module_function(mNcurses, "wstandout",
                              (&rb_wstandout),
                              1);
    rb_define_module_function(mNcurses, "wstandend",
                              (&rb_wstandend),
                              1);
    rb_define_module_function(mNcurses, "wsyncdown",
                              (&rb_wsyncdown),
                              1);
    rb_define_module_function(mNcurses, "wsyncup",
                              (&rb_wsyncup),
                              1);
    rb_define_module_function(mNcurses, "wtimeout",
                              (&rb_wtimeout),
                              2);
    rb_define_module_function(mNcurses, "wtouchln",
                              (&rb_wtouchln),
                              4);
    rb_define_module_function(mNcurses, "wvline",
                              (&rb_wvline),
                              3);
    rb_define_module_function(mNcurses, "color_content",
                              (&rb_color_content), 4);
    rb_define_module_function(mNcurses, "pair_content",
                              (&rb_pair_content), 3);
    rb_define_module_function(mNcurses, "pair_content",
                              (&rb_pair_content), 3);
#ifdef HAVE_GETWIN
    rb_define_module_function(mNcurses, "getwin",
                              (&rb_getwin), 1);
#endif
#ifdef HAVE_PUTWIN
    rb_define_module_function(mNcurses, "putwin",
                              (&rb_putwin), 2);
#endif
    rb_define_module_function(mNcurses, "unctrl",
                              (&rb_unctrl), 1);
    rb_define_module_function(mNcurses, "newterm",
                              (&rb_newterm), 3);
}


static void init_constants_3(void) {
    /* #define NCURSES_BITS(mask,shift) ((mask) << ((shift) + 8)) */

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

 /* Tradeoff on 32-bit machines ('protect' vs widec).  The others (e.g., left
  * highlight are not implemented in any terminal descriptions, anyway.
  */
    rb_define_const(mNcurses, "A_PROTECT", INT2NUM(A_PROTECT));
#ifdef A_HORIZONTAL
    rb_define_const(mNcurses, "A_HORIZONTAL", INT2NUM(A_HORIZONTAL));
#endif
#ifdef A_LEFT
    rb_define_const(mNcurses, "A_LEFT", INT2NUM(A_LEFT));
#endif
#ifdef A_LOW
    rb_define_const(mNcurses, "A_LOW", INT2NUM(A_LOW));
#endif
#ifdef A_RIGHT
    rb_define_const(mNcurses, "A_RIGHT", INT2NUM(A_RIGHT));
#endif
#ifdef A_TOP
    rb_define_const(mNcurses, "A_TOP", INT2NUM(A_TOP));
#endif
#ifdef A_VERTICAL
    rb_define_const(mNcurses, "A_VERTICAL", INT2NUM(A_VERTICAL));
#endif

    /* Pseudo-character tokens outside ASCII range.  The curses wgetch()
     * function will return any given one of these only if the corresponding
     * k- capability is defined in your terminal's terminfo entry. */
#ifdef KAY_CODE_YES
    rb_define_const(mNcurses, "KEY_CODE_YES", INT2NUM(KEY_CODE_YES));
#endif
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

 /* The keypad is arranged like this: */
 /* a1    up    a3   */
 /* left   b2  right  */
 /* c1   down   c3   */

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

 /* mouse interface */
 /* #define NCURSES_MOUSE_VERSION	1 */

 /* event masks */
    rb_define_const(mNcurses, "BUTTON1_RELEASED", INT2NUM(BUTTON1_RELEASED));
    rb_define_const(mNcurses, "BUTTON1_PRESSED", INT2NUM(BUTTON1_PRESSED));
    rb_define_const(mNcurses, "BUTTON1_CLICKED", INT2NUM(BUTTON1_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_DOUBLE_CLICKED", INT2NUM(BUTTON1_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON1_TRIPLE_CLICKED", INT2NUM(BUTTON1_TRIPLE_CLICKED));
#ifdef BUTTON1_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON1_RESERVED_EVENT", INT2NUM(BUTTON1_RESERVED_EVENT));
#endif
    rb_define_const(mNcurses, "BUTTON2_RELEASED", INT2NUM(BUTTON2_RELEASED));
    rb_define_const(mNcurses, "BUTTON2_PRESSED", INT2NUM(BUTTON2_PRESSED));
    rb_define_const(mNcurses, "BUTTON2_CLICKED", INT2NUM(BUTTON2_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_DOUBLE_CLICKED", INT2NUM(BUTTON2_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON2_TRIPLE_CLICKED", INT2NUM(BUTTON2_TRIPLE_CLICKED));
#ifdef BUTTON2_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON2_RESERVED_EVENT", INT2NUM(BUTTON2_RESERVED_EVENT));
#endif
    rb_define_const(mNcurses, "BUTTON3_RELEASED", INT2NUM(BUTTON3_RELEASED));
    rb_define_const(mNcurses, "BUTTON3_PRESSED", INT2NUM(BUTTON3_PRESSED));
    rb_define_const(mNcurses, "BUTTON3_CLICKED", INT2NUM(BUTTON3_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_DOUBLE_CLICKED", INT2NUM(BUTTON3_DOUBLE_CLICKED));
    rb_define_const(mNcurses, "BUTTON3_TRIPLE_CLICKED", INT2NUM(BUTTON3_TRIPLE_CLICKED));
#ifdef BUTTON3_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON3_RESERVED_EVENT", INT2NUM(BUTTON3_RESERVED_EVENT));
#endif
#ifdef BUTTON4_RELEASED
    rb_define_const(mNcurses, "BUTTON4_RELEASED", INT2NUM(BUTTON4_RELEASED));
#endif
#ifdef BUTTON4_PRESSED
    rb_define_const(mNcurses, "BUTTON4_PRESSED", INT2NUM(BUTTON4_PRESSED));
#endif
#ifdef BUTTON4_CLICKED
    rb_define_const(mNcurses, "BUTTON4_CLICKED", INT2NUM(BUTTON4_CLICKED));
#endif
#ifdef BUTTON4_DOUBLE_CLICKED
    rb_define_const(mNcurses, "BUTTON4_DOUBLE_CLICKED", INT2NUM(BUTTON4_DOUBLE_CLICKED));
#endif
#ifdef BUTTON4_TRIPLE_CLICKED
    rb_define_const(mNcurses, "BUTTON4_TRIPLE_CLICKED", INT2NUM(BUTTON4_TRIPLE_CLICKED));
#endif
#ifdef BUTTON4_RESERVED_EVENT
    rb_define_const(mNcurses, "BUTTON4_RESERVED_EVENT", INT2NUM(BUTTON4_RESERVED_EVENT));
#endif
#ifdef BUTTON_CTRL
    rb_define_const(mNcurses, "BUTTON_CTRL", INT2NUM(BUTTON_CTRL));
#endif
#ifdef BUTTON_CONTROL
    rb_define_const(mNcurses, "BUTTON_CTRL", INT2NUM(BUTTON_CONTROL));
#endif
    rb_define_const(mNcurses, "BUTTON_SHIFT", INT2NUM(BUTTON_SHIFT));
    rb_define_const(mNcurses, "BUTTON_ALT", INT2NUM(BUTTON_ALT));
    rb_define_const(mNcurses, "ALL_MOUSE_EVENTS", INT2NUM(ALL_MOUSE_EVENTS));
    rb_define_const(mNcurses, "REPORT_MOUSE_POSITION", INT2NUM(REPORT_MOUSE_POSITION));
}

/* typedef struct */
/* { */
/*     short id; */		/* ID to distinguish multiple devices */
/*     int x, y, z; */	/* event coordinates (character-cell) */
/*     mmask_t bstate; *//* button state bits */
/* } */
/* MEVENT; */
#ifdef HAVE_UNGETMOUSE
static VALUE rb_getmouse(VALUE dummy, VALUE rb_m)
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
static VALUE rb_ungetmouse(VALUE dummy, VALUE rb_m)
{
    MEVENT m;
    m.id = NUM2INT(rb_iv_get(rb_m, "@id"));
    m.x  = NUM2INT(rb_iv_get(rb_m, "@x"));
    m.y  = NUM2INT(rb_iv_get(rb_m, "@y"));
    m.z  = NUM2INT(rb_iv_get(rb_m, "@z"));
    m.bstate  = NUM2ULONG(rb_iv_get(rb_m, "@bstate"));
    return INT2NUM(ungetmouse(&m));
}
#endif
#ifdef HAVE_MOUSEMASK
static VALUE rb_mousemask(VALUE dummy, VALUE rb_newmask, VALUE rb_oldmask)
{
    if (rb_obj_is_instance_of(rb_oldmask, rb_cArray) != Qtrue) {
        rb_raise(rb_eArgError,
                 "oldmask (2nd argument) must be an empty Array");
        return Qnil;
    }
    {
        mmask_t oldmask, return_value;
        return_value = mousemask(NUM2ULONG(rb_newmask), &oldmask);
        rb_ary_push(rb_oldmask, INT2NUM(oldmask));
        return INT2NUM(return_value);
    }
}
#endif
#ifdef HAVE_WENCLOSE
static VALUE rb_wenclose(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    return wenclose(get_window(rb_win), NUM2INT(rb_y), NUM2INT(rb_y))
        ? Qtrue : Qfalse;
}
#endif
#ifdef HAVE_MOUSEINTERVAL
static VALUE rb_mouseinterval(VALUE dummy, VALUE rb_interval)
{ return INT2NUM(mouseinterval(NUM2INT(rb_interval))); }
#endif
#ifdef HAVE_WMOUSE_TRAFO
static VALUE rb_wmouse_trafo(VALUE dummy, VALUE rb_win, VALUE rb_pY, VALUE rb_pX,
                             VALUE rb_to_screen)
{
    if ((rb_obj_is_instance_of(rb_pY, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pY, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "pY and pX arguments must be Arrays, containing exactly one "
                 "Integer");
        return Qnil;
    }
    {
        int X =                             NUM2INT(rb_ary_pop(rb_pX));
        int Y = NUM2INT(rb_ary_pop(rb_pY));
        bool return_value =
            wmouse_trafo(get_window(rb_win), &Y, &X, RTEST(rb_to_screen));
        rb_ary_push(rb_pY, INT2NUM(Y)); rb_ary_push(rb_pX, INT2NUM(X));
        return return_value ? Qtrue : Qfalse;
    }
}
#endif
#ifdef HAVE_MCPRINT
static VALUE rb_mcprint(VALUE dummy, VALUE data, VALUE len)
{
    return INT2NUM(mcprint(STR2CSTR(data), NUM2INT(len)));
}
#endif
#ifdef HAVE_HAS_KEY
static VALUE rb_has_key(VALUE dummy, VALUE ch)
{return INT2NUM(has_key(NUM2INT(ch)));}
#endif
static VALUE rb_getyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    {
        int y,x;
        getyx(get_window(rb_win), y,x);
        rb_ary_push(rb_y, INT2NUM(y));
        rb_ary_push(rb_x, INT2NUM(x));
        return Qnil;
    }
}
#if defined(HAVE_GETATTRS) || defined(getattrs)
static VALUE rb_getattrs(VALUE dummy, VALUE rb_win)
{return INT2NUM(getattrs(get_window(rb_win)));}
#endif
static VALUE rb_getbegyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    getbegyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getmaxyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    getmaxyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getparyx(VALUE dummy, VALUE rb_win, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
    getparyx(get_window(rb_win), y,x);
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_getsyx(VALUE dummy, VALUE rb_y, VALUE rb_x)
{
    int y,x;
    if ((rb_obj_is_instance_of(rb_y, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_x, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "y and x arguments must be empty Arrays");
        return Qnil;
    }
#ifdef getsyx
    getsyx(y,x);
#else
    getsyx(&y,&x);
#endif
    rb_ary_push(rb_y, INT2NUM(y));
    rb_ary_push(rb_x, INT2NUM(x));
    return Qnil;
}
static VALUE rb_setsyx(VALUE dummy, VALUE rb_y, VALUE rb_x)
{
    int y = NUM2INT(rb_y), x = NUM2INT(rb_x);
    setsyx(y,x);
    return Qnil;
}

static VALUE rb_wprintw(int argc, VALUE * argv, VALUE dummy)
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

/* Debugging : use with libncurses_g.a */
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
#endif /* HAVE__TRACEF */
#ifdef HAVE__TRACEDUMP
static VALUE rb_tracedump(VALUE dummy, VALUE rb_label, label rb_win)
{
    _tracedump(STR2CSTR(rb_label), get_window(rb_win));
}
#endif /* HAVE__TRACEDUMP */
#ifdef HAVE__TRACEATTR
static VALUE rb_traceattr(VALUE dummy, VALUE attr)
{ return rb_str_new2(_traceattr(NUM2ULONG(attr))); }
#endif /* HAVE__TRACEATTR */
#ifdef HAVE__TRACEATTR2
static VALUE rb_traceattr2(VALUE dummy, VALUE buffer, VALUE ch)
{ return rb_str_new2(_traceattr2(NUM2INT(buffer),NUM2ULONG(ch))); }
#endif /* HAVE__TRACEATTR2 */
#ifdef HAVE__TRACEBITS
static VALUE rb_tracebits(VALUE dummy)
{ return rb_str_new2(_tracebits()); }
#endif /* HAVE__TRACEBITS */
#ifdef HAVE__TRACECHAR
static VALUE rb_tracechar(VALUE dummy, VALUE ch)
{ return rb_str_new2(_tracechar(NUM2ULONG(ch))); }
#endif /* HAVE__TRACECHAR */
#ifdef HAVE__TRACECHTYPE
static VALUE rb_tracechtype(VALUE dummy, VALUE ch)
{ return rb_str_new2(_tracechtype(NUM2ULONG(ch))); }
#endif /* HAVE__TRACECHTYPE */
#ifdef HAVE__TRACECHTYPE2
static VALUE rb_tracechtype2(VALUE dummy, VALUE buffer, VALUE ch)
{ return rb_str_new2(_tracechtype2(NUM2INT(buffer),NUM2ULONG(ch))); }
#endif /* HAVE__TRACECHTYPE2 */
#ifdef HAVE__TRACEMOUSE
static VALUE rb_tracemouse(VALUE dummy, VALUE rb_m)
{
    MEVENT m;
    m.id = NUM2INT(rb_iv_get(rb_m, "@id"));
    m.x  = NUM2INT(rb_iv_get(rb_m, "@x"));
    m.y  = NUM2INT(rb_iv_get(rb_m, "@y"));
    m.z  = NUM2INT(rb_iv_get(rb_m, "@z"));
    m.bstate  = NUM2ULONG(rb_iv_get(rb_m, "@bstate"));
    return rb_str_new2(_tracemouse(&m));
}
#endif /* HAVE__TRACEMOUSE */
#ifdef HAVE_TRACE
static VALUE rb_trace(VALUE dummy, VALUE param)
{ trace(NUM2ULONG(param)); return Qnil; }
#endif /* HAVE_TRACE */
#ifdef HAVE__NC_TRACEBITS
static VALUE rb_nc_tracebits()
{ return rb_str_new2(_nc_tracebits()); }
#endif /* HAVE__NC_TRACEBITS */

#ifdef HAVE_ASSUME_DEFAULT_COLORS
static VALUE rb_assume_default_colors(VALUE dummy, VALUE fg, VALUE bg)
{ return INT2NUM(assume_default_colors(NUM2INT(fg),NUM2INT(bg))); }
#endif  /* HAVE_ASSUME_DEFAULT_COLORS */
#ifdef HAVE_ATTR_GET
static VALUE rb_attr_get(VALUE dummy, VALUE rb_attrs, VALUE rb_pair,
                         VALUE dummy2)
{
    if ((rb_obj_is_instance_of(rb_attrs, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pair, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "attrs and pair arguments must be empty Arrays");
        return Qnil;
    }
    {
        attr_t attrs = 0;
        short  pair  = 0;
        int return_value = attr_get(&attrs, &pair, 0);
        rb_ary_push(rb_attrs, INT2NUM(attrs));
        rb_ary_push(rb_pair, INT2NUM(pair));
        return INT2NUM(return_value);
    }
}
static VALUE rb_wattr_get(VALUE dummy,VALUE win, VALUE rb_attrs, VALUE rb_pair,
                          VALUE dummy2)
{
    if ((rb_obj_is_instance_of(rb_attrs, rb_cArray) != Qtrue)
        || (rb_obj_is_instance_of(rb_pair, rb_cArray) != Qtrue)) {
        rb_raise(rb_eArgError,
                 "attrs and pair arguments must be empty Arrays");
        return Qnil;
    }
    {
        attr_t attrs = 0;
        short  pair  = 0;
        int return_value = wattr_get(get_window(win), &attrs, &pair, 0);
        rb_ary_push(rb_attrs, INT2NUM(attrs));
        rb_ary_push(rb_pair, INT2NUM(pair));
        return INT2NUM(return_value);
    }
}
#endif /* HAVE_ATTR_GET */

static void init_functions_3(void)
{
#ifdef HAVE_UNGETMOUSE
    rb_define_module_function(mNcurses, "getmouse",
                              (&rb_getmouse),
                              1);
    rb_define_module_function(mNcurses, "ungetmouse",
                              (&rb_ungetmouse),
                              1);
#endif
#ifdef HAVE_MOUSEMASK
    rb_define_module_function(mNcurses, "mousemask",
                              (&rb_mousemask),
                              1);
#endif
#ifdef HAVE_WENCLOSE
    rb_define_module_function(mNcurses, "wenclose?",
                              (&rb_wenclose),
                              1);
#endif
#ifdef HAVE_MOUSEINTERVAL
    rb_define_module_function(mNcurses, "mouseinterval",
                              (&rb_mouseinterval), 1);
#endif
#ifdef HAVE_WMOUSE_TRAFO
    rb_define_module_function(mNcurses, "wmouse_trafo",
                              (&rb_wmouse_trafo), 4);
#endif
#ifdef HAVE_MCPRINT
    rb_define_module_function(mNcurses, "mcprint",
                              (&rb_mcprint),
                              2);
#endif
#ifdef HAVE_HAS_KEY
    rb_define_module_function(mNcurses, "has_key?",
                              (&rb_has_key),
                              2);
#endif
    rb_define_module_function(mNcurses, "getyx",
                              (&rb_getyx),
                              3);
    rb_define_module_function(mNcurses, "getbegyx",
                              (&rb_getbegyx),
                              3);
    rb_define_module_function(mNcurses, "getmaxyx",
                              (&rb_getmaxyx),
                              3);
    rb_define_module_function(mNcurses, "getparyx",
                              (&rb_getparyx),
                              3);
    rb_define_module_function(mNcurses, "getsyx",
                              (&rb_getsyx),
                              2);
    rb_define_module_function(mNcurses, "setsyx",
                              (&rb_setsyx),
                              2);
#if defined(HAVE_GETATTRS) || defined(getattrs)
    rb_define_module_function(mNcurses, "getattrs",
                              (&rb_getattrs),
                              1);
#endif
#ifdef HAVE__TRACEF
    rb_define_module_function(mNcurses, "_tracef",
                              (&rb_tracef), -1);
#endif /* HAVE__TRACEF */
#ifdef HAVE__TRACEDUMP
    rb_define_module_function(mNcurses, "_tracedump",
                              (&rb_tracedump),
                              2);
#endif /* HAVE__TRACEDUMP */
#ifdef HAVE__TRACEATTR
    rb_define_module_function(mNcurses, "_traceattr",
                              (&rb_traceattr),
                              1);
#endif /* HAVE__TRACEATTR */
#ifdef HAVE__TRACEATTR2
    rb_define_module_function(mNcurses, "_traceattr2",
                              (&rb_traceattr2),
                              2);
#endif /* HAVE__TRACEATTR2 */
#ifdef HAVE__TRACEBITS
    rb_define_module_function(mNcurses, "_tracebits",
                              (&rb_tracebits),
                              0);
#endif /* HAVE__TRACEBITS */
#ifdef HAVE__TRACECHAR
    rb_define_module_function(mNcurses, "_tracechar",
                              (&rb_tracechar),
                              1);
#endif /* HAVE__TRACECHAR */
#ifdef HAVE__TRACECHTYPE
    rb_define_module_function(mNcurses, "_tracechtype",
                              (&rb_tracechtype),
                              1);
#endif /* HAVE__TRACECHTYPE */
#ifdef HAVE__TRACECHTYPE2
    rb_define_module_function(mNcurses, "_tracechtype2",
                              (&rb_tracechtype2), 2);
#endif /* HAVE__TRACECHTYPE2 */
#ifdef HAVE__TRACEMOUSE
    rb_define_module_function(mNcurses, "_tracechmouse",
                              (&rb_tracemouse),
                              1);
#endif /* HAVE__TRACEMOUSE */
#ifdef HAVE_TRACE
    rb_define_module_function(mNcurses, "trace",
                              (&rb_trace),
                              1);
#endif /* HAVE_TRACE */
#ifdef HAVE__NC_TRACEBITS
    rb_define_module_function(mNcurses, "_nc_tracebits", &rb_nc_tracebits, 0);
#endif /* HAVE__NC_TRACEBITS */
#ifdef HAVE_ASSUME_DEFAULT_COLORS
    rb_define_module_function(mNcurses, "assume_default_colors",
                              (&rb_assume_default_colors), 2);
#endif  /* HAVE_ASSUME_DEFAULT_COLORS */
#ifdef HAVE_ATTR_GET
    rb_define_module_function(mNcurses, "attr_get",
                              (&rb_attr_get),
                              3);
    rb_define_module_function(mNcurses, "wattr_get",
                              (&rb_wattr_get),
                              4);
#endif /* HAVE_ATTR_GET */
    rb_define_module_function(mNcurses, "wprintw",
                              (&rb_wprintw),
                              -1);
}

static void init_constants_4(void)
{
    /* trace masks */
#ifdef TRACE_DISABLE
    rb_define_const(mNcurses, "TRACE_DISABLE", INT2NUM(TRACE_DISABLE));
#endif
#ifdef TRACE_TIMES
    rb_define_const(mNcurses, "TRACE_TIMES", INT2NUM(TRACE_TIMES));
#endif
#ifdef TRACE_TPUTS
    rb_define_const(mNcurses, "TRACE_TPUTS", INT2NUM(TRACE_TPUTS));
#endif
#ifdef TRACE_UPDATE
    rb_define_const(mNcurses, "TRACE_UPDATE", INT2NUM(TRACE_UPDATE));
#endif
#ifdef TRACE_MOVE
    rb_define_const(mNcurses, "TRACE_MOVE", INT2NUM(TRACE_MOVE));
#endif
#ifdef TRACE_CHARPUT
    rb_define_const(mNcurses, "TRACE_CHARPUT", INT2NUM(TRACE_CHARPUT));
#endif
#ifdef TRACE_ORDINARY
    rb_define_const(mNcurses, "TRACE_ORDINARY", INT2NUM(TRACE_ORDINARY));
#endif
#ifdef TRACE_CALLS
    rb_define_const(mNcurses, "TRACE_CALLS", INT2NUM(TRACE_CALLS));
#endif
#ifdef TRACE_VIRTPUT
    rb_define_const(mNcurses, "TRACE_VIRTPUT", INT2NUM(TRACE_VIRTPUT));
#endif
#ifdef TRACE_IEVENT
    rb_define_const(mNcurses, "TRACE_IEVENT", INT2NUM(TRACE_IEVENT));
#endif
#ifdef TRACE_BITS
    rb_define_const(mNcurses, "TRACE_BITS", INT2NUM(TRACE_BITS));
#endif
#ifdef TRACE_ICALLS
    rb_define_const(mNcurses, "TRACE_ICALLS", INT2NUM(TRACE_ICALLS));
#endif
#ifdef TRACE_CCALLS
    rb_define_const(mNcurses, "TRACE_CCALLS", INT2NUM(TRACE_CCALLS));
#endif
#ifdef TRACE_MAXIMUM
    rb_define_const(mNcurses, "TRACE_MAXIMUM", INT2NUM(TRACE_MAXIMUM));
#endif
}

/* Wrap ACS_* constants (additionally) as methods of SCREEN: */
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
                 (&rb_ ## ACS),     \
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
#ifdef ACS_S3
    rb_ACS(ACS_S3)
#endif
#ifdef ACS_S7
    rb_ACS(ACS_S7)
#endif
#ifdef ACS_LEQUAL
    rb_ACS(ACS_LEQUAL)
#endif
#ifdef ACS_GEQUAL
    rb_ACS(ACS_GEQUAL)
#endif
#ifdef ACS_PI
    rb_ACS(ACS_PI)
#endif
#ifdef ACS_NEQUAL
    rb_ACS(ACS_NEQUAL)
#endif
#ifdef ACS_STERLING
    rb_ACS(ACS_STERLING)
#endif

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
#ifdef ACS_S3
    wrap_ACS(ACS_S3);
#endif
#ifdef ACS_S7
    wrap_ACS(ACS_S7);
#endif
#ifdef ACS_LEQUAL
    wrap_ACS(ACS_LEQUAL);
#endif
#ifdef ACS_GEQUAL
    wrap_ACS(ACS_GEQUAL);
#endif
#ifdef ACS_PI
    wrap_ACS(ACS_PI);
#endif
#ifdef ACS_NEQUAL
    wrap_ACS(ACS_NEQUAL);
#endif
#ifdef ACS_STERLING
    wrap_ACS(ACS_STERLING);
#endif
}


void Init_ncurses(void)
{
    mNcurses = rb_define_module("Ncurses");
    rb_iv_set(mNcurses, "@windows_hash", rb_hash_new());
    rb_iv_set(mNcurses, "@screens_hash", rb_hash_new());
    cWINDOW  = rb_define_class_under(mNcurses, "WINDOW", rb_cObject);
    cSCREEN  = rb_define_class_under(mNcurses, "SCREEN", rb_cObject);

    init_constants_1();
    init_constants_2();
    init_constants_3();
    init_constants_4();
    init_globals_1();
    init_globals_2();
    init_functions_0();
    init_functions_1();
    init_functions_2();
    init_functions_3();

    init_SCREEN_methods();
#ifdef HAVE_PANEL_H
    init_panel();
#endif
}
