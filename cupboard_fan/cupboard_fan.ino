/* Cupboard fan control.

    Reads the temp, and adjusts the fan speed accordingly.
    Temperature, and fan speed are displayed.
*/
#include "LedControl.h"
LedControl digits = LedControl(7, 5, 6, 1);

#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define TEMP2_READ_INTERVAL 2000
#define TEMP_DIFF_MAX_FAN 12.0
#define TEMP_DIFF_MIN_FAN 1.2



#define TEMP_HIST_COUNT 100
#define FAN1_TACH_PIN 2
#define FAN2_TACH_PIN 3
#define FAN_PWM_PIN 10 // This must not be changed.
#define MS_PER_MIN 60000.0
#define TIMER_COUNT_TOP 80

#define DISP_SCROLL_DELAY_MS 5000
#define NUM_DISP_DIGITS 8


// temp1 is outside thermometer
float temp1_history[TEMP_HIST_COUNT];
unsigned int temp1_pointer = 0;
float temp1 = 0.0;

// temp2 is inside thermometer & barometer
float temp2_history[TEMP_HIST_COUNT];
unsigned int temp2_pointer = 0;
unsigned long temp2_last_read = 0;
float temp2 = 0.0;
float prev_temp2 = 0.0;

float humidity;

float fan_percent = 0.0;

volatile unsigned int fan1_half_revolutions = 0;
volatile unsigned int fan2_half_revolutions = 0;
unsigned int rpm1 = 0;
unsigned int rpm2 = 0;
unsigned long previous_time = 0;

char display_buffer[NUM_DISP_DIGITS + 4]; // Need extras for null and decimal points

unsigned long delaytime = 250;


void setup() {
  Serial.begin(115200);
  digits.shutdown(0, false);
  digits.setIntensity(0, 15);
  digits.clearDisplay(0);

  pinMode(FAN1_TACH_PIN, INPUT);
  pinMode(FAN2_TACH_PIN, INPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);

/* OLD PWM
// Uncomment one of the three following lines to enable PWM on Pin 3 or Pin 11
    TCCR2A = 0x21;   // Pin 3
   
  // Set prescaler  
  TCCR2B = 0x0A;   // prescaler = /8
// Initialize duty cycle(s) to zero(0)
 OCR2A = 40;    // duty cycle for Pin 11
 OCR2B = 0;    // duty cycle for Pin 3
  */

  // generate 25kHz PWM pulse rate on Pin 3
  // Set up Fast PWM on Pin 3
  TCCR1A = 0x23;     // COM1B1, WGM11 WGM10
  // Set prescaler
  TCCR1B = 0x1A;   // WGM13 WGM12, Prescaler = /8
  // Set TOP and initialize duty cycle to zero(0)
  OCR1AH = 0x00;    // TOP DO NOT CHANGE, SETS PWM PULSE RATE
  OCR1AL = TIMER_COUNT_TOP;    // TOP DO NOT CHANGE, SETS PWM PULSE RATE
  OCR1BH = 0x00;    // duty cycle for Pin 3 (0-79) generates 1 500nS pulse even when 0
  OCR1BL = 0x00;    // duty cycle for Pin 3 (0-79) generates 1 500nS pulse even when 0


  attachInterrupt(digitalPinToInterrupt(FAN1_TACH_PIN), fan1_tick, FALLING);
  attachInterrupt(digitalPinToInterrupt(FAN2_TACH_PIN), fan2_tick, FALLING);

  dht.begin();
}

void loop () {

  static unsigned long timer = millis();

  if (millis() >= timer) {
    detachInterrupt(digitalPinToInterrupt(FAN1_TACH_PIN));

    timer += 250;

    temp1 = get_temp1();

    update_temp2();

    measure_fan_rpms();
 
    set_fan_speed(fan_percent = get_fan_percent());
   
    print_stats_to_serial();
    update_display();

    attachInterrupt(digitalPinToInterrupt(FAN1_TACH_PIN), fan1_tick, FALLING);
    attachInterrupt(digitalPinToInterrupt(FAN2_TACH_PIN), fan2_tick, FALLING);

  }

};

float get_fan_percent(){
  // This is for a potentiometer reading.
  //return (float)analogRead(A5) / 1022.0;

  if(temp1 > temp2) {
    // Outside is warmer than inside, no fans.
    return 0.0;
  } else {
    float temp_diff = temp2 - temp1;
    if(temp_diff > TEMP_DIFF_MAX_FAN) {
      return 1.0;
    } else if(temp_diff < TEMP_DIFF_MIN_FAN) {
      return 0.0;
    } else {
      return temp_diff/TEMP_DIFF_MAX_FAN;
    }
  }
  
}

void set_fan_speed(float percent) {
  if(percent <= 0.0){
    // Disable timer completely, getting rid of residual PWM signal
    TCCR1B = 0x18;
  } else {
    // Re-enable timer and set PWM
    TCCR1B = 0x1A;
    OCR1BH = 0x00;
    OCR1BL = (int)((float)TIMER_COUNT_TOP * percent); 
  }
}

void fan1_tick() {
  fan1_half_revolutions++;
}
void fan2_tick() {
  fan2_half_revolutions++;
}

void measure_fan_rpms() {
    unsigned int elapsed_ms = (millis() - previous_time);
    float elapsed_min = (float)elapsed_ms/MS_PER_MIN;
    rpm1 = (fan1_half_revolutions / 2)/elapsed_min;
    rpm2 = (fan2_half_revolutions / 2)/elapsed_min;
    previous_time = millis();
    fan1_half_revolutions = 0;
    fan2_half_revolutions = 0;
}

void update_display() {
  // 3 different display modes, scroll through them.
  unsigned int current_display = (int)(millis() / DISP_SCROLL_DELAY_MS) % 3;
  if (current_display == 0) {
    write_temps_to_buff();
  } else if (current_display == 1) {
    write_temp2_humidity_to_buff();
  } else if (current_display == 2) {
    write_fan_rpm_to_buff();
  }
  write_buffer_to_display();

}

void write_temps_to_buff() {
  char temp1c[5];
  char temp2c[5];
  dtostrf(temp1, 4, 1, temp1c);
  dtostrf(temp2, 4, 1, temp2c);
  sprintf(display_buffer, "%s %s", temp1c, temp2c);
}

void write_temp2_humidity_to_buff() {
  char temp2c[5];
  dtostrf(temp2, 4, 1, temp2c);
  sprintf(display_buffer, "%s h-%u", temp2c, (int)humidity);
}

void write_fan_rpm_to_buff() {
  sprintf(display_buffer, "%4u.%4u.", rpm1, rpm2);
}

void write_buffer_to_display() {
  digits.clearDisplay(0);
  char current_char;
  unsigned int current_digit = NUM_DISP_DIGITS - 1;
  bool next_char_is_decimal = false;
  for (unsigned int i = 0; i < sizeof(display_buffer); i++) {
    next_char_is_decimal = false;
    current_char = display_buffer[i];
    if (current_char == '\0') {
      // End of buffer
      return;
    }
    if (display_buffer[i] == '.') {
      // Should have already accounted for the decimal.
      continue;
      current_digit++;
    }
    if (i <= NUM_DISP_DIGITS && display_buffer[i + 1] == '.') {
      next_char_is_decimal = true;
    }
    digits.setChar(0, current_digit, current_char, next_char_is_decimal);
    current_digit--;
  }
}

float get_temp1() {
  float temp_sum = 0;
  float temp = analogRead(A0) * 5 / 1024.0;
  temp = temp - 0.5;
  temp = temp / 0.01;
  temp = temp + 1.1;
  temp1_history[temp1_pointer] = temp;
  temp1_pointer = (temp1_pointer + 1) % TEMP_HIST_COUNT;
  for (int i = 0; i < TEMP_HIST_COUNT; i++) {
    temp_sum += temp1_history[i];
  }
  return temp_sum / TEMP_HIST_COUNT;
}

void update_temp2() {
  float temp;
  float temp_sum = 0;

  // Can't read the int temp sensor too often
  // So, either read it, or use the previous read.
  if (millis() >= (temp2_last_read + TEMP2_READ_INTERVAL)) {
    temp = dht.readTemperature();
    humidity = dht.readHumidity();
    temp2_last_read = millis();
    prev_temp2 = temp;
  } else {
    temp = prev_temp2;
  }

  temp2_history[temp2_pointer] = temp;
  temp2_pointer = (temp2_pointer + 1) % TEMP_HIST_COUNT;
  for (int i = 0; i < TEMP_HIST_COUNT; i++) {
    temp_sum += temp2_history[i];
  }

  temp2 = temp_sum / TEMP_HIST_COUNT;
}

void print_stats_to_serial() {
  Serial.println();
  Serial.print("Ext.: ");
  Serial.print(temp1);
  Serial.print("  Int.: ");
  Serial.print(temp2);
  Serial.print("  ");
  Serial.print(humidity, 0);
  Serial.println("% humidity");
  Serial.print("fan1: ");
  Serial.print(rpm1);
  Serial.print("  fan2: ");
  Serial.print(rpm2);
  Serial.print("  pwm: ");
  Serial.print(fan_percent*100, 2);
  Serial.println("%");
}
