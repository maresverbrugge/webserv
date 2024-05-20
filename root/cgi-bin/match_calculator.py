#!/usr/bin/env python3

import os

def calculate_match_score(name1, name2):
    """
    Calculate a silly match score based on the names.
    """
    # Combine the names into a single string
    combined_name = name1.lower() + name2.lower()

    # Count the number of occurrences of each letter in the combined name
    letter_counts = {}
    for char in combined_name:
        if char.isalpha():
            letter_counts[char] = letter_counts.get(char, 0) + 1

    # Sum the ASCII values of all letters in the combined name
    total_ascii_value = sum(ord(char) for char in combined_name)

    # Calculate the match score as the sum of the ASCII values modulo 101
    match_score = total_ascii_value % 101

    return match_score

# Get names from environment variables
crush_name = os.environ.get('CRUSH_NAME', '')
your_name = os.environ.get('YOUR_NAME', '')

# Check if any two of the names are "Flen", "Felicia", and "Mares" (or "Marès")
names_set = {crush_name.lower(), your_name.lower()}
special_names_set = {"flen", "felicia", "mares"} | {"flen", "felicia", "marès"}
if len(names_set & special_names_set) >= 2:
    # Generate HTML content for strong match
    html_content = f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Match Calculator</title>
        <style>
            body {{
                background-color: pink;
                font-family: Arial, sans-serif;
                text-align: center;
                margin: 0;
                padding: 0;
            }}
            .container {{
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                flex-direction: column; /* Added to center elements vertically */
            }}
            .error-message {{
                font-size: 36px;
                font-weight: bold;
                color: red;
                margin-bottom: 20px;
            }}
            .try-again {{
                font-size: 16px;
                margin-top: 20px;
            }}
        </style>
    </head>
    <body>
        <div class="container">
            <div class="error-message">Oh no! It seems that your match is so strong it has broken the Match Calculator!</div>
            <div class="try-again">
                <form action="/submit_names.html">
                    <input type="submit" value="Repair Match Calculator">
                </form>
            </div>
        </div>
    </body>
    </html>
    """
else:
    # Calculate match score for normal case
    match_score = calculate_match_score(crush_name, your_name)

    # Generate HTML content for normal case
    html_content = f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Match Calculator</title>
        <style>
            body {{
                background-color: pink;
                font-family: Arial, sans-serif;
                text-align: center;
                margin: 0;
                padding: 0;
            }}
            .container {{
                display: flex;
                justify-content: space-between;
                align-items: center;
                padding: 20px;
                margin-top: 100px;
            }}
            .name {{
                font-size: 36px;
                font-weight: bold;
                color: red;
                flex-basis: 30%;
            }}
            .match-score {{
                font-size: 48px;
                font-weight: bold;
                margin-top: 20px;
            }}
            .try-again {{
                font-size: 16px;
                margin-top: 20px;
            }}
        </style>
    </head>
    <body>
        <div class="container">
            <div class="name">{your_name}</div>
            <div class="match-score">Match Score: {match_score}%</div>
            <div class="name">{crush_name}</div>
        </div>
        <div class="try-again">
            <form action="/submit_names.html">
                <input type="submit" value="Try Again">
            </form>
        </div>
    </body>
    </html>
    """

# Uncomment the following lines to print status line and headers
# print("Content-Type: text/html")
# print()

# Print HTML content
print(html_content)