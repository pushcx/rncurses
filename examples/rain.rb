require "ncurses"

class Raindrop
  def initialize (window)
    @window = window
    lines   = []
    columns = []
    window.getmaxyx(lines,columns)
    lines = (lines[0] <= 4) ? 1 : (lines[0] - 4)
    columns = (columns[0] <= 4) ? 1 : (columns[0] - 4)
    @current_phase = 0
    @x = rand(columns)+2
    @y = rand(lines)+2
  end

  # draw_next_phase draws the next phase of a raindrop. If this was the last
  # phase, returns 0, otherwise returns the raindrop.
  def draw_next_phase
    if (DRAWING_PROCS[@current_phase].call(@window,@y,@x))
      @current_phase += 1
      self
    end
  end

  DRAWING_PROCS = [
    Proc.new{|window,y,x|
      window.mvaddstr(y,x, ".")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y, x, "o")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y, x, "O")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y-1, x,    "-")
      window.mvaddstr(y,   x-1, "|.|")
      window.mvaddstr(y+1, x,    "-")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y-2, x,     "-")
      window.mvaddstr(y-1, x-1,  "/ \\")
      window.mvaddstr(y,   x-2,  "| O |")
      window.mvaddstr(y+1, x-1,  "\\ /")
      window.mvaddstr(y+2, x,     "-")
    },
    Proc.new{|window,y,x|
      window.mvaddstr(y-2, x,     " ")
      window.mvaddstr(y-1, x-1,  "   ")
      window.mvaddstr(y,   x-2, "     ")
      window.mvaddstr(y+1, x-1,  "   ")
      window.mvaddstr(y+2, x,     " ")
      nil
    }
  ]
  NUMBER_OF_PHASES = DRAWING_PROCS.size - 1
end

class Rain
  AVERAGE_RAINDROP_SPACE = 475.1

  def Rain.sigwinch_handler(sig = nil)
    ObjectSpace.each_object(Rain){|rain|
      rain.window_size_changed = true
    }
  end

  attr_writer :window_size_changed

  def initialize(window)
    @window = window
    @window_size_changed = true
    @raindrops = []
  end

  def fall_for_a_moment
    if (@window_size_changed)
      @window_size_changed = false
      window_size = @window.getmaxx * @window.getmaxy
      average_number_of_raindrops = window_size / AVERAGE_RAINDROP_SPACE
      @average_number_of_new_raindrops =
        average_number_of_raindrops / Raindrop::NUMBER_OF_PHASES
    end

    nf = @average_number_of_new_raindrops.floor
    nc = @average_number_of_new_raindrops.ceil
    if (nf == nc)
      n = nf
    else
      chance = @average_number_of_new_raindrops - nf
      if (rand > chance)
        n = nf
      else
        n = nc
      end
    end
    n.times{@raindrops.push(Raindrop.new(@window))}

    @raindrops = @raindrops.collect{|raindrop|
      raindrop.draw_next_phase
    }.compact
  end

  def fall(pause = 0.1)
    begin
      fall_for_a_moment
      sleep(pause)
    end while (true)
  end
end

Ncurses.initscr
begin
#  if (Ncurses.has_colors?)
#    bg = Ncurses::COLOR_BLACK
#    Ncurses.start_color
#    if (Ncurses.respond_to?("use_default_colors"))
#      if (Ncurses.use_default_colors == Ncurses::OK)
#        bg = -1
#      end
#    end
#    Ncurses.init_pair(1, Ncurses::COLOR_BLUE, bg);
#    Ncurses.init_pair(2, Ncurses::COLOR_CYAN, bg);
#  end
  Ncurses.nl()
  Ncurses.noecho()
  Ncurses.curs_set(0)
  Ncurses.timeout(0)

  rain = Rain.new(Ncurses.stdscr)

  begin
    case(Ncurses.getch())
    when 'q'[0], 'Q'[0]
      Ncurses.curs_set(1)
      Ncurses.endwin()
      exit
    when 's'[0]
      Ncurses.stdscr.nodelay(false)
    when ' '[0]
      Ncurses.stdscr.nodelay(true)
    when Ncurses::KEY_RESIZE
      Rain.sigwinch_handler
    end
    sleep(0.050)
    rain.fall_for_a_moment
    Ncurses.refresh
  end while true
ensure
  Ncurses.curs_set(1)
  Ncurses.endwin()
end
