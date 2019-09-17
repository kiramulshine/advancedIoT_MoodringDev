# Functional Specification

<b>INITIAL SETUP</b>

<ul>
<li>Wearer purchases ring and has physical product.</li>
<li>Wearer downloads app connected to ring.</li>
<li>Wearer launches the application.</li>
<li>System prompts wearer to create a profile (Required: Username (email), Password, Location -- Optional: Age, Gender).</li>
<li>Wearer logs in for first time (Username & password).</li>
<li>Wearer prompted to give Apple Health, Google Fit, or FitBit data access to application.</li> 
<li>Wearer told to turn ring on and connect to their ring via bluetooth.</li>
<li>Wearer prompted to pair application to a partner’s ring by entering partner’s username.</li>
<li>Wearer’s partner is sent a notification to accept pairing on their application.</li>
  </ul>

<b>SOFTWARE APP</b>

Wearer prompted to map the 5 basic emotions to colors
<ul>
<li>Color wheel is presented.  Pointer starts in the center of the donut, which is “neutral” or “transparent”.</li>
<li>There is an option for a “default” color.  If this is selected, a region of the color wheel is selected according to legacy emotion/color mapping research.</li>
</ul>

Wearer given option to add up to 3 more specific emotions.  Emotions are entered in a text box.
<ul>
  <li>Color wheel is presented.  Pointer starts in the center of the donut, which is “neutral” or “transparent”.
  Emotions are sorted in the “emotion list” according to color wavelength. </li>
  <li>Lower wavelengths (reds) appear near the top of the list.  Higher wavelengths (purples) appear near the bottom of the list. </li>
</ul>

There is an option to view the partner’s emotion list, so that the wearer will be able to know the colors that are mapped to their partner’s selected emotion.  This list is display only.


<b>RING FUNCTION</b>

When the wearer puts the ring on and has not selected their current mood, or has not used the phone application to map emotions to colors, it displays white light when the middle band is spun.

Wearer documents first emotion by touching bottom band of ring and scrolling left or right through single colors to align with the mood they are feeling currently.
<ul>
  <li>“Scrolling” selection is achieved by using capacitive touch sensors.
    Wearer stops on the color that aligns with his/her emotion.</li>
<li>One RGB LED changes color according to the emotion that is selected.  The user receives haptic/vibratory feedback when the mood selection changes.</li>
</ul>

Wearer spins the middle band clockwise to see the last color/mood he/she reported
<ul>
  <li>LED lights in the band turn on only while the band is spinning.  The color of the LED corresponds to the mood that is selected.</li>
  <li>Ball bearings inside of an internal track ensure alignment and smooth spinning of the middle band.</li>
  <li>A magnet in the middle band induces a current in the ring frame.  The direction of the current signifies the direction that the ring is spinning.</li>
  <li>When a current is sensed, the ring sends the mood color data to the linked phone via bluetooth.  The phone then publishes a particle event.  The event contains data corresponding to the emotion name and color.
Wearer spins the band right to see the color/mood of his/her partner’s ring.</li>
  <li>Wearer spins the middle band counterclockwise to display the mood color that is associated with the entangled ring. LED lights in the band turn on only while the band is spinning.  The color of the LED corresponds to the mood that is selected in the entangled ring.</li>
  <li>The ring is subscribed to the entangled rings published events.  The emotion ring and color is read from the event data and transferred to the ring via bluetooth connection.</li>
</ul>

Wearer looks at software application to see history of the moods he/she is documenting.


<b>OVERVIEW OF ACTIONS</b>

<ul>
<li>Wearers will scroll through colors on the lower band of the ring to log their moods.</li>
<li>Wearers will spin the center band of the ring left to view the last mood color they logged.</li>
<li>Wearers will spin the center band of the ring right to view the last mood color their partner logged.</li>
<li>Wearers will use the software application to view their mood history.</li>
<li>Wearers will use the software application to view their partner’s mood history.</li>
<li>Wearers will use the software application to view their combined mood history (with partner).</li>
<li>Wearers will use the software application to map colors to different moods.</li>
</ul>

<b>OVERVIEW OF INTENTIONS</b>

<ul>
<li>As a wearer, I want to share my mood changes with my partner, so we can have a better relationship.</li>
<li>As a wearer, I want to note my moods, so I can learn why and when they change over time.</li>
<li>As a wearer, I want to see the moods of my partner, so I can support her to be happier, healthier and more successful.</li>
<li>As a wearer, I want to share my mood with my partner, so she can support me to be happier, healthier and more successful.</li>
<li>As a wearer, I want to share my moods, so I can acknowledge them and how they are influencing my current state.</li>
<li>As a wearer, I want to track my moods, so I can be more mindful.</li>
<li>As a wearer, I want to track my partner’s mood, so I can help her be more mindful.</li>
</ul>

## Product Backlog


Future iterations will incorporate a digital engraving (can but updated in the app)
<ul>
<li>Rotating right will show your personal engraving</li>
<li>Rotating left will show a message to you from your partner</li>
</ul>

Secure data exchange by connecting to blockchain (such as Ethereum or Hyperledger)

Beautiful, interactive data visualizations in the software application, which I am learning to build with Apache Cordova (suggestions to make this faster/easier?)


