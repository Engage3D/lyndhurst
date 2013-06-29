#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);
    openNIDevice.start();
    
    // set properties for all user masks and point clouds
    //openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    openNIDevice.setUseMaskTextureAllUsers(false); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(false);
    openNIDevice.setPointCloudDrawSizeAllUsers(1); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(1); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    kinectTiltAngle = 10;
    kinectHardwareDriver.setup();
    kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
    kinWidth = openNIDevice.getWidth();
    kinHeight = openNIDevice.getHeight();
    
    
    
    // you can alternatively create a 'base' user class
    //    ofxOpenNIUser user;
    //    user.setUseMaskTexture(true);
    //    user.setUsePointCloud(true);
    //    user.setPointCloudDrawSize(2);
    //    user.setPointCloudResolution(2);
    //    openNIDevice.setBaseUserClass(user);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 18);
    
    origin.set(0.0,0.0);
    leftHand.handColor = ofColor::blue;
    rightHand.handColor = ofColor::yellow;
    butt[testButton].videoName = "movies/fingers.mov";
    
    butt[0].videoName  = "movies/01.mov";
    butt[1].videoName  = "movies/02.mov";
    butt[2].videoName  = "movies/03.mov";
    butt[3].videoName  = "movies/04.mov";
    butt[4].videoName  = "movies/05.mov";
    butt[5].videoName  = "movies/06.mov";
    butt[6].videoName  = "movies/07.mov";
    butt[7].videoName  = "movies/08.mov";
    butt[8].videoName  = "movies/09.mov";
    butt[9].videoName  = "movies/10.mov";
    butt[10].videoName = "movies/11.mov";
    butt[11].videoName = "movies/12.mov";
    butt[12].videoName = "movies/13.mov";
    butt[13].videoName = "movies/14.mov";
    butt[14].videoName = "movies/15.mov";
    butt[15].videoName = "movies/16.mov";
    butt[16].videoName = "movies/17.mov";
    butt[17].videoName = "movies/18.mov";
    butt[18].videoName = "movies/19.mov";
    butt[19].videoName = "movies/20.mov";
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    
    numUsers = openNIDevice.getNumTrackedUsers();
    if ( numUsers > 0 ) {
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(0);
        // pull head joint coordinates
        headJnt = user.getJoint(JOINT_HEAD);
        headPnt = headJnt.getProjectivePosition();
        
        // pull neck joint coordinates
        neckJnt = user.getJoint(JOINT_NECK);
        neckPnt = neckJnt.getProjectivePosition();
        
        // pull torso joint coordinates
        torsoJnt = user.getJoint(JOINT_TORSO);
        torsoPnt = torsoJnt.getProjectivePosition();
        
        // pull left Knee joint coordinates
        leftKneeJnt = user.getJoint(JOINT_LEFT_KNEE);
        leftKneePnt = leftKneeJnt.getProjectivePosition();
        
        //pull right knee joint coordinates
        rightKneeJnt = user.getJoint(JOINT_RIGHT_KNEE);
        rightKneePnt = rightKneeJnt.getProjectivePosition();
        
        // pull left hip joint coordinates
        leftHipJnt = user.getJoint(JOINT_LEFT_HIP);
        leftHipPnt = leftHipJnt.getProjectivePosition();
        
        // pull right hip coordinates
        rightHipJnt = user.getJoint(JOINT_RIGHT_HIP);
        rightHipPnt = rightHipJnt.getProjectivePosition();
        
        
        // pull right hand joint coordinates
        rightHandJnt = user.getJoint(JOINT_RIGHT_HAND);
        rightHandPnt = rightHandJnt.getProjectivePosition();
        rhWrldPos = rightHandJnt.getWorldPosition();
        rightHand.Jnt = rightHandJnt;
        rightHand.referencePnt = origin;
        rightHand.update();
        
        // pull left hand joint coordinates
        leftHandJnt = user.getJoint(JOINT_LEFT_HAND);
        leftHandPnt = leftHandJnt.getProjectivePosition();
        lhWrldPos = leftHandJnt.getWorldPosition();
        leftHand.Jnt = leftHandJnt;
        leftHand.referencePnt = origin;
        leftHand.update();
        
        videoTriggerPnt = 0.25*(leftKneePnt + rightKneePnt + leftHipPnt + rightHipPnt);
        
    }
    
    
    
    ////////////////////
    // ESTABLISH GRID //
    ////////////////////
    grid.setColor(ofColor::blueViolet);
    grid.setCorners(kinWidth, kinHeight);
    
    
    
    ////////////////////////////
    // set button coordinates //
    ////////////////////////////
    buttWidth = kinWidth/grid.Xdim;
    buttHeight = kinHeight/grid.Ydim;
    for ( i=0; i<grid.Ydim; i++ ) { // height
        for ( j=0; j<grid.Xdim; j++ ) { // width
            k = i*grid.Xdim + j;
            butt[k].setButtDimension(buttWidth, buttHeight);
            butt[k].setButtCorner(j*buttWidth, i*buttHeight);
            butt[k].label = k;
        }
    }
    
    ////////////////////////////////
    // CHECK BUTTONS FOR ACTIVITY //
    ////////////////////////////////
    // HAND CHECK ( is hand in butt )
    for(int i=0; i<grid.Xdim*grid.Ydim; i++){
#if HANDCHECK
        // check left hand
        butt[i].setHandCheck(leftHand.clkPos);
        if(butt[i].handCheck)
            butt[i].pushButton(leftHand.clkCounter);
        // check right hand
        butt[i].setHandCheck(rightHand.clkPos);
        if(butt[i].handCheck)
            butt[i].pushButton(rightHand.clkCounter);
#endif 
        // check for overhead
        butt[i].setHandCheck(videoTriggerPnt);
        overheadChk = overheadCheck(leftHandPnt, rightHandPnt, headPnt, neckPnt);
        if ( butt[i].handCheck ) {
            //if ( curr_video == -1 && !isVideoLoaded ) {
            //    videoPlayer.loadMovie(butt[testButton].videoName);
            //    isVideoLoaded = true;
            //}
            if ( overheadChk && !butt[i].isOn ) {
                butt[i].pushButton(overheadClickCounter);
                overheadClickCounter++;
            }
            if ( !overheadChk && butt[i].isOn ) {
                butt[i].pushButton(overheadClickCounter);
                overheadClickCounter++;
            }
        }
    }
    
    
    ///////////////////////////
    // HANDLE VIDEO PLAYBACK //
    ///////////////////////////
    //if ( butt[testButton].isOn && !isVideoPlaying ) {
    //    curr_video = 0;
    //    videoPlayer.loadMovie(butt[testButton].videoName);
    //    videoPlayer.setLoopState(OF_LOOP_NONE);
    //    videoImage.allocate(videoPlayer.getWidth(), videoPlayer.getHeight(), OF_IMAGE_COLOR);
    //    videoPlayer.play();
    //    isVideoPlaying = true;
    //}
    
    for ( i=0; i<numMovies; i++ ) {
        if ( butt[i].isOn && !isVideoPlaying ) {
            curr_video = 0;
            videoPlayer.loadMovie(butt[i].videoName);
            videoPlayer.setLoopState(OF_LOOP_NONE);
            videoImage.allocate(videoPlayer.getWidth(), videoPlayer.getHeight(), OF_IMAGE_COLOR);
            videoPlayer.play();
            isVideoPlaying = true;
        }
        
    }
    
    videoPlayer.update();
    if ( curr_video > -1 ) {
        videoImage.setFromPixels(videoPlayer.getPixels(), videoPlayer.getWidth(), videoPlayer.getHeight(), OF_IMAGE_COLOR);
    }
    
    if ( videoPlayer.getIsMovieDone() ) {
        videoPlayer.close();
        videoImage.clear();
        curr_video = -1;
        isVideoPlaying = false;
        isVideoLoaded = false;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(ofColor::gray);
    
    ofPushMatrix();
    //draw debug (ie., image, depth, skeleton)
    openNIDevice.drawDebug();
    ofPopMatrix();
    
    
    imageWidth = rightHandPnt.x - leftHandPnt.x;
    imageHeight = rightHandPnt.x - leftHandPnt.x;
    imageLocation.set(leftHandPnt.x, 0.5*(rightHandPnt.y + leftHandPnt.y) - imageHeight);
    
    ofSetColor(ofColor::white);
    videoImage.resize(imageWidth, imageHeight);
    
    /////////////////////
    // DRAW EVERYTHING //
    /////////////////////
    ofPushMatrix();
    ofTranslate(0.0, windowHeight - kinHeight);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);  // see through the grid and buttons
    
    grid.drawGrid();
    
    for ( i=0; i<grid.Ydim; i++ ) {
        for ( j=0; j<grid.Xdim; j++ ) {
            k = i*grid.Xdim + j;
            butt[k].drawButton();
        }
    }
    
    //ofDisableBlendMode();
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofTranslate(0.0, windowHeight - kinHeight);
    // CHECK FOR USERS
    if ( numUsers > 0 ) {
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(0); // get a reference to this user
        
        // draw the mask texture for this user
        //user.drawMask();
        user.drawSkeleton();
        
        // red for torso
        ofSetColor(ofColor::red);
        ofCircle(torsoPnt.x, torsoPnt.y, 10);
        
        rightHand.drawHand();
        leftHand.drawHand();
        rightHand.drawClk();
        leftHand.drawClk();
        
        ofSetColor(ofColor::green);
        ofCircle(videoTriggerPnt.x, videoTriggerPnt.y, 10);
    }
    
    ofSetColor(ofColor::white);
    videoImage.draw(imageLocation);
    
    ofPopMatrix();
    
    /////////////////////////
    // FEEDBACK STATEMENTS //
    /////////////////////////
    ofSetColor(ofColor::red);
    statementCounter = 0;
    
    msg[0] = ofToString(ofGetElapsedTimeMillis());
    msg[1] = ofToString(ofGetFrameRate());
    msg[2] = ofToString(openNIDevice.getFrameRate());
    
    dbplx = windowWidth - 500 ;
    
    MSG = "MILLIS: " + msg[0];
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "FPS: " + msg[1];
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "Device FPS: " + msg[2];
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "Number of Users = " + ofToString(numUsers);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "RH z position = " + ofToString(rightHand.PosAvg);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "RH velocity = " + ofToString(rightHand.Vel[0].z);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "RH clkChk = " + ofToString(rightHand.clkChk);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "FrwdChk = " + ofToString(rightHand.frwdChk);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "BackChk = " + ofToString(rightHand.bckChk);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "XYVel = " + ofToString(rightHand.VelAvg);
    verdana.drawString(MSG,dbplx,dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "button 5 ClkInt = " + ofToString(butt[5].buttClkInt);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "overheadClickCounter = " + ofToString(overheadClickCounter);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "window width & height: " + ofToString(windowWidth) + "  " + ofToString(windowHeight);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "kinect width & height: " + ofToString(kinWidth) + "  " + ofToString(kinHeight);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "kinect tilt angle: " + ofToString(kinectTiltAngle);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
}

//--------------------------------------------------------------
bool testApp::overheadCheck(ofPoint leftHand, ofPoint rightHand, ofPoint head, ofPoint neck){
    //int height = head.y - (neck.y - head.y);
    int height = head.y - 10;
    if ( leftHand.y < height && rightHand.y < height ) {
        return true;
    }
    else {
        return false;
    }
}


//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch (key) {
        case OF_KEY_UP:
            kinectTiltAngle = ofClamp(kinectTiltAngle + 1, -30,30);
            kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
            break;
            
        case OF_KEY_DOWN:
            kinectTiltAngle = ofClamp(kinectTiltAngle - 1, -30, 30);
            kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------

////////////////
// EXTRA CODE //
//////////////// 
#if 0
/*
 
 // pull shoulder coordinates
 // left
 ofxOpenNIJoint leftShldrJnt = user.getJoint(JOINT_LEFT_SHOULDER);
 ofPoint leftShldrPos = leftShldrJnt.getProjectivePosition();
 ofPoint leftShldrWrld = leftShldrJnt.getWorldPosition();
 // right
 ofxOpenNIJoint rightShldrJnt = user.getJoint(JOINT_RIGHT_SHOULDER);
 ofPoint rightShldrPos = rightShldrJnt.getProjectivePosition();
 ofPoint rightShldrWrld = rightShldrJnt.getWorldPosition();
 */ // pull shoulder info

/*
 // you can also access the pixel and texture references individually:
 
 // TEXTURE REFERENCE
 //ofTexture & tex = user.getMaskTextureReference();
 // do something with texture...
 
 // PIXEL REFERENCE
 //ofPixels & pix = user.getMaskPixels();
 // do something with the pixels...
 
 // and point clouds:
 */ // extra stuff from the original example

/*
 ofPushMatrix();
 // move it a bit more central
 ofTranslate(320, 240, 10);
 user.drawPointCloud();
 
 // you can also access the mesh:
 
 // MESH REFERENCE
 //ofMesh & mesh = user.getPointCloud();
 // do something with the point cloud mesh
 
 ofPopMatrix();
 */ // extra stuff from the original example

#endif
