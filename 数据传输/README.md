## 本实验基于esp8266WiFi模块

### 针脚定义
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

SoftwareSerial espSerial(2, 3);
SoftwareSerial photo(2, 3);
char ssid[] = "322四大金刚";        
char pass[] = "uestc322";    

WiFiClient client;                 
MySQL_Connection conn((Client*)&espSerial);
MySQL_Cursor* cursor;    
char database[] = "THZ";                   
char table[] = "esptest"; 

void readAndRecordData(){
        if (espSerial.available() > 0){
                  String data = espSerial.readStringUntil('\n');
                  data.trim();
                  Serial.println("Received data: ");             
                  char buff[128];                                         
//                  char x[5];      
//                  char y[5];
//                  char z[5];        
//                  dtostrf(4399,3,1,x);
//                  dtostrf(4399,3,1,y);
//                  dtostrf(4399,3,1,z);
                  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
                  sprintf(buff, "INSERT INTO %s.%s (data) VALUES ('%s')", database, table, data.c_str());                       
                  cur_mem->execute(buff);        
                  Serial.println("ok,success");
                  delete cur_mem;  
        }     
}

//void get_photo()
//{ 
//  MySQL_Connection conn((Client *)&photo);
//  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
//  char* rowData;
//  cur_mem->execute("SELECT image_data FROM THZ.my_images WHERE id");
//  row_values *row = NULL;
//  row = cur_mem->get_next_row();
//  if (row.lengths(0) > 0) {
//    rowData = row.field(0);
//  }
//  delete cur_mem;
//
//}

void setup()
{
  espSerial.begin(9600);
  Serial.begin(115200);
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
  delay(5000);
//  String inputString = "";
//  if (Serial.available() > 0) {
//    inputString += char(Serial.read());
//  }
//  if (inputString == "woaini") {
//  get_photo;
//  }
}
