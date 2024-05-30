function uploadFile() {
    const form = document.getElementById('uploadForm');
    const formData = new FormData(form);

    console.log(formData);
    
    fetch('/uploads', {
        method: 'POST',
        body: formData
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            alert('File uploaded successfully!');
        } else {
            alert('Error uploading file: ' + data.message);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert('An error occurred while uploading the file.');
    });
}