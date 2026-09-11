#define EN_A 10  // Enable1 L298 Pin enA
#define IN1 9    // Motor1 L298 Pin in1
#define IN2 8    // Motor1 L298 Pin in2
#define IN3 7    // Motor2 L298 Pin in3
#define IN4 6    // Motor2 L298 Pin in4
#define EN_B 5   // Enable2 L298 Pin enB
#define IR_R A0  // Left Infrared Sensor Pin
#define IR_F A1  // Front Infrared Sensor Pin
#define IR_L A2  // Right Infrared Sensor Pin
#define RED_LED A3 // Red LED Pin
#define SERVO_PIN A4 // Servo Motor Pin
#define PUMP_PIN A5  // Pump Pin

// Motor speed 
#define MOTOR_SPEED 160

// Infrared sensor thresholds
#define IR_R_THRESHOLD 245
#define IR_F_THRESHOLD 345
#define IR_L_THRESHOLD 245

// Valid value ranges for infrared sensors
#define IR_R_MIN 246
#define IR_R_MAX 850
#define IR_F_MIN 346
#define IR_F_MAX 900
#define IR_L_MIN 246
#define IR_L_MAX 850

// Servo angles and movement steps
#define CENTER_ANGLE 130
#define RANGE_ANGLE 40
#define SERVO_START_ANGLE_SETUP CENTER_ANGLE
#define SERVO_END_ANGLE_SETUP CENTER_ANGLE + RANGE_ANGLE
#define SERVO_MID_ANGLE_SETUP CENTER_ANGLE
#define SERVO_FINAL_ANGLE_SETUP CENTER_ANGLE - RANGE_ANGLE
#define SERVO_RESET_ANGLE CENTER_ANGLE

#define SERVO_R_MIN_ANGLE CENTER_ANGLE - RANGE_ANGLE
#define SERVO_R_MAX_ANGLE CENTER_ANGLE + RANGE_ANGLE
#define SERVO_F_MIN_ANGLE CENTER_ANGLE - RANGE_ANGLE
#define SERVO_F_MAX_ANGLE CENTER_ANGLE + RANGE_ANGLE
#define SERVO_L_MIN_ANGLE CENTER_ANGLE - RANGE_ANGLE
#define SERVO_L_MAX_ANGLE CENTER_ANGLE + RANGE_ANGLE

#define SERVO_STEP_SETUP 5
#define SERVO_STEP_LOOP 3
#define SERVO_PULSE_OFFSET 500
#define SERVO_ANGLE_MULTIPLIER 11

// Delay times 
#define IR_READ_DELAY 50
#define MOTOR_MOVE_DELAY 100
#define TURN_DURATION 200
#define SERVO_REFRESH_CYCLE 50
#define LED_BLINK_INTERVAL 1000 
#define LOOP_DELAY 10
#define INITIALIZATION_DELAY 500
int counter = 0;

int sR, sF, sL;
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(IR_R, INPUT);
  pinMode(IR_F, INPUT);
  pinMode(IR_L, INPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // Initial servo movement
  for (int angle = SERVO_START_ANGLE_SETUP; angle <= SERVO_END_ANGLE_SETUP; angle += SERVO_STEP_SETUP) {
    servoPulse(SERVO_PIN, angle);
  }
  for (int angle = SERVO_END_ANGLE_SETUP; angle >= SERVO_MID_ANGLE_SETUP; angle -= SERVO_STEP_SETUP) {
    servoPulse(SERVO_PIN, angle);
  }
  for (int angle = SERVO_MID_ANGLE_SETUP; angle <= SERVO_FINAL_ANGLE_SETUP; angle += SERVO_STEP_SETUP) {
    servoPulse(SERVO_PIN, angle);
  }

  servoPulse(SERVO_PIN, 130); // Return to default position
  analogWrite(EN_A, MOTOR_SPEED);
  analogWrite(EN_B, MOTOR_SPEED);
  delay(INITIALIZATION_DELAY);
}

void loop() {
  sR = analogRead(IR_R);
  sF = analogRead(IR_F);
  sL = analogRead(IR_L);
  Serial.print(sR);
  Serial.print("\t");
  Serial.print(sF);
  Serial.print("\t");
  Serial.println(sL);
  delay(IR_READ_DELAY);

  // Fire detection check
  if (sR < IR_R_THRESHOLD) {
    Stop();
    digitalWrite(PUMP_PIN, HIGH);
    // Servo movement upon fire detection from the right
    for (int angle = SERVO_R_MAX_ANGLE; angle >= SERVO_R_MIN_ANGLE; angle -= SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    for (int angle = SERVO_R_MIN_ANGLE; angle <= SERVO_RESET_ANGLE; angle += SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    servoPulse(SERVO_PIN, SERVO_RESET_ANGLE); // Return to default position

    forword();
    delay(80);

  } else if (sF < IR_F_THRESHOLD) {
    Stop();
    digitalWrite(PUMP_PIN, HIGH);
    // Servo movement upon fire detection from the front
    for (int angle = SERVO_RESET_ANGLE; angle <= SERVO_F_MAX_ANGLE; angle += SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    for (int angle = SERVO_F_MAX_ANGLE; angle >= SERVO_F_MIN_ANGLE; angle -= SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    for (int angle = SERVO_F_MIN_ANGLE; angle <= SERVO_RESET_ANGLE; angle += SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    servoPulse(SERVO_PIN, SERVO_RESET_ANGLE); // Return to default position
      
    forword();
    delay(80);

  } else if (sL < IR_L_THRESHOLD) {
    Stop();
    digitalWrite(PUMP_PIN, HIGH);
    // Servo movement upon fire detection from the left
    for (int angle = SERVO_L_MAX_ANGLE; angle >= SERVO_L_MIN_ANGLE; angle -= SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    for (int angle = SERVO_L_MIN_ANGLE; angle <= SERVO_RESET_ANGLE; angle += SERVO_STEP_LOOP) {
      servoPulse(SERVO_PIN, angle);
    }
    servoPulse(SERVO_PIN, SERVO_RESET_ANGLE); // Return to default position

    forword();
    delay(80);

  }
  // Normal movement based on sensors
  else if (sR >= IR_R_MIN && sR <= IR_R_MAX) {
    digitalWrite(PUMP_PIN, LOW);
    // backword();
    delay(MOTOR_MOVE_DELAY);
    turnRight();
    delay(TURN_DURATION);
  } else if (sF >= IR_F_MIN && sF <= IR_F_MAX) {
    digitalWrite(PUMP_PIN, LOW);
    forword();
  } else if (sL >= IR_L_MIN && sL <= IR_L_MAX) {
    digitalWrite(PUMP_PIN, LOW);
    // backword();
    delay(MOTOR_MOVE_DELAY);
    turnLeft();
    delay(TURN_DURATION);
  } else {
    digitalWrite(PUMP_PIN, LOW);
    Stop();
  }

  // --- LED blink every 3 seconds ---
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= LED_BLINK_INTERVAL) {
    previousMillis = currentMillis;
    ledState = !ledState; // Toggle state
    digitalWrite(RED_LED, ledState);
  }
  delay(LOOP_DELAY);
}

/**
 * @brief Sends a PWM pulse to the servo to control its angle.
 *
 * @param pin The pin the servo is connected to.
 * @param angle The desired angle (in degrees).
 */
void servoPulse(int pin, int angle) {
  // Converts angle to pulse width in microseconds
  int pwm = (angle * SERVO_ANGLE_MULTIPLIER) + SERVO_PULSE_OFFSET;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(SERVO_REFRESH_CYCLE); // Servo refresh cycle
}

/**
 * @brief 
 */
void forword() {
  digitalWrite(IN1, HIGH); // Right motor forward
  digitalWrite(IN2, LOW);  // Right motor backward
  digitalWrite(IN3, LOW);  // Left motor backward
  digitalWrite(IN4, HIGH); // Left motor forward
}

/**
 * @brief 
 */
void backword() {
  digitalWrite(IN1, LOW);  // Right motor forward
  digitalWrite(IN2, HIGH); // Right motor backward
  digitalWrite(IN3, HIGH); // Left motor backward
  digitalWrite(IN4, LOW);  // Left motor forward
}

/**
 * @brief 
 */
void turnRight() {
  digitalWrite(IN1, LOW);  // Right motor forward
  digitalWrite(IN2, HIGH); // Right motor backward
  digitalWrite(IN3, LOW);  // Left motor backward
  digitalWrite(IN4, HIGH); // Left motor forward
}

/**
 * @brief 
 */
void turnLeft() {
  digitalWrite(IN1, HIGH); // Right motor forward
  digitalWrite(IN2, LOW);  // Right motor backward
  digitalWrite(IN3, HIGH); // Left motor backward
  digitalWrite(IN4, LOW);  // Left motor forward
}

/**
 * @brief  
 */
void Stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}