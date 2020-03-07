#define STEP_PIN 3
#define DIR_PIN 4
#define LED_PIN 13
#define BAUD_RATE 9600

int stepper_speed = 600;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
char receivedMsg[numChars] = {0};
String command = "";
boolean newData = false;


void setup() {
    // put your setup code here, to run once:
    pinMode(LED_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT); 
    pinMode(DIR_PIN, OUTPUT);
    brake();
    Serial.begin(BAUD_RATE);
    while (!Serial) {}; // hangs until serial port is properly opened

    digitalWrite(LED_PIN, HIGH);
    while (! establish_connection()) {}
    digitalWrite(LED_PIN, LOW);
}

void loop()
{
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        command = String(receivedMsg);
        newData = false;
    }

    if (command == "forward") 
    {
        go_forward(stepper_speed);
    }
    else if (command == "backward") 
    {
        go_backward(stepper_speed);
    }
    else
    {
        brake();
    }
}

void go_forward(int SPEED){
    digitalWrite(DIR_PIN,HIGH); 
  for(int x = 0; x < 200; x++) {
    digitalWrite(STEP_PIN,HIGH); 
    delayMicroseconds(SPEED); 
    digitalWrite(STEP_PIN,LOW); 
    delayMicroseconds(SPEED); 
  }
}
void go_backward(int SPEED){
    digitalWrite(DIR_PIN,LOW); 
  for(int x = 0; x < 200; x++) {
    digitalWrite(STEP_PIN,HIGH);
    delayMicroseconds(SPEED);
    digitalWrite(STEP_PIN,LOW);
    delayMicroseconds(SPEED);
  }
}
void brake(){
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(DIR_PIN, LOW);
}

boolean establish_connection(){
    while (!(Serial.available() > 0)) {} // hangs until a msg is received
    char rcv = Serial.read();
    if (rcv == 'i') {
        Serial.println("stepper");
    }

    int start = millis();
    while (!(Serial.available() > 0)) {
        if (millis()-start > 1000) {
            return false;
        }
    }
    rcv = Serial.read();
    if (rcv == 'd') {
        // Pi has acknowledged this arduino
        return true;
    }
    else {
        return false;
    }
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(receivedMsg, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    stepper_speed = atoi(strtokIndx);     // convert this part to an integer
}
