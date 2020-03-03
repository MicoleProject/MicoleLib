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
#include "VTPlayerSenderAgent.h"
#include "MicoleStringStream.h"


VTPlayerSenderAgent::VTPlayerSenderAgent()
:MicoleAgent("VTPlayerSenderAgent","SenderAgent")
{
	sendMessage("VTPlayer Sender Agent Started");
	VTPMouseController::getInstance()->setListener(this);
	VTPMouseController::getInstance()->run();
}

VTPlayerSenderAgent::VTPlayerSenderAgent(const VTPlayerSenderAgent &)
:MicoleAgent("VTPlayerSenderAgent","SenderAgent")
{
}

VTPlayerSenderAgent::~VTPlayerSenderAgent()
{
	sendMessage("VTPlayer Sender Agent Stopped");
}

void VTPlayerSenderAgent::onMouseMove(int posx, int posy)
{
	MicoleStringStream s;
	s<< "IN VTP : pos=(" << posx << ", "<< posy << ");";
	sendMessage(s.str());
}

bool VTPlayerSenderAgent::onPressed(int button)
{
	MicoleStringStream s;
	s<< "IN VTP : press=(" << button << ");";

	sendMessage(s.str());
	return true;
}

bool VTPlayerSenderAgent::onReleased(int button)
{
	MicoleStringStream s;
	s<< "IN VTP : release=(" << button << ");";
	sendMessage(s.str());
	return true;
}
