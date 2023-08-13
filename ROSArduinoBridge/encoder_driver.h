/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
#define LEFT_ENC_PIN_A 34  
#define LEFT_ENC_PIN_B 35  

#define RIGHT_ENC_PIN_A 36  
#define RIGHT_ENC_PIN_B 39 

void initEncoder();
long readEncoderL();
long readEncoderR(); 
void resetEncoder(int i);
void resetEncoders();
