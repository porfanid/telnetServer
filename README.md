# Telnet Server Application

## Overview
This application is a Telnet server that streams ASCII art content to connected clients. The server can be configured to run on a specified port and stream a specified file. If no file is specified, the default `ascii_star_wars.txt` will be used. If no port is specified, the default port `23` will be used.

## Compilation

### Prerequisites
- CMake 3.29 or higher
- A C++20 compatible compiler
- Boost libraries (version 1.70 or higher)

### Installing Boost

#### Debian/Ubuntu
```sh
sudo apt-get update
sudo apt-get install libboost-all-dev
```

#### Red Hat/CentOS
```sh
sudo yum install boost-devel
```

#### macOS
Using Homebrew:
```sh
brew install boost
```

### Steps
1. Clone the repository:
    ```sh
    git clone https://github.com/porfanid/telnetServer.git
    cd telnetServer
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Compile the project:
    ```sh
    make
    ```

## Running the Server

### Running on Ports Above 1024
To run the server on a port above 1024, you can simply execute the compiled binary:
```sh
./telnetServer <filename> <port>
```
Example:
```sh
./telnetServer ascii_star_wars.txt 8080
```
If no filename is provided, the default `ascii_star_wars.txt` will be used. If no port is provided, the default port `23` will be used:
```sh
./telnetServer
```
or
```sh
./telnetServer ascii_star_wars.txt
```

### Running on Ports Below 1024
Ports below 1024 require root privileges. You can either run the server as root or use `authbind` to grant non-root users access to these ports.

#### Using `authbind`

##### Installation
- **Debian/Ubuntu:**
    ```sh
    sudo apt-get install authbind
    ```

- **Red Hat/CentOS:**
    ```sh
    sudo yum install authbind
    ```

- **macOS:**
  `authbind` is not natively available on macOS. Consider using ports above 1024 or running as root.

##### Granting Access with `set_user_port.sh`
1. Ensure the script has execute permissions:
    ```sh
    chmod +x set_user_port.sh
    ```

2. Run the script with the desired port and user:
    ```sh
    sudo ./set_user_port.sh <port> [username:group]
    ```
   Example:
    ```sh
    sudo ./set_user_port.sh 23 myuser:mygroup
    ```

##### Running the Server with `authbind`
After setting up `authbind`, you can run the server on a privileged port without root:
```sh
authbind --deep ./telnetServer <filename> <port>
```
Example:
```sh
authbind --deep ./telnetServer ascii_star_wars.txt 23
```

