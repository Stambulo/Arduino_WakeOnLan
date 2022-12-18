#include <SPI.h>
#include <UIPEthernet.h>


byte mac[] = { 0x00, 0x00, 0x0B, 0xAA, 0xBB, 0xCC };
// IPAddress ip(192, 168, 1, 25);

EthernetServer server(80);
EthernetClient clients[8];


void setup() {
  // Ethernet.begin(mac, ip);
  Ethernet.begin(mac);
  Serial.begin(9600);
  Serial.println("Start Ethernet");
  server.begin();
  Serial.print("Server Begin. IP - ");
  Serial.print(Ethernet.localIP());
  Serial.print("\n");
}


void loop() {
  // check for any new client connecting, and say hello (before any incoming data)
  EthernetClient newClient = server.accept();
  if (newClient) {
    for (byte i = 0; i < 8; i++) {
      if (!clients[i]) {
        newClient.print("Hello, client number: ");
        newClient.println(i);
        // Once we "accept", the client is no longer tracked by EthernetServer
        // so we must store it into our list of clients
        clients[i] = newClient;
        break;
      }
    }
  }

  // check for incoming data from all clients
  for (byte i = 0; i < 8; i++) {
    while (clients[i] && clients[i].available() > 0) {
      // read incoming data from the client
      Serial.write(clients[i].read());
    }
  }

  // stop any clients which disconnect
  for (byte i = 0; i < 8; i++) {
    if (clients[i] && !clients[i].connected()) {
      clients[i].stop();
    }
  }
}
