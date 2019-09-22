#include <Adafruit_PWMServoDriver.h>
#include "Servo.h"
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String forward_State = "off";
String backward_State = "off";
String left_State = "off";
String right_State = "off";


void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  //Set up the servos
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  reset_pos();
  delay(2000);
} ;
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // forward
            if (header.indexOf("GET /forward/on") >= 0) {
              Serial.println("Move Forward");
              forward_State = "on";
              forward(5);
              forward_State = "off";
            // backward
            } else if (header.indexOf("GET /backward/on") >= 0) {
              Serial.println("GPIO 4 on");
              backward_State = "on";
              backward(5);
              backward_State = "off";
            // left
            } else if (header.indexOf("GET /left/on") >= 0) {
              Serial.println("Left");
              left_State = "on";
              left(2);
              left_State = "off";
            // right
            } else if (header.indexOf("GET /right/on") >= 0) {
              Serial.println("Right");
              left_State = "on";
              right(2);
              left_State = "off";
            }

            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Eric's Hexapod!</h1>");
            
            // Display current state, and ON/OFF buttons for forward 
            client.println("<p>Forward - State " + forward_State + "</p>");
            // If the forward_State is off, it displays the ON button       
            if (forward_State=="off") {
              client.println("<p><a href=\"/forward/on\"><button class=\"button\">Forward</button></a></p>");
            } else {
              client.println("<p><a href=\"/forward/off\"><button class=\"button button2\">Stop</button></a></p>");
            } 

            // Display current state, and ON/OFF buttons for left  
            client.println("<p>Left - State " + left_State + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + "Right - State " + right_State + "</p>");      
            if (left_State=="off") {
              client.println("<p><a href=\"/left/on\"><button class=\"button\">Left</button></a>");
              client.println("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"); 
              client.println("<a href=\"/right/on\"><button class=\"button\">Right</button></a></p>");
            }
               
            // Display current state, and ON/OFF buttons for backward  
            client.println("<p>Backward - State " + backward_State + "</p>");
            // If the backward_State is off, it displays the ON button       
            if (backward_State=="off") {
              client.println("<p><a href=\"/backward/on\"><button class=\"button\">Backward</button></a></p>");
            } else {
              client.println("<p><a href=\"/backward/off\"><button class=\"button button2\">Stop</button></a></p>");
            }

            client.println("</body></html>");

            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

    reset_pos();
  }
}  
