#define MOTOR_ENABLE 12
#define MOTOR1 10
#define MOTOR2 9
#define LED_PIN 13

bool LED = LOW;
int motor_state = 0;
int motor_speed = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT); // enable pin
  pinMode(MOTOR1, OUTPUT);  // H-bridge side 1 input
  pinMode(MOTOR2, OUTPUT);  // H-bridge side 2 input

  digitalWrite(MOTOR_ENABLE, HIGH);

  brake();

  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, LED);
  LED = !LED;

  motor_speed += 5;
  if (motor_speed > 255) {
    motor_speed = 50;
  }
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
