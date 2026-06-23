# Godot-Arduino-Communication
A sample project that allows an Arduino and a Godot project to communicate with eachother over Wi-Fi.

## Usage
First, open up the Arduino Sample Project and change the `ssid[]` variable to your network name. Do the same for the `pass[]` variable, change it to the appropriate password associated with the entered network. Optionally, you can chose a port on which communication will take place. The default port we use is port 80. This is the default network port for HTTP traffic, used by web servers to send and receive data. You can also set a timeout per message using the `streamTimeout` constant.

After this code is uploaded to the Arduino, a message should appear that tells you the IP and port number of the successful connection.

Now we can open up the Godot Sample Project and load into our main scene `main.tscn`. Once the scene has loaded fully, press the root Control node. This node contains exported properties for the IP and port number. **Enter the same IP and port that the Arduino printed.**

Run the project by pressing F5. After connecting successfully, the Godot game will show the data being echoed through Arduino on a label in the center of the screen. This is merely for debugging purposes, but can be repurposed into responsive UI or similar. The ToggleButton on top toggles the builtin-LED on the Arduino.

## Building Upon This
In both the Arduino and Godot sample projects, methods exist that define incoming- and outgoing traffic behaviour. These methods are called `HandleIncoming()` and `HandleOutgoing()` respectively. (Capitalised differently based on C++ or C#.)

Defining your own functionality is as simple as rewriting the body in these methods with your own desired behaviour.
