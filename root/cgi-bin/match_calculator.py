# This script generates a match score between two names for a dating website.

import os

def calculate_match_score(name1, name2):
    combined_name = name1.lower() + name2.lower()

    letter_counts = {}
    for char in combined_name:
        if char.isalpha():
            letter_counts[char] = letter_counts.get(char, 0) + 1

    total_ascii_value = sum(ord(char) for char in combined_name)
    match_score = total_ascii_value % 101
    return match_score

# Get names from environment variables
crush_name = os.environ.get('CRUSH_NAME', '')
your_name = os.environ.get('YOUR_NAME', '')

names_set = {crush_name.lower(), your_name.lower()}
special_names_set = {"flen", "felicia", "mares"} | {"flen", "felicia", "marès"}
if len(names_set & special_names_set) >= 2: # If both names are in the special set
    body_content = f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Match Calculator</title>
	    <link rel="icon" type="image/x-icon" href="/img/favicon.ico">
        <link rel="stylesheet" href="/styles/match.css">
    </head>
    <body>
        <div class="special">
            <div class="error-message">Oh no! It seems that your match is so strong it has broken the Match Calculator!</div>
            <a href="/pages/testers/match.html">Try again</a>
            or go back to the <a href="/pages/index.html">Homepage
        </div>
    </body>
    </html>
    """
else: # If the names are not in the special set
    match_score = calculate_match_score(crush_name, your_name)

    body_content = f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Match Calculator</title>
	    <link rel="icon" type="image/x-icon" href="/img/favicon.ico">
        # <style>
        #     body {{
        #         background-color: pink;
        #         font-family: Arial, sans-serif;
        #         text-align: center;
        #         margin: 0;
        #         padding: 0;
        #     }}
        #     .container {{
        #         display: flex;
        #         justify-content: space-between;
        #         align-items: center;
        #         padding: 20px;
        #         margin-top: 100px;
        #     }}
        #     .name {{
        #         font-size: 36px;
        #         font-weight: bold;
        #         color: red;
        #         flex-basis: 30%;
        #     }}
        #     .match-score {{
        #         font-size: 48px;
        #         font-weight: bold;
        #         margin-top: 20px;
        #     }}
        #     .try-again {{
        #         font-size: 16px;
        #         margin-top: 20px;
        #     }}
        # </style>
        <link rel="stylesheet" href="/styles/match.css">
    </head>
    <body>
        <div class="normal">
            <div class="name">{your_name}</div>
            <div class="match-score">Match Score: {match_score}%</div>
            <div class="name">{crush_name}</div>
        </div>
        <a href="/pages/testers/match.html">Try again</a>
        or go back to the <a href="/pages/index.html">Homepage
    </body>
    </html>
    """

content_length = len(body_content.encode('utf-8'))
header_content = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {content_length}\r\n\r\n"
print(header_content)
print(body_content)