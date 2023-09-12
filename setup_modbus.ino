void setup_modbus() {

   pinMode(MAX485_RE_NEG, OUTPUT);
   digitalWrite(MAX485_RE_NEG, 0);
   
  node.begin(1, Serial);
  node.preTransmission(preTransmission);         
  node.postTransmission(postTransmission);

  uint8_t result;

result = node.writeSingleRegister(6,10);

}
void preTransmission(){       
  digitalWrite(MAX485_RE_NEG, 1);             
}
void postTransmission(){
  digitalWrite(MAX485_RE_NEG, 0);
}