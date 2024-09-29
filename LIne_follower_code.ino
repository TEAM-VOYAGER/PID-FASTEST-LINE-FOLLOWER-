#include "CytronMotorDriver.h"
CytronMD motor1(PWM_PWM, 11,10);
CytronMD motor2(PWM_PWM, 3, 9);  
//global variables starts
int s[8];
int threshold = 512;  // Default threshold
int min_threshold = 0, max_threshold = 1023;
float current_error, prev_current_error;
float average;
char turn;
int bal = 0;
const int max_pwm = 250; 
int sensor_position = 0;

int min_readings[8];
int max_readings[8];
int sensor_readings[8];
//Ends here
// PID constants

float kp = 18.55;    // Proportional gain
float ki = 0.0001;    // Integral gain
float kd = 5.7;  // Derivative 

void setup() {
  Serial.begin(9600);
  calibrate_sensors();  // Call calibration function during setup
}

void loop() {
  PID_LINE_FOLLOW();  // Line follow using PID
}

void calibrate_sensors() {
  // Initialize min and max values for the sensors
  for (int k = 0; k < 8; k++) {
    min_readings[k] = analogRead(A0 + k);
    max_readings[k] = analogRead(A0 + k);
  }

  // Move the robot to calibrate sensor readings
  motor1.setSpeed(100);
  motor2.setSpeed(-100);
  delay(1000);  // Allow some time for calibration

  // Read sensor values multiple times to find min and max
  for (int i = 0; i < 100; i++) {
    for (byte j = 0; j < 8; j++) {
      sensor_readings[j] = analogRead(A0 + j);
      if (sensor_readings[j] < min_readings[j]) min_readings[j] = sensor_readings[j];
      if (sensor_readings[j] > max_readings[j]) max_readings[j] = sensor_readings[j];
    }
    delay(10); // Short delay between readings
  }

  // Calculate the threshold as the average of min and max values
  int sum_min = 0, sum_max = 0;
  for (int i = 0; i < 8; i++) {
    sum_min += min_readings[i];
    sum_max += max_readings[i];
  }
  min_threshold = sum_min / 8;
  max_threshold = sum_max / 8;
  threshold = (min_threshold + max_threshold) / 2;

  Serial.print("Calibration completed. Threshold set to: ");
  Serial.println(threshold);
  delay(500);
}

void sensor_reading() {
  sensor_position = 0;
  int active_sensors = 0;

  for (byte i = 0; i < 8; i++) {
    s[i] = analogRead(A0 + i);  // Read analog value from A0 to A7
    if (s[i] > threshold) s[i] = 1;
    else s[i] = 0;
    if (s[i] == 1) active_sensors++;
  }

  sensor_position = (s[0] * 1 + s[1] * 2 + s[2] * 4 + s[3] * 8 + s[4] * 16 + s[5] * 32 + s[6] * 64 + s[7] * 128);
  if (active_sensors > 0) {
    average = sensor_position / active_sensors;  // Average value
  }
}

void PID_LINE_FOLLOW() {
  int P, I, D, PID;
  int left_motor, right_motor;
  int turn_speed = 170;  // Turn speed, adjust as needed

  float setpoint = 12;     // Target value for the sensor

  // Read sensors and compute the average position
  sensor_reading();


  // Calculate PID components
  current_error = setpoint - average;
  P = current_error * kp;
  I += current_error;  // Accumulate the integral term
  D = kd * (current_error - prev_current_error);

  // Compute PID value and prevent integral windup
  PID = P + (ki * I) + D;
  I = constrain(I, -255, 255); // Prevent integral windup
  prev_current_error = current_error;

  left_motor = max_pwm +PID;
  right_motor = max_pwm - PID;

  // Ensure motor speed is within valid range
  left_motor = constrain(left_motor, -max_pwm, max_pwm);
  right_motor = constrain(right_motor, -max_pwm, max_pwm);

  // Set motor speeds using CytronMD
  motor2.setSpeed(left_motor+bal);  // Left motor
  motor1.setSpeed(right_motor);  // Right motor

  // Handle turns when sensors lose line
  if ((s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7]) == 0) {
    if (turn != 's') {
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      if (turn == 'r') {
        motor1.setSpeed(-turn_speed-bal);
        motor2.setSpeed(turn_speed);
      } else {
        motor1.setSpeed(turn_speed+bal);
        motor2.setSpeed(-turn_speed);
      }
      while (s[3] == 0 && s[4] == 0) sensor_reading();
      turn = 's';
    }
  }

  if (s[0] == 0 && s[7] == 1) turn = 'l';
  if (s[7] == 0 && s[0] == 1) turn = 'r';

  else if ((s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7]) == 8) {
    sensor_reading();
    if ((s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7]) == 8) {
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      while ((s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7]) == 8) sensor_reading();
    } else if ((s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7]) == 0) turn = 'r';
  }

}
void show_analog_value() {
  for (short int i = 7; i >= 0; i--) {
    Serial.print(String(analogRead(A0 + i)) + " ");
  }
  delay(100);
}
