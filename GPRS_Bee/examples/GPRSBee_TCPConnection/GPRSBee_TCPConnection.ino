#include "GPRSInterface.h"
#include "SoftwareSerial.h"

char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n";
char buffer[512];
GPRSInterface gprsInterface(8,7,9,9600,"cmnet");
void setup(){
  Serial.begin(9600);
  // use DHCP
  gprsInterface.init();
  // attempt DHCP
  while(false == gprsInterface.connect()) {
      Serial.println("gprs connect error");
      delay(2000);
  }

  // successful DHCP
  Serial.print("IP Address is ");
  Serial.println(gprsInterface.getIPAddress());

  TCPSocketConnection sock;
  if(false == sock.connect("mbed.org", 80)) {
      Serial.println("connect error");
  }else{
      Serial.println("connect mbed.org success");
  }

  Serial.println("waiting to fetch...");
  sock.send_all(http_cmd, sizeof(http_cmd)-1);
  while (true) {
      int ret = sock.receive(buffer, sizeof(buffer)-1);
      if (ret <= 0){
          Serial.println("fetch over...");
          break; 
      }
      buffer[ret] = '\0';
      Serial.print("Recv: ");
      Serial.print(ret);
      Serial.print(" bytes: ");
      Serial.println(buffer);
  }
  sock.close();
  gprsInterface.disconnect();
}

void loop(){

}
