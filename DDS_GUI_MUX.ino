# include "AD9910.h"
# include <SPI.h>

# define cs 5
# define rst 16
# define update 17
# define sdio 23
# define sclk 18

# define CLOCKSPEED 1000000

#define MCS_LOCK_BYTES "MCS#"
char MCS_USART_buffer[72] = {0}; // string to hold incoming data
bool MCS_USART_flag = false;
uint8_t MCS_USART_index = 0;

AD9910 DDS(cs, rst, update, sdio, sclk);

//double freq   = 400000000.0;
double amp = 700.0;

long Single_Frequency = 0;
long Start_Frequency = 0;
long Stop_Frequency = 0;
long Step_Frequency = 0;

//int addressA = //digital pin;
//int addressB =  ;
//int addressC =  ;

//int A = 0; //store pin A address
//int B = 0;
//int C = 0;

void setup()
{
  //pinMode(25, OUTPUT);
  // Serial
  Serial.begin(115200);
  // SPI
  SPI.begin();
  // DDS
  DDS.begin();

  //pinMode(addressA, OUTPUT);
  //pinMode(addressB, OUTPUT);
  //pinMode(addressC, OUTPUT);

}

void loop() {
  // digitalWrite(25, HIGH);
  //Serial.println("Working..");
  selected_mode_and_frequency();

  // ----------------- Frequency Sweep ------------------------\\
  /* for (int i = 0;i<910;i+=10){
       DDS.set_freq(i,0);
       Serial.print("Setting Frequency: ");
       Serial.println(1000000*(i+1));
       delay(50);
    }
    for (int j=40;j>0;j--){
       DDS.set_freq(freq*j,0);
       Serial.print("Setting Frequency: ");
       Serial.println(1000000*(j+1));
       delay(50);
    }

    // ------------------- Amp Sweep --------------------------\\
    for (int i = 0;i<10;i++){
      DDS.set_Amp(amp*i);
      Serial.println(amp*i);
      delay(1000);
    }
    for (int j = 10; j>0;j--){
      DDS.set_Amp(amp*j);
      Serial.println(amp*j);
      delay(1000);
    }*/

  //DDS.set_freq(freq);
  //DDS.set_Amp(amp);
}

// ESP32 + GUI
// Format data yang dikirim agar bisa diproses
//
//  MCS#[Frequency_Mode]=[PARAMETER_A];[PARAMETER_B];[PARAMETER_C];
//  contoh: MCS#1=100;123;999;
//
// janga lupa kirim pakai newline \n

void selected_mode_and_frequency() {
  while (Serial.available()) {
    char first_place_data_in = Serial.read();
    MCS_USART_buffer[MCS_USART_index++] = first_place_data_in;
    if (first_place_data_in == '\n') {
      MCS_USART_buffer[MCS_USART_index] = '\0';
      MCS_USART_index = 0;
      //if(strstr(MCS_USART_buffer, MCS_LOCK_BYTES) != NULL
      MCS_USART_flag = true;
    }
  }

  if (MCS_USART_flag) {
    //CLEAN BUFFER
    //char *_token, *_buffer;
    //_buffer = _token = strtok(MCS_USART_buffer, MCS_LOCK_BYTES);
    //while(_token != NULL){
    //_buffer = _token;
    //_token  = strtok(NULL, MCS_LOCK_BYTES);
    //}
    // //for (; (_token = strtok(NULL, MCS_LOCK_BYTES)) != NULL; _buffer = _token);

    char* _buffer = strtok(MCS_USART_buffer, MCS_LOCK_BYTES);
    // char Frequency_Mode;
    //Buffer Process
    char *_item = strtok(_buffer, "=");
    uint8_t Frequency_Mode = atoi(_item);
    // char *_item = strtok(_buffer, "=");
    //uint8_t Arus_yang_dipilih = atoi(_item);
    _item = strtok (NULL, ";");
    uint32_t _param_a = atol(_item);
    _item = strtok (NULL, ";");
    uint32_t _param_b = atol(_item);
    _item = strtok (NULL, ";");
    uint32_t _param_c = atol(_item);


    switch (Frequency_Mode) {
        case 1:
        A = bitRead(_param_a, 0); //_param_a adalah alamat yg disesuaikan di GUI
        B = bitRead(_param_a, 1);
        C = bitRead(_param_a, 2);
        digitalWrite(addressA, A);
        digitalWrite(addressB, B);
        digitalWrite(addressC, C);
        digitalWrite(outputpin, HIGH);
        break;
      case 2:
        A = bitRead(_param_a, 0); //_param_a adalah alamat yg disesuaikan di GUI
        B = bitRead(_param_a, 1);
        C = bitRead(_param_a, 2);
        digitalWrite(addressA, A);
        digitalWrite(addressB, B);
        digitalWrite(addressC, C);
        digitalWrite(outputpin, HIGH);
        break;
      case 3:
        A = bitRead(_param_a, 0); //_param_a adalah alamat yg disesuaikan di GUI
        B = bitRead(_param_a, 1);
        C = bitRead(_param_a, 2);
        digitalWrite(addressA, A);
        digitalWrite(addressB, B);
        digitalWrite(addressC, C);
        digitalWrite(outputpin, HIGH);
        break;
      case 4:
        A = bitRead(_param_a, 0); //_param_a adalah alamat yg disesuaikan di GUI
        B = bitRead(_param_a, 1);
        C = bitRead(_param_a, 2);
        digitalWrite(addressA, A);
        digitalWrite(addressB, B);
        digitalWrite(addressC, C);
        digitalWrite(outputpin, HIGH);
        break;
      case 5:
        A = bitRead(_param_a, 0); //_param_a adalah alamat yg disesuaikan di GUI
        B = bitRead(_param_a, 1);
        C = bitRead(_param_a, 2);
        digitalWrite(addressA, A);
        digitalWrite(addressB, B);
        digitalWrite(addressC, C);
        digitalWrite(outputpin, HIGH);
        break;
      case 11: //Single Frequency
        Single_Frequency = _param_a;
        DDS.set_freq(Single_Frequency, 0);
        DDS.set_Amp(amp);
        Serial.println(_param_a);
        break;
      case 12: //Sweep Frequency
        Start_Frequency = _param_a;
        Stop_Frequency = _param_b;
        Stop_Frequency = _param_c;

        for (int i = Start_Frequency; i < Stop_Frequency; i++) {
          DDS.set_freq(i = i + Step_Frequency, 0);
          //Serial.print("Setting Frequency: ");
          Serial.println((i = i + Step_Frequency));
          //Serial.println(_param_a);
          //Serial.println(_param_b);
          //Serial.println(_param_c);
          delay(50);
        }
        for (int j = Stop_Frequency; j > Start_Frequency; j--) {
          DDS.set_freq(j = j - Step_Frequency, 0);
          //Serial.print("Setting Frequency: ");
          //Serial.println((j + Step_Frequency));
          Serial.println((j -= Step_Frequency));
          delay(50);
        }
        break;
    }
    MCS_USART_flag = false;
  }
}
