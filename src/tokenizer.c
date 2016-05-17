#pragma once

#include "tokenizer.h"

//TODO: make buffer a struct that can be manipulated in a local rather than global way

char fPeekChar(FILE* f){
	char out = fgetc(f);
	ungetc(out,f);
	return out;
}

char fConsumeChar(FILE* f){
	return fgetc(f);
}

char peekChar(){
	return fPeekChar(stdin);
}

char consumeChar(){
	return fConsumeChar(stdin);
}