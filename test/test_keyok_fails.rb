#!/usr/bin/env ruby
require "ncurses"

# call should fail, but must not terminate the ruby interpreter
begin
  Ncurses.keyok(22, true)
rescue Ncurses::Exception, NoMethodError
  exit 0
else
  exit 1
end
