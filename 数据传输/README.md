## 本实验分为两部分，stm32通过esp8266WiFi模块连接到MySQL数据库，以及服务器端成像数据的转移

### esp8266针脚定义
 ![image](https://github.com/SingleLCH/2023THZ/blob/main/%E6%95%B0%E6%8D%AE%E4%BC%A0%E8%BE%93/esp8266%20define/esp8266.png)
 
 ---

### esp8266部分代码
```c++
#include <ESP8266WiFi.h>            
#include <MySQL_Connection.h>    
#include <MySQL_Cursor.h>
#include <SoftwareSerial.h>


#define id  1
IPAddress server_addr(47,108,223,15);   
char user[] = "root";             
char password[] = "Wsad080874";        

SoftwareSerial espSerial(7, 8);
//SoftwareSerial Serial(2, 3);
char ssid[] = "322四大金刚";        
char pass[] = "uestc322";    

WiFiClient client;                 
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;    
char database[] = "THZ";                   
char table[] = "esptest"; 

void readAndRecordData(){
           Serial.println("Received data: ");             
           char buff[128];                                         
           int x=0;      
           int y=0;
           int z=0;        
           x = Serial.parseInt();  
           y = Serial.parseInt();
           z = Serial.parseInt();
           MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
           sprintf(buff, "INSERT INTO %s.%s (x,y,z) VALUES ('%d','%d','%d')", database, table,x,y,z);                       
           cur_mem->execute(buff);        
           Serial.println("ok,success");
           delete cur_mem;       
}


void setup()
{
  Serial.begin(9600);
  while (!Serial);      
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pass);         
  while (WiFi.status() != WL_CONNECTED) {
           
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());     

  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user, password))       
    Serial.println("OK.");  
  else
    Serial.println("FAILED.");
  cursor = new MySQL_Cursor(&conn);  
}

void loop()
{
if (Serial.available() > 0){ 
  readAndRecordData();
  delay(100); 
}       
}
```

## THZ.py为服务器端代码
