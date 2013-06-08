#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxHardwareDriver.h"

#define NUMPOS 30
#define NUMVEL 29

class GRID{
public:
    int Xdim = 5;
    int Ydim = 3;
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
    
    void setRegions(){
        
    }
};

class BUTTON{
public:
    ofPoint topLeft;
    ofColor color = ofColor::magenta;
    ofPoint refPnt;
    double offSetDist;
    int height;
    int width;
    int buttClkInt=0;
    bool handCheck;
    bool isOn=false;
    
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
        ofSetColor(color);
        ofRect(topLeft.x, topLeft.y, width, height);
    }
    void pushButton(int clkCounter){
        if (!isOn) {
            if ( clkCounter != buttClkInt ){
                isOn = true;
                buttClkInt = clkCounter;
                setButtColor(ofColor::turquoise);
            }
        }
        else {
            if ( clkCounter != buttClkInt ) {
                isOn = false;
                buttClkInt = clkCounter;
                setButtColor(ofColor::magenta);
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
    ofPoint clkPos;
    ofPoint referencePnt;
    ofPoint clkOffset;
    ofxOpenNIJoint Jnt;
    ofColor clkColor = ofColor::black;
    
    bool frwdChk = false;
    bool bckChk = false;
    bool clkChk = false;
    
    double dT;
    double VelTol = 3.0;
    double XYvel;
    double XYvelAvg;
    double clickTime;
    double clickResetTime = 5.0e5;
    
    int i=0;
    int clkCounter=0;
    
    /////////////
    // MEMBERS //
    /////////////
    void updateHand(){
        if ( ofGetSystemTimeMicros()-clickTime > clickResetTime )
            clkChk=false;
        
        if ( !clkChk ) {
            ofPoint frwdClkPos, bckClkPos;
            
            frwdChk = bckChk = clkChk = false;
            WrldPos = Jnt.getWorldPosition();
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
            
            Vel[NUMVEL] = (Pos[NUMPOS] - Pos[NUMPOS-1])/dT;
            
            // check for forward back motion
            if ( Vel[NUMVEL].z > -VelTol && Vel[NUMVEL].z < VelTol )
                Vel[NUMVEL].z = 0.0;
            
            for ( i=0; i<NUMVEL; i++ )
                Vel[i] = Vel[i+1];
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
                clkCounter++;
                clickTime = ofGetSystemTimeMicros();
            }
            
            
            clkPos = referencePnt + clkOffset;
        }
        
    }
    
    void drawClk(){
        ofSetColor(clkColor);
        ofCircle(clkPos.x, clkPos.y, 5);
    }
};


class testApp : public ofBaseApp{
    
public:
    
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
    
    bool buttonCheck(ofPoint pnt, ofPoint btnPnt);
    
    int kinWidth, kinHeight;
    
    // button stuff
    BUTTON butt[6];
    int buttWidth=60, buttHeight=60, numButts=6;
    
    double dT = 0.5;
    double VelTol = 0.5;
    
	ofxOpenNI openNIDevice;
    ofxHardwareDriver kinectHardwareDriver;
    ofTrueTypeFont verdana;
    
    GRID grid;
    
    HAND rightHand;
    HAND leftHand;
    
};


#endif
