
int cePin = 7;
int rwPin = 8;
int oePin = 2;

int a[2] = {3,4};
int d[3] = {10,11,12};

int tEW = 40;
int tACE = 55;
int tAA = 55;
int tWR = 0;

#define ASIZE(a) (sizeof(a)/sizeof(a[0]))

void set_address_pins(word addr)
{
  digitalWrite(cePin, HIGH);

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
  
  delayMicroseconds(55);
  digitalWrite(cePin, LOW);
  delayMicroseconds(55);
  Serial.println("");
}

void mem_write(byte val, word addr)
{
  set_address_pins(addr);
  //set data pins
  digitalWrite(rwPin, LOW);
  digitalWrite(oePin, HIGH);
  delayMicroseconds(55);

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
  delayMicroseconds(55); //do we need this?

  digitalWrite(rwPin, HIGH);  
  delayMicroseconds(55);
  digitalWrite(cePin, HIGH);
}

byte mem_read(word addr)
{
  set_address_pins(addr);

  digitalWrite(oePin, LOW);  //max 25u
  digitalWrite(rwPin, HIGH);
  delayMicroseconds(55); //do we need this?
  
  byte data = 0x0;
  Serial.print("READ: ");

  for (int i = ASIZE(d) - 1; i >= 0; i--) {
    pinMode(d[i], INPUT);
    
    if (digitalRead(d[i]) != LOW) {
      Serial.print("1");
      data |= (0x1 << i);
    } else {
      Serial.print("0");
    }
  }
  
  Serial.println("");
  digitalWrite(oePin, HIGH);
  digitalWrite(cePin, HIGH);
  
  return data;
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
    mem_write(values[i], i);
    Serial.print("Writing: ");
    Serial.print(values[i]);
    Serial.print(" to ");
    Serial.println(i);
    delay(1000);

    /*    
    if (mem_read(i) == values[i]) {
      Serial.println("YEAH!");
    } else {
      Serial.println
      ("BOO!");
    }
    */
  }

  for (int i = 3; i >=0; i--) {
    word val = mem_read(i);
    
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









