require "mkmf"

$CFLAGS  += " -g"
$CXXFLAGS  = $CFLAGS

have_header("unistd.h")
unless have_header("ncurses.h")
  unless have_header("ncurses/curses.h")
    unless have_header("curses.h")
      raise "ncurses header file not found"
    end
  end
end
unless have_library("ncurses", "wmove")
  raise "ncurses library not found"
end

puts "checking which debugging functions to wrap..."
have_func("_tracef")
have_func("_tracedump")
have_func("_nc_tracebits")
have_func("_traceattr")
have_func("_traceattr2")
have_func("_tracechar")
have_func("_tracechtype")
have_func("_tracechtype2")
have_func("_tracemouse")

puts "checking for other functions that appeared after ncurses version 5.0..."
have_func("assume_default_colors")
have_func("attr_get")

puts "checking for the panel library..."
have_header("panel.h")
have_library("panel", "panel_hidden")
create_makefile('ncurses')

makefile = IO.readlines("Makefile").collect{|line|
	line.chomp!
	line.gsub("gcc", "g++")
}	

line_no = makefile.index(makefile.grep(/^install:/)[0])

match_inst_prefix = /:(.*\/)/.match(makefile[line_no])
if (match_inst_prefix == nil)
  raise("could not deduce installation directory from generated Makefile")
end
makefile[line_no+1,0] = "\t@$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0644, true)' ncurses.rb $(sitelibdir)$(target_prefix)/ncurses.rb"

File.open("Makefile", "w") {|f|
  f.puts(makefile)
}
