/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
#define LEFT_ENC_PIN_A 35  
#define LEFT_ENC_PIN_B 34  

#define RIGHT_ENC_PIN_A 36  
#define RIGHT_ENC_PIN_B 39 

void initEncoder();
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
