int pic =1;
byte Semaforo=0b00000000;
unsigned long previousMillis = 0;
const long interval = 2000;

// set up the 'digital' feed
AdafruitIO_Feed * RED = io.feed("RED");
AdafruitIO_Feed * GREEN = io.feed("GREEN");
AdafruitIO_Feed * RTC = io.feed("RTC");


void setup() {
  
  // start the serial connection
  Serial.begin(9600);
  io.connect();

  RED ->onMessage(handleMessage);
  GREEN ->onMessage(handleMessage2);
  RTC ->save(pic);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }
  RED->get();
  GREEN->get();
  RTC->get();
}

void loop() {
  io.run();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    RTC->save(pic);
  }
  return;

  if (Serial.available()>0)
  {
    hourpic=Serial.read();
    Serial.write(Semaforo);
  }
}


void handleMessage(AdafruitIO_Data *data) {
  bitWrite(Semaforo,0,data->toPinLevel());
}
void handleMessage2(AdafruitIO_Data *data) {
  bitWrite(Semaforo,1,data->toPinLevel());
}
