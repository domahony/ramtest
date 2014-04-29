int cePin = 7;
int rwPin = 8;
int oePin = 2;

int a[2] = {3,4};
int d[3] = {10,11,12};

//int tACE = 0;
//int tWR = 0;
//int One = 0;

//int tAS = 0;
//int tDW = 0;
//int tDH = 0;
//int tAOE = 0;

#define ASIZE(a) (sizeof(a)/sizeof(a[0]))

void set_address_pins(word addr)
{

  Serial.print("ADDR: ");
  for (int i = ASIZE(a) - 1; i >= 0; i--) {    
    if ((addr >> i) & 0x1) {
      digitalWrite(a[i], HIGH);
      Serial.print("1");
    } else {
      digitalWrite(a[i], LOW);
      Serial.print("0");
    }  
  } 
  Serial.println(""); 
}


void setup()
{
  Serial.begin(9600);
  pinMode(cePin, OUTPUT);
  pinMode(rwPin, OUTPUT);
  pinMode(oePin, OUTPUT);
  
  for (int i = 0; i < ASIZE(a); i++) {
    pinMode(a[i], OUTPUT);
  }
  
  for (int i = 0; i < ASIZE(d); i++) {
    pinMode(d[i], OUTPUT);
  }

  digitalWrite(cePin, HIGH);
  digitalWrite(rwPin, HIGH);
  digitalWrite(oePin, HIGH);

}

void loop()
{
  word values[4];
  for (int i = 0; i < 4; i++) {
    values[i] = random(0, 8);
    write_ce(values[i], i);
    Serial.print("Writing: ");
    Serial.print(values[i]);
    Serial.print(" to ");
    Serial.println(i);
    delay(1000);
  }

  read_setup();
  
  for (int i = 0; i < 4; i++) {
    word val = read1(i);
    
    if (val == values[i]) {
      Serial.print("Sucessfully read ");
      Serial.print(val);
      Serial.print(" from ");
      Serial.println(i);
    } else {
      Serial.print("Expected ");
      Serial.print(values[i]);
      Serial.print(" got ");
      Serial.print(val);
      Serial.print(" from ");
      Serial.println(i);    
    }
  }    
  
  for (int i = 0; i < 4; i++) {
    word val = read1(i);
    
    if (val == values[i]) {
      Serial.print("Sucessfully read ");
      Serial.print(val);
      Serial.print(" from ");
      Serial.println(i);
    } else {
      Serial.print("Expected ");
      Serial.print(values[i]);
      Serial.print(" got ");
      Serial.print(val);
      Serial.print(" from ");
      Serial.println(i);    
    }  
  }
  
  delay(5000);
}

void write_ce(byte val, word loc)
{
  //set ce HIGH
  digitalWrite(cePin, HIGH);
  //set rw to high
  digitalWrite(rwPin, HIGH);
  //set data pins to output

  for (int i = 0; i < ASIZE(d); i++) {
    pinMode(d[i], OUTPUT);
  }
  
  //write address pins
  set_address_pins(loc);
  //set rw to low
  digitalWrite(rwPin, LOW);
  //tAS
  //set ce low
  digitalWrite(cePin, LOW);
  //write data pins

  Serial.print("WRITE: ");
  for (int i = ASIZE(d) - 1; i >= 0; i--) {
    pinMode(d[i], OUTPUT);
    if ((val >> i) & 0x1) {
      digitalWrite(d[i], HIGH);
      Serial.print("1");
    } else {
      digitalWrite(d[i], LOW);
      Serial.print("0");
    }
  }
  Serial.println("");  
  //tDW
  //set ce high
  digitalWrite(cePin, HIGH);
  //set rw high
  digitalWrite(rwPin, LOW);
  //tDH
}

void read_setup()
{
  //set ce HIGH
  digitalWrite(cePin, HIGH);
  //set data pins to input
  for (int i = 0; i < ASIZE(d); i++) {
    pinMode(d[i], INPUT);
  }
  //set ce low
  digitalWrite(cePin, LOW);
  //tACE
  //set rw high
  digitalWrite(rwPin, HIGH);
  //set oe low
  digitalWrite(oePin, LOW);
  //tAOE
}

byte read1(word loc)
{
  //set address pins
  set_address_pins(loc);
  //read, read, read until address changes again
  //delay(1000); ZZZZ
  byte data = 0x0;
  Serial.print("READ: ");
  for (int i = ASIZE(d) - 1; i >= 0; i--) {    
    if (digitalRead(d[i]) != LOW) {
      Serial.print("1");
      data |= (0x1 << i);
    } else {
      Serial.print("0");
    }
  }
  
  Serial.println("");
  return data;
}
