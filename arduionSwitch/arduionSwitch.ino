/*

 功能：继电器由数字口驱动，吸合1S，释放1S。
 */

// Relay 连接到 3引脚
// 引脚定义:
int Relay = 3;

// 复位后初始化内容
void setup() {                
  // 初始化数字端口为输出模式
  pinMode(Relay, OUTPUT);     
}
// 使用独立模块开发板LY-51S连接详情：
// http://doflye.net/viewthread.php?tid=5294&extra=
// 主循环
void loop() {
  digitalWrite(Relay, HIGH);   // Relay引脚置高电平
  delay(1000);               // 延时1秒
  digitalWrite(Relay, LOW);    // Relay引脚变为低电平
  delay(1000);               // 延时1秒
}


