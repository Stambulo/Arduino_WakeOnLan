#include <SPI.h>
#include <UIPEthernet.h>


byte mac[] = { 0x00, 0x00, 0x0B, 0xAA, 0xBB, 0xCC };
String readString;

EthernetServer server(80);
EthernetClient clients[8];


void setup() {
  Ethernet.begin(mac);
  Serial.begin(9600);
  Serial.println("Start Ethernet");
  server.begin();
  Serial.print("Server Begin. IP - ");
  Serial.print(Ethernet.localIP());
  Serial.print("\n");
}


void loop() {
  EthernetClient newClient = server.accept();
  if (newClient) {
    for (byte i = 0; i < 8; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        break;
      }
    }
  }

  // check for incoming data from all clients
  for (byte i = 0; i < 8; i++) {
    while (clients[i] && clients[i].available() > 0) {
      char c = clients[i].read();
      if (readString.length() < 100) {
        readString += c;
      }
      if (c == 0x0D) {
        checkRequest(readString);
        sendResponse(clients[i]);
      }
    }
    readString = "";
  }

  // stop any clients which disconnect
  for (byte i = 0; i < 8; i++) {
    if (clients[i] && !clients[i].connected()) {
      clients[i].stop();
    }
  }
}

void checkRequest(String request) {
  if (readString.indexOf("?MAINPC") > -1) {
    Serial.println(">>> Main PC starts !");
  } else {
    if (readString.indexOf("?MINIPC") > -1) {
      Serial.println(">>> Mini PC starts !");
    }
  }
}

void sendResponse(EthernetClient client) {
  //display the webpage
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<HTML>");
  client.println("<HEAD>");
  client.println("<TITLE> ARDUINO ETHERNET</TITLE>");
  client.println("</HEAD>");
  client.println("<BODY>");
  client.println("<hr>");
  client.println("<H1 style=\"color:green;text-align:center\">ARDUINO WAKE ON LAN SERVER</H1>");
  client.println("<hr>");
  client.println("<br>");
  client.println("<H2 style=\"text-align:center\"><a href=\"/?MINIPC\"\">Turn On MiniPC</a><br></H2>");
  client.println("<H2 style=\"text-align:center\"><a href=\"/?MAINPC\"\">Turn On MainPC</a><br></H2>");
  client.println("<br>");
  client.println("</BODY>");
  client.println("</HTML>");
  client.stop();
}
