# Ncurses-Ruby Is A Ruby Module For Accessing The Fsf'S Ncurses Library
# (C) 2002 Tobias Peters <T-Peters@Berlios.De>
#
# This Module Is Free Software; You Can Redistribute It And/Or
# Modify It Under The Terms Of The Gnu Lesser General Public
# License As Published By The Free Software Foundation; Either
# Version 2 Of The License, Or (At Your Option) Any Later Version.
#
# This Module Is Distributed In The Hope That It Will Be Useful,
# But Without Any Warranty; Without Even The Implied Warranty Of
# Merchantability Or Fitness For A Particular Purpose.  See The Gnu
# Lesser General Public License For More Details.
#
# You Should Have Received A Copy Of The Gnu Lesser General Public
# License Along With This Module; If Not, Write To The Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, Ma 02111-1307  Usa

require "ncurses.so"

def Ncurses._XOPEN_CURSES
  Ncurses::XOPEN_CURSES
end
def Ncurses._SUBWIN  
  Ncurses::SUBWIN
end
def Ncurses._ENDLINE 
  Ncurses::ENDLINE
end
def Ncurses._FULLWIN 
  Ncurses::FULLWIN
end
def Ncurses._SCROLLWIN
  Ncurses::SCROLLWIN
end
def Ncurses._ISPAD   
  Ncurses::ISPAD
end
def Ncurses._HASMOVED
  Ncurses::HASMOVED
end
def Ncurses._WRAPPED 
  Ncurses::WRAPPED
end
def Ncurses._NOCHANGE
  Ncurses::NOCHANGE
end
def Ncurses._NEWINDEX
  Ncurses::NEWINDEX
end
module Ncurses
  module Destroy_checker; def destroyed?; @destroyed; end; end
  class WINDOW
    include Destroy_checker
    def method_missing(name, *args)
      name = name.to_s
      if (name[0,2] == "mv")
	test_name = name.dup
	test_name[2,0] = "w" # insert "w" after"mv"
	if (Ncurses.respond_to?(test_name))
	  return Ncurses.send(test_name, self, *args)
	end
      end
      test_name = "w" + name
      if (Ncurses.respond_to?(test_name))
	return Ncurses.send(test_name, self, *args)
      end
      Ncurses.send(name, self, *args)
    end
    def del
      Ncurses.delwin(self)
    end
    alias delete del
    def WINDOW.new(*args)
      Ncurses.newwin(*args)
    end
  end
  class SCREEN
    include Destroy_checker
    def del
      Ncurses.delscreen(self)
    end
    alias delete del
  end
  class MEVENT
    attr_accessor :id, :x,:y,:z, :bstate
  end
  GETSTR_LIMIT = 1024
end

def Ncurses.inchnstr(str,n)
  Ncurses.winchnstr(Ncurses.stdscr, str, n)
end
def Ncurses.inchstr(str)
  Ncurses.winchstr(Ncurses.stdscr, str)
end
def Ncurses.mvinchnstr(y,x, str, n)
  Ncurses.mvwinchnstr(Ncurses.stdscr, y,x, str, n)
end
def Ncurses.mvinchstr(y,x, str)
  Ncurses.mvwinchstr(Ncurses.stdscr, y,x, str)
end
def Ncurses.mvwinchnstr(win, y,x, str, n)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.winchnstr(win,str,n)
  end
end
def Ncurses.mvwinchstr(win, y,x, str)
  maxy = []; maxx = []; getmaxyx(win, naxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.mvwinchnstr(win, y,x, str, maxx[0]+1)
end
def Ncurses.winchstr(win, str)
  maxy = []; maxx = []; getmaxyx(win, naxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.winchnstr(win, str, maxx[0]+1)
end

def Ncurses.getnstr(str,n)
  Ncurses.wgetnstr(Ncurses.stdscr, str, n)
end
def Ncurses.mvgetnstr(y,x, str, n)
  Ncurses.mvwgetnstr(Ncurses.stdscr, y,x, str, n)
end
def Ncurses.mvwgetnstr(win, y,x, str, n)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.wgetnstr(win,str,n)
  end
end

def Ncurses.innstr(str,n)
  Ncurses.winnstr(Ncurses.stdscr, str, n)
end
def Ncurses.instr(str)
  Ncurses.winstr(Ncurses.stdscr, str)
end
def Ncurses.mvinnstr(y,x, str, n)
  Ncurses.mvwinnstr(Ncurses.stdscr, y,x, str, n)
end
def Ncurses.mvinstr(y,x, str)
  Ncurses.mvwinstr(Ncurses.stdscr, y,x, str)
end
def Ncurses.mvwinnstr(win, y,x, str, n)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    Ncurses.winnstr(win,str,n)
  end
end
def Ncurses.mvwinstr(win, y,x, str)
  maxy = []; maxx = []; getmaxyx(win, naxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.mvwinnstr(win, y,x, str, maxx[0]+1)
end
def Ncurses.winstr(win, str)
  maxy = []; maxx = []; getmaxyx(win, naxy,maxx)
  return Ncurses::ERR if (maxx[0] == Ncurses::ERR)
  Ncurses.winnstr(win, str, maxx[0]+1)
end

def Ncurses.mouse_trafo(pY, pX, to_screen)
  Ncurses.wmouse_trafo(Ncurses.stdscr, pY, pX, to_screen)
end

def Ncurses.getcurx(win)
  x = []; y = []; Ncurses.getyx(win, y,x); x[0]
end
def Ncurses.getcury(win)
  x = []; y = []; Ncurses.getyx(win, y,x); y[0]
end
def Ncurses.getbegx(win)
  x = []; y = []; Ncurses.getbegyx(win, y,x); x[0]
end
def Ncurses.getbegy(win)
  x = []; y = []; Ncurses.getbegyx(win, y,x); y[0]
end
def Ncurses.getmaxx(win)
  x = []; y = []; Ncurses.getmaxyx(win, y,x); x[0]
end
def Ncurses.getmaxy(win)
  x = []; y = []; Ncurses.getmaxyx(win, y,x); y[0]
end
def Ncurses.getparx(win)
  x = []; y = []; Ncurses.getparyx(win, y,x); x[0]
end
def Ncurses.getpary(win)
  x = []; y = []; Ncurses.getparyx(win, y,x); y[0]
end
def Ncurses.erase
  Ncurses.werase(Ncurses.stdscr)
end
def Ncurses.getstr(str)
  Ncurses.getnstr(str, Ncurses::GETSTR_LIMIT)
end
def Ncurses.mvgetstr(y,x, str)
  Ncurses.mvgetnstr(y,x, str, Ncurses::GETSTR_LIMIT)
end
def Ncurses.mvwgetstr(win, y,x, str)
  Ncurses.mvwgetnstr(win, y,x, str, Ncurses::GETSTR_LIMIT)
end
def Ncurses.wgetstr(str)
  Ncurses.wgetnstr(str, Ncurses::GETSTR_LIMIT)
end


def Ncurses.mvprintw(*args)
  Ncurses.mvwprintw(Ncurses.stdscr, *args)
end
def Ncurses.mvwprintw(win, y,x, *args)
  if (Ncurses.wmove(win,y,x) == Ncurses::ERR) 
    Ncurses::ERR
  else
    wprintw(win, *args)
  end
end
def Ncurses.printw(*args)
  Ncurses.wprintw(Ncurses.stdscr, *args)
end
def Ncurses.touchline(win, start, count)
  Ncurses.wtouchln(win, start, count, 1)
end
def Ncurses.touchwin(win)
  wtouchln(win, 0, getmaxy(win), 1)
end
