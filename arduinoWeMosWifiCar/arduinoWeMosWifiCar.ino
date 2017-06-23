#include<ESP8266WiFi.h>
#include<ESP8266WiFiMulti.h>

WiFiServer server(80);//初始化服务器端口80

const int N1 = D3;
const int N2 = D4;
const int N3 = D6;
const int N4 = D7;
const int PWM_ENA = D1;
const int PWM_ENB = D2;

void setup() {
  Serial.begin(9600);

  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);
  pinMode(PWM_ENA, OUTPUT);
  pinMode(PWM_ENB, OUTPUT);

  WiFi.mode(WIFI_AP);//作为热点
  WiFi.softAP("Hello_IOT", "12345678"); //热点名称和密码
  server.begin();//开启HTTP Server，默认IP192.168.4.1
  IPAddress HTTPS_ServerIP = WiFi.softAPIP();//获取Server IP
  Serial.print("服务器地址:");
  Serial.println(HTTPS_ServerIP);
  //使用ESP自带的LED，HIGH灯灭，可换其它管脚
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

  //检查是否有浏览器端的客户访问ESP8266

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("设备连接:");
  String request = client.readStringUntil('\r');
  Serial.println(request);
  String val;
  if (request.indexOf("/U") != -1) {
    val = "前进中...";
    Forword();
  } else if (request.indexOf("/D") != -1) {
    val = "后退中";
    Backword();
  } else if (request.indexOf("/L") != -1) {
    val = "左转中...";
    TurnLeft();
  } else if (request.indexOf("/R") != -1) {
    val = "右转中...";
    TurnRight();
  } else if (request.indexOf("/S") != -1) {
    val = "停车...";
    Stop();
  }
  //给客户端的反馈信息
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type:text/html; charset=UTF-8\r\n\r\n";
  s += "<!DOCTYPEHTML>\r\n<html>\r\n";
  s += "<a href='/U'>前进</a><br/><br/>";
  s += "<a href='/D'>后退</a><br/><br/>";
  s += "<a href='/L'>左转</a><br/><br/>";
  s += "<a href='/R'>右转</a><br/><br/>";
  s += "<a href='/S'>停车</a><br/><br/>";
  s += String(val) + "</html>\n";
  client.flush();
  client.print(s);
  delay(1);
  Serial.println("断开连接.");
}

void Forword()
{
  Serial.println("前进");
  digitalWrite(N1, LOW);
  digitalWrite(N2, HIGH);
  digitalWrite(N3, LOW);
  digitalWrite(N4, HIGH);
}
void Backword()
{
  Serial.println("后退");
  digitalWrite(N1, HIGH);
  digitalWrite(N2, LOW);
  digitalWrite(N3, HIGH);
  digitalWrite(N4, LOW);
}
void TurnLeft()
{
  Serial.println("左转");
  digitalWrite(N1, HIGH);
  digitalWrite(N2, LOW);
  digitalWrite(N3, LOW);
  digitalWrite(N4, HIGH);
}
void TurnRight()
{
  Serial.println("右转");
  digitalWrite(N1, LOW);
  digitalWrite(N2, HIGH);
  digitalWrite(N3, HIGH);
  digitalWrite(N4, LOW);
}
void Stop()
{
  Serial.println("停止");
  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);
  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
}
void PWM_PLUS()
{
  const int rate = 700;
  digitalWrite(PWM_ENA, HIGH);
  delayMicroseconds(rate);
  digitalWrite(PWM_ENA, LOW);
  delayMicroseconds(1000 - rate);

  digitalWrite(PWM_ENB, HIGH);
  delayMicroseconds(rate);
  digitalWrite(PWM_ENB, LOW);
  delayMicroseconds(1000 - rate);
}
