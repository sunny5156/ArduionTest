const int N1 = D3;
const int N2 = D4;
const int N3 = D6;
const int N4 = D7;
const int PWM_ENA = D1;
const int PWM_ENB = D2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("shhnihao");
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);
  pinMode(PWM_ENA, OUTPUT);

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
void loop() {
  // put your main code here, to run repeatedly:
  PWM_PLUS(); //调速函数，通过ENA输出PWM脉冲
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == 'U')
    {
      Forword();
    }
    else if (c == 'D')
    {
      Backword();
    }
    else if (c == 'L')
    {
      TurnLeft();
    }
    else if (c == 'R')
    {
      TurnRight();
    }
    else if (c == 'S')
    {
      Stop();
    }
  }
}
