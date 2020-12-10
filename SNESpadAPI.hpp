/*
Copyright (c) 2014-2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include guard
#pragma once

SNESpadAPI::SNESpadAPI(void)
{
	// Empty
}

void SNESpadAPI::begin(void){
	// release all buttons
	end();
}

void SNESpadAPI::end(void){
	memset(&_report, 0x00, sizeof(_report));
	SendReport(&_report, sizeof(_report));
}

void SNESpadAPI::write(void){ 
	SendReport(&_report, sizeof(_report)); 
}


void SNESpadAPI::press(uint8_t b){ 
	_report.buttons |= (uint8_t)1 << (b - 1); 
}


void SNESpadAPI::release(uint8_t b){ 
	_report.buttons &= ~((uint8_t)1 << (b - 1)); 
}


void SNESpadAPI::releaseAll(void){ 
	memset(&_report, 0x00, sizeof(_report)); 
}

void SNESpadAPI::buttons(uint8_t b){ 
	_report.buttons = b; 
}


void SNESpadAPI::xAxis(int8_t a){
	_report.xAxis = a; 
}


void SNESpadAPI::yAxis(int8_t a){
	_report.yAxis = a; 
}

/*
void SNESpadAPI::zAxis(int8_t a){ 
	_report.zAxis = a; 
}


void SNESpadAPI::rxAxis(int16_t a){ 
	_report.rxAxis = a; 
}


void SNESpadAPI::ryAxis(int16_t a){ 
	_report.ryAxis = a; 
}


void SNESpadAPI::rzAxis(int8_t a){ 
	_report.rzAxis = a; 
}
*/
/*
void SNESpadAPI::dPad1(int8_t d){ 
	_report.dPad1 = d; 
}*/
