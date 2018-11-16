#include "textImgGenerator.h"

//-----------------------------------------
void textImgGenerator::setup(int width, int height)
{
	_canvas.allocate(width, height, GL_RGBA);
	_canvas2.allocate(width, height, GL_RGB);
	loadFont(width * 0.6);
	loadImg();
}

//-----------------------------------------
void textImgGenerator::draw()
{
	for (auto iter : _dictionary)
	{
		wstring ws(1, iter);
		string s = ws2s(ws);
	}
}

//-----------------------------------------
void textImgGenerator::setText(wstring& text)
{
	for (auto& iter : text)
	{
		if (_dictionary.find(iter) == _dictionary.end())
		{
			_dictionary.insert((iter));
		}
	}
}


//-----------------------------------------
void textImgGenerator::start(string path, string pre, int startIdx)
{
	_path = path;
	_pre = pre;
	saveToImage(startIdx);
}

//-----------------------------------------
void textImgGenerator::generatorText(int size)
{
	ofSeedRandom();
	_dictionary.clear();
	wchar_t wc = 0x4E00;
	int i = 0;
	while (i < size)
	{;
		if (_dictionary.find(wc) == _dictionary.end())
		{
			_dictionary.insert((wc));
			wc++;
			i++;
		}
	}
	cout << "Generator text finish" << endl;
}

//-----------------------------------------
void textImgGenerator::saveToImage(int startIdx)
{
	int i = startIdx;
	ofRectangle bound;
	for (auto& iter : _dictionary)
	{
		wstring ws(1, iter);
		string s = ws2s(ws);

		bound = _font.getStringBoundingBox(s, 0, 0);
		_canvas.begin();
		ofPushStyle();
		ofClear(0, 255);
		ofSetColor(0);
		ofFill();
		ofRect(0, 0, _canvas.getWidth(), _canvas.getHeight());
		_imgSet[rand() % _imgSet.size()].draw(0, 0);
		ofSetColor(255);
		_font.drawString(
			s
			, _canvas.getWidth() * 0.5 - (bound.getWidth() + bound.x) * 0.5
			, _canvas.getHeight() * 0.5 + (bound.y * -0.5)
		);
		ofPopStyle();
		_canvas.end();


		_canvas2.begin();
		ofPushStyle();
		ofSetColor(255);
		ofClear(0, 255);
		_imgSet[rand() % _imgSet.size()].draw(0, 0);

		ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);
		_canvas.draw(0, 0);
		ofPopStyle();
		_canvas2.end();

		string path = _path + _pre + ofToString(i, 8, '0') + ".jpg";
		ofPixels pix;
		_canvas2.readToPixels(pix);
		ofSaveImage(pix, path);
		pix.clear();
		cout << "Save image success : " << path << endl;
		i++;


		//--------------------------------
		if (i > 1000)
		{
			_font.reloadFont();
		}
	}
}

//--------------------------------
void textImgGenerator::loadFont(int size)
{

	_font.load("fonts/font.ttc", size);
	_font.setGlobalDpi(72);
}

//--------------------------------
void textImgGenerator::loadImg()
{
	ofDirectory dir("images/");
	dir.allowExt("jpg");
	dir.listDir();

	_imgSet.resize(dir.size());
	int i = 0;
	for (auto& iter : dir)
	{

		if (!_imgSet[i].load(iter.getAbsolutePath()))
		{
			continue;
		}
		i++;
	}
}

//-----------------------------------------
void textImgGenerator::loadDictionary()
{
	json input;
	ifstream in("data/_dictionary.txt");
	string data;
	getline(in, data);
	in.close();

	input = json::parse(data);
	setText(s2ws(input["data"]));
}

//-----------------------------------------
void textImgGenerator::saveDictionary()
{
	if (_dictionary.size() <= 0)
	{
		return;
	}

	json output;
	wstring s;
	for (const auto& iter : _dictionary)
	{
		s += iter;
	}
	output["data"] = ws2s(s);
	ofstream out("data/_dictionary.txt");
	out << output << endl;
	out.close();
}
