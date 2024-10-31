const express = require("express");
const { exec } = require("child_process");
const fs = require("fs");

const app = express();
const port = 3000;

// Function to compile the C++ code
function compileDriverCode(callback) {
  exec(
    "g++ -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lboost_system -lboost_filesystem -pthread driverCode.cpp -o driverCode",
    (error, stdout, stderr) => {
      if (error) {
        console.error(`Error compiling driverCode: ${error.message}`);
        return callback(error);
      }
      if (stderr) {
        console.error(`Compile stderr: ${stderr}`);
      }
      console.log(`Compile stdout: ${stdout}`);
      callback(null);
    }
  );
}

// Route to execute C++ program on GET request
app.get("/", (req, res) => {
  res.send("Hello World! welcome to my program");
});
app.get("/search", (req, res) => {
  // The string to pass to the C++ program
  const query = req.query.queryWord;
  if (!query) {
    return res.status(400).send("queryWord parameter is required");
  }
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

// Compile the C++ code and start the server
compileDriverCode((error) => {
  if (error) {
    console.error("Failed to compile driverCode. Server not started.");
    process.exit(1);
  } else {
    app.listen(port, () => {
      console.log(`Server is running on port ${port}`);
    });
  }
});
