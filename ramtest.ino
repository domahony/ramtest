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
 
   delayMicroseconds(1);
  Serial.println(""); 
}

void mem_write(byte val, word addr)
{

  digitalWrite(cePin, HIGH);
  for (int i = ASIZE(d) - 1; i >= 0; i--) {
    pinMode(d[i], OUTPUT);
    digitalWrite(d[i], LOW);
  }  
  digitalWrite(oePin, LOW);
  
  delayMicroseconds(1);
  digitalWrite(rwPin, HIGH);
  set_address_pins(addr);
  digitalWrite(rwPin, LOW);

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
  digitalWrite(rwPin, HIGH);
  delayMicroseconds(1);  //tWZ
  digitalWrite(rwPin, LOW);
  delayMicroseconds(1);  //tWZ

  digitalWrite(rwPin, HIGH);
  digitalWrite(oePin, HIGH);
}

void setup_read()
{
  digitalWrite(cePin, LOW);
  delayMicroseconds(1);
  digitalWrite(oePin, LOW);  //max 25u
  for (int i = ASIZE(d) - 1; i >= 0; i--) {
    pinMode(d[i], INPUT);
  }  
  delayMicroseconds(1);
  digitalWrite(rwPin, HIGH);
}

byte mem_read(word addr)
{
  set_address_pins(addr);
  
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
  digitalWrite(cePin, LOW);
  delayMicroseconds(1);
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

  setup_read();
  for (int i = 0; i < 4; i++) {
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
  
  for (int i = 0; i < 4; i++) {
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

  digitalWrite(cePin, HIGH);
  
  delay(5000);
}

void write_ce(byte value, word loc)
{
  //set ce HIGH
  //set rw to high
  //set data pins to output
  //write address pins
  //set rw to low
  //tAS
  //set ce low
  //write data pins
  //tDW
  //set ce high
  //set rw high
}

void read_setup()
{
  //set ce high
  //set data pins to input
  //set ce low
  //tACE
  //set rw high
  //set oe low
  //tAOE
}

byte read1(word loc)
{
  /set address pins
  //tAA
  //read, read, read until address changes again
}









