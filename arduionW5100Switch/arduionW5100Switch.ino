#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};//mac地址
IPAddress ip(192, 168, 2, 177);//ip地址
EthernetServer server(80);//访问端口
String fakename = "Nginx/1.8.0 (ATmega328p/Ubuntu 12.04 LTS)"; //装逼参数(伪装服务器）
String res = "", path = "" ,httpget = "",flag = "";
int SwitchPIN = 3;//继电器
EthernetClient client;

void setup() {
  Serial.begin(9600);
  //设置继电器
  pinMode(SwitchPIN, OUTPUT);   
  //启动eth
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("ip:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  client = server.available();
  if (client) {
    Serial.println("new client");
    delay(200);
    res = "";
    path = "";
    httpget = "";
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
              httpget = res.substring(res.indexOf('?') + 1, res.indexOf(" HTTP"));
              path = res.substring(res.indexOf("GET ") + 4, res.indexOf('?'));
            } else {
              path = res.substring(res.indexOf("GET ") + 4, res.indexOf(" HTTP"));
            }
            Serial.println(res);
            Serial.println("GET:" + httpget);
            Serial.println("path:" + path);

            if(path == "/"){
              client.println("HTTP/1.1 200 OK");
              client.println("Server: Nginx/1.8.0 (ATmega328p/Ubuntu 12.04 LTS)");//装逼参数
              client.println("Content-Type: text/html;charset='UTF-8'");
              client.println("Connection: close");  
              client.println();
              client.println("<!DOCTYPE HTML><html><head><title>智能家居控制</title></head><body><h1>200 Success</h1><p>欢迎访问智能家居控制系统</p></body></html>");
              break;
            }

            if(httpget){
              flag = httpget.substring(httpget.indexOf('flag=')+1,httpget.indexOf('flag=')+2);
              if(flag){
                controlSwitch(flag);//控制开关
              }
            }
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
    delay(10);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//浏览器返回json
void returnJson(String status,String msg){
    client.println("HTTP/1.1 200 OK");
    client.println("Server: Nginx/1.8.0 (ATmega328p/Ubuntu 12.04 LTS)");//装逼参数
    client.println("Content-Type: text/html;charset='UTF-8'");
    client.println("Connection: close");  
    client.println();
    client.println("{\"status\":"+status+",\"data\":\"\",\"msg\":\""+msg+"\"}");
}
//控制开关
void controlSwitch(String type){
  if(type == "1"){
    Serial.println("开");
    digitalWrite(SwitchPIN, LOW);    // Relay引脚变为低电平
    returnJson("1","开关开启");
  }else if(type == "0"){
    Serial.println("关");
    digitalWrite(SwitchPIN, HIGH);   // Relay引脚置高电平
    returnJson("0","开关关闭");
  }else if(type == "2"){
    Serial.println("重启");
    digitalWrite(SwitchPIN, HIGH);   // Relay引脚置高电平
    delay(200);
    digitalWrite(SwitchPIN, LOW);    // Relay引脚变为低电平
    returnJson("2","开关重启");
  }else{
    returnJson("-1","参数错误");
  }
}




