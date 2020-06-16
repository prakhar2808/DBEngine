require 'socket'

s = TCPSocket.new 'localhost', 12345

s.write("/home/prakhar/multithreadedServer/a.txt\n")

s.each_line do |line|
  puts line
end

s.close
