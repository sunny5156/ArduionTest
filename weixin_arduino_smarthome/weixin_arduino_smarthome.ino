#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>  
 
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient client;
char server[] = "wsd.cxiangnet.cn";
IPAddress ip(192,168,2,177);
String str = "";
dht DHT;
 
#define DHT11_PIN 2      		//定义DHT11传感器用的变量，DHT11输入输出引脚连接在Arduino UNO 模拟端口0
//byte dht11_dat[5];   			//存储从DHT11传感器采样的温湿度值的数组
char light='0';			//初始化开关状态
char c;
byte temprature = 28;		//初始化温度
byte humdity=32;			//初始化湿度

unsigned long lastConnectionTime = 0;          // 以毫秒为单位，最后一次连接服务器时间
boolean lastConnected = false;                 // 上一次连接状态
const unsigned long postingInterval = 3*1000;  // 两次连接的时间间隔

void setup(){
    	//DDRC |= _BV(DHT11_PIN);  		// 配置DHT11_PIN配置为输出
	//PORTC |= _BV(DHT11_PIN);  		// 第DHT11_PIN位置1，其它位不变
	//pinMode(5, OUTPUT);			//LED灯接入数字5口
	Serial.begin(9600);
	Ethernet.begin(mac,ip);			//如果为固定IP方式为Ethernet.begin(mac, ip)
}
  
void loop(void){ 

    if (!client.connected() && lastConnected) {	//断开连接
        Serial.println("disconnecting.");
        client.stop();
    }
 
    //if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {    //重新连接
	Serial.println("read data ");
        int chk = DHT.read11(DHT11_PIN); 
  	//DHT11();			        //调用 DHT11()和read_dht11_dat()子函数
 	humdity = DHT.humidity; 		//读温度传感器值
        Serial.println(humdity);
        
	temprature = DHT.temperature;
        Serial.println(temprature);		        //读湿度传感器值
	getData();			        //上传数据
	delay(10000);
    //}
    lastConnected = client.connected();
}

/********************获取命令回传数值子函数********************/
void getData(void) {
    if (client.connect(server,80)) {
        Serial.println("connecting websever...");
        str = "";
        str = str +"GET /upload.php?token=arduino&tempdata=";
        str = str +temprature;
        str = str+"&humddata=";
        str = str+humdity;
        str = str+" HTTP/1.1";
        Serial.println(str);
        client.println(str);
        client.println("Host: wsd.cxiangnet.cn");
        //client.print("GET /upload.php?token=arduino&tempdata=");// 发送HTTP GET 请求
        //Serial.println("GET /upload.php?token=arduino&tempdata=");
        //client.print(temprature);
        //Serial.println("GET /upload.php?token=arduino&tempdata="+temprature);
        //client.print("&humddata=");
        //client.print(humdity);
        //Serial.println("GET /upload.php?token=arduino&humddata="+humdity);
        //client.println(" HTTP/1.1");
        //client.println("Host: ws");
        client.println("Connection: close");
        client.println();
        lastConnectionTime = millis();
    }else {
        Serial.println("connection failed");
        Serial.println("disconnecting.");
        client.stop();
    }
}





