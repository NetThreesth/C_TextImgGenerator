#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	_textGenerator.setup(64, 64);
	//_textGenerator.loadDictionary();


	ofAddListener(_server.newResponseEvent, this, &ofApp::newResponse);
	_server.setTimeoutSeconds(30);
	_server.start();
	
	requestMsg("http://35.236.167.99:5000/3sth/api/v1.0/getTodayMsg/");
	_canStart = false;
	_canExit = false;
}

//--------------------------------------------------------------
void ofApp::update() {

	if (_canStart)
	{
		_canStart = false;
		_textGenerator.start("output/", "test", getImageIndex("output/"));
		exeDCGAN();
		string pattern = checkOutput();
		uploadPattern("http://35.236.167.99:5000/3sth/api/v1.0/uploadPattern/", pattern);
	}

	if (_canExit)
	{
		std::exit(1);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	switch (key)
	{
	case 's':
	{
		_textGenerator.saveDictionary();
		break;
	}
	case 'o':
	{
		_textGenerator.start("output/", "test");
		break;
	}
	}
	
}

//--------------------------------------------------------------
int ofApp::getImageIndex(string path)
{
	ofDirectory dir(path);
	dir.allowExt("jpg");
	dir.listDir();


	return dir.numFiles();
}

//--------------------------------------------------------------
void ofApp::requestMsg(string url)
{
	ofxHttpForm form;
	form.action = url;
	form.method = OFX_HTTP_GET;
	_server.addForm(form);
}

//--------------------------------------------------------------
void ofApp::uploadPattern(string url, string & pattern)
{
	ofxHttpForm form;
	form.action = url;
	form.method = OFX_HTTP_POST;
	form.addFormField("pattern", pattern);
	_server.addForm(form);
}

//--------------------------------------------------------------
void ofApp::newResponse(ofxHttpResponse & response)
{
	if (response.status == 200)
	{
		string msg = response.responseBody.getText();
		json j = json::parse(msg);
		if (j["active"] == "getTodayMsg")
		{
			handleMsg(j);
		}
		else if (j["active"] == "uploadPattern")
		{
			_canExit = true;
		}
		else
		{
			ofLog(OF_LOG_WARNING, "[ofApp::newResponse]unknow active");
		}

	}
	else
	{
		ofLog(OF_LOG_ERROR, "[ofApp::newResponse]get response failed");
	}
}

//--------------------------------------------------------------
void ofApp::handleMsg(json& obj)
{
	if (obj.find("msg") == obj.end())
	{
		ofLog(OF_LOG_NOTICE, "[ofApp::handleMsg]empty");
		return;
	}
	for(const auto& val : obj["msg"])
	{
		_textGenerator.setText(s2ws(val));
	}
	//_textGenerator.saveDictionary();
	ofLog(OF_LOG_NOTICE, "[ofApp::handleMsg]Finish");
	_canStart = true;
}

//--------------------------------------------------------------
string ofApp::checkOutput()
{
	ofxIO::ByteBuffer buffer;
	ofxIO::ByteBufferUtils::loadFromFile("../samples/single_test_1.png", buffer);
	string base64Buffer = ofxIO::Base64Encoding::encode(buffer);
	
	return base64Buffer;

}

//--------------------------------------------------------------
void ofApp::exeDCGAN()
{
	string cmd = cDCGAN_CMD;
	system(cmd.c_str());
}
