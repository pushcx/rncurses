#!/usr/bin/env ruby
def sys(i)
  puts i
  system i
end

Version = ARGV[0]
raise "Version number must be given as argument" if (Version.type != String)

dir  = File.dirname(__FILE__)
base = File.basename(dir)
base = "ncurses-ruby" if base == "."

files = IO.readlines(dir + "/MANIFEST").collect{|filename|filename.chomp}

sys "mkdir #{base}-#{Version}"
files.each{|filename|
  if filename.index "/"
    sys "mkdir -p #{base}-#{Version}/#{File.dirname(filename)}"
  end
  sys "cp #{dir}/#{filename} #{base}-#{Version}/#{filename}"
}
sys "tar cIf #{base}-#{Version}.tar.bz2 #{base}-#{Version}"
sys "rm -r #{base}-#{Version}/"
