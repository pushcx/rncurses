#!/usr/bin/env ruby

# $Id: make_dist.rb,v 1.4 2002/02/26 11:04:38 t-peters Exp $

def sys(i)
  puts i
  system i
end

dir  = File.dirname(__FILE__)
base = File.basename(dir)
base = "ncurses-ruby" if base == "."

files = IO.readlines(dir + "/MANIFEST").collect{|filename|filename.chomp}

Version = File.new("#{dir}/VERSION").readline.chomp!

sys "mkdir #{base}-#{Version}"
files.each{|filename|
  if filename.index "/"
    sys "mkdir -p #{base}-#{Version}/#{File.dirname(filename)}"
  end
  sys "cp #{dir}/#{filename} #{base}-#{Version}/#{filename}"
}
sys "tar cIf #{base}-#{Version}.tar.bz2 #{base}-#{Version}"
sys "rm -r #{base}-#{Version}/"
