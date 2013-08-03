#include "testApp.h"

#define FRAMERATE 30

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetFrameRate(FRAMERATE);
    
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
    openNIDevice.setMaxNumUsers(1);
    
    kinectTiltAngle = 3;
    kinectHardwareDriver.setup();
    kinectHardwareDriver.setTiltAngle(kinectTiltAngle);
    kinWidth = openNIDevice.getWidth();
    kinHeight = openNIDevice.getHeight();
    
    // UserTracked offsets
    UserTrackingOffsetLeft=100;
    UserTrackingOffsetRight=kinWidth-100;
    UserTrackingOffsetTop=100;
    UserTrackingOffsetBottom=kinHeight-100;
    
    ///////////////////////////////////
    // ESTABLISH LOCAL & REMOTE GRID //
    ///////////////////////////////////
    LocalGrid.setColor(ofColor::white);
    LocalGrid.setCorners(30,kinWidth-30,0.0,kinHeight);
    RemoteGrid.setColor(ofColor::black);
    RemoteGrid.setCorners(0.0, kinWidth, 0.0, kinHeight);
    
    ////////////////////////////
    // set button coordinates //
    ////////////////////////////
    buttWidth = LocalGrid.Xstride;
    buttHeight = LocalGrid.Ystride;
    for ( i=0; i<LocalGrid.Ydim; i++ ) { // height
        for ( j=0; j<LocalGrid.Xdim; j++ ) { // width
            k = i*LocalGrid.Xdim + j;
            butt[k].setButtDimension(buttWidth, buttHeight);
            butt[k].setButtCorner(j*buttWidth + LocalGrid.left, i*buttHeight + LocalGrid.top);
            butt[k].color = backgroundColor;
            butt[k].inactiveColor = backgroundColor;
            butt[k].label = k;
        }
    }
    
    // you can alternatively create a 'base' user class
    //    ofxOpenNIUser user;
    //    user.setUseMaskTexture(true);
    //    user.setUsePointCloud(true);
    //    user.setPointCloudDrawSize(2);
    //    user.setPointCloudResolution(2);
    //    openNIDevice.setBaseUserClass(user);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 18);
    
    Tri[0].set(0.0, 20.0);
    Tri[1].set(20.0, 0.0);
    Tri[2].set(0.0, 0.0);
    RecogTri[0].set(kinWidth, 20.0);
    RecogTri[1].set(kinWidth, 0.0);
    RecogTri[2].set(kinWidth-20.0, 0.0);
    
    origin.set(0.0,0.0);
    leftHand.handColor = ofColor::blue;
    rightHand.handColor = ofColor::yellow;
    
    //ball.b_Toss = true;
    //ball.screen_height = windowHeight;
    //ball.screen_width = windowWidth;
    //ball.Position.set(origin);
    
    /* Asian Movies */
    movie[0].name =  "movies/01.mov";
    movie[1].name =  "movies/02.mov";
    movie[2].name =  "movies/03.mov";
    movie[3].name =  "movies/04.mov";
    movie[4].name =  "movies/05.mov";
    movie[5].name =  "movies/06.mov";
    movie[6].name =  "movies/07.mov";
    movie[7].name =  "movies/08.mov";
    movie[8].name =  "movies/09.mov";
    movie[9].name =  "movies/10.mov";
    movie[10].name = "movies/11.mov";
    movie[11].name = "movies/12.mov";
    movie[12].name = "movies/13.mov";
    movie[13].name = "movies/14.mov";
    movie[14].name = "movies/15.mov";
    movie[15].name = "movies/16.mov";
    movie[16].name = "movies/17.mov";
    movie[17].name = "movies/18.mov";
    movie[18].name = "movies/19.mov";
    movie[19].name = "movies/20.mov";
    
    /* sc1 Movies : all roughly 100MB, 11=5MB*/
    movie[7].name = "movies/sc1.1.mov";
    movie[8].name = "movies/sc1.2.mov";
    movie[9].name = "movies/sc1.3.mov";
    movie[10].name = "movies/sc1.4.mov";
    movie[11].name = "movies/sc1.5.mov";
    movie[12].name = "movies/sc1.6.mov";
    movie[13].name = "movies/sc1.7.mov";
    movie[14].name = "movies/sc1.8.mov";
    movie[15].name = "movies/sc1.9.mov";
    movie[16].name = "movies/sc1.10.mov";
    movie[17].name = "movies/sc1.11.mov";
    movie[18].name = "movies/sc1.12.mov";
    
    /* sc2 Movies : all roughly 15 MB, 12=144MB */
    movie[7].name = "movies/sc2.1.mov";
    movie[8].name = "movies/sc2.2.mov";
    movie[9].name = "movies/sc2.3.mov";
    movie[10].name = "movies/sc2.4.mov";
    movie[11].name = "movies/sc2.5.mov";
    movie[12].name = "movies/sc2.6.mov";
    movie[13].name = "movies/sc2.7.mov";
    movie[14].name = "movies/sc2.8.mov";
    movie[15].name = "movies/sc2.9.mov";
    movie[16].name = "movies/sc2.10.mov";
    movie[17].name = "movies/sc2.11.mov";
    movie[18].name = "movies/sc2.12.mov";
    
    for ( i=0; i<20; i++ ) movie[i].load();
    
    PhaseDuration[0] = 0.30*60.0; // boot time for the program nothing can be selected
    PhaseDuration[1] = 60.00*60.0; // first phase of performance; nothing can be selected
    PhaseDuration[2] = 60.00*60.0; // second phase of performance; videos can be played but not sent
    PhaseDuration[3] = 60.00*60.0; // third phase of performance; videos that are stopped will be sent
    PhaseDuration[4] = 60.0*60.0; // fourth phase of performance; videos will be arriving and playing
    
    PhaseTime[0] = PhaseDuration[0];
    for ( i=1; i<5; i++ ) PhaseTime[i] = PhaseTime[i-1] + PhaseDuration[i];
    
    
    b_Phase[0] = true;  
    b_Phase[1] = false;
    b_Phase[2] = false;
    b_Phase[3] = false;
    b_Phase[4] = false;
    
    movsToSend.push_back(-1);
    
    for ( i=0; i<numMovies; i++ ) videoLoaded[i] = false;
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    currentTime = ofGetElapsedTimeMillis()*0.001;
    
    for ( i=0; i<4; i++ ) {
        b_Phase[i+1] = false;
        if ( currentTime > PhaseTime[i] && currentTime < PhaseTime[i+1]) {
            b_Phase[i+1] = true;
            b_Phase[i] = false;
            Phase = i+1;
        }
    }
    
    
    //if ( b_Phase[2] || b_Phase[3] ) {
    if ( !b_Phase[4] ) {
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
            
            //videoTriggerPnt = 0.25*(leftKneePnt + rightKneePnt + leftHipPnt + rightHipPnt);
            videoTriggerPnt = torsoPnt;
            
            if ( torsoPnt.x > UserTrackingOffsetLeft && torsoPnt.x < UserTrackingOffsetRight
                && torsoPnt.y > UserTrackingOffsetTop && torsoPnt.y < UserTrackingOffsetBottom ) {
                isUserTracked = true;
            }
            else {
                isUserTracked=false;
            }
            
        }
        
        ////////////////////////////////
        // CHECK BUTTONS FOR ACTIVITY //
        ////////////////////////////////
        // HAND CHECK ( is hand in butt )
        for( i=0; i<LocalGrid.Xdim*LocalGrid.Ydim; i++){
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
            if ( butt[i].handCheck && curr_video == -1 ) {
                // turn button on
                if ( overheadChk && !butt[i].isOn ) {
                    butt[i].pushButton(overheadClickCounter);
                    overheadClickCounter++;
                }
                // turn button off
                if ( !overheadChk && butt[i].isOn ) {
                    butt[i].pushButton(overheadClickCounter);
                    overheadClickCounter++;
                }
            }
            
            // check for overhead stop
            handDistance = sqrt((leftHandPnt.x - rightHandPnt.x)*(leftHandPnt.x - rightHandPnt.x) + (leftHandPnt.y - rightHandPnt.y)*(leftHandPnt.y - rightHandPnt.y));
            if ( handDistance < handDistanceStopTolerance){
                StopCheck = true;
                stopTime = ofGetElapsedTimeMillis()*0.001;
            }
            
            
            
            for ( j=0; j<numButts; j++ )
                if (StopCheck)
                    butt[i].isOn = false;
            
            if ( currentTime - stopTime > playTimeOffset )
                StopCheck = false;
            
            // stop everything
            if ( overheadChk && StopCheck ) {
                if ( b_Phase[3] && movsToSend.back() != curr_video && curr_video != -1 ) {
                    movsToSend.push_back(curr_video);
                }
                localMov.stop();
                isVideoPlaying = false;
                curr_video = -1;
            }
        }
        
        ///////////////////////////
        // HANDLE VIDEO PLAYBACK //
        ///////////////////////////
        imageWidth = rightHandPnt.x - leftHandPnt.x;
        imageHeight = rightHandPnt.x - leftHandPnt.x;
        
        if ( b_Phase[2] || b_Phase[3] ) {
            for ( i=0; i<numMovies; i++ ) {
                if ( butt[i].isOn && !isVideoPlaying ) {
                    curr_video = i;
                    currVideoName = movie[i].name;
                    localMov.name = currVideoName;
                    localMov.load();
                    localMov.play();
                    isVideoPlaying = true;
                }
            }
        }
        
        if ( curr_video != -1){
            localMov.Loc.set(leftHandPnt.x, 0.5*(rightHandPnt.y + leftHandPnt.y) - imageHeight);
            localMov.update();
            
            if ( localMov.Player.getIsMovieDone() ) {
                localMov.stop();
                curr_video = -1;
                isVideoPlaying = false;
                isVideoLoaded = false;
            }
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(backgroundColor);
    ofBackground(backgroundColor);
    
    
    //ofPushMatrix();
    ////draw debug (ie., image, depth, skeleton)
    //openNIDevice.drawDebug();
    //ofPopMatrix();
    
    
    /////////////////////
    // DRAW EVERYTHING //
    /////////////////////
    ofPushMatrix();
    glScaled(-1.0, 1.0, 1.0);
    ofTranslate(-kinWidth, windowHeight - kinHeight);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);  // see through the grid and buttons
    
    LocalGrid.drawGrid();
    
    ofSetColor(ofColor::steelBlue);
    ofLine(UserTrackingOffsetLeft, 0.0, UserTrackingOffsetLeft, kinHeight);
    ofLine(UserTrackingOffsetRight, 0.0, UserTrackingOffsetRight, kinHeight);
    
    for ( i=0; i<LocalGrid.Ydim; i++ ) {
        for ( j=0; j<LocalGrid.Xdim; j++ ) {
            k = i*LocalGrid.Xdim + j;
            butt[k].drawButton();
        }
    }
    
    //ofDisableBlendMode();
    
    // CHECK FOR USERS
    if ( numUsers > 0 && !b_Phase[4] ) {
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(0); // get a reference to this user
        
        // draw the mask texture for this user
        //user.drawMask();
        user.drawSkeleton();
        
        // red for torso
        ofSetColor(ofColor::red);
        ofCircle(torsoPnt.x, torsoPnt.y, 10);
        
        //rightHand.drawHand();
        leftHand.drawHand();
        rightHand.drawClk();
        leftHand.drawClk();
        
        ofSetColor(ofColor::green);
        ofCircle(videoTriggerPnt.x, videoTriggerPnt.y, 10);
    }
    
    ofSetColor(ofColor::white);
    if ( curr_video != -1 && !b_Phase[4]) {
        localMov.Image.resize(imageWidth, imageHeight);
        localMov.Image.draw(localMov.Loc);
    }
    
    //ofSetColor(phaseColors[Phase]);
    //ofTriangle(Tri[0], Tri[1], Tri[2]);
    
    numUsers = openNIDevice.getNumTrackedUsers();
    if ( isUserTracked && openNIDevice.getNumTrackedUsers()==1 ) { ofSetColor(ofColor::green); }
    else if ( numUsers == 1 && !isUserTracked ) { ofSetColor(ofColor::yellow); }
    else if ( numUsers == 0 && !isUserTracked ) { ofSetColor(ofColor::red); }
    ofTriangle(RecogTri[0], RecogTri[1], RecogTri[2]);
    ofSetColor(backgroundColor);
    
    
    ofPopMatrix();
    
    // draw remote grid
    RemoteGrid.drawGrid();
    
    if ( b_Phase[3] || b_Phase[4] ) {
        
        for ( i=1; i<movsToSend.size(); i++ ) {
            j = movsToSend.at(i);
            
            if ( !movie[j].loaded )  movie[j].load();
            if ( !movie[j].playing ) movie[j].play();
            
            movie[j].update();
            movie[j].Loc.set(RemoteGrid.regionCorner[LocToRemMap[j]]);
            movie[j].Image.resize(RemoteGrid.Xstride, RemoteGrid.Ystride);
            ofSetColor(ofColor::white);
            movie[j].Image.draw(movie[j].Loc);
            
        }
        
    }
    
    
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
    
    MSG = "isUserTracked = " + ofToString(isUserTracked);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "torsoPnt = " + ofToString(torsoPnt);
    verdana.drawString(MSG, dbplx, dbply + dbpo*statementCounter);
    statementCounter++;
    
    MSG = "RH velocity = " + ofToString(rightHand.Vel[0].x);
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
    
    MSG = "handDistance: " + ofToString(handDistance);
    verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
    statementCounter++;
    
    MSG = "stopTimeoffset: " + ofToString(ofGetSystemTimeMicros() - stopTime);
    verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
    statementCounter++;
    
    MSG = "stopCheck " + ofToString(StopCheck);
    verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
    statementCounter++;
    
    MSG = "isVideoPlaying: " + ofToString(isVideoPlaying);
    verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
    statementCounter++;
    
    MSG = "currentVideoPlaying: " + ofToString(curr_video);
    verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
    statementCounter++;
    
    for ( i=0; i<5; i++ ) {
        MSG = "b_Phase["+ofToString(i)+"]: " + ofToString(b_Phase[i]);
        verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
        statementCounter++;
    }
    
    MSG = "Phase: " + ofToString(Phase);
    verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
    statementCounter++;
    
    for ( i=0; i<movsToSend.size(); i++ ) {
        MSG = "movsToSend: " + ofToString(movsToSend.at(i));
        verdana.drawString(MSG, dbplx, dbply+dbpo*statementCounter);
        statementCounter++;
    }
    
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
            
        case '0':
            PhaseTime[0] = currentTime + PhaseDuration[0];
            for ( i=1; i<5; i++ ) PhaseTime[i] = PhaseTime[i-1] + PhaseDuration[i];
            b_Phase[0] = true;
            for ( i=1; i<5; i++ )  b_Phase[i] = false;
            
            for ( i=0; i<numMovies; i++ ) movie[i].stop();
            movsToSend.clear();
            movsToSend.push_back(-1);
            
            break;
            
        case '1':
            PhaseTime[0] = 0.0;
            PhaseTime[1] = currentTime + PhaseDuration[1];
            for ( i=2; i<5; i++ ) PhaseTime[i] = PhaseTime[i-1] + PhaseDuration[i];
            for ( i=0; i<5; i++ )  b_Phase[i] = false;
            b_Phase[1] = true;
            
            for ( i=0; i<numMovies; i++ ) movie[i].stop();
            movsToSend.clear();
            movsToSend.push_back(-1);
            
            break;
            
            
        case '2':
            PhaseTime[0] = 0.0;
            PhaseTime[1] = 0.01;
            PhaseTime[2] = currentTime + PhaseDuration[1];
            for ( i=3; i<5; i++ ) PhaseTime[i] = PhaseTime[i-1] + PhaseDuration[i];
            for ( i=0; i<5; i++ )  b_Phase[i] = false;
            b_Phase[2] = true;
            
            for ( i=0; i<numMovies; i++ ) movie[i].stop();
            
            movsToSend.clear();
            movsToSend.push_back(-1);
            
            break;
            
        case '3':
            PhaseTime[0] = 0.0;
            PhaseTime[1] = 0.01;
            PhaseTime[2] = 0.02;
            PhaseTime[3] = currentTime + PhaseDuration[3];
            PhaseTime[4] = currentTime + PhaseDuration[4] + PhaseTime[3];
            for ( i=0; i<5; i++ )  b_Phase[i] = false;
            b_Phase[3] = true;
            
            for ( i=0; i<numMovies; i++ ) movie[i].stop();
            
            movsToSend.clear();
            movsToSend.push_back(-1);
            break;
            
        case '4':
            PhaseTime[0] = 0.0;
            PhaseTime[1] = 0.01;
            PhaseTime[2] = 0.02;
            PhaseTime[3] = 0.03;
            PhaseTime[4] = currentTime + PhaseDuration[4];
            for ( i=0; i<5; i++ )  b_Phase[i] = false;
            b_Phase[1] = true;
            
            for ( i=0; i<numMovies; i++ ) movie[i].stop();
            movsToSend.clear();
            movsToSend.push_back(-1);
            
            break;
            
        case 'r':
            
            openNIDevice.resetUserTracking( 0 , true);
            
            
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
