#!/usr/bin/env ruby
require "ncurses"

term = Ncurses.newterm(nil, 1, 0)

Ncurses.start_color

Ncurses.endwin
