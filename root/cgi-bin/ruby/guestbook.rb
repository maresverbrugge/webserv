#!/usr/bin/env ruby
require 'cgi'
require 'fileutils'

# Path to store messages
MESSAGE_FILE = "./root/guestbook/guestbook.txt"

def save_message(name, message)
  File.open(MESSAGE_FILE, 'a') do |file|
    file.write("#{name}: #{message}\n\n")
  end
end

def read_messages
  return "" unless File.exist?(MESSAGE_FILE)
  File.read(MESSAGE_FILE).gsub("\n", "<br>")
end

# Retrieve form data from environment variables
name = (ENV['name'] || '').dup
message = (ENV['message'] || '').dup
message.gsub!('+', ' ')

if !name.empty? && !message.empty?
  save_message(name, message)
end

messages = read_messages

puts "HTTP/1.1 200 OK\r\n"
puts "Content-Type: text/html\r\n\r\n"
puts <<-HTML
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Guestbook</title>
    <link rel="icon" type="image/x-icon" href="/img/favicon.ico">
    <link rel="stylesheet" href="/styles/styles.css">
</head>
<body>
  <div class="container">
    <h1>Guestbook</h1>
    <p>Go back to the <a href="/pages/index.html">homepage</a></p>
    <form action="/cgi-bin/ruby/guestbook.rb" method="post">
        <label for="name">Name:</label><br>
        <input type="text" id="name" name="name"><br>
        <label for="message">Message:</label><br>
        <textarea id="message" name="message"></textarea><br>
        <input type="submit" value="Submit">
    </form>
    <h2>Messages</h2>
    <div id="messages">
        #{messages}
    </div>
  </div>
</body>
</html>
HTML
