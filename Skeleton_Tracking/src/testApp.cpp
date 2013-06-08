#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
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
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(1); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(1); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    
    kinectHardwareDriver.setup();
    kinectHardwareDriver.setTiltAngle(0);
    
    
    
    // you can alternatively create a 'base' user class
    //    ofxOpenNIUser user;
    //    user.setUseMaskTexture(true);
    //    user.setUsePointCloud(true);
    //    user.setPointCloudDrawSize(2);
    //    user.setPointCloudResolution(2);
    //    openNIDevice.setBaseUserClass(user);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    
    kinWidth = openNIDevice.getWidth();
    kinHeight = openNIDevice.getHeight();
    
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(ofColor::gray);
    
    //ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
    //openNIDevice.drawDebug();
    //ofPopMatrix();
    
    ofPushMatrix();
    // use a blend mode so we can see 'through' the mask(s)
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    grid.setColor(ofColor::seaGreen);
    grid.setCorners(kinWidth, kinHeight);
    grid.drawGrid();
    
    
    
    string msg;
    int i;
    
    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();
    // iterate through users
    for (int j = 0; j < numUsers; j++){
        
        // get a reference to this user
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(j);
        
        // draw the mask texture for this user
        //user.drawMask();
        user.drawSkeleton();
        
        // pull torso joint coordinates
        ofxOpenNIJoint torsoJnt = user.getJoint(JOINT_TORSO);
        ofPoint torsoPnt = torsoJnt.getProjectivePosition();
        
        // pull right hand joint coordinates
        ofxOpenNIJoint rightHandJnt = user.getJoint(JOINT_RIGHT_HAND);
        ofPoint rightHandPnt = rightHandJnt.getProjectivePosition();
        ofPoint rhWrldPos = rightHandJnt.getWorldPosition();
        rightHand.Jnt = rightHandJnt;
        rightHand.referencePnt = torsoPnt;
        rightHand.updateHand();
        
        // pull left hand joint coordinates
        ofxOpenNIJoint leftHandJnt = user.getJoint(JOINT_LEFT_HAND);
        ofPoint leftHandPnt = leftHandJnt.getProjectivePosition();
        ofPoint lhWrldPos = leftHandJnt.getWorldPosition();
        leftHand.Jnt = leftHandJnt;
        leftHand.referencePnt = torsoPnt;
        leftHand.updateHand();
        
        // pull shoulder coordinates
        // left
        ofxOpenNIJoint leftShldrJnt = user.getJoint(JOINT_LEFT_SHOULDER);
        ofPoint leftShldrPos = leftShldrJnt.getProjectivePosition();
        ofPoint leftShldrWrld = leftShldrJnt.getWorldPosition();
        // right
        ofxOpenNIJoint rightShldrJnt = user.getJoint(JOINT_RIGHT_SHOULDER);
        ofPoint rightShldrPos = rightShldrJnt.getProjectivePosition();
        ofPoint rightShldrWrld = rightShldrJnt.getWorldPosition();
        
        /*
         // you can also access the pixel and texture references individually:
         
         // TEXTURE REFERENCE
         //ofTexture & tex = user.getMaskTextureReference();
         // do something with texture...
         
         // PIXEL REFERENCE
         //ofPixels & pix = user.getMaskPixels();
         // do something with the pixels...
         
         // and point clouds:
         */
        
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
         */
        
        ofPushMatrix();
        
        // yellow for right hand
        ofSetColor(ofColor::yellow);
        ofCircle(rightHandPnt.x, rightHandPnt.y,10);
        
        // blue for left hand
        ofSetColor(ofColor::blue);
        ofCircle(leftHandPnt.x, leftHandPnt.y,10);
        
        // red for torso
        ofSetColor(ofColor::red);
        ofCircle(torsoPnt.x, torsoPnt.y, 10);
        
        // draw where the clicks happen
        rightHand.drawClk();
        leftHand.drawClk();
        
        ofPopMatrix();
        
        verdana.drawString(msg, 20, 690);
        
        ////////////////////////////
        // set button coordinates //
        ////////////////////////////
        
        for(i=0; i<numButts; i++){
            butt[i].setButtDimension(buttWidth, buttHeight);
            butt[i].setRefPnt(torsoPnt);
            butt[i].setOffsetDistance(leftShldrPos, rightShldrPos);
        }
        
        butt[0].setButtCorner(torsoPnt.x-butt[0].offSetDist, torsoPnt.y-2*buttHeight+10);
        butt[1].setButtCorner(torsoPnt.x-butt[1].offSetDist, torsoPnt.y-buttHeight/2);
        butt[2].setButtCorner(torsoPnt.x-butt[2].offSetDist, torsoPnt.y+1.0*buttHeight-10);
        butt[3].setButtCorner(torsoPnt.x+butt[3].offSetDist-buttWidth, torsoPnt.y-2*buttHeight+10);
        butt[4].setButtCorner(torsoPnt.x+butt[4].offSetDist-buttWidth, torsoPnt.y-buttHeight/2);
        butt[5].setButtCorner(torsoPnt.x+butt[5].offSetDist-buttWidth, torsoPnt.y+1.0*buttHeight-10);
        
        // HAND CHECK ( is hand in butt )
        for(i=0; i<numButts; i++){
            //left hand buttons
            if(i<numButts/2){
                butt[i].setHandCheck(leftHand.clkPos);
                if(butt[i].handCheck)
                    butt[i].pushButton(leftHand.clkCounter);
            }
            //right hand buttons
            else{
                butt[i].setHandCheck(rightHand.clkPos);
                if(butt[i].handCheck)
                    butt[i].pushButton(rightHand.clkCounter);
            }
            butt[i].drawButton();
        }
        
        ofSetColor(ofColor::red);
        msg = "RH z position = " + ofToString(rightHand.PosAvg);
        verdana.drawString(msg,20,660);
        msg = "RH velocity = " + ofToString(rightHand.Vel[0].z);
        verdana.drawString(msg, 20, 690);
        msg = "RH clkChk = " + ofToString(rightHand.clkChk);
        verdana.drawString(msg, 20, 720);
        msg = "FrwdChk = " + ofToString(rightHand.frwdChk);
        verdana.drawString(msg, 20, 750);
        msg = "BackChk = " + ofToString(rightHand.bckChk);
        verdana.drawString(msg, 20, 780);
        msg = "XYVel = " + ofToString(rightHand.VelAvg);
        verdana.drawString(msg,20,810);
        
    }
    //ofDisableBlendMode();
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(ofColor::red);
    msg = "Number of Users = " + ofToString(numUsers);
    
    verdana.drawString(msg,20,630);
    
    // draw some info regarding frame counts etc
    ofSetColor(ofColor::green);
	msg = "MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
	verdana.drawString(msg, 20, 600);
    
    //msg = "kinWidth = " + ofToString(kinWidth) + "  kinHeight = " + ofToString(kinHeight);
    //verdana.drawString(msg, 20, 660);
    
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
bool testApp::buttonCheck(ofPoint pnt, ofPoint btnPnt){
    if ( pnt.x > btnPnt.x && pnt.x < btnPnt.x+buttWidth && pnt.y > btnPnt.y && pnt.y < btnPnt.y+buttHeight)
        return true;
    else {
        return false;
    }
}

//--------------------------------------------------------------
