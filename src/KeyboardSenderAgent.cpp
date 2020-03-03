/*
 * This file is part of Micole Architecture
 *
 * Copyright (C) 2007 Micole partners
 *
 * Micole Architecture is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU Lesser General 
 * Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any 
 * later version.
 *
 * Micole Architecture is distributed in the hope that it will be 
 * useful, * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU Lesser General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Micole Architecture.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "KeyboardSenderAgent.h"

#include "ReachinMicoleApplication.h"
#include "MicoleStringStream.h"

KeyboardSenderAgent::KeyboardSenderAgent()
:MicoleAgent("KeyboardSenderAgent","SenderAgent"), _keyboardListener(new KeyboardListener), _keyb(new Keyboard)
{
	_keyboardListener->_kbssa = this;
	ReachinMicoleApplication *rma = ReachinMicoleApplication::getInstance();
	//rma->getDisplay()->children->add(_keyb.get());
	_keyb->strokes->route(_keyboardListener);
	rma->sendBusMessage("KEYBOARD INPUT CREATED");
}

//must not be used
KeyboardSenderAgent::KeyboardSenderAgent(const KeyboardSenderAgent &)
:MicoleAgent("KeyboardSenderAgent","SenderAgent"), _keyboardListener(NULL)
{
	_keyboardListener->_kbssa = this;
}

KeyboardSenderAgent::~KeyboardSenderAgent()
{
}

void KeyboardSenderAgent::KeyboardListener::evaluate(MFInt32*character)
{
	MicoleStringStream s;
	//for (MField< int >::const_iterator it = character->get().begin() ; it != character->get().end() ; it++)
	//{
//		s << "IN KBD : key=" << *it;
		s << "IN KBD : key=" << character->get()[0]<<";";

		_kbssa->sendMessage(s.str().c_str());

		s.flush();
		//ReachinMicoleApplication::getInstance()->sendBusMessage("TEST KBD");
		//ReachinMicoleApplication::getInstance()->sendBusMessage(s.str().c_str());
	//}
}

