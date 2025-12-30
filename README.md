# Information Security Project

A client–server based application developed for information security experiments.  
The server runs on **Linux/Unix systems**, while the client application is implemented using **C++/CLI (.NET)** and runs on **Windows** only.

---

# Compiling the Server (Unix / Linux)

### Clone the Repository
```bash
git clone https://github.com/2mem333/InformationSecurityProject
```
```bash
cd InformationSecurityProject/LinuxServer
```

### Compile the Server
```bash
g++ -std=c++17 -Wall -pthread LinuxServer.cpp -o server
```

Compilation Error (Unknown Pseudo-Op)
If you encounter an unknown pseudo-op compilation error, reinstall the required build tools:

```bash
sudo apt install --reinstall build-essential binutils gcc g++
```

### Run the Server
You can run the server with a custom IP and port, or use the default configuration.

Default Configuration
`IP: 0.0.0.0`
`Port: 9090`

```bash
./server
```

Custom IP and Port
```bash
./server <IP_ADDRESS> <PORT>
```

Example:
```bash
./server 192.168.1.10 8080
```

# Compiling the Client Software (Windows Only)
The client application is implemented using C++/CLI (.NET Framework) and works only on Windows operating systems.

### Requirements
Windows OS

### Build Instructions
Open the client project in Visual Studio
Use the default Visual Studio build configuration

### Client Configuration
**1.** Before running the client, update the server connection parameters.

**2.** Open the SendReceivePacket.h file

**3.** Locate the global variables:

```bash
host
port
```

**4.** Set them to match the IP address and port used when starting the server


