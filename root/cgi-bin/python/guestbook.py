#!/usr/bin/env python3
import cgi
import os
# Path to store messages
MESSAGE_FILE = "./root/guestbook/guestbook.txt"
def save_message(name, message):
    with open(MESSAGE_FILE, 'a') as file:
        file.write(f"{name}: {message}\n\n")
def read_messages():
    if not os.path.exists(MESSAGE_FILE):
        return ""
    with open(MESSAGE_FILE, 'r') as file:
        return file.read().replace("\n", "<br>")
def main():
    print("HTTP/1.1 200 OK\r\nContent-type: text/html\r\n")
    name = os.environ.get('name', '')
    message = os.environ.get('message', '')
    message = message.replace('+', ' ')
    if name and message:
        save_message(name, message)
    messages = read_messages()
    print(f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Guestbook</title>
        <link rel="icon" type="image/x-icon" href="/img/favicon.ico">
	    <link rel="stylesheet" href="/styles/styles.css">
    </head>
    <body>
        <h1>Guestbook</h1>
        go back to the <a href="/pages/index.html">homepage</a><br><br>
        <form action="/cgi-bin/guestbook.py" method="post">
            <label for="name">Name:</label><br>
            <input type="text" id="name" name="name"><br>
            <label for="message">Message:</label><br>
            <textarea id="message" name="message"></textarea><br>
            <input type="submit" value="Submit">
        </form>
        <h2>Messages</h2>
        <div id="messages">{messages}</div>
    </body>
    </html>
    """)
if __name__ == "__main__":
    main()