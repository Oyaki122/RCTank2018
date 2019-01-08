int UD, LR, reset, shot, IRreceive, Speed;
int LIFE = 3;
int count = 5;

void setup() {
  Serial.begin(9600);

  pinMode(A0, INPUT);   //コントローラー上下
  pinMode(A1, INPUT);   //コントローラー左右
  pinMode(11, INPUT);   //発射入力
  pinMode(17, OUTPUT);  //ライフ1
  pinMode(18, OUTPUT);  //ライフ2
  pinMode(19, OUTPUT);  //ライフ3
  pinMode(4, OUTPUT);   //音
  pinMode(2, OUTPUT);   //バイブレーション

  pinMode(7, INPUT_PULLUP);  //赤外線受信
  pinMode(3, OUTPUT);         //赤外線送信

  pinMode(5, OUTPUT);  // IN1(左モーター)
  pinMode(6, OUTPUT);  // IN2

  pinMode(9, OUTPUT);   // IN1（右モーター〕
  pinMode(10, OUTPUT);  // IN2

  pinMode(12, INPUT);  //場所確認ブザー
}

void loop() {
  IRreceive = digitalRead(7);  //赤外線受信
  shot = digitalRead(11);  //発射入力
  Serial.print(IRreceive);
  Serial.print(":");
  Serial.print(LIFE);
  Serial.print(":");
  Serial.print(shot);
  Serial.print(":");
  Serial.print(UD);
  Serial.print(":");
  Serial.print(LR);
  Serial.print(":");
  Serial.println(Speed);
  
  switch (LIFE) {
    case 3:
      digitalWrite(17, LOW);
      digitalWrite(18, LOW);
      digitalWrite(19, LOW);
      break;
    case 2:
      digitalWrite(17, LOW);
      digitalWrite(18, LOW);
      digitalWrite(19, HIGH);
      break;
    case 1:
      digitalWrite(17, LOW);
      digitalWrite(18, HIGH);
      digitalWrite(19, HIGH);
      break;
  }

  switch (IRreceive) {
    case 0:
      count = count - 1;
      if (count == 0) {
        LIFE = LIFE - 1;
        digitalWrite(2, HIGH);
        tone(4,300,1000);
        count = 5;
        delay(1000);
        digitalWrite(2, LOW);
        break;

      case 1:
        if (LIFE >= 1) {
          count = 5;
          UD = (pulseIn(A0, HIGH, 2000) + 10) *  8/ 1000;
          LR = (pulseIn(A1, HIGH, 2000) + 10) *  8/ 1000;
          if (UD == 0) UD = (analogRead(A0) <= 100) ? 0 : 8;
          if (LR == 0) LR = (analogRead(A1) <= 100) ? 0 : 8;
          digitalWrite(2,LOW);

          if ((UD > 4) && (LR == 4)) {  //上
            analogWrite(5, 0);
            analogWrite(9, 0);
            Speed = abs(UD-4)*32+127;
            analogWrite(6, Speed);
            analogWrite(10, Speed);
          }

          else if ((UD < 4) && (LR == 4)) {  //下
            analogWrite(6, 0);
            analogWrite(10, 0);
            Speed = 255;
            analogWrite(5, Speed);
            analogWrite(9, Speed);
          }

          else if ((UD == 4) && (4 < LR)) {  //右
            digitalWrite(5, LOW);
            digitalWrite(10, LOW);
            digitalWrite(6, HIGH);
            digitalWrite(9, HIGH);
          }

          else if ((UD == 4) && (4 > LR)) {  //左
            digitalWrite(6, LOW);
            digitalWrite(9, LOW);
            digitalWrite(5, HIGH);
            digitalWrite(10, HIGH);
          }

          else if ((4 < UD) && (4 < LR)) {  //第一象限
            digitalWrite(5, LOW);
            digitalWrite(9, LOW);
            Speed=255-abs(LR-4)*40;
            analogWrite(6, 255);
            analogWrite(10, Speed);
          }

          else if ((4 < UD) && (LR < 4)) {  //第二象限
            digitalWrite(5, LOW);
            digitalWrite(9, LOW);
            Speed=255-abs(LR-4)*40;
            analogWrite(6, Speed);
            analogWrite(10, 255);
          }

          else if ((UD < 4) && (LR < 4)) {  //第三象限
            digitalWrite(6, LOW);
            digitalWrite(10, LOW);
            Speed=255-abs(LR-4)*40;
            analogWrite(5, Speed);
            analogWrite(9, 255);
          }

          else if ((UD < 4) && (4 < LR)) {  //第四象限
            digitalWrite(6, LOW);
            digitalWrite(10, LOW);
            Speed=255-abs(LR-4)*40;
            analogWrite(5, 255);
            analogWrite(9, Speed);
          }

          else {
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(9, HIGH);
            digitalWrite(10, HIGH);
            Speed = 0;
          }

          if (shot == LOW) { 
            analogWrite(6, 0);
            analogWrite(10, 0);
            analogWrite(5,255);
            analogWrite(9, 255);
            tone(3, 38000, 500);
            digitalWrite(17, HIGH);
            digitalWrite(18, HIGH);
            digitalWrite(19, HIGH);
            digitalWrite(2, HIGH);
            delay(150);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(9, HIGH);
            digitalWrite(10, HIGH);
            delay(850);  
            digitalWrite(17, LOW);
            digitalWrite(18, HIGH);
            digitalWrite(19, HIGH);
            digitalWrite(2, HIGH);
            tone(4,1200,700);
            delay(1000);     
            digitalWrite(17, LOW);
            digitalWrite(18, LOW);
            digitalWrite(19, HIGH);
            digitalWrite(2, HIGH);
            tone(4,1200,700);
            delay(1000); 
            digitalWrite(17, LOW);
            digitalWrite(18, LOW);
            digitalWrite(19, LOW);
            digitalWrite(2, HIGH);
            tone(4,1800,500);
            delay(500);
          }

        }

        else if (LIFE <= 0) {
          analogWrite(5, 0);
          analogWrite(6, 0);
          analogWrite(9, 0);
          analogWrite(10, 0);
          Speed = 0;

          digitalWrite(17, HIGH);
          digitalWrite(18, HIGH);
          digitalWrite(19, HIGH);
          digitalWrite(2, HIGH);
          tone(4, 987);
          delay(750);
          digitalWrite(17, LOW);
          digitalWrite(18, LOW);
          digitalWrite(19, LOW);
          digitalWrite(2, LOW);
          tone(4, 783);
          delay(750);
        }
        break;
      }
  }
  if(digitalRead(12)==LOW){
    tone(4,1000,1400);
  }else{
    noTone(4);
  }
}
