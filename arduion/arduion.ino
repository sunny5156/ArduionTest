#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};//mac地址
IPAddress ip(192, 168, 2, 177);//ip地址
EthernetServer server(80);//访问端口
String fakename="Nginx/1.8.0 (Ubuntu 12.04 LTS)";//装逼参数(伪装服务器）
 
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("ip:");
  Serial.println(Ethernet.localIP());
}
 
 
void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    String res="" ,query="",path="";
    int resend = 1;
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        //只读取一行
        if (c != '\n' && resend) {
          res += c;
        } else {
          resend = 0;
        }
 
        if (c == '\n' && currentLineIsBlank) {
          //判断GET头完整性
          if ((res.indexOf("GET ") != -1) && (res.indexOf(" HTTP") != -1)) {
            //判断是否存在get参数
            if (res.indexOf('?') != -1) {
              query = res.substring(res.indexOf('?') + 1, res.indexOf(" HTTP"));
              path = res.substring(res.indexOf("GET ") + 4, res.indexOf('?'));
            } else {
              path = res.substring(res.indexOf("GET ") + 4, res.indexOf(" HTTP"));
            }
             
            Serial.println(res);
            Serial.println("GET:"+query);
            Serial.println("path:"+path);
            client.println("HTTP/1.1 200 OK");
            client.println("Server: "+fakename);//装逼参数
            client.println("Content-Type: text/html");
            client.println("Connection: close");  
            client.println();
            client.println("<!DOCTYPE HTML><html><head></head><body><h1>200 Success</h1>GET:"+query+"<br />PATH:"+path+"<hr /><p>"+fakename+"</p></body></html>");
            break;
          } else {
            //GET头不完整 返回错误信息
            client.println("HTTP/1.1 403 Forbidden");
            client.println("Server: "+fakename);//装逼参数(伪装服务器）
            client.println("Content-Type: text/html");
            client.println("Connection: close");  
            client.println();
            client.println("<!DOCTYPE HTML><html><head></head><body><h1>403 Forbidden</h1><hr /><p>"+fakename+"</p></body></html>");
            break;
          }
        }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          }
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
       
    }
  }
