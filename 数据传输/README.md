## 本实验分为两部分，stm32通过Esp8266WiFi模块连接到MySQL数据库，以及服务器端成像数据的转移

### Esp8266针脚定义
 ![image](https://github.com/SingleLCH/2023THZ/blob/main/%E6%95%B0%E6%8D%AE%E4%BC%A0%E8%BE%93/esp8266%20define/esp8266.png)
 
 ---

### Esp8266部分代码
### 数据定义：先发数据，当数据struct结束时，发char 'b',结束数据传输以及校验功能
  
```c++
#include <ESP8266WiFi.h>            
#include <MySQL_Connection.h>    
#include <MySQL_Cursor.h>
#include <SoftwareSerial.h>

IPAddress server_addr(47,108,223,15);   
char user[] = "root";             
char password[] = "Wsad080874";

char receive_data;
char x=0;      
char y=0;
char z=0;   

char ssid[] = "Mysqlserver";        
char pass[] = "Wsad1234+";    

WiFiClient client;                 
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;    
char database[] = "THZ";                   
char table[] = "esptest"; 

void readAndRecordData(){
        if (Serial.available() > 0){
           char buff[128];
           receive_data = Serial.read();
           if(receive_data=='a'){                                              
           x = Serial.parseInt();  
           y = Serial.parseInt();
           z = Serial.parseInt();                             
           MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
           sprintf(buff, "INSERT INTO %s.%s (x,y,z) VALUES ('%d','%d','%d')", database, table, x, y, z);                       
           cur_mem->execute(buff);        
           Serial.println("ok,success");
           delete cur_mem; 
           } 
        }     
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
    
  readAndRecordData();        

}
```
## 数据传输部分代码
### 第二版
### 本次更新解决了只能读取8bits的问题

```c++
void readAndRecordData(){
  char input[30];
  int32_t x = 0, y = 0, z = 0;

  if (Serial.available()) {
    int count = Serial.readBytesUntil('b', input, sizeof(input) - 1);
    input[count] = 0;

    int numValues = sscanf(input, "%d %d %d", &x, &y, &z);
    if (numValues == 3) {
     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
     sprintf(buff, "INSERT INTO %s.%s (x,y,z) VALUES ('%d','%d','%d')", database, table, x, y, z);                       
     cur_mem->execute(buff);        
     Serial.println("ok,success");
     delete cur_mem;
    } else {
      Serial.println("Invalid input");
    }
  }     
}
```

## 数据传输代码
### 第三版
### 本次更新加入了mqtt协议，对接python脚本

```c++

void readAndRecordData(){
  char input[30];
  int32_t x = 0, y = 0, z = 0;

  if (Serial.available()) {
    int count = Serial.readBytesUntil('b', input, sizeof(input) - 1);
    input[count] = 0;

    int numValues = sscanf(input, "%d %d %d", &x, &y, &z);
    if (numValues == 3) {
     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
     char buff[128];
     sprintf(buff, "INSERT INTO %s.%s (x,y,z) VALUES ('%d','%d','%d')", database, table, x, y, z);                       
     cur_mem->execute(buff);        
     Serial.println("ok,success");
     delete cur_mem;
    }if (numValues != 3){
    mqtt_client.setServer(mqtt_server, 1883); 
    while (!mqtt_client.connected()) {
    if (mqtt_client.connect("ESP8266Client")) {
      Serial.println("connected to MQTT broker");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqtt_client.state());
      delay(2000);
    }
  }   
       mqtt_client.publish("pythonstart", "start"); 
       Serial.println("Start"); 
      }
    
  }     
}

```
### 临时
```c++
void readAndRecordData(){
  char input[30];
  int32_t x = 0, y = 0, z = 0;
  char c;
  c = Serial.read();   
    if ( c == 'b') {
    int count = Serial.readBytes(input, sizeof(input) - 1);
    input[count] = 0;

    // int numValues = sscanf(input, "%d %d %d", &x, &y, &z);
    
     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
     char buff[128];
     sprintf(buff, "INSERT INTO %s.%s (x,y,z) VALUES ('%d','%d','%d')", database, table, x, y, z);                       
     cur_mem->execute(buff);        
     Serial.println("ok,success");
     delete cur_mem;
    
    }    
    if (c == 'a' ){
    mqtt_client.setServer(mqtt_server, 1883); 
    while (!mqtt_client.connected()) {
    if (mqtt_client.connect("ESP8266Client")) {
      Serial.println("connected to MQTT broker");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqtt_client.state());
      delay(2000);
    }
  }   
       mqtt_client.publish("pythonstart", "start"); 
       Serial.println("Start"); 
      }
    
  
}
```

## THZ.py为服务器端代码
### 本此更新加入了基于mqtt协议的启动代码，从而实现了python成像脚本的自动运行
