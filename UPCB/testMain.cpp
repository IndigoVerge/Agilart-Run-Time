/*******************************************************************************
 * Agilart Run-time runs on microcontrollers or other compatible hardware and
 * allows you to create, execute and monitor programs remotely via simple web API.
 * To make programs, Agilart Visual Designer can be used at www.agilart.com.
 *
 * Copyright (C) 2012  CompletIT (www.agilart.com), Ivan Dragoev (idragoev@completit.com),
 * Bobi Rakova (brakova@completit.com)
 *
 * This file is part of Agilart Run-time.
 *
 * Agilart Run-time is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Agilart Run-time is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/
#include <iostream>
using namespace std;

class BAR
{
public:
	virtual int add_stuff(int a, int b) = 0;
};

class BARDerieved : public BAR
{
public:
	int add_stuff(int a, int b)
	 {
	  return a+b;
	 }
};

class FOO
{
public:
	 int (BAR::*fptr)(int a, int b);

	 int add_stuff(int a, int b)
	 {
		 return a+b;
	 }

	 void call_adder(int a, int b, BAR* myBar)
	 {
		 fptr = &BAR::add_stuff;
		 cout<<(myBar->*fptr)(a,b)<<endl;
	 }
};

int main2( int argc, char* argv[] )
{
	BARDerieved* bar = new BARDerieved();
	FOO foo;
	foo.call_adder( 1,2, bar );

	return 0;
}
