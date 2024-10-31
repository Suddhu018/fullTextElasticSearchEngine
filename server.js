const express = require("express");
const { exec } = require("child_process");

const app = express();
const port = 3000;

// Route to execute C++ program on GET request
app.get("/", (req, res) => {
  res.send("Hello World I am server");
});
app.get("/search", (req, res) => {
  // The string to pass to the C++ program
  const query = req.query.queryWord;
  // Execute the C++ program with the string as an argument
  exec(`./driverCode ${query}`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error executing program: ${error.message}`);
      return res.status(500).send(`Error: ${error.message}`);
    }
    if (stderr) {
      console.error(`Program stderr: ${stderr}`);
      return res.status(500).send(`Stderr: ${stderr}`);
    }

    // Send the output of the C++ program back to the frontend
    res.send(`Program Output: ${stdout}`);
  });
});

// Start the server
app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});
