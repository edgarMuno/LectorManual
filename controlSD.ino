void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    return;
  }
  if(file.print(message)) {
    //Serial.println("File written");
  } else {
 //   Serial.println("Write failed");
  }
  file.close();
}

String readFile(fs::FS &fs, const char * path){

    String cadena="";
    File file = fs.open(path);
    if(!file){
      //  Serial.println("Failed to open file for reading");
        return cadena;
    }

   //Serial.print("Read from file: ");
    while(file.available()){
       char cara= file.read();
       cadena=cadena+cara;
    }
    file.close();
    return cadena;
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  //Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
   
    return;
  }
  if(file.print(message)) {
     myNex.writeNum("bt2.val",0);
  } else {
  }
  file.close();
}




bool guardar(String fileNa,String datos){
  File file = SD.open(fileNa);//fileNa="/data.txt", datos"/data.txt", "Reading ID, Date, Hour, Temperature \r\n"
  if(!file) {
    writeFile(SD,fileNa.c_str(),"Date, Hour, Muestras,valor \r\n");
  }
  
  appendFile(SD,fileNa.c_str(),datos.c_str());
  
  file.close();

}
