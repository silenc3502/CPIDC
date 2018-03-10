# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Explicit Section

I'll write simulation results of P gain(Kp), I gain(Ki), D gain(Kd) at here.  
Below image represents the P Controller.  
It has big vibration(Overshoot increase) and can't convergence.  
It's just divergence(not a vector divergence).  

![p_control](./images/Kp.gif)

Below image represents the D gain.  
It's good to catch vibration(Overshoot decrease).  
And it has another problem that is the noise.  
D Controller is very weak when it has noise.  
So, when we design electric system then need to design LPF(Low Pass Filter).  
It's the reason that we need stochastic calculus.  

![d_control](./images/Kd.gif)

Below image represents the I gain.  
It's good to adjust steady-state error.  
However it makes increase of overshoot and slow to go our goal.  
But, we need to use it to make precision control(reduce steady-state error).  

![i_control](./images/Ki.gif)

Below image represents the PID Controller.  
It's go to the control goal fast(Kp).
And there are no big vibration(Kd).
And it has no big steady-state error(Ki).

![pid_control](./images/pid.gif)

## How to Setting Steering Hyperparamters

We know P coefficient can makes to go goal fast.
However it makes big overshoot.
And I coefficient is good to catch Steady-State Error.
However computer can't do integral operations well.
So I give it to small value to reduce integral error.
And D coefficient can decrease the overshoot.

First I test the any P, I, D value to start.
After I adjust P to go goal fast.
And adjust D value to cat big vibration.
When I increase it then the body decrease the vibration.
After I adjust I value to catch Steady-State Error.

So, finally my P, I, D coefficient is [13.0, 0.02, 300.0].
