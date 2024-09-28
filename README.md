<h1>PID-FASTEST-LINE-FOLLOWER</h1>

<p>
This project showcases an <strong>autonomous robot</strong> capable of following a path using <strong>real-time feedback control</strong>. The robot utilizes a <strong>set of IR sensors</strong> to detect the path and maintain alignment, while a <strong>PID algorithm</strong> ensures smooth and precise navigation. This allows the robot to react quickly to deviations and stay on track efficiently.
</p>


<img src="https://github.com/user-attachments/assets/7671b889-13c1-4521-abed-c3fab65bc24b"/>



<h2>Key Features</h2>
<ul>
  <li><strong>Precise Path Tracking:</strong> Multiple IR sensors detect the line and surroundings.</li>
  <li><strong>PID Control Algorithm:</strong> Ensures real-time correction for smooth movement.</li>
  <li><strong>High Speed:</strong> Optimally tuned PID values enable fast and accurate line following.</li>
</ul>

<h2>Components Required</h2>

<h3>1. Microcontroller</h3>
<p><strong>Arduino Nano</strong>: Chosen for its compact size and efficiency, it provides enough resources to handle the sensor input and process the PID algorithm, controlling the motors for precise navigation.</p>

<h3>2. Motor Driver</h3>
<p><strong>Cytron MDD3A Motor Driver</strong>: High-performance motor driver with fast switching speeds, operating at <strong>16V</strong>. It provides real-time responsiveness needed for high-speed operations.</p>

<h3>3. Motors</h3>
<p><strong>N20 Gear Motor (600RPM, 12V)</strong>: These motors offer a perfect balance between speed and control, running at <strong>12V</strong>. Their <strong>600RPM</strong> configuration provides the necessary torque and precision for the robot's fast movement.</p>

<h3>4. Battery</h3>
<p><strong>16V Orange LiPo Battery</strong>: The battery provides stable power to the system, ensuring consistent operation for the microcontroller and motors, essential for high-speed line following.</p>

<h2>Robot Dimensions</h2>
<p>
The robot has a compact and efficient design, with dimensions of <strong>12 cm (L) x 5 cm (W) x 6 cm (H)</strong>. This size ensures agility and optimal performance, making it suitable for high-speed line-following tasks.
</p>


<h2>Working Principle</h2>
<p>
The robot uses <strong>infrared (IR) sensors</strong> to detect the line on the surface. The feedback from these sensors is processed by the <strong>PID control algorithm</strong>, which adjusts the motor speeds in real-time to keep the robot aligned with the line.
</p>
<ul>
  <li><strong>Proportional (P):</strong> Corrects the position based on immediate errors.</li>
  <li><strong>Integral (I):</strong> Eliminates residual offsets by accounting for past errors.</li>
  <li><strong>Derivative (D):</strong> Predicts future errors to prevent overshooting.</li>
</ul>

<p>This balance allows the robot to follow the path smoothly, even at high speeds.</p>

```cpp
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
```

<h2>How to Use</h2>
<ol>
  <li><strong>Assemble the components</strong> as per the circuit diagram provided in our Repository which is in the form of schematic.</li>
  <li>Upload the <strong>Arduino code</strong> that implements the PID algorithm.</li>
  <li>Calibrate the <strong>PID parameters</strong> (P, I, and D) based on your environment.</li>
  <li>Power up the robot with the <strong>16V LiPo battery</strong> and watch it follow the line at high speed!</li>
</ol>

<h2>Conclusion</h2>
<p>
This project demonstrates the implementation of a <strong>high-speed, PID-controlled line-following robot</strong>, ideal for competitive robotics or learning control systems. The combination of real-time feedback and precise motor control results in efficient navigation along a path, adapting to obstacles and deviations.
</p>
