// Motor A connections

int ENCA = 2; // YELLOW
int ENCB = 3; // WHITE
int pwm = 5; // BLACK
int in1 = 6; // RED
int in2 = 7; // WHITE wire connected to motor driver

volatile int posi = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;


void setup() {
	// Set input pins to read postion of motor
	pinMode(ENCA,INPUT);
  	pinMode(ENCB,INPUT);
	attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
	
	// Set all the motor control pins to outputs
	pinMode(pwm, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

void loop() {
	// motor power
	float pwr = 100;
	
	// motor direction
	int dir = 1;

	// signal the motor
	setMotor(dir,pwr,pwm,IN1,IN2);

}

// This function sets motor parameters
void setMotor(int dir, int pwmVal, int PWM, int in1, int in2){
	analogWrite(PWM,pwmVal);
	if(dir == 1){
	  digitalWrite(in1,HIGH);
	  digitalWrite(in2,LOW);
	}
	else if(dir == -1){
	  digitalWrite(in1,LOW);
	  digitalWrite(in2,HIGH);
	}
	else{
	  digitalWrite(in1,LOW);
	  digitalWrite(in2,LOW);
	}  
}

//This function reads current position of encoder
void readEncoder(){ 
	  int b = digitalRead(ENCB);
	  if(b > 0){
	    posi++;
	  }
	  else{
	    posi--;
	  }
}
