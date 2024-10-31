const express = require("express");
const net = require("net");

const app = express();
const port = 3000;

// Route to execute search query
app.get("/search", (req, res) => {
  const query = req.query.queryWord;

  if (!query) {
    return res.status(400).send("Query word is required");
  }

  // Create a connection to the C++ server
  const client = new net.Socket();
  client.connect(8080, "127.0.0.1", () => {
    console.log("Connected to C++ server");
    client.write(query); // Send the query to the C++ server
  });

  client.on("data", (data) => {
    console.log("Received from C++:", data.toString());
    res.send(`Program Output: ${data.toString()}`);
    client.destroy(); // Close the connection after receiving the response
  });

  client.on("error", (err) => {
    console.error(`Error: ${err.message}`);
    res.status(500).send(`Error: ${err.message}`);
  });
});

// Start the server
app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});
