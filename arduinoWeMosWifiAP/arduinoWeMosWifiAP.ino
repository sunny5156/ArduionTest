#include<ESP8266WiFi.h>
#include<ESP8266WiFiMulti.h>

WiFiServer server(80);//初始化服务器端口80

void setup() {

  Serial.begin(9600);

  WiFi.mode(WIFI_AP);//作为热点
  WiFi.softAP("Hello_IOT", "12345678"); //热点名称和密码
  server.begin();//开启HTTP Server，默认IP192.168.4.1

  IPAddress HTTPS_ServerIP = WiFi.softAPIP();//获取Server IP
  Serial.print("1:");
  Serial.println(HTTPS_ServerIP);

  //使用ESP自带的LED，HIGH灯灭，可换其它管脚
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

  //检查是否有浏览器端的客户访问ESP8266

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("Somebody has connect:)");
  String request = client.readStringUntil('\r');
  Serial.println(request);
  String val;
  if (request.indexOf("/OFF") != -1) {
    val = "OFF";
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (request.indexOf("/ON") != -1) {
    val = "ON";
    digitalWrite(LED_BUILTIN, LOW);
  }
  //给客户端的反馈信息
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type:text/html; charset=UTF-8\r\n\r\n";
  s += "<!DOCTYPEHTML>\r\n<html>\r\n";
  s += "<a href='/U'>前进</a><br/>";
  s += "<a href='/D'>后退</a><br/>";
  s += "<a href='/L'>左转</a><br/>";
  s += "<a href='/R'>右转</a><br/>";
  s += String(val) + "</html>\n";
  client.flush();
  client.print(s);
  delay(1);
  Serial.println("Client disconected.");
}
