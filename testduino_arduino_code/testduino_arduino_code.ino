//Enable Serial Communication: Once connected, open serial monitor. Wait 2 seconds. 
#define digitalHIGH 950 
#define digitalLOW 25

int digitalStart=2; boolean verbosity=false, diagnosticsFlag=false;

void setup() {}

boolean check=true; int a;
void loop() {
  
    check=testAnalogPins();                       //Test Analog Read
    if (check && verbosity)
      Serial.println("Passed AnalogRead");
  
 
    if (check) 
      check=testDigitalWrite();          //Test Digital Write

    if (check && verbosity)
      Serial.println("Passed DigitalWrite");
      
     if (check)                                     //Test Digital Read HIGH
      check=testDigitalRead(HIGH); 
 
    if (check && verbosity)
      Serial.println("Passed DigitalRead(HIGH)");
    
     if (check)                                    //Test Digital Read LOW
      check=testDigitalRead(LOW);    
  
    if (check && verbosity)
     Serial.println("Passed DigitalRead(LOW)");
    
    if (check) 
      passedTest();
    else 
      failedTest();
      
    if (diagnosticsFlag)
      diagnose();
      
    diagnosticsFlag=true;
}

void diagnose() {
  String msg="Diagnostics Menu: \na - analog read \nd - digital LOW \nD - digital HIGH \nr - digital read \nt - rerun test";
  Serial.println(msg); 
  char r;
  r=Serial.read();
  while (r != 't') {
    switch(r) {
      case 'a':
        analogPrint();
        break;
      case 'd':
        initDigitalMode(OUTPUT);
        initDigitalState(LOW);
        Serial.println("Digital pins set LOW");
        break;
      case 'D':
        initDigitalMode(OUTPUT);
        initDigitalState(HIGH);
        Serial.println("Digital pins set HIGH");
        break;
      case 'r':
        digitalPrint();
        break;
    }
    r=Serial.read(); 
  }

}

void digitalPrint() {
  initDigitalMode(INPUT);
  for (int i=digitalStart; i<14; i++) {
    Serial.print(i); Serial.print(": "); Serial.println(digitalRead(i));
  }
}


boolean testDigitalRead(boolean state) {
  int d;
  initDigitalMode(OUTPUT);
  initDigitalState(state);
  for(int i=digitalStart; i<14; i++) {
    pinMode(i,INPUT);
    d=digitalRead(i);
    pinMode(i,OUTPUT);
    digitalWrite(i,state);
    
    if (verbosity) {
     Serial.print("Pin "); Serial.print(i); 
     if (d==HIGH)
       Serial.println(": HIGH");
     else
       Serial.println(": LOW");
    }
   
   if (d != state) {
     if (verbosity) {
       Serial.print("Error. Could not read a ");
       if (state==LOW)
         Serial.print("LOW");
       else
         Serial.print("HIGH");
       Serial.print(" value on pin "); Serial.println(i);
     }
     return false;
   }
  } 
  return true;
}



boolean testDigitalWrite() {
  int start;
  initDigitalMode(OUTPUT);
  initDigitalState(LOW);
  delay(100);
  
  a=analogRead(A0);
   if (analogRead(A0) > digitalLOW) {
     if (verbosity)
        Serial.println("Error. Some or all of the digital pins are not writing low");
     return false;
   }



  for (int i=digitalStart; i<14; i++) {  //check all pins can be set HIGH
    digitalWrite(i,HIGH);
    delay(1);
    a=analogRead(A0);
    if (verbosity)
      Serial.print("Pin "); Serial.print(i); Serial.print(": "); Serial.println(a);
    
    if (a < 30) {
      if (verbosity) {
        Serial.print("Error. Pin "); Serial.print(i); Serial.println(" is not writing HIGH");
      }
      return false;
    }
    digitalWrite(i,LOW);
  }
  return true;
}

boolean testAnalogPins() {
  int a;
  initDigitalMode(OUTPUT);
  initDigitalState(HIGH);
  
  if (verbosity)
    Serial.println("Analog Readings: ");
  
  for (int i=A0; i<A6; i++) {
    a=analogRead(i);
    if (verbosity) {
      Serial.print("Pin "); Serial.print(i+146,HEX); Serial.print(": "); Serial.println(a);
    }
    
    if (a < 900) {
      if (verbosity) {
        Serial.print("Error testing Analog Pins. Expected value is > "); Serial.println(String(digitalHIGH));
        return false;
      }
      return false;
    }
  }  
  return true;
}

void signalComPinDisconnect() {
  unsigned long startTime;
  startTime=millis();
  initDigitalMode(OUTPUT);
  while (millis()-startTime < 2000) {
    initDigitalState(HIGH);
    delay(50);
    initDigitalState(LOW);
    delay(50);
  }
}


void analogPrint() {
  for(int i=A0; i<A5; i++) {
    Serial.print(analogRead(i)); Serial.print(" ");
  } 
  Serial.println(analogRead(A5));
}

void passedTest() {
  boolean infinite=true;
  if (verbosity)
    Serial.println("Arduino Passed Test");
  else 
    Serial.begin(9600);
  
  verbosity=true;
  digitalStart=2;
  
   initDigitalMode(OUTPUT);
  
   while(Serial.read() != 't'){
     initDigitalState(HIGH);
     delay(200);
     initDigitalState(LOW);
     delay(200);
 };
 
  Serial.println("\n");
}

void failedTest() {
  boolean next=false;
  if (verbosity)
    Serial.println("Arduino Failed Test"); 
  else
    Serial.begin(9600);
  
  verbosity=true;
  digitalStart=2;
  
  initDigitalMode(OUTPUT);
  initDigitalState(HIGH);
    
 while(Serial.read() != 't'){};
 Serial.println("\n");
}  

void initDigitalMode(boolean mode) {
    for (int i=digitalStart; i<14; i++) { 
    pinMode(i,mode);
  }
}

void initDigitalState(boolean state) {
    for (int i=digitalStart; i<14; i++) { 
      digitalWrite(i,state);
  }
}
