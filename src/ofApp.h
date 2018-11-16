#pragma once

#include "constParameter.h"
#include "textImgGenerator.h"
#include "ofxHttpUtils.h"
#include "ofxIO.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	int getImageIndex(string path);

private:
	bool _canExit;
	textImgGenerator _textGenerator;

#pragma region Http Response
public:
	void requestMsg(string url);
	void uploadPattern(string url, string& pattern);
	void newResponse(ofxHttpResponse & response);
private:
	ofxHttpUtils _server;
	bool _canStart;
#pragma endregion

#pragma region Json
public:
	void handleMsg(json& obj);
#pragma endregion

#pragma region DCGAN
public:
	string checkOutput();
	void exeDCGAN();
#pragma endregion



};
