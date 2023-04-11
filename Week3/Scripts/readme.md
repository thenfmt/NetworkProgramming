### Installation

1. Download the program files and save them in a directory on your computer.
2. Open a terminal window and navigate to the directory where the program files are saved.

### Running the Server Program

1. In the terminal window, navigate to the directory containing the server program file.
2. Type the following command to run the server program: `./server portNumber`
3. Replace `portNumber` with the desired port number to use for the server. For example: `./server 8080`
4. Press Enter to run the command.

### Running the Client Programs

#### Sender Client

1. In a new terminal window, navigate to the directory containing the sender client program file.
2. Type the following command to run the sender client program: `./sender_client ipAddress portNumber`
3. Replace `ipAddress` with the IP address of the server. For example: `./sender_client 127.0.0.1 8080`
4. Replace `portNumber` with the port number used by the server.
5. Press Enter to run the command.

#### Receiver Client

1. In a new terminal window, navigate to the directory containing the receiver client program file.
2. Type the following command to run the receiver client program: `./receiver_client ipAddress portNumber`
3. Replace `ipAddress` with the IP address of the server. For example: `./receiver_client 127.0.0.2 8080`
4. Replace `portNumber` with the port number used by the server.
5. Press Enter to run the command.

### Sending and Receiving Messages

1. Once you have run the sender client program, you can enter a string to send to the server.
2. The server will then process the string and send a response back to the receiver client.
3. The receiver client will display the response on the screen.
4. You can repeat this process by entering another string and sending it to the server, until you are finished and want to exit the programs by entering an empty input in the sender client.
