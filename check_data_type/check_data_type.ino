null = 0;
unsigned long tim;
int a = null;
void setup() {
  tim = millis();
  Serial.begin(115200);
  //types(tim);
  Serial.print(a);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}

void types(String a){Serial.println("it's a String");}
void types(int a)   {Serial.println("it's an int");}
void types(char* a) {Serial.println("it's a char*");}
void types(float a) {Serial.println("it's a float");} 
