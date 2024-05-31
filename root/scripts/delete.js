function deleteFile(filename) {
    fetch(filename, { method: 'DELETE' })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert('File deleted successfully!');
                location.reload();
            } else {
                alert('Error deleting file: ' + data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while deleting the file.');
        });
}