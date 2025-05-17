// Save the text to log.txt using POST
function saveText() {
  const text = document.getElementById('textInput').value;

  fetch('/saveText', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({ text })
  })
  .then(res => res.text())
  .then(msg => {
    document.getElementById('message').textContent = msg;
  })
  .catch(err => {
    document.getElementById('message').textContent = 'Error: ' + err.message;
  });
}

// Load and display the contents of log.txt using GET
function loadFile() {
  fetch('/readFile')
    .then(res => {
      if (!res.ok) throw new Error("Could not read file");
      return res.text();
    })
    .then(data => {
      document.getElementById('fileContent').textContent = data || '(File is empty)';
    })
    .catch(err => {
      document.getElementById('fileContent').textContent = 'Error: ' + err.message;
    });
}


