 #include <Keypad.h>

const byte n_rows = 4;
const byte n_cols = 4;

char keyCaptured[3]={'0','0','0'};

char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
String buf="";
byte colPins[n_rows] = {D3, D2, D1, D0};
byte rowPins[n_cols] = {D7, D6, D5, D4};

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 

void send_buf(String buff)
{
  Serial.print("Bufer enviado: "); 
  Serial.println(buff); 
  buff="";  
}

void setup(){
  Serial.begin(115200);
}

void loop()
{
    char myKey = myKeypad.getKey();
   
    if (myKey != NULL)
    {
      if (myKey == '#')
       {
        buf="";
       }
      else if (myKey != '*')
       {
         Serial.print("Key pressed: ");
         buf+=myKey;
         Serial.println(buf);
       }
      else if (myKey == '*')
       {
        send_buf(buf);
        buf="";
       }
     }
}


