# Sprint 3: Connectivity

### Sprint Catalog

<ul>
  <li>Connect bluetooth plugin to app and app to ring</li>
  <li>Send data from app to ring and vice versa</li>
  <li>Get bluefruit app working for demo as back up for sending RGB values back and forth from app to ring </li>
  <li>Capacitive touch refinement (make faster and more accurate)</li>
  <li>Functionality of motion of middle band (when turn clockwise, show your last reported emotion, when turn counterclockwise, show partners</li>
  <li>Source and set up ability to send data from app to a cloud service to be read by the other app</li>
  <li>Make physical form wireless</li>
  <li>Build and order the flexible board</li>
  <li>Create file to be printed for small prototype for Sprint 4, down to 1 inch in diameter </li>
 </ul>
  

### Sprint Review  

<br> We were able to build out all items, except for the connection of the rings to each other through the app. This proved to be much more complicated than we anticipated. We do, however, have functionality of connection of the apps to a cloud service (Adafruit IO) and also of the apps sending data to the rings. What we need to set up now is sending data from the ring to the app, the app grabbing information from the cloud, and then sending it back to their respective rings. We may not develop this out during the course of this project, but we plan to after the semester ends as we continue this project.
<br><br>Though the ring doesn't look much different, it functions much more smoothly and how we ultimately want it to. For the next sprint we are ready to print the small form factor, but this is delayed because of backed up printers. Also, we will not be able to set that up until the flexible board arrives, which likely won't be until after the final demo.  Even though the board won't be ready for the final demo, we would like to demonstrate  our next steps, and explain how we will fit all of our components into a ring sized form factor.

### End of Sprint Prototype

#### Code
<a href="https://github.com/daraghbyrne/advancediot2018/blob/moodRing-dev/students/kmulshin_crwillia/sprint-3/Sprint3.ino">Linke to Arduino Code</a>
<br><br><a href="https://github.com/kiramulshine/MoodringApp">Link to App Code (HTML, CSS, JavaScript)</a>

#### Images

<br><b>Physical Prototype</b><br>

<br><img src="https://i.imgur.com/jzFcMivl.jpg" title="source: imgur.com" />
<br><img src="https://i.imgur.com/1iqXFbFl.jpg" title="source: imgur.com" />
<br><img src="https://i.imgur.com/bZ6DKiCl.jpg" title="source: imgur.com" />

<br><b>Phone App Updates</b><br>

<br><b>Connect to ring over bluetooth & update colors/map page:</b><br>
<br><img src="https://i.imgur.com/3DtFrb3l.png" title="source: imgur.com" /><br>
<br><b>Navigate back to other pages from update colors page</b><br>
<br><img src="https://i.imgur.com/ZsgCvJKl.png" title="source: imgur.com" />
<br><b>When bluetooth connection isn't set up and try to send color value update</b><br>
<br><img src="https://i.imgur.com/mePlOLzl.png" title="source: imgur.com" />
<br><b>Sending your stored moodmap data over cloud to Adafruit IO console</b><br>
<br><img src="https://i.imgur.com/ELQZEkwl.png" title="source: imgur.com" /><br>
<br><b>Partner's Moodmap Page (Where data from cloud will be sent to show you his/her moodmap and recently reported moods)</b><br>
<br><img src="https://i.imgur.com/XYO4zWHl.png" title="source: imgur.com" /><br>

#### Videos

<a href="https://youtu.be/uHxGHeBIEQk">Link to Ring Function YouTube Video</a>

<a href="https://www.youtube.com/watch?v=fme155xK1QM">Link to Ring App YouTube Video</a>

#### Circuit Diagrams

<br><a href="https://imgur.com/W57jv7J"><img src="https://i.imgur.com/W57jv7J.jpg" title="source: imgur.com" /></a>


### Sprint Retrospective 

<br>What went well? We still work very well together as a group. Its easier to debug problems when you have support and other eyes to look at code. We also are always in alignment on how to develop the product. 
<br><br>What didn't go well? BLE connection was really tough!!! We can send data to ring now, but not back to app quite yet, but its coming!
<br><br>What could we do differently? Spend more time working all in one place

### Next

#### Priorities
<ul>
  <li>Sending data back to app and displaying it over BLE</li>
  <li>Smaller form factor</li>
  <li>User testing</li>
  <li>Implementing flex board</li>
  <li>Building out more of the Front End in the app</li>
  <li>Trying to get the devices to connect to each other through cloud and app</li>
  <li>Looking into pulling data from the cloud (can push data now)</li>
</ul>
