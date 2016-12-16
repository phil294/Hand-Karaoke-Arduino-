#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define LEDS 24

/* uv abstands sensor */
int trigger = 6;
int echo = 2;

/* lautsprecher */
int piezo = 4;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, 3, NEO_GRB + NEO_KHZ800);
int gamma[] = {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,   90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255 };

/* lcd */
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void resetLcd() {
    lcd.setCursor(0, 0);
    lcd.print("                  ");
    lcd.setCursor(0, 1);
    lcd.print("                  ");
}

void resetLeds() {
    for (int i = 0; i < LEDS; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
}
/*
0 255 0 0
1 0 255 0
2 0 0 255
3 255 255 0
4 0 255 255
5 255 255 255
*/
/*
printf("%02x%02x%02x\n",r,g,b);
*/
int r=255,g=0,b=0;
int led_color_state = 0;
int led_color_step = 15;
void fillLeds(int led) { // up to led
    if(led_color_state==0) {
      r -= led_color_step;
      g += led_color_step;
      if(r <= 0)
        led_color_state = 1;
    } else if(led_color_state==1) {
      g -= led_color_step;
      b += led_color_step;
      if(g <= 0)
        led_color_state=2;
    } else if(led_color_state==2) {
      b-=led_color_step;
      r+=led_color_step;
      g+=led_color_step;
      if(b<=0)
        led_color_state=3;
    }else if(led_color_state==3) {
      r-=led_color_step;
      b+=led_color_step;
      if(r<=0) 
        led_color_state=4;
    }else if(led_color_state==4) {
      r+=led_color_step;
      g-=led_color_step;
      b-=led_color_step;
      if(r>=255)
        led_color_state=0;
    }
    for (int i=0; i<= led; i++) {
        strip.setPixelColor(i, r,g,b);
    }
    strip.show();
}
void paintLed(int led, int r, int g, int b) {
    strip.setPixelColor(led, r,g,b);
    strip.show();
}

typedef struct Note {
  long freq;
  char const * name;
  int duration;
} Note;
struct Note newNote(char const* name, int freq) {
  Note note;
  note.freq = freq;
  note.name = name;
  return note;
}

        /* piezo play */
        void playNote(struct Note note, float speed) {

          Serial.println(note.freq);
          Serial.println(note.name);
          Serial.println(note.duration);
        
          bool silent = false;
          bool dumb = false;
          
          float freq = note.freq;
          float dur = note.duration; // 400
          float delai = note.duration * 1.3; // 400*1.3=520
          dur *= speed;
          delai *= speed;
          if(silent) {
            delai += dur - 10; // 910
            dur = 10; // 10
          }
          if(dumb || freq <= 0) {
              delay(dur);
            } else {
              tone(piezo, freq, dur);
          }
          delay(delai);
          //noTone(piezo);
        }

/* all notes */
Note NOTES[] {
  newNote("0", 0), newNote("B0", 31), newNote("C1", 33), newNote("CS1", 35), newNote("D1", 37), newNote("DS1", 39), newNote("E1", 41), newNote("F1", 44), newNote("FS1", 46), newNote("G1", 49), newNote("GS1", 52), newNote("A1", 55), newNote("AS1", 58), newNote("B1", 62), newNote("C2", 65), newNote("CS2", 69), newNote("D2", 73), newNote("DS2", 78), newNote("E2", 82), newNote("F2", 87), newNote("FS2", 93), newNote("G2", 98), newNote("GS2", 104), newNote("A2", 110), newNote("AS2", 117), newNote("B2", 123), newNote("C3", 131), newNote("CS3", 139), newNote("D3", 147), newNote("DS3", 156), newNote("E3", 165), newNote("F3", 175), newNote("FS3", 185), newNote("G3", 196), newNote("GS3", 208), newNote("A3", 220), newNote("AS3", 233), newNote("B3", 247), newNote("C4", 262), newNote("CS4", 277), newNote("D4", 294), newNote("DS4", 311), newNote("E4", 330), newNote("F4", 349), newNote("FS4", 370), newNote("G4", 392), newNote("GS4", 415), newNote("A4", 440), newNote("AS4", 466), newNote("B4", 494), newNote("C5", 523), newNote("CS5", 554), newNote("D5", 587), newNote("DS5", 622), newNote("E5", 659), newNote("F5", 698), newNote("FS5", 740), newNote("G5", 784), newNote("GS5", 831), newNote("A5", 880), newNote("AS5", 932), newNote("B5", 988), newNote("C6", 1047), newNote("CS6", 1109), newNote("D6", 1175), newNote("DS6", 1245), newNote("E6", 1319), newNote("F6", 1397), newNote("FS6", 1480), newNote("G6", 1568), newNote("GS6", 1661), newNote("A6", 1760), newNote("AS6", 1865), newNote("B6", 1976), newNote("C7", 2093), newNote("CS7", 2217), newNote("D7", 2349), newNote("DS7", 2489), newNote("E7", 2637), newNote("F7", 2794), newNote("FS7", 2960), newNote("G7", 3136), newNote("GS7", 3322), newNote("A7", 3520), newNote("AS7", 3729), newNote("B7", 3951), newNote("C8", 4186), newNote("CS8", 4435), newNote("D8", 4699), newNote("DS8", 4978)
};

Note makeNote(const char* name, int dur) {
  if(dur < 100) {
    dur = 1000 / dur;
  }
  for(int i=0; i<90; i++) {
    if(strcmp(name, NOTES[i].name) == 0) {
      Note ret;
      ret.duration = dur;
      ret.name = NOTES[i].name;
      ret.freq = NOTES[i].freq;
      int freq = ret.freq;
      return ret;
    }
  }
}

Note song_stayin_alive[] = {
  makeNote("B4", 400),
  makeNote("A4", 400),
  makeNote("G4", 400),
  makeNote("FS4", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("0", 100),
  makeNote("0", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("0", 100),
  makeNote("0", 400),

  makeNote("B4", 400),
  makeNote("A4", 400),
  makeNote("G4", 400),
  makeNote("FS4", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("0", 100),
  makeNote("0", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("0", 100),
  makeNote("0", 400),

  makeNote("B4", 400),
  makeNote("A4", 400),
  makeNote("G4", 400),
  makeNote("FS4", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("0", 100),
  makeNote("0", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("0", 100),
  makeNote("0", 400),

  makeNote("B4", 400),
  makeNote("A4", 400),
  makeNote("G4", 400),
  makeNote("FS4", 400),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("G4", 100),
  makeNote("B4", 1300),
  makeNote("A4", 1300),
  makeNote("B4", 100),
  makeNote("A4", 100),
  makeNote("G4", 100),
  makeNote("G4", 1600),
  makeNote("FS4", 1200)
};
int song_stayin_alive_length = sizeof( song_stayin_alive) / sizeof(struct Note);

Note song_imperial_march[] = {
  makeNote("A4", 400),
  makeNote("A4", 400),
  makeNote("A4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 800),
  makeNote("E5", 400),
  makeNote("E5", 400),
  makeNote("E5", 400),
  makeNote("F5", 300),
  makeNote("C5", 100),
  makeNote("GS4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 800),
  makeNote("A5", 400),
  makeNote("A4", 300),
  makeNote("A4", 100),
  makeNote("A5", 400),
  makeNote("GS5", 300),
  makeNote("G5", 100),
  makeNote("FS5", 100),
  makeNote("F5", 100),
  makeNote("FS5", 100),
  makeNote("0", 300),
  makeNote("D5", 200),
  makeNote("G5", 400),
  makeNote("FS5", 300),
  makeNote("F5", 100),
  makeNote("E5", 100),
  makeNote("DS5", 100),
  makeNote("E5", 100),
  makeNote("0", 300),
  makeNote("E4", 200),
  makeNote("G4", 400),
  makeNote("E4", 300),
  makeNote("C5", 100),
  makeNote("A4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 800)
};
int song_imperial_march_length = sizeof( song_imperial_march) / sizeof(struct Note);
float song_imperial_march_speed = 1.3;

Note song_maus[] = {
  makeNote("F5", 400),
  makeNote("A5", 300),
  makeNote("F5", 100),
  makeNote("C5", 400),
  makeNote("C5", 400),

  makeNote("C5", 300),
  makeNote("F5", 100),
  makeNote("C5", 300),
  makeNote("A4", 100),
  makeNote("F4", 150),
  makeNote("0", 200),
  makeNote("F4", 50),
  makeNote("E4", 200),
  makeNote("0", 200),

  makeNote("F4", 400),
  makeNote("A4", 400),
  makeNote("C5", 200),
  makeNote("D5", 200),
  makeNote("0", 400),
  
  makeNote("F5", 400),
  makeNote("0", 400),
  makeNote("0", 200),
  makeNote("C5", 200),
  makeNote("A4", 400),

  makeNote("D5", 200),
  makeNote("D5", 400),
  makeNote("D5", 900),
  makeNote("A4", 100),

  makeNote("C5", 600),
  makeNote("C5", 200),
  makeNote("A4", 200),
  makeNote("C5", 400),
  makeNote("D5", 600),

  makeNote("D5", 400),
  makeNote("D5", 600),
  makeNote("A4", 200),

  makeNote("C5", 400),
  makeNote("C5", 200),
  makeNote("C5", 200),
  makeNote("A4", 200),
  makeNote("C5", 400),
  makeNote("D5", 200),

  makeNote("0", 200),
  makeNote("F5", 400),
  makeNote("F5", 200),
  makeNote("D5", 400),
  makeNote("F5", 400),

  makeNote("GS5", 400),
  makeNote("F5", 400),
  makeNote("D5", 200),
  makeNote("F5", 400),
  makeNote("G5", 2600),
  makeNote("0", 800),

  makeNote("F5", 400),
  makeNote("A5", 300),
  makeNote("F5", 100),
  makeNote("C5", 400),
  makeNote("C5", 400),

  makeNote("C5", 300),
  makeNote("F5", 100),
  makeNote("C5", 300),
  makeNote("A4", 100),
  makeNote("F4", 150),
  makeNote("0", 200),
  makeNote("F4", 50),
  makeNote("E4", 200),
  makeNote("0", 200),

  makeNote("F4", 400),
  makeNote("A4", 400),
  makeNote("C5", 200),
  makeNote("D5", 200),
  makeNote("0", 400),

  makeNote("F5", 800)
};
int song_maus_length = sizeof( song_maus) / sizeof(struct Note);
float song_maus_speed = 1.0;

Note song_barbie[] = {
  makeNote("GS5", 200),
  makeNote("E5", 200),
  makeNote("GS5", 200),
  makeNote("CS6", 200),
  makeNote("A5", 800),

  makeNote("FS5", 200),
  makeNote("DS5", 200),
  makeNote("FS5", 200),
  makeNote("B5", 200),
  makeNote("GS5", 400),
  makeNote("FS5", 200),
  makeNote("E5", 200),

  makeNote("0", 400),
  makeNote("E5", 200),
  makeNote("CS5", 200),
  makeNote("FS5", 400),
  makeNote("CS5", 400),

  makeNote("0", 400),
  makeNote("FS5", 200),
  makeNote("E5", 200),
  makeNote("GS5", 400),
  makeNote("FS5", 400),

  makeNote("GS5", 200),
  makeNote("E5", 200),
  makeNote("GS5", 200),
  makeNote("CS6", 200),
  makeNote("A5", 600),
  makeNote("A5", 200),

  makeNote("FS5", 200),
  makeNote("DS5", 200),
  makeNote("FS5", 200),
  makeNote("B5", 200),
  makeNote("GS5", 400),
  makeNote("FS5", 200),
  makeNote("E5", 200),

  makeNote("0", 200),
  makeNote("E5", 200),
  makeNote("E5", 200),
  makeNote("CS5", 200),
  makeNote("FS5", 400),
  makeNote("CS5", 400),

  makeNote("FS5", 200),
  makeNote("FS5", 200),
  makeNote("FS5", 200),
  makeNote("E5", 200),
  makeNote("GS5", 400),
  makeNote("FS5", 200),
  makeNote("E5", 200),

  makeNote("E5", 200),
  makeNote("E5", 200),
  makeNote("CS5", 200),
  makeNote("B4", 200),
  makeNote("E5", 200),
  makeNote("E5", 200),
  makeNote("CS5", 200),
  makeNote("B4", 200),

  makeNote("B5", 200),
  makeNote("0", 200),
  makeNote("GS5", 200),
  makeNote("0", 200),
  makeNote("E5", 400),
  makeNote("CS5", 200),
  makeNote("B4", 200),

  makeNote("E5", 200),
  makeNote("E5", 200),
  makeNote("CS5", 200),
  makeNote("B4", 200),
  makeNote("E5", 200),
  makeNote("E5", 200),
  makeNote("CS5", 200),
  makeNote("B4", 200),

  makeNote("0", 200),
  makeNote("CS6", 200),
  makeNote("B5", 200),
  makeNote("CS6", 200),
  makeNote("0", 200),
  makeNote("CS6", 200),
  makeNote("B5", 200),
  makeNote("CS6", 200)
};
int song_barbie_length = sizeof( song_barbie) / sizeof(struct Note);
float song_barbie_speed = 1.0;

Note *song;
  
Note *song_tries;

int song_length;

float song_speed;

void setup() {
  Serial.begin (9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  lcd.begin(16, 2);
  strip.setBrightness(255);
  strip.begin();

  //tone(piezo, 440, 1000);

}

Note nearest(Note note) {
  float min_diff = 1000;
  int min_c = -1;
  int c;
  for (c = 0; c < 90; c++) {
    float diff = abs(NOTES[c].freq - note.freq);
    if (diff < min_diff) {
      min_diff = diff;
      min_c = c;
    }
  }
  if (min_c > -1) {
    return NOTES[min_c];
  } else {
    return NOTES[0];
  }
}

float get_entfernung() {
    digitalWrite(trigger, LOW);
    delay(5);
    digitalWrite(trigger, HIGH);
    delay(10);
    digitalWrite(trigger, LOW);
    long dauer = pulseIn(echo, HIGH);
    return (dauer / 2) / 29.1;
}

float scale = 0.05; // = 24.0 / 80 / 12 * 2 ,  führt zu A2 - B6 (ca 1970 Hz)

double dist_to_freq(float entfernung) {
    double x = entfernung * scale;
    double freq = 110.0 * pow(2, x); // -> entfernung 0 = A2, 110 Hz
    freq += 220; // -> entfernung 0 = 110 + 220 = E4
    return freq;
}
float freq_to_dist(double freq) {
    float dist = ( log10( (freq-220) / 110.0 ) / log10(2) ); // log2(freq-220)
    dist /= scale;
    return dist;
}
int dist_to_led(float entfernung) {
    return entfernung * 25.0 / 80 - 1; // 0 - 30 cm ^ 25 leds
}

bool choose_song = true;
bool play = false;
bool choose_replay = false;
bool replay = false;
int song_cur_i = 0;

void loop() {

  if(choose_song) {
    
      lcd.setCursor(0, 0);
      lcd.print("Blau=Vader, Gr=      ");
      lcd.setCursor(0, 1);
      lcd.print("Maus, Rot=Barbie     ");

      paintLed(23, 0, 0, 255);
      paintLed(12, 0, 255, 0);
      paintLed(1, 255,0,0);
      float entfernung = get_entfernung();
      int led = dist_to_led(entfernung);
      bool play_now = false;
      if(led == 23) {
          song = song_imperial_march;
          song_length = song_imperial_march_length;
          song_speed = song_imperial_march_speed;
          play_now  = true;
      }
      if(led == 12) {
          song = song_maus;
          song_length = song_maus_length;
          song_speed = song_maus_speed;
          play_now = true;
      }
      if(led == 1) {
          song = song_barbie;
          song_length = song_barbie_length;
          song_speed = song_barbie_speed;
          play_now = true;
      }

      if(play_now) {
          choose_song = false;
          song_cur_i = 0;
          play = true;
          int sizee = sizeof(struct Note) * song_length;
          song_tries = (Note*) malloc( sizee );
          resetLeds();
          resetLcd();
              float note_cur_entfernung = freq_to_dist(song[0].freq);
              int led = dist_to_led(note_cur_entfernung);
              paintLed(led, 255,255,255);
          delay(1500);
      }
      
      delay(100);
  }
  
  if (play) {
      Serial.print("no. "); Serial.println(song_cur_i);
      
      /* tatsächliche note */
      Note note_cur = song[song_cur_i];
  
      /* gesungene note */
      Note note_custom;
      /* get entfernung */
      float entfernung = get_entfernung();
  
      /* berechne gesungene note */
      if (entfernung < 80 && entfernung > 0 && note_cur.freq > 0) {
          note_custom.freq = dist_to_freq(entfernung);
          Note note_custom_nearest = nearest(note_custom);
          note_custom.name = note_custom_nearest.name; // todo?
      } else {
          note_custom.freq = 0;
          note_custom.name = "x";
      }
      note_custom.duration = note_cur.duration;
      song_tries[song_cur_i] = note_custom;
  
      /* error */
      int error = abs(note_custom.freq - note_cur.freq);
      int error_perc = ((float)error / note_cur.freq) * 100;
  
      /* led  (tatsächliche note) */
      float note_cur_entfernung = freq_to_dist(note_cur.freq);
      Serial.print("freq: "); // 349
      Serial.println(note_cur.freq);
      int led = dist_to_led(note_cur_entfernung);
      resetLeds();
      fillLeds(led);
  
      /* lcd */
      lcd.setCursor(0, 0);
      lcd.print(note_cur.name);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Fehler: ");
      lcd.print(error_perc);
      lcd.print("%          ");
      //Serial.println(note_cur.name);
  
      /* tatsächliche note: piep */
      playNote(note_cur, song_speed);
  
      song_cur_i++;
        if(song_cur_i >= song_length) {
          Serial.println("end");
          play = false;
          choose_replay = true;
          resetLeds();
          resetLcd();
          delay(2500);
      }
  }

  if(choose_replay) {
      lcd.setCursor(0, 0);
      lcd.print("Rot: Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("Gr: Wiedergabe     ");
      
      paintLed(5, 255,0,0);
      paintLed(15, 0, 255, 0);
      float entfernung = get_entfernung();
      int led = dist_to_led(entfernung);
      if(led == 5) {
          choose_replay = false;
          choose_song = true;
          free(song_tries);
          resetLeds();
          resetLcd();
      }
      if(led == 15) {
          choose_replay = false;
          replay = true;
          song_cur_i = 0;
          resetLeds();
          resetLcd();
          delay(1500);
      }
      delay(100);
  }

  if(replay) {
      Note note_cur = song[song_cur_i];
      Note note_custom = song_tries[song_cur_i];
      
      /* error */
      int error = abs(note_custom.freq - note_cur.freq);
      int error_perc = ((float)error / note_cur.freq) * 100;
      /* led  (tatsächliche note) */
      float note_cur_entfernung = freq_to_dist(note_cur.freq);
      int led = dist_to_led(note_cur_entfernung);
      resetLeds();
      fillLeds(led);
      /* lcd */
      lcd.setCursor(0, 0);
      lcd.print(note_cur.name);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(error_perc);
      lcd.print("% (");
      lcd.print(note_custom.name);
      lcd.print(")           ");
      /* gesungene note: piep */
      playNote(note_custom, song_speed);
      
      song_cur_i++;
       if(song_cur_i >= song_length) {
          replay = false;
          choose_replay = true;
          resetLeds();
          resetLcd();
          delay(2500);
      }
  }

}
