#include <Servo.h>

#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)
#define _EMA_ALPHA 0.5

#define _TARGET_LOW  180.0
#define _TARGET_HIGH 360.0
#define _DUTY_MIN 500
#define _DUTY_NEU 1600
#define _DUTY_MAX 2700

float dist_ema, dist_prev = _DIST_MAX;
unsigned long last_sampling_time;

Servo myservo;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(_DUTY_MIN);

  dist_prev = _DIST_MIN;
  Serial.begin(57600);
}

void loop() {
  float dist_raw;

  if (millis() < (last_sampling_time + INTERVAL))
    return;

  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
    dist_raw = dist_prev;
    digitalWrite(PIN_LED, 1);
  } else if (dist_raw < _DIST_MIN) {
    dist_raw = dist_prev;
    digitalWrite(PIN_LED, 1);
  } else {
    dist_prev = dist_raw;
    digitalWrite(PIN_LED, 0);
  }

  dist_ema = _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_ema;


  int servoAngle = map(dist_ema, _TARGET_LOW, _TARGET_HIGH, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(servoAngle);

  Serial.print("Min:");
  Serial.print(_DIST_MIN);
  Serial.print(",Low:");
  Serial.print(_TARGET_LOW);
  Serial.print(",dist:");
  Serial.print(dist_raw);
  Serial.print(",Servo:");
  Serial.print(myservo.read());
  Serial.print(",High:");
  Serial.print(_TARGET_HIGH);
  Serial.print(",Max:");
  Serial.print(_DIST_MAX);
  Serial.print(",ema:");
  Serial.print(dist_ema);
  Serial.println("");

  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
