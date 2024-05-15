import cgi
import random

# Generate three random colors.
color1 = "%06x" % random.randint(0, 0xFFFFFF)
color2 = "%06x" % random.randint(0, 0xFFFFFF)
color3 = "%06x" % random.randint(0, 0xFFFFFF)

# Calculate the width of each color block.
block_width = "33.2%"

# Print the HTML content.
print(f"""
<html>
<head>
    <title>Random Background Color</title>
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
""")