#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxHardwareDriver.h"

#define NUMPOS 50
#define NUMVEL 49

#define HANDCHECK 0

class BALL{
public:
    int i, j, k;
    bool b_Toss = false;
    bool throwIt_1 = false;         // has the ball achieved a velocity worth throwing?
    bool throwIt_2 = false;         // has the ball achieved a velocity worth throwing and then decreased?
    bool released = false;          // has ball been released
    bool caught = true;             // has bell been caught
    bool stop = false;              // has the overhead stop command be given
    double VelMag[NUMVEL+1];
    double TossVelTol=0.4;
    double maxTossVel = 0.0;
    double dT;
    double screen_height;
    double screen_width;
    double tossWaitTime = 0.5;
    double releaseTime = 0.0;
    double catchTime = 0.0;
    double currentTime = 0.0;
    double playTime = 0.0;
    double somethingIsNotRight=0.0;
    ofPoint VEL[NUMVEL+1];
    ofPoint Pos[NUMPOS+1];
    ofPoint Position;
    ofPoint Velocity;
    ofPoint travelVelocity;
    ofPoint Hand;
    
    void update(){
        
        if ( b_Toss ) {
            
            currentTime = ofGetElapsedTimeMillis()*0.001;       // get current time from system
            dT = ofGetFrameRate();                              // get current timestep
            /* Bump oldest position data and make room
             for most recent. */
            for (i=0; i<NUMPOS; i++)
                Pos[i] = Pos[i+1];
            
            // Store most recent position data.
            Pos[NUMPOS] = Hand;
            
            /* Bump oldest velocity data and make room for most recent */
            /* Bump oldest valocity magnitude data and make room for most recent */
            for ( i=0; i<NUMVEL; i++ ) {
                VEL[i] = VEL[i+1];
                VelMag[i] = VelMag[i+1];
            }
            
            // Store most recent velocity.
            VEL[NUMVEL] = (Pos[NUMPOS] - Pos[NUMPOS-1])/dT;
            // Store most recent Velocity Magnitude.
            VelMag[NUMVEL] = sqrt(VEL[i].x*VEL[i].x + VEL[i].y*VEL[i].y + VEL[i].z*VEL[i].z);
            
            // If the ball has not been released the following happens.
            /* Test if any of the velocities have exceeded the TossVelocityTolerance -> make throwit_1 true
             and make throwIt_2 false. */
            /* Check if the most recent Velocity Magnitude has dropped below the TossVelocityTolerance */
            if ( !released ) {
                
                maxTossVel = 0.0;
                throwIt_1 = throwIt_2 = false;
                
                for ( i=0; i<NUMVEL; i++ ) {
                    if ( VelMag[i] > TossVelTol ) {
                        throwIt_1 = true;
                        throwIt_2 = false;
                    }
                    if ( VelMag[i]>maxTossVel ) {
                        maxTossVel = VelMag[i];
                        travelVelocity.set(VEL[i].x, VEL[i].y);
                    }
                }
                
                if ( VelMag[NUMVEL] < TossVelTol )
                    throwIt_2 = true;
                
                if ( throwIt_1 && throwIt_2 && (currentTime - catchTime > tossWaitTime) ) {
                    released = true;
                    caught = false;
                    releaseTime = currentTime;
                }
                
            }
            
            // If the ball has been released
            /* Check if the Position of the ball is outside the screen. */
            else {
                throwIt_1 = throwIt_2 = false;
                if ( Position.x > screen_width || Position.x < 0.0
                    || Position.y > screen_height || Position.y < 0.0 ) {
                    released = false;
                    stop = true;
                    caught = false;
                }
                else {
                    stop = false;
                }
            }
            
            // If the ball has been released, then let it travel.
            if ( released ) {
                Position.set(Position.x + travelVelocity.x*dT*0.5,
                             Position.y + travelVelocity.y*dT*0.5,
                             Position.z + travelVelocity.z*dT*0.5);
            }
            // If the ball has not been released, set the position at the hand and record the time.
            else {
                Position.set(Hand.x, Hand.y);
                if ( ! caught ) {
                    catchTime = currentTime;
                    caught = true;
                }
            }
        }
    }
    
};

class GRID{
public:
    int Xdim = 5;
    int Ydim = 4;
    int NumRgns = Xdim*Ydim;
    double Width,Height;
    double stride;
    ofPoint corner[4];
    ofColor color = ofColor::black;
    
    
    
    void setColor(ofColor collar){
        color = collar;
    }
    
    void setCorners(double width, double height){
        Width = width;
        Height = height;
        corner[0].set(0,height);
        corner[1].set(0,0);
        corner[2].set(width,0);
        corner[3].set(width,height);
    }
    
    void drawGrid(){
        ofSetColor(color);
        ofLine(corner[0], corner[1]);
        ofLine(corner[1], corner[2]);
        ofLine(corner[2], corner[3]);
        ofLine(corner[3], corner[0]);
        
        int j;
        stride = Width/(double)Xdim;
        for ( int i=0; i<Xdim; i++ ) {
            j = (double)i +1;
            ofLine(stride*j, 0.0, stride*j, Height);
        }
        
        stride = Height/(double)Ydim;
        for ( int i=0; i<Ydim; i++ ) {
            j = (double)i +1;
            ofLine(0.0, stride*j, Width, stride*j);
        }
    }
    
    void checkRegions(){
        
    }
};

class BUTTON{
public:
    ofPoint topLeft;
    ofColor color = ofColor::magenta;
    ofPoint refPnt;
    double offSetDist;
    int label;
    int height;
    int width;
    int buttClkInt=-1;
    bool handCheck;
    bool isOn=false;
    string msg;
    string videoName;
    
    void setHandCheck(ofPoint hand){
        if(hand.x>topLeft.x && hand.y>topLeft.y && hand.x<topLeft.x+width && hand.y<topLeft.y+height) handCheck=true;
        else handCheck=false;
    }
    void setButtColor(ofColor c){
        color = c;
    }
    void setButtDimension(int w, int h){
        width = w;
        height = h;
    }
    void setButtCorner(int x, int y){
        topLeft.x = x;
        topLeft.y = y;
    }
    void setRefPnt(ofPoint pnt){
        refPnt = pnt;
    }
    void setOffsetDistance(ofPoint left, ofPoint right){
        offSetDist = right.x - left.x;
        offSetDist = 2.0*offSetDist;
    }
    void drawButton(){
        if ( isOn ) {
            setButtColor(ofColor::turquoise);
        } else {
            setButtColor(ofColor::magenta);
        }
        ofEnableAlphaBlending();
        ofSetColor(color,127);
        ofRect(topLeft.x, topLeft.y, width, height);
        ofDisableAlphaBlending();
        msg = ofToString(label);
    }
    void pushButton(int &clkCounter){
        if (!isOn) {
            if ( clkCounter != buttClkInt ){
                isOn = true;
                buttClkInt = clkCounter;
            }
        }
        else {
            if ( clkCounter != buttClkInt ) {
                isOn = false;
                buttClkInt = clkCounter;
            }
        }
    }
};

class HAND{
public:
    HAND(){};
    ~HAND(){};
    
    ofPoint Pos[NUMPOS+1];
    ofPoint Vel[NUMVEL+1];
    ofPoint PosAvg;
    ofPoint VelAvg;
    ofPoint WrldPos;
    ofPoint ProjPos;
    ofPoint clkPos;
    ofPoint referencePnt;
    ofPoint clkOffset;
    ofxOpenNIJoint Jnt;
    ofColor clkColor = ofColor::black;
    ofColor handColor;
    
    bool frwdChk = false;
    bool bckChk = false;
    bool clkChk = false;
    
    double dT;
    double VelTol = 4.0;
    double XYvel;
    double XYvelAvg;
    double clickTime;
    double clickResetTime = 5.0e5;
    
    int i=0;
    int clkCounter=0;
    
    /////////////
    // MEMBERS //
    /////////////
    void update(){
        WrldPos = Jnt.getWorldPosition();
        ProjPos = Jnt.getProjectivePosition();
        PosAvg.set(0.0,0.0,0.0);
        XYvelAvg = 0.0;
        dT = ofGetFrameRate();
        dT = 1.0/dT;
        
        for (i=0; i<NUMPOS; i++) {
            Pos[i] = Pos[i+1];
            PosAvg += Pos[i];
        }
        Pos[NUMPOS] = WrldPos*0.001;
        PosAvg += Pos[NUMPOS];
        PosAvg /= (double)NUMPOS;
        
        Vel[NUMVEL] = (Pos[NUMPOS] - Pos[NUMPOS-1])/dT;            // check for forward back motion
        if ( Vel[NUMVEL].z > -VelTol && Vel[NUMVEL].z < VelTol )
            Vel[NUMVEL].z = 0.0;
        
        for ( i=0; i<NUMVEL; i++ )
            Vel[i] = Vel[i+1];
        
        clickCheck();
        
    }
    
    void clickCheck(){
        if ( ofGetSystemTimeMicros()-clickTime > clickResetTime )
            clkChk=false;
        
        if ( !clkChk ) {
            ofPoint frwdClkPos, bckClkPos;
            
            frwdChk = bckChk = clkChk = false;
            
            for ( i=0; i<NUMVEL; i++ ) {
                if ( Vel[i].z < -VelTol ) {
                    frwdChk = true;
                    frwdClkPos = Jnt.getProjectivePosition();
                }
                if ( Vel[i].z > VelTol && frwdChk && !bckChk ) {
                    bckChk = true;
                    bckClkPos = Jnt.getProjectivePosition();
                }
                XYvel = sqrt(Vel[i].x*Vel[i].x + Vel[i].y*Vel[i].y);
                if ( XYvel > VelTol )
                    frwdChk = bckChk = false;
                
                VelAvg += Vel[i];
            }
            
            VelAvg /= (double)NUMVEL;
            
            if ( frwdChk && bckChk ) {
                clkChk = true;
                clkOffset = (frwdClkPos + bckClkPos)*0.5 - referencePnt;
                clickTime = ofGetSystemTimeMicros();
                clkCounter++;
            }
            
            clkPos = referencePnt + clkOffset;
        }
    }
    
    void drawClk(){
        ofSetColor(clkColor);
        ofCircle(clkPos.x, clkPos.y, 5);
    }
    void drawHand(){
        ofSetColor(handColor);
        ofCircle(ProjPos.x, ProjPos.y,10);
    }
};


class testApp : public ofBaseApp{
    
public:
    
    int windowWidth, windowHeight;
    int i, j, k;
    string msg[3];
    string MSG;
    int dbplx = 640;  // debug print location x
    int dbply = 40;  // debug print location y
    int dbpo = 24;   // debug print offset
    int statementCounter = 0;
    
    ofPoint origin;
    
    ///////////////////////////
    // standard of functions //
    ///////////////////////////
	void setup();
	void update();
	void draw();
    void exit();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    void userEvent(ofxOpenNIUserEvent & event);
    
    bool overheadCheck(ofPoint leftHand, ofPoint rightHand, ofPoint head, ofPoint neck);
    int overheadClickCounter = 0;
    bool overheadChk = false;
    
    int RIGHTHAND = 10000;
    int LEFTHAND = 20000;
    
    int MaxNumUsers = 1;
    
    int kinWidth, kinHeight;
    
    int buttWidth=60, buttHeight=60, numButts=5;
    BUTTON butt[20];
    
	ofxOpenNI openNIDevice;
    int numUsers = 0;
    ofxHardwareDriver kinectHardwareDriver;
    int kinectTiltAngle;
    ofTrueTypeFont verdana;
    
    GRID grid;
    
    // RIGHT HAND STUFF
    HAND rightHand;
    ofxOpenNIJoint rightHandJnt;
    ofPoint rightHandPnt;
    ofPoint rhWrldPos;
    
    // LEFT HAND STUFF
    HAND leftHand;
    ofxOpenNIJoint leftHandJnt;
    ofPoint leftHandPnt;
    ofPoint lhWrldPos;
    
    // MISCELLANEOUS body parts STUFF
    ofPoint torsoPnt;
    ofxOpenNIJoint headJnt;
    
    ofPoint neckPnt;
    ofxOpenNIJoint neckJnt;
    
    ofPoint headPnt;
    ofxOpenNIJoint torsoJnt;
    
    ofPoint leftKneePnt;
    ofxOpenNIJoint leftKneeJnt;
    
    ofPoint rightKneePnt;
    ofxOpenNIJoint rightKneeJnt;
    
    ofPoint leftHipPnt;
    ofxOpenNIJoint leftHipJnt;
    
    ofPoint rightHipPnt;
    ofxOpenNIJoint rightHipJnt;
    
    ofPoint videoTriggerPnt;
    
    
    // video player handles

    double imageWidth, imageHeight;
    double handDistance = 1000.0;
    double stopTime = 0.0;
    double playTimeOffset = 1.0;
    double handDistanceStopTolerance = 20.0;
    string videoName;
    string currVideoName;
    int curr_video = -1;
    bool isVideoPlaying = false;
    bool isVideoLoaded = false;
    bool StopCheck = false;
    int testButton = 14;
    static const int numMovies = 20;
    ofVideoPlayer videoPlayer[numMovies];
    ofImage videoImage[numMovies];
    ofPoint videoLoc[numMovies];
    
    //BALL ball;
    
    ofVideoPlayer tempPlayer[5];
    ofImage tempImage[5];
    ofPoint tempLoc[5];
    int numTestVids = 4;
    
    
};


#endif
