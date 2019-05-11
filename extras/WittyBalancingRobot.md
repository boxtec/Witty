# Witty Balancing Robot

## How to use the Gyro
### Introduction
Gyro are used on drones to guarantie the stability; a good modelization and tricky programming is required. Many balancing robots are available on the market; they rarely satisfy their users, spending time trying to adjust parameters and put the robot back up.
The Witty is indeed more difficult to program, but it never falls down! What is interesting is motors fight against a linear inertia and a rotation inertia. Start slowly, it moves as a simple robot. Put full power, it rotates. Imagine all the trick you can play with, understanding the accelerator and gyro signals and getting the desired or a surprising behaviour.

##### Joe Robot
One of the first robot balancing acts seems to date from 2001 with Joe, developed at EPFL and well documented by [F.Gasser](https://www.semanticscholar.org/author/Felix-Grasser/50172806) et [A.D'Arrigo](https://www.semanticscholar.org/author/Aldo-D'Arrigo/48159685).  
Since 2012, integrated sensors have since facilitated implementation, and the number of projects has exploded, but the descriptions are of the type "kitchen rule". Few understand what it does and why it works, if it works.  
Adjustment problems are always tricky, and mathematics is not very useful if the data is worthless.

<p align="center"><table width="90%" align="center"><tr>
<td width="50%"><img src="https://git.boxtec.ch/didel/Witty/raw/branch/master/extras/images/joe-bot1.png" width="300"></td>
<td><img src="https://git.boxtec.ch/didel/Witty/raw/branch/master/extras/images/joe-bot2.png" width="300"></td>
</tr></table></p>

One can find theory with pretty formulas, there are libraries that apply these formulas in floating point. The problem is that a calculation can only give accurate results if the data is accurate and if one can act accurately.  
Let's study in detail the components of a balancing robot and their behavior.  
Our project uses a Gy521 and 6mm geared motors, the goal was to build small. Our explanations will try to be general.

### Accelerometer and Gyro
An accelerometer measures gravity along three axes. So if an axis is exactly vertical, the gravity is 1 and varies little with the angle. On both perpendicular axes, gravity is zero and varies very rapidly. With these three signals we can know the position of the sensor in space. But they are terribly noisy and must be filtered, which introduces delays.  
In addition, the accelerometer measures both static and dynamic accelerations.  
You have to use a low-pass filter to get the gravity and a high-pass filter if one needs the dynamic acceleration.  
Three-axis accelerometer as the Gy521/MPU-6050 provides gyro values x, y, z.  

![witty](images/wittybot1.png?raw=true "Witty Bot") ![gyro1](images/gyro-axes.png?raw=true "Gyro Axes")  

On the Witty, some values are more interesting to know. In the position of the picture, acceleration Y is close to -1G, Z close to zero. X will stay at zero on the usually flat test surface but the roll value is interesting to know. Play with Demo program 6 to get familiar with the accelerator parameters the Gy521 provides.  

The gyro values are more difficult to handle. By knowing the initial position and integrating the variations, we obtain the current position. But values are very noisy and need to be filtered.

### The MPU6050 Sensor on the Gy521 Module
<table border="0">
<tr><td width="200">
![GY521](images/gy521.png?raw=true "GY521 Modul")
</td><td>
The Gy521 module accepts a voltage of 3 to 5V, which is necessary to develop at 5V and then navigate to 3.7V having possibly retouched the parameters.
The doc of the MPU6050 is scary with 120 control registers at first glance. In fact all these registers have a correct default value for our use, except one, necessary to wake up the circuit. Two other registers will be commented on later.
The module is I2C, we must add the pull-up resistor (4k7). We can ignore the 4 additional signals.
</td></tr>
</table>

### Reading the MPU6050 sensor
The set-up of the MPU6050 is simple: Its I2C address is 0x68 (7-bit address) and for standard operations, there is only register 0x6B to clear. 

The data of the sensors, plus the temperature that can be ignored, are prepared in 16-bit registers. The internal 6050 controller update these registers every 4ms (260Hz) or less depending register xx.  
Arduino document under  https://playground.arduino.cc/Main/MPU-6050 the program that displays the values ​​of the registers on the Serial terminal (5698 octets, 490variables).  
You can find on http://www.didel.com/Witty.zip the equivalent program using TerSer and I2cTwi lib (1616 octets, 67 variables).

## Witty Motor
As detailed on [WittySpecs.pdf](https://www.didel.com/WittySpecs.pdf), the Bo30 motor has a gear factor of 1:96 and a wheel of 32mm. Speed and acceleration is evaluated with the 16-slot encoder on a similar motor (no load). The oscilloscope shows full speed is reached after less than 60 degrees, and speed is 200ms/turn, that is 5t/s, 50 cm/s.  

The Witty specs explain how to control the motor on/off, PWM and PFM. Getting familiar with the Witty reactions, use the pushbutton feature of the demo program to prepare several behaviours to test.  

### Control
Control theory uses beautiful mathematics and professional tools like MathLab and LabView running on powerful processors. Reading sensors, removing noise, applying control models, setting the motor parameters imply a set of tasks that must be the smallest amount of time, to be reactive. This mean every task must be under control for its execution time. Use floating point, ok, but what is the reaction time? Has my robot enough inertia to accept such a low response time?  
We have to program the Witty with that constant idea: keep the soft compact and fast.

### Control principles
<center><img src="https://git.boxtec.ch/didel/Witty/raw/branch/master/extras/images/regulator1.png" width="420"><img src="https://git.boxtec.ch/didel/Witty/raw/branch/master/extras/images/regulator2.png" width="420"></center>

Let us suppose, as for traditional balancing robot, we want to keep the Witty vertical.  vertical. If he leans forward, he must be advanced. If the speed is proportional to the error (the angle) one has a setting P for proportional.  
An effort, friction, the fact that Pegasus is not balanced initially, can prevent reaching the desired position; it's the static error. We add to correct a fraction of the integral of the error, therefore the sum of the differences between the desired position and the successive positions. It's component I, integration.  
One can still worry about oscillations, and take into account the variation of the gap. It is the component D, derived.  
We then have a complete PID setting, but we will drop the D because the sensor fluctuates too much and too quickly, the variation of displacement is embedded in this noise.
The simplified formula for a PI regulator is given below, with 
* U (t) the correction to be made (PFM motor value) 
* e(t) position error at time t
* Int (e (t)) the integral errors since the beginning.
Kp and Ki are constants that will have to be determined experimentally, in the absence of precise modeling.

**U(t) = Kp∗e(t) + Ki∗int(e(t))**

### Sensor Noise
![sensornoise](images/sensor-noise.png?raw=true "Sensor Noise")  
To reduce the noise of the sensors, it is necessary to average and there are three simple ways to average, plus very complex techniques. The simple average sums up e.g. 4 measurements and gives a result 4 times less often, which is obviously not favorable. The sliding average measure the average of the previous 4 measures. 
Weighted average give a weight to the stored measures, so it is possible to react faster or slower to the last measure, and get something like a low pass or high pass filter. See the web for details.  
(our old document in French: [Moyenne.pdf](https://www.didel.com/Moyennes.pdf)).

### Derive and integrate
To derive, we calculate the difference between two consecutive values. You must memorize the current value after calculating the difference.
Note that the difference is signed and its absolute value is the same size as the subtracted values.  
To integrate, we sum simply, We integrate the difference between the desired position and the current position. It is necessary to avoid an overflow and to saturate the integrated value, possibly to signal if it takes an excessive value.  
You have to initialize correctly when you engage.

#### Principles of programming
```//Variables
int  v8, prevV8;
inu diff, itgr
```

Anytime v8 is read:

```//Pour dériver
diff =  v8 - prevV8;
prevV8; = v8;

//Pour intégrer
itgr += v8;
```

## Test Environment and useful Functions
![witty](images/xyz1.png?raw=true "XYZ")  
The AcZ component is horizontal and will play the main role in the algorithm. <br>The GyX value can be integrated to give the angle or directly used as a derivative of the acceleration. It is very parasitized and it is necessary to use a suitable filter. <br>Name of the main variables:  

```int16_t AcZ;   // read on the sensor
int16_t mAcZ;  // moving average
int16_t corAcZ;  // mAcZ- AcZini
int16_t dAcZ    // integrated value
int8_t pfmL pfmR  // pfm of the 2 motorrs
```

We simplify the problem if we consider only AcZ. At reset, the robot is stationary in the desired position and the initial values ​​after filtering are stored.  
The same pfm value is given for both engines. The consequence is that Pegase will slowly turn on itself. It will involve GyY, but without compass, there will always be drift.  
Visualizing data and calculations is essential at every step.  
A P setting will check the quality of the Kp parameter signals. Then we add the effect of the integral.  

### Debugging Tools
The TerSer.h library is used instead of the Serial library. The memory space is greatly reduced and the display of insignificant zeros avoids the ziz-zag displays which prevent the evolution of the variables from being clearly visible.
The program is in Pegase.zip.
Values displayed by G521AcXYZSerNum.ino:  
<pre>
  AcX= 004D   AcY= 2091   AcZ= 0227 
  AcX= 0039   AcY= 2073   AcZ= 0220 
  AcX= 0046   AcY= 2079   AcZ= 0225 
  AcX= 004B   AcY= 2074   AcZ= 0226 
  AcX= 0055   AcY= 2069   AcZ= 0216 
  AcX= 0044   AcY= 2080   AcZ= 0213
</pre>
  
The Oled display offers the same display possibilities as the terminal, not drop-down and with additional graphics. The write time is similar, about 50 ms, which corresponds to the 50ms cycle that seems to be good to achieve.

## PFM Calculation
Let's start with a P setting. The Pfm is proportional to AcZ corrected by the initial offset. We check on the support that the engine is running in the right direction. The multiplicative factor Kp must be treated differently, using the floating point does not make sense. Let's define a logical speed. Kp is a constant of type x / 2n. We multiply by an integer and shift.  
The gear table equates to 16 logical speeds a PFM between 0 and 80.


### Optimization
The register 25 = 0x19 Sample Rate Divider has a value linked to that of the register 26 = 0x1A Configuration. The default value is 0 and known applications do not attempt to modify it. A specialist advises R25 value 22, R26 value 4 (going to 6 should slow down the reaction since the filtering delay changes to 19ms instead of 20ms).

## References
* https://positron-libre.blog/robots/pendule-inverse-vertibot.php
* https://www.pc-control.co.uk/pid_control.htm  emphasize D (speed control)
* https://theautomization.com/pid-control-basics-in-detail-part-1/

