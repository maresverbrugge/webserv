import cgi
import random

# Generate three random colors.
color1 = "%06x" % random.randint(0, 0xFFFFFF)
color2 = "%06x" % random.randint(0, 0xFFFFFF)
color3 = "%06x" % random.randint(0, 0xFFFFFF)

# Calculate the width of each color block.
block_width = "33.2%"

# Generate the HTML content.
body_content = f"""<html>
<head>
    <title>Random Background Color</title>
	<link rel="icon" type="image/x-icon" href="/img/favicon.ico">
    <style>
        .color-block {{
            width: {block_width};
            height: 99vh;
            display: inline-block;
        }}
        .color-text {{
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 24px;
            color: white;
        }}
    </style>
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

# Calculate the length of the body content
content_length = len(body_content.encode('utf-8'))

# Generate header content
header_content = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {content_length}\r\n\r\n"

# Print header content
print(header_content)

# Print body content
print(body_content)