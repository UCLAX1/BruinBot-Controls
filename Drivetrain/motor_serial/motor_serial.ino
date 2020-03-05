#define MOTOR_ENABLE 12
#define MOTOR1 10
#define MOTOR2 9
#define LED_PIN 13
bool LED = LOW;
int motor_state = 0;
int motor_speed = 100;
char received_byte = '0'; // 1 -> forward, 2 -> backward, 3 -> stop
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT); // enable pin, speed control
  pinMode(MOTOR1, OUTPUT);  // ch1 input
  pinMode(MOTOR2, OUTPUT);  // ch2 input
  digitalWrite(MOTOR_ENABLE, HIGH);
  brake();
  Serial.begin(9600);
}
void loop()
{
  if(Serial.available() > 0)
  {
    received_byte = Serial.read();
    if (received_byte == '1') 
    {
      go_forward(motor_speed);
      digitalWrite(LED_PIN, HIGH);
    }
    else if (received_byte == '2') 
    {
      go_backward(motor_speed);
      digitalWrite(LED_PIN, LOW);
    }
    else
    {
      brake();
    }
  }
}
void go_forward(int SPEED){
  analogWrite(MOTOR1, SPEED);
  digitalWrite(MOTOR2, LOW);
}
void go_backward(int SPEED){
  digitalWrite(MOTOR1, LOW);
  analogWrite(MOTOR2, SPEED);
}
void brake(){
  digitalWrite(MOTOR1, LOW);
  digitalWrite(MOTOR2, LOW);
}
/*
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, LED);
  LED = !LED;
  Serial.println(motor_speed);
  if (motor_state == 0){
    go_forward(motor_speed);
    motor_state = 1;
  }
  else if (motor_state == 1){
    go_backward(motor_speed);
    motor_state = 2;
  }
  else {
    brake();
    motor_state = 0;
  }
  delay(1000);
}
*/