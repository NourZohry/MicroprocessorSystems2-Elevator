//The motor is very fast so you can add delay( ) below to see it for debugging purposes
//Search the page for "UNCOMMENT" and you will find the delay functions
//They are in lines 209 and 225 Ty :)

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 2; //two columns
char keys[ROWS][COLS] = {
  {'1','2'},
  {'3','4'},
  {'5','6'},
  {'7','0'}
};
byte rowPins[ROWS] = {7,8,9,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int pressed_floors[] = {-1,-1,-1,-1,-1,-1,-1,-1}; 
int pressed_count;
int floor_count = 0;
int efloor=0;
int count=0;
bool done = 0;
bool stop = 0;
int switchfloor = -1;
unsigned long previousMillis=0; //for 200ms
unsigned long previousMillisElevator=-1000; //for 1000ms = 1 second waiting
bool pressed = 0;
bool direction = 1;

void setup()
{
//Serial.begin(9600);  ////////////////////REMOVE
	
for (int i = 4; i < 6; i++)
	pinMode(i,OUTPUT); 
	
pinMode(A5,INPUT);	
pinMode(0,OUTPUT);
pinMode(1,OUTPUT);
pinMode(3,OUTPUT);
pinMode(13,OUTPUT);

pinMode(2,OUTPUT);

attachInterrupt(digitalPinToInterrupt(2), WeightToggle, CHANGE); 

}


void loop()
{

	//Serial.println(analogRead(A4)); //717 for motor to stop
	if (analogRead(A4) >= 717)
		digitalWrite(2,HIGH);
	else
		digitalWrite(2,LOW);
	
	char key;
	
	if (!stop)
	{
	
		if (!key)//(!done)
		{
		key = keypad.getKey();
		pressed_floors[key-48] = 1;
		}
		
		
		if (direction == 1)
		{
			//for (int i = 0; i<8; i++)
			for (int i = 0; i<8; i++)
			{
				if(pressed_floors[i] == 1)
				{
				pressed_count = i;
				break;
				}
			}
		}
		else if (direction == 0)
		{
			//for (int i = 7; i>=0; i--) 
			for (int i = 7; i>=0; i--) 
			{
				if(pressed_floors[i] == 1)
				{
				pressed_count = i;
				break;
				}
			}
		}
		
		
		
		if (pressed_floors[pressed_count])
		{
			unsigned long currentMillisElevator = millis();
			if (currentMillisElevator - previousMillisElevator >= 1000) {
				if (pressed_count > efloor)
				{
					IncreaseFloor();
					direction = 1;
				}
				else if (pressed_count < efloor)
				{
					DecreaseFloor();
					direction = 0;
				}
				else if (pressed_count == efloor)
				{
					done=0;
					pressed_floors[pressed_count] = -1;
		
					previousMillisElevator=millis();
				}
			}
		}
	}
	
	
	
	if (analogRead(A5) != 0 && pressed == 0)
	{
		pressed = 1;
		previousMillis=millis();
	}
	else if (analogRead(A5) == 0)
	{
		pressed=0;
	}
	if (pressed == 1)
	{
		
		unsigned long currentMillis = millis();
	
		if ((unsigned long)(currentMillis-previousMillis >= 200)) 
		{
			previousMillis = currentMillis;
	
			switch (analogRead(A5))
			{
			case 1021: switchfloor=0;break;
			case 929: switchfloor=1;break;
			case 851: switchfloor=2;break;
			case 786: switchfloor=3;break;
			case 730: switchfloor=4;break;
			case 681: switchfloor=5;break;
			case 638: switchfloor=6;break;
			case 601: switchfloor=7;break;
			//default:switchfloor=anynumber; break;
			}
			pressed_floors[switchfloor]=1;
		}
		
	}
		
	
	Print7Segment(efloor);
	
}


void MoveForward(int motor1,int motor2,int motor3,int motor4) //Movement
{
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, LOW);
        digitalWrite(motor3, LOW);
        digitalWrite(motor4, HIGH);
//millis(speed);
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor3, LOW);
        digitalWrite(motor4, LOW);
//millis(speed);
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor3, HIGH);
        digitalWrite(motor4, LOW);
//millis(speed);
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, LOW);
        digitalWrite(motor3, HIGH);
        digitalWrite(motor4, HIGH);
//millis(speed);
}

void Print7Segment(int efloor)
{
digitalWrite(4,efloor%2);
digitalWrite(5,(efloor/2)%2);
digitalWrite(6,(efloor/4)%2);
}

void WeightToggle()
{
stop = !stop;
}

void IncreaseFloor()
{
	done=1;
	if (count!= 32 )
	{
		MoveForward(13,3,1,0);
		count=count+4;
		//delay(50); ////////////////////UNCOMMENT TO DEBUG
	}
	else if (count == 32 )
	{
		count = 0;
		efloor=efloor+1;
	}
}

void DecreaseFloor()
{
	done=1;
	if (count!= 32 )
	{
		MoveForward(0,1,3,13);
		count=count+4;
		//delay(50); ////////////////////UNCOMMENT TO DEBUG
	}
		else if (count == 32 )
	{
		count = 0;
		efloor=efloor-1;
	}
}