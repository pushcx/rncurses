#!/usr/bin/env ruby
require "ncurses"

# call should fail, but must not terminate the ruby interpreter
begin
  Ncurses.start_color
rescue Ncurses::Exception, NoMethodError
  exit 0
else
  exit 1
end
