const char* ssid = "Home";//type your ssid
const char* password = "ZibgolimalekIangh14$%**&95";//type your password

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "Amineh_copy.h" //Our HTML webpage contents


//ESP8266WebServer server(80); //Server on port 80
//WiFiClient client;//client 
//
////===============================================================
//// This routine is executed when you open its IP in browser
////===============================================================
//void handleRoot() {
// server.send(200, "text/html", index_html);
//}
//
////==============================================================
////                  SETUP
////==============================================================
//void setup(void){
//  Serial.begin(9600);
//  
//  WiFi.begin(ssid, password);     //Connect to your WiFi router
//  Serial.println("");
//
//  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  //If connection successful show IP address in serial monitor
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
// 
//  server.on("/", handleRoot);      //Which routine to handle at root location
//
//  server.begin();                  //Start server
//  Serial.println("HTTP server started");
//}
////==============================================================
////                     LOOP
////==============================================================
//void loop(void){
//  server.handleClient();          //Handle client requests
//
//  while (client.connected() || client.available())
//  {
//    if (client.available())
//    {
//      String line = client.readStringUntil('\n');
//      Serial.println(line);
//    }
//  }
//   
//  
//}

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());

}


// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 200\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            index_html + 
            "\r\n";
  return htmlPage;
}


void loop()
{
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
        else{
          Serial.print(line);
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
//    client.stop();
//    Serial.println("[Client disonnected]");
  }
}
