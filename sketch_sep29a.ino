#include <SoftwareSerial.h>
// defining communicationion pins for Software Serial
# define GSM_RX 10 // Connect TX of GSM module
# define GSM_TX 11 // Connect RX of GSM module


// Defining interfacing pins for Motor Control (ie pins to L293D)
// Code is designed such that the two motor terminals are named A & B. and is assumed that the motor will rotate clock wise when A terminal is HIGH and B is LOW
# define LEFT_A 5  
# define LEFT_B 7
# define RIGHT_A 2
# define RIGHT_B 4
# define enA 3 //pwm pins to control speed of the motor
#define enB 6
int a;
// Defining the key press to drive the car.
# define INC '3'
# define DEC '1'
# define FORWARD '2'
# define RIGHT '6'
# define LEFT '4'
# define BACKWARD '8'
# define STOP '5'
# define SLOW1 '*' //slowest speed
# define SLOW2 '0' //slower speed
# define SLOW3 '#' //fastest speed
int sensorpin = 0; //collecting data from distance sensor
            
int val = 0;int Front_D = 0;

SoftwareSerial gsm_board(GSM_RX,GSM_TX);

boolean call_flag=0;
int i=0,x=-1;
char n;
void gsm_initilaize();// used to inilitize the gsm and chk all its parameters
void move(); // used to control motor outputs.
void forward(); //functions to set pins to control car movement
void backward();
void left();
void right();
void stop();

void setup() {
  // put your setup code here, to run once:
  gsm_board.begin(9600);
  Serial.begin(9600); //setting the baud rate in the serial monitor
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(LEFT_A,OUTPUT);
  pinMode(LEFT_B,OUTPUT);
  pinMode(RIGHT_A,OUTPUT);
  pinMode(RIGHT_B,OUTPUT);
  stop();
  gsm_initilaize();

}
//////////////////////////////setup ends/////////////////////////////

/////////////////////loop begins///////////////////////////
void loop() {
 
  move();


  String gsm_data; // to hold incomming communication from GSM module
   
  while(gsm_board.available())
  {
    char c=gsm_board.read();
    gsm_data+=c;
    delay(10);
  }  //read serial data and store it to gsm_data STRING instance;
  
  if(!call_flag) // if call is not in connected, checking for ring
  {
    x=gsm_data.indexOf("RING");
    if(x>-1)
    {
      delay(5000); // change this delay value to change the delay in attending the call by your car.
      gsm_board.println("ATA");
      call_flag=1;
    }
  }  // ring test over, call flag high if sim rings
  if(call_flag) // if call is connected
  {
    x=gsm_data.indexOf("DTMF"); //checking dtmf and storing approprietly
    if(x>-1)
    {
      n=gsm_data[x+6];
      Serial.println(n);
      move(); // call motor control function to initiate the change demanded by controller
    }
    x=gsm_data.indexOf("NO CARRIER"); // Checking whether call is still connected or not
    if(x>-1)
    {
      gsm_board.println("ATH");
      call_flag=0;
    }
  }
  
}
//////////////////////////////loop ends/////////////////////////////

/////////////////////gsm inilitize begins///////////////////////////

void gsm_initilaize()
{
  boolean gsm_Ready=1;
  Serial.println("initializing GSM module");
  while(gsm_Ready>0)
  {
   gsm_board.println("AT");
   Serial.println("AT"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("OK")>0)
       gsm_Ready=0;
   }
   delay(2000);
  }
  Serial.println("AT READY");
  // GSM MODULE REPLIED 'OK' TO 'AT' INPUT, INDICAING THE MODULE IS OK AND FUNCTIONING
  
  boolean ntw_Ready=1;
  Serial.println("finding network");
  while(ntw_Ready>0)
  {
   gsm_board.println("AT+CPIN?");
   Serial.println("AT+CPIN?"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("+CPIN: READY")>0)
       ntw_Ready=0;
   }
   delay(2000);
  }
  Serial.println("NTW READY");

// GSM MODULE REPLIED '+CPIN:READY' TO 'AT+CPIN?' INPUT, INDICAING THE NETWORK IS OK AND FUNCTIONING
  
  boolean DTMF_Ready=1;
  Serial.println("turning DTMF ON");
  while(DTMF_Ready>0)
  {
   gsm_board.println("AT+DDET=1");
   Serial.println("AT+DDET=1"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("OK")>0)
       DTMF_Ready=0;
   }
   delay(2000);
  }
  Serial.println("DTMF READY");// GSM MODULE REPLIED '+OK' TO 'AT+DDET=1?' INPUT, INDICAING THE DTMF IS ON;
}

//////////////////////////////gsm inilitization ends/////////////////////////////

/////////////////////move begins///////////////////////////

void move()
{
  switch(n)
  {
   case FORWARD:
    forward();
    break;
   case BACKWARD:
    backward();
    break;
   case RIGHT:
    right();
    break;
   case LEFT:
    left();
    break;
   case STOP:
    stop();
    break;
  case SLOW1:
  slow1();
  break;
  case SLOW2:
  slow2();
  break;
  case SLOW3:
  slow3();
  break;
  case INC:
  inc();
  break;
  case DEC:
  dec();
   }
}
void inc()
{
  if(a<250)
  {a=a+10;
analogWrite(enB, a);analogWrite(enA, a);
  }
}
void dec()
{
  if(a>100)
 {
  a=a-10;
  analogWrite(enB, a);analogWrite(enA, a);
  }
 }
/////////////////////move ends///////////////////////////

/////////////////movement control function begins///////////////////
void slow1()
{
 analogWrite(enB, 100);analogWrite(enA, 100);//setting up speed

a=100;
}
void slow2()
{
 analogWrite(enB, 170);analogWrite(enA, 170);//setting up speed
a=170;
}
void slow3()
{
 analogWrite(enB, 250);analogWrite(enA, 250);//setting up speed

a=250;}
void forward()//making a robot car move forward
{
  digitalWrite(LEFT_A,LOW);
  digitalWrite(LEFT_B,HIGH);
  digitalWrite(RIGHT_A,HIGH);
  digitalWrite(RIGHT_B,LOW);
}

void backward()//making a robot car moce backward
{
  digitalWrite(LEFT_A,HIGH);
  digitalWrite(LEFT_B,LOW);
  digitalWrite(RIGHT_A,LOW);
  digitalWrite(RIGHT_B,HIGH);
}

void left()//making my robot car rotate left direction
{
  digitalWrite(LEFT_A,LOW);
  digitalWrite(LEFT_B,HIGH);
  digitalWrite(RIGHT_A,LOW);
  digitalWrite(RIGHT_B,HIGH);
}
void right()//making my robot car rotate in right direction
{
  digitalWrite(LEFT_A,HIGH);
  digitalWrite(LEFT_B,LOW);
  digitalWrite(RIGHT_A,HIGH);
  digitalWrite(RIGHT_B,LOW);
}
void stop()//to stop the robot from moving
{
  digitalWrite(LEFT_A,LOW);
  digitalWrite(LEFT_B,LOW);
  digitalWrite(RIGHT_A,LOW);
  digitalWrite(RIGHT_B,LOW);
}
