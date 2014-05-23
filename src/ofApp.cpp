#include "ofApp.h"
#include "entities.h"
#include "Poco/RegularExpression.h"
#include "Poco/URI.h"

using Poco::RegularExpression;

float Video::width = 320;
float Video::height = 240;
ofxRetroPixel Video::retro;
ofFbo Video::screen;
int Video::pixelSize = 30;


string ofDecodeEntities(std::string& str)
{
    char* dest = new char[str.length()];
    decode_html_entities_utf8(dest, str.c_str());
    return std::string(dest);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFullscreen(true);
    
    Video::retro.setup(ofGetWidth(), ofGetHeight());
    Video::screen.allocate(ofGetWidth(), ofGetHeight());
    
    string home = Poco::Path::home();
    string path = home+"Music/iTunes/iTunes Music Library.xml";
    ofLogNotice("library") << path;
    string text = ofBufferFromFile( path ).getText();
   
    int options = Poco::RegularExpression::RE_DOTALL|Poco::RegularExpression::RE_UNGREEDY;
    RegularExpression re1("<key>Music Video</key><true/>[\\s]+<key>Location</key><string>(.+)</string>", options);
    
    RegularExpression::MatchVec matches;
    string::size_type offset = 0;
    Poco::RegularExpression::MatchVec matchVec;
    
    while (re1.match(text, offset, matchVec)) {
        offset = matchVec[0].offset + matchVec[0].length;
        string path = text.substr(matchVec[1].offset, matchVec[1].length);
        ofStringReplace(path, "file://localhost", "");
        string decoded;
        Poco::URI::decode(path, decoded);
        decoded = ofDecodeEntities(decoded);
        paths.push_back(decoded);
    }
    
    
    if(paths.size()<CELLS) {
        ofLogNotice() << "Not enough videos!";
        ofExit();
    }

    for (int i=0; i<CELLS; i++)
    {
        string path = getRandomPath();
        ofLogNotice() << path;
        videos[i].setLoopState(OF_LOOP_NONE);
        videos[i].open(getRandomPath());
        videos[i].setVolume((i==SPECIAL_CELL) ? 1.0 : 0);
        videos[i].play();
    }
    
    ofLogNotice() << "found " << paths.size() << " Music Videos";
    
    bDrawGrid = true;
    vOver = SPECIAL_CELL;
    lastTick=0;
    nTicks=0;
}

//--------------------------------------------------------------
string ofApp::getRandomPath() {
    string path;
    do {
        int n = ofRandom(paths.size());
        path = paths[n];
    } while( find(history.begin(), history.end(), path) != history.end() );
    
    history.push_back(path);
    if(history.size()>100) {
        history.erase(history.begin());
    }
    return path;
}


//--------------------------------------------------------------
void ofApp::update(){
    float now = ofGetElapsedTimef();
    if(now-lastTick>1) {
        lastTick=now;
        tick(nTicks++);
    }
    
    for(int i=0; i<CELLS; i++) {
        videos[i].update();
        if(videos[i].getIsMovieDone()) {
            videos[i].close();
            videos[i].open(getRandomPath());
            videos[i].setVolume((i==SPECIAL_CELL) ? 1.0 : 0);
            videos[i].play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::tick(int index){
    //ofLogNotice("tick") << index;
    Video::pixelSize = ofRandom(20, 50);
}


//--------------------------------------------------------------
void ofApp::draw(){
    videos[vOver].drawBackground();
    
    if(bDrawGrid) {
        for(int i=0; i<CELLS; i++) {
            videos[i].draw();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key==' ') {
        bDrawGrid=!bDrawGrid;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    vOver = SPECIAL_CELL;
    for(int i=0; i<CELLS; i++) {
        videos[i].setVolume(0.0);
        
        if(videos[i].bounds.inside(x, y)) {
            vOver = i;
        }
    }
    
    videos[vOver].setVolume(1.0);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::startVideo(int i, string path) {
    
}

//--------------------------------------------------------------
void ofApp::videoClicked(int i) {
    if(i==SPECIAL_CELL) {
        
    } else {
        string path = videos[i].getMoviePath();
        videos[SPECIAL_CELL].close();
        videos[SPECIAL_CELL].open(path);
        videos[SPECIAL_CELL].play();
        
        videos[i].close();
        videos[i].open(getRandomPath());
        videos[i].setVolume(0);
        videos[i].play();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    for(int i=0; i<CELLS; i++) {
        if(videos[i].bounds.inside(x, y)) {
            videoClicked(i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    int totalWidth = Video::width*GRID_SIZE;
    int totalHeight = Video::height*GRID_SIZE;
    
    ofVec2f start;
    start.x = (w/2.0)-(totalWidth/2.0);
    start.y = (h/2.0)-(totalHeight/2.0);
    
    for (int i=0; i<CELLS; i++)
    {
        int x = start.x + ( i % GRID_SIZE * Video::width);
        int y = start.y + ( i / GRID_SIZE * Video::height);
        videos[i].bounds.set(x, y, Video::width, Video::height);
        ofLogNotice() << videos[i].bounds;
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
