# https://forums.sonicretro.org/index.php?threads/help-understanding-a-mario-physics-guide.34457/

Right now, however, I am being somewhat thrown off by the way the data is presented. For example, skidding deceleration ... subtracts 0 blocks, 0 pixels, 1 subpixel, 10 subsub-pixels, and 0 subsubsubpixels from the appropriate values?
Where did you even get those numbers? Next to "Skid Deceleration (normal)" I see "0020", which is 2 subpixels (or 0x20 = 32 subsubpixels (or Sonic-style subpixels)).

For a start, it's in hex; as such, each is 16 times smaller than the previous element. So there are 16 pixels to a block, 16 subpixels to a pixel, 16 subsubpixels to a subpixel; this means 256 subsubpixels to a pixel.

In other words, 1 subpixel = 1/16 pixel = 0.0625 pixels; 1 subsubpixel = 1/256 pixel = 0.00390625 pixels. "0020" is therefore 2 subpixels = 0.125 pixels. "0280" is 2 pixels and 8 subpixels, or 0x28 = 40 subpixels. Either way, it's 2.5 pixels.

Here, let me translate the bulk of it into pure pixels:

```
GROUND PHYSICS
 
Flat Ground Physics
 
                                px/f
Max velocity (walking)        - 1.5
Max velocity (running)        - 2.5
Max velocity (sprinting)      - 3.5
End-of-level walk speed       - 1.25
Airship cutscene walk speed   - 2
 
                                px/f^2
Walk/run/sprint acceleration  - 0.0546875
Stop Deceleration (normal)    - 0.0546875
Stop Deceleration (ice)       - 0.01171875
Skid Deceleration (normal)    - 0.125
Skid Deceleration (ice)       - 0.046875
 
Sloped Ground Physics
 
                                px/f
Uphill maximum walk speed     - 0.8125
Uphill maximum run speed      - 1.375
Sliding max speed             - 3.9375
 
                                px/f^2
Added downhill speed (26 deg) - 0.125
Added downhill speed (45 deg) - 0.1875
Sliding accel/decel (26 deg)  - 0.0625
Sliding accel/decel (45 deg)  - 0.125
 
MIDAIR PHYSICS
 
If Small, Big, Fiery or Hammer Mario:
- Releasing B and/or Left/Right has no effect on your speed.
- Holding the direction of motion accelerates you at a rate of 0.0546875 px/f^2 (up to 1.5, 2.5, or 3.5 px/f)
- Holding the direction opposite motion decelerates you at a rate of 0.125 px/f^2
 
If Raccoon or Tanooki Mario and NOT in flight mode,
- Releasing Left/Right decelerates you at a rate of about 0.0625 px/f^2 until you reach a speed of 1.4375 px/f
- Holding the direction of motion accelerates you at a rate of 0.0546875 px/f^2 (up to 1.5 or 2.5 px/f)
- Holding the direction opposite motion decelerates you at a rate of 0.1875 px/f^2
 
If Raccoon or Tanooki Mario and in flight mode,
- Even when holding B and the direction of flight, you will decelerate at a rate of 0.015625 px/f^2 until you reach a speed of 1.4375 px/f
- Releasing Left/Right decelerates you at a rate of 0.0625 px/f^2 until you reach a speed of 1.4375 px/f
- Holding the direction of motion accelerates you at a rate of 0.0546875 px/f^2 (up to 1.4375 px/f)
- Holding the direction opposite motion decelerates you at a rate of 0.1875 px/f^2
 
JUMP PHYSICS
 
                                px/f
Jump, X Velocity < 1 px/f (-Y)  3.4375
Jump, X Velocity > 1 px/f (-Y)  3.5625
Jump, X Velocity > 2 px/f (-Y)  3.6875
Jump, X Velocity > 3 px/f (-Y)  3.9375
Enemy Stomp Speed (-Y)          4
Weak Enemy Stomp (-Y)           3
Maximum Downward Speed (+Y)     4.3125
Raccoon Flying (-Y)             1.5
Raccoon Slowed Descent (+Y)     1
 
                                px/f^2
Jump Gravity
  (A held, Y Vel < -2 px/f)     0.0625
  (Y Vel > -2 px/f)             0.3125
 
SWIMMING PHYSICS
 
                                px/f
Underwater max walk speed       1
Maximum underwater Y-speed (+Y) 2
Max swimming speed              3
Jump out of water init vel (-Y) 3.1875
 
                                px/f^2
Y Acceleration (moving up)      0.0625
Y Acceleration (moving down)    0.03125
Y Acceleration (at surface, +Y) 0.046875
X Acceleration (get to speed)   0.0234375
X Acceleration (no direction)   0.0078125
X Acceleration (turning around) 0.03125

```
https://www.scq.ubc.ca/acceleration-due-to-gravity-super-mario-brothers/

