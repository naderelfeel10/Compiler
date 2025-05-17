const express = require('express');
const fs = require('fs');
const app = express();
const path = require('path');

// Serve static files (frontend)
app.use(express.static(path.join(__dirname, '../')));
app.use(express.json());

// Save text to input.txt
app.post('/saveText', (req, res) => {
  const text = req.body.text;
  fs.writeFile('input.txt', text, 'utf8', (err) => {
    if (err) return res.status(500).send('Error writing file');
    res.send('File saved successfully!');
  });
});

// Read from output.txt
const filePath = path.join(__dirname, '../../output.txt');
app.get('/readFile', (req, res) => {
  fs.readFile(filePath, 'utf8', (err, data) => {
    if (err) return res.status(500).send('Error reading file');
    res.send(data);
  });
});

// Generate and serve collapsible HTML tree from output.txt
app.get('/tree', (req, res) => {
  console.log("/tree");
  fs.readFile(filePath, 'utf8', (err, parseTree) => {
    if (err) {
      console.error('Error reading tree:', err);
      return res.status(500).send('Error reading tree: ' + err.message);
    }

    const lines = parseTree.split('\n');
    const statements = [];
    let currentBlock = [];

    for (const line of lines) {
      if (line.includes("├── statement") || line.includes("└── statement")) {
        if (currentBlock.length > 0) {
          statements.push(currentBlock.join("\n"));
          currentBlock = [];
        }
      }
      currentBlock.push(line);
    }
    if (currentBlock.length > 0) {
      statements.push(currentBlock.join("\n"));
    }

    const html = `<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Collapsible Parse Tree</title>
  <style>
    body {
      background: #1e1e1e;
      color: #ccc;
      font-family: monospace;
      padding: 20px;
    }
    .statement {
      border: 1px solid #444;
      margin: 10px 0 10px 20px;
      border-radius: 8px;
    }
    .header {
      background-color: #333;
      padding: 6px 12px;
      cursor: pointer;
      font-weight: bold;
      user-select: none;
    }
    .content {
      background-color: #2a2a2a;
      padding: 10px;
      white-space: pre;
      display: none;
    }
    .statement.open .content {
      display: block;
    }
  </style>
</head>
<body>
<h2>Collapsible Parse Tree</h2>
${statements
  .map(
    (block) => `
<div class="statement">
  <div class="header">statement</div>
  <div class="content">${block.replace(/</g, "&lt;").replace(/>/g, "&gt;")}</div>
</div>
`
  )
  .join("\n")}
<script>
  document.addEventListener("DOMContentLoaded", () => {
    document.querySelectorAll(".header").forEach((header) => {
      header.addEventListener("click", () => {
        header.parentElement.classList.toggle("open");
      });
    });
  });
</script>
</body>
</html>`;

    res.send(html);
  });
});


// Start the server
const port = 3000;
app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
  console.log(`View parse tree at http://localhost:${port}/tree`);
});
