// Stepper control and Boxs push control

#define DIR 22
#define STEP 23
#define LIM_SW 34
#define MPUSH_1 13
#define MPUSH_2 27

#define steps_per_rev 1000

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
}
void loop()
{
  digitalWrite(DIR, HIGH);
  Serial.println("Spinning Clockwise...");
  
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); 
  
  digitalWrite(DIR, LOW);
  Serial.println("Spinning Anti-Clockwise...");

  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
}
