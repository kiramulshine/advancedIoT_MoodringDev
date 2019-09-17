# Sprint 2: Adding Intelligence

### Sprint Catalog

The following features are planned to be implemented before the completion of sprint 2:

<br>1 - Connect app to ring over bluetooth                        [Ongoing]
<br>2 - Send color HEX values from app to ring                    [Ongoing]
<br>3 - Send color HEX values and times from ring to app          [Ongoing]
<br>3 - Improvements to form of physical ring                     [Ongoing]
<br>4 - Connect physical circuit components                       [Ongoing]
<br>5 - Integrate prototype 1 into single microcontroller         [Ongoing]
<br>6 - Build second device                                       [Ongoing]
<br>7 - Connect to second device                                  [Ongoing]


### Sprint Review  

At the end of this sprint we completed most of the tasks we set for ourselves. We created large scale forms of the ring to show function, set up bluetooth connection from ring to phone app, integrated the prototypes from sprint 1 into one microcontroller for each and decided how we will connect the rings going forward. At this point, we're ready to focus on connecting the rings to each other, improving sending data over bluetooth, and creating front end for connection of the rings on the app.


### End of Sprint Prototype

#### Updates to Phone App
<ul>
  <li><a href="https://github.com/kiramulshine/MoodringApp">Link to phone app code</a> 
  <li>Bluetooth connection
  <li>Improved front end</li>
  <li>Show set up/choosing colors on phone</li>
  <li>
</ul>

#### Images of Phone App

<br><img src="https://i.imgur.com/XwfnEG4m.png" title="source: imgur.com" />
<br><img src="https://i.imgur.com/JirX8hDm.png" title="source: imgur.com" />
<br><img src="https://i.imgur.com/GXCNYllm.png" title="source: imgur.com" />
 
 
 #### Updates to Physical Prototype & Circuit
 <ul>
  <br> One of the main objectives for this sprint was to create a prototype that showed how the physical aspects of the ring would function, specifically how the middle band would spin, where the capacitive touch inputs would be located, and lastly how the hall effect sensors would be triggered with a magnet.  All of these objectives have been completed.  
  <br>
  <br> Not many improvements have been made to the circuitry for this sprint.  This will be a focus for the next sprint.  The plan is to create a flexible PCB that can be attached to the chassis of the right underneath the middle band.  LEDs, hall effect sensors, and the bluetooth microcontroller can be surface mounted to this PCB.  Lastly, a flexible battery can be incorporated into the ring chassis and connected to the PCB.
 </ul>
 
 #### Wiring Diagram
 
 <br><a href="https://imgur.com/W57jv7J"><img src="https://i.imgur.com/W57jv7J.jpg" title="source: imgur.com" /></a>
 
 #### Images of Physical Prototype
 
 <br><a href="https://imgur.com/QkgCBH1"><img src="https://i.imgur.com/QkgCBH1.png" title="source: imgur.com" /></a>
 <br><a href="https://imgur.com/bcgUlrj"><img src="https://i.imgur.com/bcgUlrj.jpg" title="source: imgur.com" /></a>
 <br><a href="https://imgur.com/KBZweYW"><img src="https://i.imgur.com/KBZweYW.jpg" title="source: imgur.com" /></a>
 <br><a href="https://imgur.com/53xhpuJ"><img src="https://i.imgur.com/53xhpuJ.jpg" title="source: imgur.com" /></a>
 
 #### Code for Physical Prototype & Circuit
 
 <a href="https://github.com/daraghbyrne/advancediot2018/tree/moodRing-dev/students/kmulshin_crwillia/sprint-2/Proto2">Link to Code</a>
 
 #### Video of Physical Prototype &Circuit
 
 <a href="https://youtu.be/GfTulJOd1cg">Link to Video Demo</a>
 

### Sprint Retrospective 

#### Difficulties:
<ul>
  <li> Choosing the right board (we ended on Bluefruit for now) </li>
  <li> Connecting app over Bluetooth (learning curve to debug the JS and apply to our project)</li>
  <li> Capacitive touch library implementation with what we were using </li>
  <li> 3D printing (timing - took forever to get this printed since the queue was so long)</li>
  <li> The tolerance of the 3D printing machine played a role in the quality of function of the physical protoytpes.  The ring that was printed on the NVBot required lots of filing to get everything to fit and there is binding between the middle band and the ball bearings.  That ring that was printed on the Ultimaker required no filing or sanding and the middle band spins very smoothly.</li>
 </ul>
 
#### What went well
<br> We like working on this project, so that is helpful to get things done. We help each other out when stuck and let each other have space to figure things out if we want to learn to do it on our own. 
<br>

#### What could we do differently?

<br> We could spend more time working together in the same space, because it is more efficient to troubleshoot problems this way
<br>
<br>

### Next

#### Priorities
<ul>
  <li>Connecting the rings to each other</li>
  <li>Exchanging data between rings</li>
  <li>Updates to the phone app (research and build functionality to connect to another person's ring)</li>
  <li>Front end of app for "after set up" use</li>
  <li>Improve form factor</li>
  <li>Design and source flexible PCB</li>
  <li>Source surface mount LEDs, hall effect sensors, and bluetooth chip</li>
  <li>Source flexible battery</li>
 </ul>

