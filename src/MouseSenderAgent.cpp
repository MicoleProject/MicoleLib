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
#include "MouseSenderAgent.h"
#include "ReachinMicoleApplication.h"
#include <MicoleStringStream.h>

#include <Shape.h>
#include <Appearance.h>
#include <Material.h>
#include <Box.h>
/*#include <Sphere.h>*/

MouseSenderAgent::MouseSenderAgent(int interval):
	MicoleAgentThread("MouseSenderAgent","SenderAgent"), 
	_mouseMoveListener(new MouseMoveListener), 
	_leftMouseButtonListener(new MouseButtonListener("BT1_")),
	_rightMouseButtonListener(new MouseButtonListener("BT2_")),
	_mouse(new Mouse()),
	mouse_coordinates(new SFVec3f())
{
	_mouseMoveListener->_mssa=this;
	_leftMouseButtonListener->_mssa=this;
	_rightMouseButtonListener->_mssa=this;
	_mouse->position->route(_mouseMoveListener);
	_mouse->left_button->route(_leftMouseButtonListener);
	_mouse->right_button->route(_rightMouseButtonListener);
	_mouseMoveListener->route(_leftMouseButtonListener->mouse_coordinates);
	_mouseMoveListener->route(_rightMouseButtonListener->mouse_coordinates);
	sendMessage("MOUSE INPUT CREATED");
//	ReachinMicoleApplication::getInstance()->sendIvyMessage("MOUSE INPUT CREATED");

	_coordUpdInterval=interval;

/*
	Transform *xform1 = new Reachin::Transform();
	xform1->translation->set( Vec3f(0.05, 0.0, 0.0) );
	Shape *shape1 = new Shape();
	Shape *shape2 = new Shape();
	Appearance *app1 = new Appearance();
	Material *mat1 = new Material();
	Box *boxX = new Box();
	Box *boxY = new Box();
	boxX->size->set( Vec3f(0.005, 0.001, 0.001) );
	boxY->size->set( Vec3f(0.001, 0.005, 0.001) );
	mat1->diffuse_color->set( RGB(1.0, 0.0, 0.0) );
	app1->material->set( mat1 );
	shape1->appearance->set( app1 );
	shape2->appearance->set( app1 );
	shape1->geometry->set( boxX );
	shape2->geometry->set( boxY );
	xform1->children->add( shape1 );
	xform1->children->add( shape2 );
	ReachinMicoleApplication::getInstance()->getDisplay()->children->add( xform1 );
	mouse_coordinates->route(xform1->translation );*/

	start();
}

MouseSenderAgent::~MouseSenderAgent()
{

}

//	: mouse_coordinates(new SFVec2f())
MouseSenderAgent::MouseMoveListener::MouseMoveListener()  
{
}

void MouseSenderAgent::run()
{
	MicoleStringStream ss;
	Vec2f p;

	while(getState() == ACTIVE)
	{
		if(_coordUpdInterval > 0)
		{
			p = _mouseMoveListener->get();
		
/*			if(p.x < -0.16)
				p.x = -0.16;
			else if (p.x > 0.16)
				p.x = 0.16;

			if(p.y < -0.11)
				p.y = -0.11;
			else if (p.y > 0.11)
				p.y = 0.11;
*/
			ss << "IN MSE : pos=(";
			ss << p.x << ", " << p.y << ");";
		
			sendMessage(ss.str().c_str());
	
			ss.flush();
	
			::Sleep(_coordUpdInterval);
		}
		else
			::Sleep(50);
	}
}

void MouseSenderAgent::setCoordinateUpdateInterval(int interval)
{
	_coordUpdInterval=interval;
}


void MouseSenderAgent::MouseMoveListener::evaluate(SFVec2f *coord)
{
	MicoleStringStream ss;
	Vec2f in = coord->get();
	
	if(in.x < -0.15)
		in.x = -0.15;
	else if (in.x > 0.15)
		in.x = 0.15;

	if(in.y < -0.11)
		in.y = -0.11;
	else if (in.y > 0.11)
		in.y = 0.11;

	value =  Vec2f(in.x, in.y);
	_mssa->mouse_coordinates->set(Vec3f(in.x, in.y, 0));
	_mssa->mouse_coordinates->touch();
	
	/*ss << "IN MSE : pos=(";
	ss << mouse_coordinates->get().x << ", " << mouse_coordinates->get().y << ");";
	_mssa->sendMessage(ss.str().c_str());

	_mssa->sendMessage("coin");*/
}

MouseSenderAgent::MouseButtonListener::MouseButtonListener(string button)
:m_button(new SFBool()), mouse_coordinates(new SFVec2f())
{
	m_strButton=button;
}

void MouseSenderAgent::MouseButtonListener::evaluate(SFBool *mbutton)
{
	value =  mbutton;
	m_button->set(value);
	if(mbutton->get()==true)
		sendCoordinates(m_strButton + "PRESSED");
	else
		sendCoordinates(m_strButton + "RELEASED");
	m_button->touch();
}

/**
* state : "BT(1|2)_(PRESSED|RELEASED)"
**/
void MouseSenderAgent::MouseButtonListener::sendCoordinates(string state)
{
	MicoleStringStream ss;
	ss << "IN MSE : pos=(";
	ss << mouse_coordinates->get().x << ", " << mouse_coordinates->get().y << ");";
	ss << " evt=(" << state << ");";
	_mssa->sendMessage(ss.str().c_str());
}
