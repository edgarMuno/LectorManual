void NivelBat(){
  int16_t adc0;

   adc0 = ads.readADC_SingleEnded(0);
   float Volt = (adc0 * 0.1875)/1000;
   int nivel=86.957*Volt-281.74;
   if(nivel>100){
    nivel=100;
   }else if(nivel<0){
    nivel=0;
   }
    myNex.writeNum("j0.val",nivel);
}
void extraer_carga(String salida1, String* separado, String sep){
 //String salida1=String(carga);
 bool eval=true;
 int k=0;
 
while (eval){
      int x1=(salida1.indexOf(sep));
      if (x1!=-1){
      String resto1=salida1.substring(x1+1,salida1.length());
      String temp1=salida1.substring(0,x1);
      salida1=resto1;
      separado[k]=temp1;
      k+=1;
      }
      else{
       if(salida1.length()>0){
        separado[k]=salida1;
        salida1="";
       }
        eval=false;
      }
      
    }
}

void stringtochar(String into, char* salchar){
  int longi=into.length();
  //salchar[longi];
 
  into.toCharArray(salchar,longi+1);
}
