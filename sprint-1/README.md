# Sprint 1: MVP



### Sprint Catalog


<img src="https://i.imgur.com/EtoEtYk.jpg" width="500" height="700" title="source: imgur.com" />

#### Sprint goals written in above image: 
<br>1 - Capacative Touch 
<br>2 - Connection to lights
<br>3 - Spin Mechanism, Physical
<br>4 - Magnet Current Detection
<br>5 - Current Connection to Light Displays
<br>6 - App Wireframe (setup, connection to phone, moodmap)
<br>7 - Setup App Page (Moodmap)
<br>8 - Submit color data in app and store
<br>9 - Research for data collection & detection
<br>10 - Moodboard

### Sprint Review  

We were able to complete all goals outlined above for Sprint 1, and more. We are more prepared to jump into Sprint 2 because we have had time to do more research before diving into it, which is what drove us above and beyond our Sprint 1 goals. 

We built the capacitive touch functionality and spin mechanism that detects magnet current. We connected both of these functions to different light displays. We also built the software application wireframe, frame work, front end for mapping colors to emotions (colors that will be sent to the ring) and completed research to prep for adding connectivity of App to Ring in Sprint 2.

### End of Sprint Prototype

#### 1a - Capacitive Touch Code

<a href="https://github.com/daraghbyrne/advancediot2018/blob/moodRing-dev/students/kmulshin_crwillia/sprint-1/Proto1/Proto1.ino">Link to code</a>

#### 1b - Capacitive Touch Video

<a href="https://youtu.be/ZYRMuWhojls">Link to YouTube Video</a>

#### 1c - Capacitive Touch Wiring Diagram

![Imgur](https://imgur.com/uFR3Y3n.jpg)

#### 2a - Spin Mechanism Code

<a href="https://github.com/daraghbyrne/advancediot2018/blob/moodRing-dev/students/kmulshin_crwillia/sprint-1/Proto1-B/Proto1-B">Link to code</a>

#### 2b - Spin Mechanism Video

<a href="https://www.youtube.com/watch?v=VYtvcwj3XVI">Link to YouTube Video</a>

#### 2c - Spin Mechanism Wiring Diagram

![Imgur](https://i.imgur.com/FUiP8gB.jpg)

#### 3a - Software App Wireframe

<a href="https://marvelapp.com/2f7a5id"> Link to App Wireframe </a> : shows flow of first use setup, connecting to Bluetooth, Moodmapping and connecting to another wearer's ring

#### 3b - Software App Code 
<br>Progress:
<ul>
  <li>Secured Apple Developers platform membership
  <li>Decided to build app with Apache Cordova so can build with HTML, CSS, JS and deploy across platforms: ios, android, osx, windows
  <li>Started building functional app prototype for ios
  <li>Created front end for initial color selection
  <li>Connecting to PHP for local storage
  <li>Created WebSQL data base for storage
  <li>Storing HEX values with JS
  <li>Need to figure out how to save them locally and output them to then send to ring via Bluetooth connection
<ul>

#### Screenshots of some of the app pages 
<br><img src="https://i.imgur.com/rCWSEqtl.png" title="source: imgur.com" />
<br><img src="https://i.imgur.com/hRChKZ1l.png" title="source: imgur.com" />
<br><img src="https://i.imgur.com/lNk5XzLl.png" title="source: imgur.com" />
<br><img src="https://i.imgur.com/GrCWTfnl.png" title="source: imgur.com" />
<br><img src="https://i.imgur.com/2336k7Zl.png" title="source: imgur.com" />

### Sprint Retrospective 
#### What went well?
<br> We are all incredibly invested in this project, so each of us invested the right amount of time to reach our ambitious sprint goals and we could rely on each other to meet deliverable goals. When someone said they would do something, they did it and often exceed their goal. We all contributed to supporting each other when we needed help, which sped up efficiency too. Each of us focused on the areas we were strongest or wanted to learn, which kept our motivation up, but we made all major design decisions together and shared progress during the week as we moved along to keep each other motivated. For example, posting our progress to slack as we were building kept us moving quickly. This worked well and we'll probably continue to work this way. 
#### What went wrong during the sprint cycle?
<br> From a software standpoint, it was difficult to figure out how to organize the code to start building the app. This happened because none of us have built one before, so the person tackling app development put in the time to learn how to set it up and this shouldn't be a problem anymore. The main app problem is now storing data on the app so it can be sent to the ring itself. This is a problem to be resolved for Sprint 2.
<br>
<br> On the physical prototype front, it seems that we will need magnets with a stronger magnetic than anticipated for the Hall effect sensors to read them properly.  We will obtain some rare earth magnets for Sprint 2 and incorporate them into the physical prototype.
<br>
#### What could we do differently to improve?
<br>More planning and research before diving into building could help us to improve efficiency even more. Still, we were able to reach all our Sprint 1 goals.

### Next

#### Sprint 2 Focus:
<br>1 - Connecting app to ring over bluetooth
<br>2 - Sending color HEX values from app to ring
<br>3 - Sending color HEX values and times from ring to app
<br>3 - Improvements to form of physical ring 
<br>4 - Connecting physical circuit components
<br>5 - Integrating prototype 1 into single microcontroller
<br>6 - Connect to 2nd wearable device and build other device 
