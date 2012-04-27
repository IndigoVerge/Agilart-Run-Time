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

#define SwitchDeviceName "Conditional Switch"
#define SwitchDeviceType "LogicalDevice"
#define SwitchDeviceDescr "Conditional Block"
#define SwitchDeviceGroup "Conditional"
#define SwitchDeviceVersion "0.0.1"
#define LoopInputCP "LoopInput"
#define LoopInputCPDescr "Loop Input"
#define LoopOutputCP "Output connection point"
#define LoopOutputCPDescr "Loop Output connection point"
#define LoopConditionCP "Condition"
#define LoopConditionCPDescr "Loop Condition connection point"

#define IntType "int"

#define SwitchBlockInput 1
#define SwitchBlockConditionInput 2
#define SwitchBlockOutput 3

#include "SwitchBlock.h"

SwitchBlock::SwitchBlock() : BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	// inputs
	this->switchInConnPoint = new InConnectionPoint(SwitchBlockInput, LoopInputCP, LoopInputCPDescr, IntType, new int(0));
	this->switchInConnPoint->RegisterValueChangedHandler( this );

	this->switchCondConnPoint = new InConnectionPoint(SwitchBlockConditionInput, LoopConditionCP, LoopConditionCPDescr, IntType, new int(1));
	this->switchCondConnPoint->RegisterValueChangedHandler( this );

	// outputs
	this->switchOutConnPoint = new OutConnectionPoint(SwitchBlockOutput, LoopOutputCP, LoopOutputCPDescr, IntType, new int(0));
	this->switchCondConnPoint->RegisterRefreshHandler( this );

	this->connectionPoints->push_back(this->switchInConnPoint);
	this->connectionPoints->push_back(this->switchOutConnPoint);
	this->connectionPoints->push_back(this->switchCondConnPoint);

	this->SetName( SwitchDeviceName );
}

SwitchBlock::~SwitchBlock() {
	// TODO Auto-generated destructor stub
}

const char* SwitchBlock::Type() {
	return SwitchDeviceType;
}

const char* SwitchBlock::Version() {
	return SwitchDeviceVersion;
}

const char* SwitchBlock::Description() {
	return SwitchDeviceDescr;
}

const char* SwitchBlock::GroupName() {
	return SwitchDeviceGroup;
}

BaseDevice* SwitchBlock::CreateInstance() {
	BaseDevice* device = new SwitchBlock();
	return device;
}

void SwitchBlock::CalculateOutputs()
{
	int conditionValue = *(int*) this->switchCondConnPoint->Value();
	int inValue = *(int*) this->switchInConnPoint->Value();
	int result = 0;

	if( conditionValue == 1 && inValue == 1)
		result = 1;

	void* resultPointer = new int(result);
	this->switchOutConnPoint->SetValue( resultPointer );
}
