 #include <ESP8266WiFi.h>
 #include <ESP8266WebServer.h>  
   
 #define output2  2  
 #define output0  0  
   
 // Create the objects for server and client  
 WiFiServer server(80);  
 WiFiClient client;  
   
 const char* ssid   = "ESP_AP";         // This is the SSID that ESP32 will broadcast  
 const char* password = "12345678";     // password should be atleast 8 characters to make it work  
   
 // Create the global variable  
 String http;  
 String output2State = "off";  
 String output0State = "off";   
   
 void setup() {  
  Serial.begin(9600);  
  pinMode(output2, OUTPUT);  
  pinMode(output0, OUTPUT);  

  digitalWrite(output2, LOW);  
  digitalWrite(output0, LOW);  

  Serial.print("Connecting to ");  
  Serial.println(ssid);  
   
  // Create the ESP32 access point    
  WiFi.softAP(ssid, password);  
   
  Serial.println( "" );  
  Serial.println( "WiFi AP is now running" );  
  Serial.println( "IP address: " );  
  Serial.println( WiFi.softAPIP() );  
   
  // Start our ESP32 server  
  server.begin();  
 }  
   
 void loop(){     
  if ( client = server.available() ) {  // Checks if a new client tries to connect to our server  
   Serial.println("New Client.");  
   String clientData = "";  
   while ( client.connected() ) {    // Wait until the client finish sending HTTP request  
    if ( client.available() ) {     // If there is a data,  
     char c = client.read();      //  read one character  
     http += c;            //  then parse it  
     Serial.write(c);    
     if (c == '\n') {         // If the character is carriage return,   
                      //  it means end of http request from client  
      if (clientData.length() == 0) { //  Now that the clientData is cleared,  
       sendResponse();        //    perform the necessary action  
       updateLED();  
       updateWebpage();  
       break;  
      } else {  
       clientData = "";       //  First, clear the clientData  
      }  
     } else if (c != '\r') {      // Or if the character is NOT new line  
      clientData += c;        //  store the character to the clientData variable  
     }  
    }  
   }   
   http = "";  
   client.stop();      // Disconnect the client.  
   Serial.println("Client disconnected.");  
   Serial.println("");  
  }  
 }  
   
 void sendResponse() {  
  // Send the HTTP response headers  
  client.println("HTTP/1.1 200 OK");  
  client.println("Content-type:text/html");  
  client.println("Connection: close");  
  client.println();   
 }  
   
 void updateWebpage() {    
  // Send the whole HTML  
  client.println("<!DOCTYPE html><html>");  
  client.println("<head>");  
  client.println("<title>LED Status</title>");  
  client.println("</head>");
  
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println("</style></head>");  
    
  // Web Page Heading  
  client.println("<body><h1>ESP Web Server!</h1>");  
   
  // Display buttons for Blue LED  
  client.println("<p>1. GPIO 2 is " + output2State + "</p>");    
  if (output2State == "off") {  
   client.println("<p><a href=\"/2/on\"><button>Turn ON</button></a></p>");  
  } else {  
   client.println("<p><a href=\"/2/off\"><button>Turn OFF</button></a></p>");  
  }   
   
  client.print("<hr>");  
    
  // Display buttons for Green LED  
  client.println("<p>2. GPIO 0 is " + output0State + "</p>");    
  if (output0State == "off") {  
   client.println("<p><a href=\"/0/on\"><button>Turn ON</button></a></p>");  
  } else {  
   client.println("<p><a href=\"/0/off\"><button>Turn OFF</button></a></p>");  
  }     
   
  client.println("</body></html>");  
  client.println();  
 }  
   
 void updateLED() {    
  if(http.indexOf("GET /2/on") >= 0) {  
   Serial.println("GPIO 2 LED on");  
   output2State = "on";  
   digitalWrite(output2, HIGH);  
  } else if (http.indexOf("GET /2/off") >= 0) {  
   Serial.println("GPIO 2 LED off");  
   output2State = "off";  
   digitalWrite(output2, LOW);  
  } else if (http.indexOf("GET /0/on") >= 0) {  
   Serial.println("GPIO 0 LED on");  
   output0State = "on";  
   digitalWrite(output0, HIGH);  
  } else if (http.indexOf("GET /0/off") >= 0) {  
   Serial.println("GPIO 0 off");  
   output0State = "off";  
   digitalWrite(output0, LOW);   
  }   
 }  
