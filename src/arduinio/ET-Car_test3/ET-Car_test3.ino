#include "BluetoothSerial.h"
#include <string.h>
BluetoothSerial SerialBT;

char names[] = "ET-Car_1";

int motor1_1 = D2;
int motor1_2 = D3;
int motor2_1 = D4;
int motor2_2 = D5;
int UD_state = 0; // 전진 후진 상태 // 1 = 전진 / 0 = 정지 / -1 = 후진
int LR_state = 0; // 방향 상태 // 1 = 왼쪽 / 2 = 오른쪽 / 0 = 직진 / -1 = 뒤로 왼쪽 / -2 = 뒤로 오른쪽

void ET_up();
void ET_down();
void ET_left();
void ET_down_left();
void ET_right();
void ET_down_right();
void ET_stop();

void setup() {
  pinMode(motor1_1, OUTPUT);
  pinMode(motor1_2, OUTPUT);
  pinMode(motor2_1, OUTPUT);
  pinMode(motor2_2, OUTPUT);

  ledcAttachPin(motor1_1, 0);  // 1-1번모터
  ledcSetup(0, 4000, 8);
  ledcAttachPin(motor1_2, 1);  // 1-2번모터
  ledcSetup(1, 4000, 8);
  ledcAttachPin(motor2_1, 2);  // 2-1번모터
  ledcSetup(2, 4000, 8);
  ledcAttachPin(motor2_2, 3);  // 2-2번모터
  ledcSetup(3, 4000, 8);
  
  Serial.begin(115200);
  SerialBT.begin(names);

  while(!SerialBT.connected(1000)) {
    Serial.println("ㄱㄷ");
  }
  delay(1000);                      

  String message = names + String("연결됨ㅇ");
  Serial.println(message);          
  SerialBT.println(message.c_str());
}

void loop() {
  String str="";
  char cmd;
  int first;
  int second;
  int third;
  int fourth;
  int str_len;

  if(SerialBT.available()){
    for(;;)
    {
      cmd = (char)SerialBT.read();
      if(cmd == '\\' )
      {
        break;
      }
      str += cmd;
    }
    Serial.print("입력값 = ");
    Serial.println(str);     
  } else {
    return;
  }
  
  first = str.indexOf("_");
  second = str.indexOf("_",first+1);
  third = str.indexOf("_", second+1);
  fourth = str.indexOf("_", third+1);
  str_len = str.length();
  
  String F_left_speed = str.substring(0,first);
  String F_right_speed = str.substring(first+1, second);
  String ET_direction = str.substring(second+1, third);
  String B_left_speed = str.substring(third+1, fourth);
  String B_right_speed = str.substring(fourth+1, str_len);

  Serial.print("F_left_speed : ");
  Serial.println(F_left_speed);
  Serial.print("F_right_speed : ");
  Serial.println(F_right_speed);
  Serial.print("ET_direction : ");
  Serial.println(ET_direction);
  Serial.print("B_left_speed : ");
  Serial.println(B_left_speed);
  Serial.print("B_right_speed :");
  Serial.println(B_right_speed);
  
  if (ET_direction== "u"){
    if (UD_state == -1 || LR_state == -1 || LR_state == -2){
      ET_stop(F_left_speed, F_right_speed);
      delay(300);
    }
    ET_up(F_left_speed, F_right_speed);
  }

  if (ET_direction== "d"){
    if(UD_state == 1 || LR_state == 1 || LR_state == 2){
      ET_stop(B_left_speed, B_right_speed);
      delay(300);
    }
    ET_down(B_left_speed, B_right_speed);
  }

  if (ET_direction== "r"){
    if(UD_state == 1 || UD_state == 0){
      ET_right(F_left_speed, F_right_speed);
    }
    if(UD_state == -1 ){
      ET_down_right(B_left_speed, B_right_speed);
    }
  }

  if (ET_direction== "l"){
    if(UD_state == 1 || UD_state == 0){
      ET_left(F_left_speed, F_right_speed);
    }
    if(UD_state == -1){
      ET_down_left(B_left_speed, B_right_speed);
    }
  }
  
  if (ET_direction== "s"){
    ET_stop(F_left_speed, F_right_speed);
  }
}

void ET_up(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,HIGH);  
  //digitalWrite(motor1_2,LOW);  
  //digitalWrite(motor2_1,HIGH);
  //digitalWrite(motor2_2,LOW);
  
  Serial.println("앞");
  
  ledcWrite(0, Left_speed.toInt());
  ledcWrite(1, 0);
  ledcWrite(2, Right_speed.toInt());
  ledcWrite(3, 0);
  
  UD_state = 1;
  LR_state = 0;
}

void ET_down(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,HIGH);
  //digitalWrite(motor1_2,HIGH);
  //digitalWrite(motor2_1,HIGH);
  //digitalWrite(motor2_2,HIGH);
  
  Serial.println("뒤");

  ledcWrite(0, Left_speed.toInt());
  ledcWrite(1, Left_speed.toInt());
  ledcWrite(2, Right_speed.toInt());
  ledcWrite(3, Right_speed.toInt());
  
  UD_state = -1;
  LR_state = 0;
}

void ET_left(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,LOW);  // 왼쪽 바퀴 멈춤
  //digitalWrite(motor1_2,LOW);  // 왼쪽 바퀴 멈춤
  //digitalWrite(motor2_1,HIGH);  // 오른쪽 바퀴 작동
  //digitalWrite(motor2_2,LOW);   // 오른쪽 바퀴 작동
  
  Serial.println("앞으로왼쪽");

  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, Right_speed.toInt());
  ledcWrite(3, 0);
  
  LR_state = 1;
  UD_state = 1;
}

void ET_down_left(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,LOW);
  //digitalWrite(motor1_2,LOW);
  //digitalWrite(motor2_1,HIGH);
  //digitalWrite(motor2_2,HIGH);
  
  Serial.println("뒤로왼쪽");

  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, Right_speed.toInt());
  ledcWrite(3, Right_speed.toInt());
  
  LR_state = -1;
  UD_state = -1;
}

void ET_right(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,HIGH);
  //digitalWrite(motor1_2,LOW);
  //digitalWrite(motor2_1,LOW);
  //digitalWrite(motor2_2,LOW);
  
  Serial.println("앞으로오른쪽");

  ledcWrite(0, Left_speed.toInt());
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  
  LR_state = 2;
  UD_state = 1;
}

void ET_down_right(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,HIGH);
  //digitalWrite(motor1_2,HIGH);
  //digitalWrite(motor2_1,LOW);
  //digitalWrite(motor2_2,LOW);
  
  Serial.println("뒤로오른쪽");

  ledcWrite(0, Left_speed.toInt());
  ledcWrite(1, Left_speed.toInt());
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  
  LR_state = -2;
  UD_state = -1;
}

void ET_stop(String Left_speed, String Right_speed){
  //digitalWrite(motor1_1,LOW);
  //digitalWrite(motor1_2,LOW);
  //digitalWrite(motor2_1,LOW);
  //digitalWrite(motor2_2,LOW);
  
  Serial.println("멈춤");

  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  
  LR_state = 0;
  UD_state = 0;
}
