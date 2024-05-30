# This script generates a random color palette for an interior design website.

import cgi
import random

color1 = "%06x" % random.randint(0, 0xFFFFFF)
color2 = "%06x" % random.randint(0, 0xFFFFFF)
color3 = "%06x" % random.randint(0, 0xFFFFFF)

body_content = f"""<html>
<head>
    <title>Random Background Color</title>
    <link rel="stylesheet" href="/styles/interior.css">
</head>
<body>
    <div class="color-block" style="background-color: #{color1};"></div>
    <div class="color-block" style="background-color: #{color2}; position: relative;">
        <div class="color-text">Your ideal home interior palette is #{color1} #{color2} #{color3}</div>
    </div>
    <div class="color-block" style="background-color: #{color3};"></div>
</body>
</html>
"""

content_length = len(body_content.encode('utf-8'))
header_content = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {content_length}\r\n\r\n"
print(header_content)
print(body_content)