#!/usr/bin/env ruby

# $Id: make_dist.rb,v 1.5 2003/03/22 22:55:33 t-peters Exp $

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
sys "tar cjf #{base}-#{Version}.tar.bz2 #{base}-#{Version}"

# check if we create a binary distribution for a mingw extension
binary_description = `file ncurses.so`

if ((binary_description =~ /\s(windows)\s/i)                  &&
    (binary_description =~ /\s(pe)|(portable executable)\s/i) &&
    (binary_description =~ /\s(dll)\s/i))
  sys "cp ncurses.so README.binary #{base}-#{Version}"
  Dir.glob("#{base}-#{Version}/README*").each{|textfile|
    text = IO.readlines(textfile).map{|line|line.chomp + "\r\n"}
    File.open(textfile + ".txt", "wb"){|outfd| outfd.write(text.join)}
    sys "rm #{textfile}"
  }
  sys "rm #{base}-#{Version}/{MANIFEST,make_dist.rb}"
  sys "zip -9 -r #{base}-#{Version}-i686-mingw32.zip #{base}-#{Version}"
end

sys "rm -r #{base}-#{Version}/"
