import random

color1 = "%06x" % random.randint(0, 0xFFFFFF)
color2 = "%06x" % random.randint(0, 0xFFFFFF)
color3 = "%06x" % random.randint(0, 0xFFFFFF)

body_content = f"""<html>
<head>
    <title>Random Background Color</title>
	<link rel="icon" type="image/x-icon" href="/img/favicon.ico">
    <link rel="stylesheet" href="/styles/interior.css">
    <style>
        .color-block-container {{
            display: flex;
            justify-content: space-between; /* Distribute color blocks evenly */
            padding: 20px; /* Add padding for spacing */
        }}
        .color-block {{
            flex: 1; /* Each color block takes equal space */
            height: 100vh; /* Set height to full viewport height */
        }}
        .color-text {{
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            color: white; /* Ensure text is readable */
        }}
    </style>
</head>
<body>
    <div class="color-block-container">
        <div class="color-block" style="background-color: #{color1};"></div>
        <div class="color-block" style="background-color: #{color2}; position: relative;">
            <div class="color-text">Your ideal home interior palette is #{color1} #{color2} #{color3}</div>
            <div style="text-align: center; padding-top: 80px;">
                Go back to the <a href="/pages/index.html">Homepage</a>
                or
                <a href="/cgi-bin/interior_design.py">Generate</a> new colours
            </div>
        </div>
        <div class="color-block" style="background-color: #{color3};"></div>
    </div>
</body>
</html>
"""

content_length = len(body_content.encode('utf-8'))
header_content = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {content_length}\r\n\r\n"
print(header_content)
print(body_content)