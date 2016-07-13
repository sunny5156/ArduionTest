const int echopin=3; // echo接5端口
const int trigpin=2; // trig接4端口  左侧超声波测距器




void setup()
{
  Serial.begin(9600);  
  pinMode(echopin,INPUT); //设定echo为输入模式
  pinMode(trigpin,OUTPUT);//设定trig为输出模式


  
}

void loop()
{
  // *********************************** 左方测距******/
   digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
delayMicroseconds(10);
digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
float distanceL = pulseIn(echopin,HIGH);//接收高电平时间
  distanceL = distanceL/58.0;//计算距离
  Serial.print("distanceL : ");  //输出距离
  Serial.print(distanceL);
  Serial.println("cm");  //输出单位
  
delay(100);   //循环间隔100uS

}

