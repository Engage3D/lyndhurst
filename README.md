Lucid Streaming
=========

Development of a motion controlled video playback system using the Microsoft Kinect and OpenFrameworks. Use of Low-Latency (LoLa) Cameras were used to transmit video feeds of performers from one site in Chattanooga, TN to another. The Performance occured in early August 2013 for the 2nd GigTank Demo Day. The two sites used were the Chattanooga Public Library's 4th Floor and the Chattanooga Theatre Centre.

Video of the proof of concept:
http://vimeo.com/69187410

Video of performance:
http://vimeo.com/71937199

Written in collaboration with Lawton Shoemake, Forrest Pruitt, Bill Brock, and Paul Muren.

Skeleton Tracking makes use of OpenFrameworks and XCode, while the cloud server makes use of Node.JS and Websockets. Here are some general guidelines to set these up; your milage may vary dependent on version of OSX/Xcode and changes in Node.JS/OpenFrameworks following Summer 2013.

== Setting Up the Cloud Server ==
To Run the Cloud Server, place it onto the machine you wish to be your passthrough server. Make sure Node is installed (http://www.nodejs.org/), and then run 'npm install websockets' to add the needed package to the directory. It will attempt to compile the package natively; if this fails, it's okay, but performance issues can sometimes be solved by making sure you have nativly compiled packages.

The cloud server can then be run by running 'node cloudServer.js'.

This server makes use of ports 5250 & 5251. Make sure traffic entering/exiting these ports are not being blocked by a firewall. 

== Setting Up OpenFrameworks ==
Download OpenFrameworks from http://www.openframeworks.cc/ if you do not already have it. Place the directory included in this repository called 'SkeletonTracking' into the <of-root>/apps/myApps/ directory.
You should then be able to simply open the XCode Project provided. 
Note: 
While this project was designed only for use between the Chattanooga Public Library and the Theatre Centre, it can be configured to work between any two points.You can do this by modifying the lines containing the IP Addresses at the top of the testApp.h file to match your network configuration.

For a visualzation of how the network needs to be routed, see the included PDF in the docs/ subdirectory.
