#define PIN_LED 7
unsigned int count;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial){
    ; 
  }
}

void loop() {
  digitalWrite(PIN_LED, 0);
  delay(1000);
  while(1){
  digitalWrite(PIN_LED, 0);
  delay(100);
  digitalWrite(PIN_LED, 1);
  delay(100);
  count = count + 1;
  Serial.println(count);
  if (count == 5){
     digitalWrite(PIN_LED, 1);
      exit(0); 
  }
  }

}

