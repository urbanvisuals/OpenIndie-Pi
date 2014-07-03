#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
    ofDisableAntiAliasing();
	//-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading indiePi.xml";
    
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.load("indiePi.xml") ){
		message = "indiePi.xml loaded!";
	}else{
    
        // Ok, we didn't get a file, but that's ok, we're going
        // to go ahead and start making some stuff anyways! First, to
        // make a correct XML document, we need a root element, so
        // let's do that:
        
        XML.addChild("INDIEPI");
        
        // now we set our current element so we're on the right
        // element, so when we add new nodes, they're still inside
        // the root element
        XML.setTo("INDIEPIE");
        
		message = "unable to load indiePi.xml check data/ folder";
	}
    
	//read the colors from XML or, if they don't exist, because we've
    // loaded them from elsewhere, let's just make some stuff up

    //the double forward slash //RED means: search from the root of the xml for a tag could RED
    //otherwise the search would be relative to where you are in the xml structure. // = absolute
    if(XML.exists("//HOUR")) {
        hour	= XML.getValue<int>("//HOUR");
    } else {
        hour = 23;
    }
       
    if(XML.exists("//MINUTE")) {
        minute	= XML.getValue<int>("//MINUTE");
    } else {
        minute = 0;
    }
    
    if(XML.exists("//SECOND")) {
        second	= XML.getValue<int>("//SECOND");
    } else {
        second = 0;
    }
    if(XML.exists("//VIDEO_PATH")) {
        vidPath	= XML.getValue("//VIDEO_PATH");
    } else {
        vidPath = "movie.mov";
    }
    
    

    
	//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 20 );
    
    ofDisableAntiAliasing();
    
    /*
     //-----------
     //the string is printed at the top of the app
     //to give the user some feedback
     message = "loading mySettings.xml";
     
     //we load our settings file
     //if it doesn't exist we can still make one
     //by hitting the 's' key
     if( XML.load("mySettings.xml") ){
     message = "mySettings.xml loaded!";
     }else{
     
     // Ok, we didn't get a file, but that's ok, we're going
     // to go ahead and start making some stuff anyways! First, to
     // make a correct XML document, we need a root element, so
     // let's do that:
     
     XML.addChild("DRAWING");
     
     // now we set our current element so we're on the right
     // element, so when we add new nodes, they're still inside
     // the root element
     XML.setTo("DRAWING");
     
     message = "unable to load mySettings.xml check data/ folder";
     }
     
     */

     openIndieBG.loadImage("openindie.png");
	
    consoleListener.setup(this);
}

//--------------------------------------------------------------
void testApp::update(){
	//we change the background color here based on the values
	//affected by the mouse position
    
    realHour = ofGetHours();
    realMinute = ofGetMinutes();
    realSecond = ofGetSeconds();
    

}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(225);

    int realTime = (realHour * 60) + realMinute;
    int schedTime = (hour * 60) + minute;
    int minuteDiff = schedTime % realTime;
    int hourDiff = schedTime / realTime;
    
    
	if (schedTime - realTime >= 120){
    	//ofBackground(255,0,0);  // Sets the background color to green
        openIndieBG.draw(0,0);
        string timer = "THE MOVIE WILL START IN ";
        timer += ofToString(hourDiff);
        timer += " HOURS AND ";
        timer += ofToString(minuteDiff);
        timer += " MINUTES ";
    	TTF.drawString(timer, 400, 600);
    }
    
	if (schedTime - realTime >= 60){
    	//ofBackground(255,255,0);  // Sets the background color to green
        openIndieBG.draw(0,0);
        string timer = "THE MOVIE WILL START IN ";
        timer += ofToString(hourDiff);
        timer += " HOUR AND ";
        timer += ofToString(minuteDiff);
        timer += " MINUTES ";
    	TTF.drawString(timer, 400, 600);
    }
    

	if (schedTime - realTime > 0){
    	//ofBackground(0,255,0);  // Sets the background color to green
        openIndieBG.draw(0,0);
        string timer = "THE MOVIE WILL START IN ";
        timer += ofToString(minuteDiff);
        timer += " MINUTES ";
    	TTF.drawString(timer, 400, 600);
    }
    
    // time and scheduled times are the same!
	if (schedTime - realTime == 0){
        //ofBackground(255, 255, 255);
        if (!isPlaying){ 
            startMovie();
        }
    }
    
	if (schedTime - realTime < 0){
        //ofBackground(0, 0, 255);
        openIndieBG.draw(0,0);
        string timer = "THE MOVIE HAS PLAYED.";

    	TTF.drawString(timer, 400, 600);
    }

    
	
	//omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	//draw a smaller version in the lower right
	//int scaledHeight	= omxPlayer.getHeight()/4;
	//int scaledWidth		= omxPlayer.getWidth()/4;
	//omxPlayer.draw(ofGetWidth()-scaledWidth, ofGetHeight()-scaledHeight, scaledWidth, scaledHeight);
    
	//ofDrawBitmapString(omxPlayer.getInfo(), 60, 60, ofColor(ofColor::black));
    
    string info = "THE MOVIE STARTS AT " +ofToString(hour) ;
    info += ":" +ofToString(minute) + "\n";
    info += "THE CURRENT TIME IS "+ofToString(realHour) ;
    info += ":" + ofToString(realMinute)+"\n\n";

    //info += "FPS: "+ofToString(ofGetFrameRate(),0)+"\n\n";
   
    
    TTF.drawString(info, 400, 800);
	
}

void testApp::startMovie(){

    //string videoPath = ofToDataPath("../../../video/bbb_1080p.mov", true);
    
    //Somewhat like ofFboSettings we may have a lot of options so this is the current model
    ofxOMXPlayerSettings settings;
    settings.videoPath = vidPath;
    settings.useHDMIForAudio = true;    //default true
    settings.enableTexture = false;     //default true
    settings.enableLooping = false;     //default true
    settings.enableAudio = true;        //default true, save resources by disabling
    //settings.doFlipTexture = true;        //default false
    
    
    //so either pass in the settings
    omxPlayer.setup(settings);

    isPlaying = true;

}

//--------------------------------------------------------------
//  EVENT Listener for OMXPlayer

void testApp::onCharacterReceived(SSHKeyListenerEventData& e)
{
    keyPressed((int)e.character);
}

void testApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
    ofLogVerbose(__func__) << " RECEIVED";
    //isPlaying = false;
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    ofLogVerbose(__func__) << "key: " << key;
    switch (key) 
    {
        case 'e':
        {
            break;
        }
        case 'x':
        {
            break;
        }
        case 'p':
        {
            ofLogVerbose() << "pause: " << !omxPlayer.isPaused();
            omxPlayer.setPaused(!omxPlayer.isPaused());
            break;
        }
        case 's':
        {
        if (!isPlaying){ 
            startMovie();
        }
            break;
        }
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
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}



