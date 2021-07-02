#include <Arduino.h>
#include <U8g2lib.h>
#include <FlexCAN.h>



#define OLED_MOSI 5
#define OLED_CLK 7
#define OLED_DC 9
#define OLED_CS 11
#define OLED_RESET 10

int ALARM_TRIGGER_PIN = 14;
int SILENCE_BTN_LED_PIN = 23;
int SILENCE_BTN_PIN = 1;
int SILENCE_RELAY_PIN = 19;
int TEENSY_LED_PIN = 13;

bool alarm = false;
bool alarm_silenced = false;
int alarm_reason = 0;
char alarm_reasons_text[5][10] = {
  "All good.",
  "Unknown.",
  "Inverter",
  "Weak cell",
  "Too cold"
};


float cell_volts[3] = {1.0, 1.0, 1.0};
float temps[3] = {14.0, 16.90, 9.40};
char labels[3][5] = {"Min:", "Avg:", "Max:"};
char temp_labels[3][9] = {"Outside:", "Avg:", "Max:"};
float soc = 94.59;
float pack_volts = 36.0098;
float pack_current = -999.047998;


  
U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI display(U8G2_R2, OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);



void setup(void) {
    delay(500);
    Serial.begin(115200);
    Serial.write("Starting up!");

    // Configure alarm pins
    pinMode(SILENCE_BTN_LED_PIN, OUTPUT);
    pinMode(ALARM_TRIGGER_PIN, INPUT);
    pinMode(SILENCE_BTN_PIN, INPUT_PULLUP);
    pinMode(SILENCE_RELAY_PIN, OUTPUT);
    pinMode(TEENSY_LED_PIN, OUTPUT);
    digitalWrite(TEENSY_LED_PIN, HIGH);

    // Setup screen
    display.begin();


    //CANBUS setup
    Can0.begin(125000);  


      

}

void loop(void) {
    alarm_state();
    read_canbus();
    draw_status();
}

void draw_status() {
  display.clearBuffer();

  // Pack voltage
  display.setFont(u8g2_font_timB12_tr);
  char pack_volts_s[8];
  String pack_volts_str = String(pack_volts, 1);
  pack_volts_str.concat("V");
  pack_volts_str.toCharArray(pack_volts_s, 8);
  display.drawStr(0, 12, pack_volts_s);

  // Pack current (avg)
  char pack_current_s[9];
  String pack_current_str = String(pack_current, 1);
  pack_current_str.concat("A");
  pack_current_str.toCharArray(pack_current_s, 9);
  display.drawStr(70, 12, pack_current_s);

  display.drawHLine(0, 14, 128);
  display.drawVLine(43, 14, 64);
  display.drawVLine(86, 14, 64);


  // Draw min/avg/max labels
  display.setFont(u8g2_font_tinytim_tr);
  for(int i = 0; i < 3; i++){
    display.drawStr(0, 21 + i*16, labels[i]);
    display.drawStr(45, 21 + i*16, temp_labels[i]);
  }

  display.setFont(u8g2_font_timB08_tf);

  // Cell voltages
  char cell_volts_s[7];
  String cell_volts_str;
  for(int i = 0; i < 3; i++){
    cell_volts_str = String(cell_volts[i], 3);
    cell_volts_str.concat("V");
    cell_volts_str.toCharArray(cell_volts_s, 7);
    display.drawStr(5, 30 + i*16, cell_volts_s);
  }

  // Temperatures
  char temps_s[6];
  String temps_str;
  for(int i = 0; i < 3; i++){
    temps_str = String(temps[i], 0);
    temps_str.concat((char)0xb0);
    temps_str.concat("C");
    temps_str.toCharArray(temps_s, 6);
    display.drawStr(55, 30 + i*16, temps_s);
  }


  // Status & alarm reason
  display.setFont(u8g2_font_open_iconic_check_2x_t);  
  char status_s[2];
  if(alarm){
    if(alarm_silenced){
      String((char)0x42).toCharArray(status_s, 2);
    } else {
      String((char)0x44).toCharArray(status_s, 2);
    }
  } else {
    String((char)0x40).toCharArray(status_s, 2);
  }
  display.drawStr(101, 32, status_s);

  display.setFont(u8g2_font_tinytim_tr);
  display.drawStr(88, 36, alarm_reasons_text[alarm_reason]);
  
  display.setFont(u8g2_font_timB10_tr);
  char soc_s[8];
  String soc_str = String(soc, 1);
  soc_str.concat("%");
  soc_str.toCharArray(soc_s, 8);
  display.drawStr(90, 60, soc_s);

  
  display.sendBuffer();
}




void alarm_state() {
  if(alarm == true){

      if(alarm_silenced == false){
        digitalWrite(SILENCE_BTN_LED_PIN, HIGH);
        delay(80);
        digitalWrite(SILENCE_BTN_LED_PIN, LOW);
        delay(80);
      } else {
        digitalWrite(SILENCE_BTN_LED_PIN, HIGH);
      }
      
      if(digitalRead(ALARM_TRIGGER_PIN) == false){ // not pressed
        alarm = false;
        alarm_silenced = false;
        alarm_reason = 0;
      } else {
        if(alarm_silenced == false and digitalRead(SILENCE_BTN_PIN) == false){ // pressed
          alarm_silenced = true;
        }
      }
  } else {
      if(digitalRead(ALARM_TRIGGER_PIN) == true){ //pressed
        alarm = true;
        alarm_silenced = false;
        alarm_reason = 1;
      } else {
        digitalWrite(SILENCE_BTN_LED_PIN, LOW);
      }
  }
  if(alarm_silenced == true){
    digitalWrite(SILENCE_RELAY_PIN, HIGH);
  } else {
    digitalWrite(SILENCE_RELAY_PIN, LOW);
  }
}


void read_canbus(){
    CAN_message_t inMsg;
    if(Can0.available()){
      Can0.read(inMsg);
      if(inMsg.id == 0x100){
        cell_volts[2] = (float)((inMsg.buf[6] << 8) | inMsg.buf[7])/10000.0;
        soc = (float)inMsg.buf[2]/2;
        pack_volts = (float)((inMsg.buf[0] << 8) | inMsg.buf[1])/10.0;
        signed short int current_bytes = ((inMsg.buf[3] << 8) | inMsg.buf[4]);
        pack_current = current_bytes/10.0;

        
      } else if(inMsg.id == 0x101){
        temps[0] = (float)inMsg.buf[0];
        temps[1] = (float)inMsg.buf[1];
        temps[2] = (float)inMsg.buf[2];
        cell_volts[0] = (float)((inMsg.buf[5] << 8) | inMsg.buf[6])/10000.0;
        cell_volts[1] = (float)((inMsg.buf[3] << 8) | inMsg.buf[4])/10000.0;
      }
    }
}
