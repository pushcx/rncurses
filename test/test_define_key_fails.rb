#!/usr/bin/env ruby
require "ncurses"

# call should fail, but must not terminate the ruby interpreter
begin
  Ncurses.define_key("Hi!", 22)
rescue Ncurses::Exception, NoMethodError
  exit 0
else
  exit 1
end
