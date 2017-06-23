char byte1;  
String str1 = "";  
  
void setup()  
{  
  Serial.begin(9600);  
  pinMode(13, OUTPUT);   
  
}  
void loop()  
{  
  while (Serial.available() > 0)  
  {  
      str1 += char(Serial.read());  
  }  
  if (str1.length() > 4)  
  {  
    Serial.println(str1);  
    str1 = "";  
  }  
  Serial.println("Good night W5100");  
  digitalWrite(13, HIGH);   // set the LED on  
  delay(500);              // wait for a second  
  digitalWrite(13, LOW);    // set the LED off  
  delay(500);              // wait for a second  
}  
