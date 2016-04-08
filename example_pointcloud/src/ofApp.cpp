#include "ofMain.h"
#include "ofxMultiKinectV2.h"

//========================================================================
class ofApp : public ofBaseApp{
    ofxMultiKinectV2 kinect0;
    
    ofEasyCam ecam;
    ofVboMesh mesh;
    
    
    float irXres = 512;
    float irYres = 424;
    float colorXres = 1920;
    float colorYres = 1080;
    
    float irXdeg = 70.6;
    float irYdeg = 60;
    float colorXdeg = 84.1;
    float colorYdeg = 53.8;
    
    
    
public:
    
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        
        kinect0.open(true, true, 0, 0);
        // Note :
        // Default OpenCL device might not be optimal.
        // e.g. Intel HD Graphics will be chosen instead of GeForce.
        // To avoid it, specify OpenCL device index manually like following.
        // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
        
        kinect0.start();
        
        mesh.setUsage(GL_DYNAMIC_DRAW);
        mesh.setMode(OF_PRIMITIVE_POINTS);
        
        ecam.setAutoDistance(false);
        ecam.setDistance(200);
        
    }
    
    void update() {
        kinect0.update();
        
        if (kinect0.isFrameNew()) {
            
            mesh.clear();
            {
                int step = 2;
                int h = kinect0.getDepthPixelsRef().getHeight();
                int w = kinect0.getDepthPixelsRef().getWidth();
                
                int startH = irXres * ((irXdeg - colorXdeg) / 2)/irXdeg;
                int endH = irXres - startH;
                
                int startW = 0;
                int endW = kinect0.getDepthPixelsRef().getWidth();;
                
                int startColorW = colorXres * (((colorXdeg - irXdeg)/2)/colorXdeg);
                int endColorW = colorXres - startColorW;
                
               
                for(int y = startH; y < endH; y += step) {
                    for(int x = startW; x < endW; x += step) {
                        
                        float dist = kinect0.getDistanceAt(x, y);
                        if(dist > 1 && dist < 5000) {
                            ofVec3f pt = kinect0.getWorldCoordinateAt(x, y, dist);
                            
                            ofColor c = kinect0.getColorAt(startColorW + (x / irXres) * (endColorW - startColorW),
                                                           (y / irYres) * colorYres);
                            
                            mesh.addColor(c);
                            mesh.addVertex(pt);
                        }
                        
                    }
                }

            }
        }
        
        
        
    }
    
    void draw()
    {
        ofClear(0);
        
        if (mesh.getVertices().size()) {
            ofPushStyle();
            glPointSize(4);
            ecam.begin();
            ofScale(1, 1, -1);
            ofDrawAxis(100);
            ofPushMatrix();
            ofTranslate(0, 0, -100);
            mesh.draw();
            ofPopMatrix();
            ecam.end();
            ofPopStyle();
        }
        
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
        ofDrawBitmapStringHighlight("Device Count : " + ofToString(ofxMultiKinectV2::getDeviceCount()), 10, 40);
    }


    void keyPressed(int key)
    {
        if (key == 'a') {
            irXdeg += 1;
        }
        
        if (key == 'z') {
            irXdeg -= 1;
        }
        
        if (key == 's') {
            irYdeg += 1;
        }
        
        if (key == 'x') {
            irYdeg += 1;
        }
        
        if (key == 'd') {
            colorXdeg += 1;
        }
        
        if (key == 'c') {
            colorXdeg -= 1;
        }
        
        if (key == 'f') {
            colorYdeg += 1;
        }
        
        if (key == 'v') {
            colorYdeg += 1;
        }
        
        
        
        
        ofLogNotice() << irXdeg << "\t" << irYdeg << "\t" << colorXdeg << "\t" << colorYdeg;
        
    }







};

//#include "ofAppGLFWWindow.h"
//========================================================================
int main( ){
    ofSetupOpenGL(1920,1080,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
