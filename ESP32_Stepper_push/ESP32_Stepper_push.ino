// Stepper control and Boxs push control

#define DIR 22
#define STEP 23
#define LIM_SW 34
#define LIM_PRESSED 1 // Logic state when limit switch is activated

#define DEBUG

#define MPUSH_1 13
#define MPUSH_2 27
#define MCHAN   0
#define PUSH_DURATION 100

const uint32_t step_LUT[5] = {
  40,
  80,
  120,
  160,
  5000
};

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  pinMode(MPUSH_1, OUTPUT);
  pinMode(MPUSH_2, OUTPUT);
  ledcSetup(MCHAN, 10000, 8);
  ledcAttachPin(MPUSH_1, MCHAN);
#ifdef DEBUG
  Serial.println("Starting...");
#endif

}
unsigned long millis_push = 0;
void mpush_io(int8_t inout) {
  if (inout == 1) {
    ledcWrite(MCHAN, 100);
    digitalWrite(MPUSH_2, LOW);
  } else if (inout == -1) {
    ledcWrite(MCHAN, 0);
    digitalWrite(MPUSH_2, HIGH);
  } else {
    ledcWrite(MCHAN, 0);
    digitalWrite(MPUSH_2, LOW);
  }
}

uint8_t main_fsm = 0;
char input[10];
uint8_t input_idx = 0;
int box_number = 0;

uint8_t stepper_fsm = 0;
uint8_t stepper_start = 0;
uint8_t stepper_service = 0;
uint32_t step_cnt = 0;
void loop()
{

  switch (main_fsm) {
    case 0:// Wait for serial input
      if (Serial.available() > 0) {
        input[input_idx] = Serial.read();
        if (input[input_idx] == '\n') {
          main_fsm = 1;// Go parse command
          break;
        }
        input_idx++;
        if (input_idx > 10) {
          input_idx = 0;
          memset(input, 0, 10);
        }

      }
      break;// case 0

    case 1:// Command parser
      switch (input[0]) {
        case 'p':// Format -> "p n" where n is number 1 to 5
        case 'P':
          if (input_idx != 3) {
            input_idx = 0;
            break;
          }

          input_idx = 0;

          box_number = atoi((char *)(input + 2));
          if (box_number > 5)
            box_number = 5;
          stepper_start = 1;
          memset(input, 0, 10);
          break;

        case 'T':// State polling
        case 't':
          if (input_idx != 1) {
            input_idx = 0;
            break;
          }

          input_idx = 0;

          if (stepper_service == 1) {
            Serial.print("T");
          } else {
            Serial.print("F");
          }
          input_idx = 0;
          memset(input, 0, 10);
          break;
      }

      main_fsm = 0;
      break;// case 1
  }// main fsm

  switch (stepper_fsm) {
    case 0:// Idle State
      if (stepper_start == 1) {
        if (digitalRead(LIM_SW == LIM_PRESSED)) {// If already homed, re-do homing
          stepper_fsm = 2;// Back out a bit for more precise homing
#ifdef DEBUG
          Serial.println("Soft homing");
#endif
        } else { // If not. Do the whole homing process
          stepper_fsm = 1;// Start homing process
#ifdef DEBUG
          Serial.println("Full homing");
#endif
        }
        stepper_start = 0;
        stepper_service = 0;
      }
      break;

    case 1:// Approaching Home quick
      digitalWrite(DIR, LOW);// CCW
      if (digitalRead(LIM_SW == LIM_PRESSED)) {
        stepper_fsm = 2;// Back out a bit for more precise homing
#ifdef DEBUG
        Serial.println("Home detected");
#endif
      }
      digitalWrite(STEP, HIGH);
      delayMicroseconds(2000);
      digitalWrite(STEP, LOW);
      delayMicroseconds(2000);

      break;

    case 2:// Back out a bit for more precise homing
      digitalWrite(DIR, HIGH);// CW
      if (step_cnt == 100) {
        step_cnt = 0;
        stepper_fsm = 3;// Re-approach home but slower
#ifdef DEBUG
        Serial.println("Re-approaching");
#endif
      }
      digitalWrite(STEP, HIGH);
      delayMicroseconds(4000);
      digitalWrite(STEP, LOW);
      delayMicroseconds(4000);
      step_cnt++;

      break;

    case 3:// Re-approach home but slower
      digitalWrite(DIR, LOW);// CCW
      if (digitalRead(LIM_SW == LIM_PRESSED)) {
        stepper_fsm = 4;// Move the requested box.
#ifdef DEBUG
        Serial.println("Homed!");
#endif
      }
      digitalWrite(STEP, HIGH);
      delayMicroseconds(7000);
      digitalWrite(STEP, LOW);
      delayMicroseconds(7000);

      break;

    case 4:// Move the requested box.
      digitalWrite(DIR, HIGH);// CW
      digitalWrite(STEP, HIGH);
      delayMicroseconds(7000);
      digitalWrite(STEP, LOW);
      delayMicroseconds(7000);
      step_cnt++;
      if (step_cnt == step_LUT[box_number - 1]) {
        box_number = 0;
        step_cnt = 0;
        stepper_fsm = 5;// Push the box out.
#ifdef DEBUG
        Serial.println("Box stationed");
#endif
        millis_push = millis();
      }

      break;

    case 5:// DC motor push the box out.
      mpush_io(1);
      if ((millis() - millis_push) > 100) {
        millis_push = millis();
        mpush_io(0);
        stepper_fsm = 6;// Pull the arm back.
#ifdef DEBUG
        Serial.println("Box pushed");
#endif
      }

      break;

    case 6:// DC motr pull the arm back.
      mpush_io(-1);
      if ((millis() - millis_push) > 100) {
        millis_push = millis();
        mpush_io(0);
        stepper_fsm = 7;// Pull the arm back.
#ifdef DEBUG
        Serial.println("Arm retracted");
#endif
      }

      break;

    case 7:// Clean-up
#ifdef DEBUG
      Serial.println("Cleaned up");
#endif
      stepper_start = 0;
      stepper_service = 1;
      stepper_fsm = 0;

      break;
  }

}
