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
#include "VTPlayerOutputAgent.h"
#include <VTPDynamicIcon.h>
#include "ReachinMicoleApplication.h"

#include <string>
#include <vector>

using namespace std;


vector<string> explodestr(string& delimiter,std::string& input)
{
	vector<string> result;
	unsigned int ndxS=0, ndxE;
	do
	{
		if ((ndxE = (unsigned int)(input.find(delimiter, ndxS))) == -1)
			ndxE = (unsigned int)(input.length());
		result.push_back(input.substr(ndxS,ndxE-ndxS));
		ndxS = ndxE + delimiter.length();
	}while (ndxS < (unsigned int)(input.length()));

	return result;
}


VTPlayerOutputAgent::VTPlayerOutputAgent()
: MicoleAgent("VTPlayerOutputAgent","OutputAgent"), _left(new VTPStaticIcon(4,4)), _right(new VTPStaticIcon(4,4)),  _sN(new VTPStaticIcon(4,4)), _sS(new VTPStaticIcon(4,4)), _sE(new VTPStaticIcon(4,4)), _sW(new VTPStaticIcon(4,4)), _sNE(new VTPStaticIcon(4,4)), _sNW(new VTPStaticIcon(4,4)), _sSE(new VTPStaticIcon(4,4)), _sSW(new VTPStaticIcon(4,4))
{
	bindMessage( "^OUT VTP : (.*)=\\((.*)\\);$", BUS_CALLBACK_OF(VTPlayerOutputAgent, handleStatic ));
	bindMessage( "^OUT VTP : (.*)=\\((.*)\\); dur=(.*);$", BUS_CALLBACK_OF(VTPlayerOutputAgent, handleDynamic ));
	bindMessage( "^OUT VTP : dir=(.*); type=(.*); cell=(.*);$", BUS_CALLBACK_OF(VTPlayerOutputAgent, handleDirection ));
	_vtpctrl = VTPMouseController::getInstance();
	_vtpctrl->run();

	//@TODO: add something in the VTPlayer API to be able to load one pattern with one instruction
	_sN->setXY(0, 0, '1');
	_sN->setXY(1, 0, '1');
	_sN->setXY(2, 0, '1');
	_sN->setXY(3, 0, '1');

	_sS->setXY(0, 3, '1');
	_sS->setXY(1, 3, '1');
	_sS->setXY(2, 3, '1');
	_sS->setXY(3, 3, '1');

	_sE->setXY(0, 0, '1');
	_sE->setXY(0, 1, '1');
	_sE->setXY(0, 2, '1');
	_sE->setXY(0, 3, '1');

	_sW->setXY(3, 0, '1');
	_sW->setXY(3, 1, '1');
	_sW->setXY(3, 2, '1');
	_sW->setXY(3, 3, '1');

	_sNE->setXY(1, 0, '1');
	_sNE->setXY(2, 0, '1');
	_sNE->setXY(3, 0, '1');
	_sNE->setXY(3, 1, '1');
	_sNE->setXY(3, 2, '1');

	_sNW->setXY(1, 3, '1');
	_sNW->setXY(2, 3, '1');
	_sNW->setXY(3, 3, '1');
	_sNW->setXY(3, 2, '1');
	_sNW->setXY(3, 1, '1');

	_sSE->setXY(0, 1, '1');
	_sSE->setXY(0, 2, '1');
	_sSE->setXY(0, 3, '1');
	_sSE->setXY(1, 3, '1');
	_sSE->setXY(2, 3, '1');

	_sSW->setXY(3, 1, '1');
	_sSW->setXY(3, 2, '1');
	_sSW->setXY(3, 3, '1');
	_sSW->setXY(2, 3, '1');
	_sSW->setXY(1, 3, '1');
}

VTPlayerOutputAgent::~VTPlayerOutputAgent()
{
	delete _left;
	delete _right;
	delete _vtpctrl;
	//unbind ?
}

void VTPlayerOutputAgent::handleStatic(MicoleBus *app, int argc, const char **argv)
{
	VTPStaticIcon newIcon = VTPStaticIcon(4,4);

	if (argc >= 1)
	{
		const char * pChar = argv[1];
		if (strlen(argv[1]) >= 16)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 4; i++)
				{
					newIcon.setXY(i, j, *pChar);
					pChar++;
				}
			}
		}
	}

	if (!strcmp(argv[0],"left"))
		_vtpctrl->setLeftIcon(&newIcon);
	else
		_vtpctrl->setRightIcon(&newIcon);
}


void VTPlayerOutputAgent::handleDynamic(MicoleBus *app, int argc, const char **argv)
{
	VTPDynamicIcon ico = VTPDynamicIcon(atof(argv[2]));
	VTPStaticIcon frame = VTPStaticIcon(4,4);

	string iconsList = string(argv[1]);

	vector<string> arr = explodestr(string(","), iconsList);

	for (unsigned int k = 0; k < arr.size(); k++)
	{
		const char * pChar = arr[k].c_str();
		if (strlen(arr[k].c_str()) >= 16)
		{
			for (int j=0; j<4; j++)
			{
				for (int i=0; i<4; i++)
				{
					frame.setXY(i,j,*pChar);
					pChar++;
				}
			}
		}
		ico.add(frame);
	}

	if (!strcmp(argv[0],"left"))
		_vtpctrl->setLeftIcon(&ico);
	else
		_vtpctrl->setRightIcon(&ico);
}


//"^OUT VTP : dir=(.*); type=(.*); cell=(.*);$"
void VTPlayerOutputAgent::handleDirection ( MicoleBus *app, int argc, const char **argv )
{
	if (argc < 3)
		return;

	//pretty ugly... we should modify some stuff in this agent...
	VTPStaticIcon *tobedisplayed = NULL;
	//at this time we consider we only want static icons...
	if (strcmp(argv[0], "N") == 0)
		tobedisplayed = _sN;
	else if (strcmp(argv[0], "S") == 0)
		tobedisplayed = _sS;
	else if (strcmp(argv[0], "E") == 0)
		tobedisplayed = _sE;
	else if (strcmp(argv[0], "W") == 0)
		tobedisplayed = _sW;
	else if (strcmp(argv[0], "NE") == 0)
		tobedisplayed = _sNE;
	else if (strcmp(argv[0], "NW") == 0)
		tobedisplayed = _sNW;
	else if (strcmp(argv[0], "SE") == 0)
		tobedisplayed = _sSE;
	else if (strcmp(argv[0], "SW") == 0)
		tobedisplayed = _sSW;
	if (!tobedisplayed)
		return;

	if (strcmp(argv[2], "right") == 0)
		_vtpctrl->setRightIcon(tobedisplayed);
	else if (strcmp(argv[2], "left") == 0)
		_vtpctrl->setLeftIcon(tobedisplayed);
}
