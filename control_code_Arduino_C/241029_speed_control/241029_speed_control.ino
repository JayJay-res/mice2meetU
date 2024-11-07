#include <Servo.h>

Servo servo_tilt, servo_door;

int PIN_SERVO_TILT = 11;
int PIN_BNC_TILT = 3;
int PIN_OVERRIDE_TILT = 4;
int PIN_POT_TILT = A7;

int PIN_SERVO_DOOR = 12;
int PIN_BNC_DOOR = 3; // = 5
int PIN_OVERRIDE_DOOR = 4; // = 6
int PIN_POT_DOOR = A7;

int POS_TILT_LOW = 80, POS_TILT_HIGH = 140;
int POS_DOOR_OPEN = 140, POS_DOOR_CLOSE = 80; // ##### NOT CALIBRATED YET
int current_pos_tilt, current_pos_door;
int current_state_tilt, current_state_door;

// timer in ms // ##### UNUSED
int TIMER_TILT_NEXT_LOW = 2000, TIMER_TILT_NEXT_HIGH = 2000;

int bnc_tilt_in, override_tilt_in, pot_tilt_spd_raw, servo_tilt_spd;
int bnc_door_in, override_door_in, pot_door_spd_raw, servo_door_spd;




void setup() {
  pinMode(PIN_BNC_TILT, INPUT);
  pinMode(PIN_OVERRIDE_TILT, INPUT_PULLUP); // Pull-up
  pinMode(PIN_POT_TILT, INPUT);
  pinMode(PIN_BNC_DOOR, INPUT);
  pinMode(PIN_OVERRIDE_DOOR, INPUT_PULLUP); // Pull-up
  pinMode(PIN_POT_DOOR, INPUT);
  
  servo_tilt.attach(PIN_SERVO_TILT);
  servo_tilt.write(POS_TILT_HIGH);
  current_pos_tilt = POS_TILT_HIGH;
  tiltStateTest();
  
  servo_door.attach(PIN_SERVO_DOOR);
  servo_door.write(POS_DOOR_CLOSE);
  current_pos_door = POS_DOOR_CLOSE;
  doorStateTest();
  
  Serial.begin(9600);
}




void loop()
{
  bnc_tilt_in = digitalRead(PIN_BNC_TILT);
  override_tilt_in = !(digitalRead(PIN_OVERRIDE_TILT)); // '1' if override switch is ON.
  bnc_door_in = digitalRead(PIN_BNC_DOOR);
  override_door_in = !(digitalRead(PIN_OVERRIDE_DOOR)); // '1' if override switch is ON.
  
  tiltRoutine();
  doorRoutine();
}




void tiltRoutine()
{
  pot_tilt_spd_raw = analogRead(PIN_POT_TILT);
  servo_tilt_spd = map(pot_tilt_spd_raw, 0, 1023, 80, 10); // ##### Set variable for max and min speed
  
  if((override_tilt_in) && (!current_state_tilt)) // If override is ON, AND if current tilt state is HIGH // ################ override condition RE CHECK
  {
    tiltLow();
  }
  else if((override_tilt_in) && (current_state_tilt)) // If override is ON, AND if current tilt state is LOW // ################ override condition RE CHECK
  {
    tiltHigh();
  }
  else if((!override_tilt_in) && (bnc_tilt_in)) // If override is OFF, AND if BNC is ON
  {
    tiltLow();
    tiltStateTest();
  }
  else if((!override_tilt_in) && (!bnc_tilt_in)) // If override is OFF, AND if BNC is OFF
  {
    tiltHigh();
    tiltStateTest();
  }
}

void tiltHigh()
{
  if(current_pos_tilt < POS_TILT_HIGH)
  {
    current_pos_tilt++;
    servo_tilt.write(current_pos_tilt); // servo_tilt.write(++current_pos_tilt)
    delay(servo_tilt_spd);
  }
}

void tiltLow()
{
  if(current_pos_tilt > POS_TILT_LOW)
  {
    current_pos_tilt--;
    servo_tilt.write(current_pos_tilt); // servo_tilt.write(--current_pos_tilt)
    delay(servo_tilt_spd);
  }
}

void tiltStateTest()
{
  if(current_pos_tilt == POS_TILT_HIGH)
  {
    current_state_tilt = 0;
  }
  else if(current_pos_tilt == POS_TILT_LOW)
  {
    current_state_tilt = 1;
  }
}




void doorRoutine()
{
  pot_door_spd_raw = analogRead(PIN_POT_DOOR);
  servo_door_spd = map(pot_door_spd_raw, 0, 1023, 80, 10); // ##### Set variable for max and min speed
  
  if((override_door_in) && (!current_state_door)) // If override is ON, AND if current door state is CLOSED // ################ override condition RE CHECK
  {
    doorOpen();
  }
  else if((override_door_in) && (current_state_door)) // If override is ON, AND if current door state is OPEN // ################ override condition RE CHECK
  {
    doorClose();
  }
  else if((!override_door_in) && (bnc_door_in))
  {
    doorOpen();
    doorStateTest();
  }
  else if((!override_door_in) && (!bnc_door_in))
  {
    doorClose();
    doorStateTest();
  }
}

void doorOpen()
{
  if(current_pos_door < POS_DOOR_OPEN)
  {
    current_pos_door++;
    servo_door.write(current_pos_door); // servo_door.write(++current_pos_door)
    delay(servo_door_spd);
  }
}

void doorClose()
{
  if(current_pos_door > POS_DOOR_CLOSE)
  {
    current_pos_door--;
    servo_door.write(current_pos_door); // servo_door.write(--current_pos_door)
    delay(servo_door_spd);
  }
}

void doorStateTest()
{
  if(current_pos_door == POS_DOOR_CLOSE)
  {
    current_state_door = 0;
  }
  else if(current_pos_door == POS_DOOR_OPEN)
  {
    current_state_door = 1;
  }
}




// For debugging
void tempDebugD1(int pin_num)
{
  int debug_val_D1;
  debug_val_D1 = digitalRead(pin_num);
  Serial.print("Digital@");
  Serial.print(pin_num);
  Serial.print("@");
  Serial.print(debug_val_D1);
  Serial.println();
}

// For debugging
void tempDebugA1(int pin_num)
{
  int debug_val_A1;
  debug_val_A1 = analogRead(pin_num);
  Serial.print("Analog@");
  Serial.print(pin_num);
  Serial.print("@");
  Serial.print(debug_val_A1);
  Serial.println();
}
