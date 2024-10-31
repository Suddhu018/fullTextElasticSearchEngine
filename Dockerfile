# Use a Node.js runtime with a newer distribution
FROM node:14-bullseye

# Install dependencies for C++ and Boost
RUN apt-get update && apt-get install -y \
    g++ \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy package.json and package-lock.json
COPY package*.json ./

# Install Node.js dependencies
RUN npm install

# Copy the rest of the application code
COPY . .

# Compile the C++ code
RUN g++ -std=c++17 -I/usr/include -L/usr/lib -lboost_system -lboost_filesystem -pthread driverCode.cpp -o driverCode

# Ensure the driverCode binary is executable
RUN chmod +x driverCode

# Expose the port the app runs on
EXPOSE 3000

# Command to run the app
CMD ["node", "server.js"]
