function uploadFile() {
    const form = document.getElementById('uploadForm');
    const fileInput = document.getElementById('fileInput');

    if (fileInput.files.length === 0) {
        alert('No file selected!');
        return;
    }

    const formData = new FormData(form);

    fetch('/uploads', {
        method: 'POST',
        body: formData
    })
    .then(response => {
        if (response.ok) {
            // If the status code is 200, parse the response as JSON
            return response.json();
        } else {
            // If the status code is not 200, parse the response as text
            return response.text().then(html => {
                // Show the HTML response in the browser
                document.body.innerHTML = html;
                throw new Error('Server error: ' + response.status);
            });
        }
    })
    .then(data => {
        if (data.success) {
            alert('File uploaded successfully!');
            location.reload();
        }
    })
    .catch(error => {
        console.error(error);
    });
}