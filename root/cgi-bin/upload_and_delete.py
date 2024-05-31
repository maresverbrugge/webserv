import os
import sys

def generate_http_response():
    # Directory containing deletable files
    directory = './root/uploads'
    
    # HTML template
    html_template = '''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Upload and Delete</title>
    <link rel="icon" type="image/x-icon" href="/img/favicon.ico">
    <link rel="stylesheet" href="/styles/styles.css">
    <script src="/scripts/upload.js"></script>
    <script src="/scripts/delete.js"></script>
</head>
<body>
    <div class="container">
        <h1>Upload and Delete</h1>
        <p>
            Would you like to upload something or go back to the <a href="/pages/index.html">homepage</a>?
        </p>
        <form id="uploadForm">
            <input type="file" name="file" id="fileInput">
            <input type="button" value="Upload" onclick="uploadFile()">
        </form>
        <h2>List of Files</h2>
        <ul>
            {file_list}
        </ul>
    </div>
</body>
</html>
'''

    # Generate the list of files with delete buttons
    file_list_html = ''
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if os.path.isfile(file_path):
            file_list_html += f'''
            <li>
                <a href="/uploads/{filename}" target="_blank">{filename}</a>
                <button onclick="deleteFile('/uploads/{filename}')">Delete</button>
            </li>
            '''

    # Fill the template with the file list
    html_content = html_template.format(file_list=file_list_html)
    
    # Generate HTTP response
    response = "HTTP/1.1 200 OK\r\n"
    response += f"Content-Length: {len(html_content)}\r\n"
    response += "Content-Type: text/html\r\n"
    response += "Connection: close\r\n"
    response += "\r\n"
    response += html_content
    
    # Write the generated response to standard output
    sys.stdout.write(response)

# Call the function to generate the HTTP response
generate_http_response()