#pragma once

#define GRID_SIZE 3
#define CELLS GRID_SIZE*GRID_SIZE
#define SPECIAL_CELL 4


#include "ofMain.h"
#include "ofxRetroPixel.h"

class Video : public ofVideoPlayer {
public:
    ofRectangle bounds;
    static float width;
    static float height;
    
    static ofxRetroPixel retro;
    static ofFbo screen;
    static int pixelSize;
    
    void open(string path) {
        loadMovie(path);
    }
    
    void drawBackground() {
        screen.begin();
        ofClear(0);
        float ratio = ofGetHeight() / getHeight();
        float w = getWidth() * ratio;
        float x = (ofGetWidth()/2.0)-(w/2.0);
        ofVideoPlayer::draw(x, 0, w, ofGetHeight());
        screen.end();
        
        retro.draw(screen.getTextureReference(), pixelSize, true, true);
    }
    void draw() {
        ofVideoPlayer::draw(bounds);
    }
    
    void ff() {
        float pos = ofVideoPlayer::getPosition();
    }
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void tick(int index);
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void startVideo(int i, string path);
        void videoClicked(int i);
        string getRandomPath();
        bool bDrawGrid;
        float lastTick;
        int nTicks;
        int vOver;
        vector<string> history;
        vector<string> paths;
        Video videos[CELLS];
};

extern "C" size_t decode_html_entities_utf8(char *dest, const char *src);

string ofDecodeEntities(std::string& str);