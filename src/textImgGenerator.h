#pragma once

#include "constParameter.h"
#include "ofxTrueTypeFontUC.h"


class textImgGenerator
{
public:
	void setup(int width, int height);
	void draw();
	void setText(wstring& text);
	void start(string path, string pre, int startIdx = 0);
	void generatorText(int size);

	
private:
	void saveToImage(int startIdx);
	void loadFont(int size);
	void loadImg();

public:
	void loadDictionary();
	void saveDictionary();


private:

	ofxTrueTypeFontUC _font;
	ofFbo _canvas, _canvas2;
	set<wchar_t> _dictionary;

	string _path, _pre;

	vector<ofImage> _imgSet;
};