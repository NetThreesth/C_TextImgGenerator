#pragma once

#include "ofMain.h"
#include "json.hpp"
using json = nlohmann::json;

static string ws2s(wstring wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, 0);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, 0);
	return strTo;
	
}

//--------------------------------------------------------------
static wstring s2ws(const string & str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

const string cDCGAN_CMD = "python data/metaAI/main.py --dataset=output --input_height=64 --output_height=64 --epoch=30 --train --generate_test_images=1";