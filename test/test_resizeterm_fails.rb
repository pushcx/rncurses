#!/usr/bin/env ruby
require "ncurses"

# call should fail, but must not terminate the ruby interpreter
begin
  Ncurses.resizeterm(22, 22)
rescue Ncurses::Exception, NoMethodError
  exit 0
else
  exit 1
end
