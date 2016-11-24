#pragma once

#include <assert.h>
#include "../types/token.h"

int parseInt(char * number){
	assert(is_integer(number));
	int val = 0;
	int isNegative = 0; //boolean
	int charIndex = 0;
	if(number[0] == '-'){
		isNegative = 1;
		charIndex = 1;
	}
	int len = strlen(number);
	for(; charIndex < len ; charIndex ++){
		val *= 10;
		val += number[charIndex]-'0';
	}
	if(isNegative)
		val = -val;
	return val;
}

float parseFloat(char * number){
	assert(is_float(number));
	int integerPart = 0;
	float decimalPart = 0;
	int isNegative = 0; //boolean
	int charIndex = 0;
	int decimalPos = 0;
	if(number[0] == '-'){
		isNegative = 1;
		charIndex = 1;
	}
	int len = strlen(number);
	for(; charIndex < len ; charIndex++){
		if(number[charIndex] == '.'){
			decimalPos = charIndex;
			break;
		}
		integerPart = 10*integerPart + number[charIndex] - '0';
	}
	if(decimalPos){
		charIndex = len-1;
		for(; charIndex > decimalPos ; charIndex--){
			decimalPart = (decimalPart + number[charIndex] - '0')/10;
		}
	}
	return decimalPart + integerPart;
}