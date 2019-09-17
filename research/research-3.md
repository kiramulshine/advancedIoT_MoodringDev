# Research Sprint 3
## Connectivity and Preferences Research

_Prompt: Identify 3-5 opportunities around connectivity, networking or user preferences that would enhance your application and that you could potentially integrate.  Plan and consider the other devices you might like to allow access to your products data and consider how that happens.


### Summary

We would like to incorporate ways to connect that are <b>not intrusive</b>, so we are avoiding the typical ways people connect now, like push notifications to their phones. The point of the Moodring2.0 is to foster emotional reflection and awareness with oneself and to improve the emotional relationships of partners, so that relationships grow stronger and people make more rational decisions because they are more emotionally aware. 

There are a few opportunities to use the data collected from the Moodrings to increase emotional reflection and awareness in a non-intrusive way.

### Findings

<br>Email: This can be done through HTML Forms</br>
<ul>
  <li>Could prompt wearer to check in on emotional changes during the week for self &/or partner</li>
  <li>Could send weekly emotion history info, so this is where wearers would get updates instead of going into the phone app. The idea would be to keep them out of their phones and just using the ring as much as we can.</li>
</ul>

<br><a href="https://www.npmjs.com/package/cordova-rds-plugin-x-toast">Non-Intrusive Push Notifications (buttonless) in the app</a></br>

<br><a href="https://developer.apple.com/documentation/healthkit"> Apple Healthkit integration </a></br>

<br>Haptic feedback/reminders

### Relevance

<br><b>Email</b> or <b>non-intrusive push notifications</b> are relevant so that we can provide seamless and useful feedback to wearers about their emotions and those of their partners. This will support us to meet our goals of improving emotional understanding for individuals and understanding and relationships with their partners.</br>

<br>Aligning our app with the <b>Healthkit for Apple </b>will make it so that we can show what emotions are reported after certain tracked "health activities", i.e. what emotion was reported when you woke up, what emotion was reported after you ran, etc.</br>

<br><b>Haptic feedback </b>, like pressure or vibration in the physical ring, could be used to remind people to track their mood or when a partner updates his/her moods (in replace of push notifications)</br>

#### API Description

In order for the Moodring2.0 to communicate with other devices in the ways that we have described, there are a few key variables that we would need to identify.  These include:

<li>emotion_label[x]: The label of the emotion used in the emotion map on the phone application.  Stored as String in an array as 5-7 emotions can be mapped.</li>
<li>emotion_color[x]: The color of the emotion used in the emotion map on the phone application.  Stored as a HEX value in an array as 5-7 emotions can be mapped.</li>
<li>emotion_select_time(emotion_label[x], emotion_color[x]):  The time at which an emotion is selected on the ring.  This variable also records the emotion label and color of the selected emotion.</li>
<li>emotion_self_report_time(emotion_label[x], emotion_color[x]):  The time at which the middle band on the ring is spun to display the current emotion.  This variable also records the emotion label and color of the reported self emotion.</li>
<li>emotion_partner_report_time(emotion_label[x], emotion_color[x]): The time at which the middle band of the ring is spun to display the emotion their partner.  This variable also records the emotion label and color of the reported partner emotion.</li>

#### API Usage

The API variables could be passed to the HTML forms and Apple Healthkit to build a time-based mood map.  Data related to which emotions the user as well as their partner were feeling along with the time at which they were feeling those emotions can be recorded.  Trends can be mapped and tied to related events or activities.
