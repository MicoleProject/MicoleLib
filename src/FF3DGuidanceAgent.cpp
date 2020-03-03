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
#include "FF3DGuidanceAgent.h"
#include "define.h"


#include "FF3DSphericalForceModel.h"
#include "FF3DAttractivePointForceModel.h"
#include "FF3DDiscForceModel.h"
#include "FF3DLinearForceModel.h"
#include "FF3DPlanarForceModel.h"
#include "FF3DPicobHGForceModel.h"
#include "FF3DPicobGForceModel.h"

/*#include <Shape.h>
#include <Sphere.h>
#include <Cone.h>
#include <Appearance.h>
#include <FrictionalSurface.h>*/


FF3DGuidanceAgent::FF3DGuidanceAgent()
: Child( new FF3DGuidanceAgentCollider ), MicoleAgent("FF3DGuidanceAgent","Output"), _fm(NULL), _transform(new Transform())
{
	((FF3DGuidanceAgentCollider *)this->collider.get())->_parent = this;

	//add FF3DGuidanceAgent in scene
	Display * dis = ReachinMicoleApplication::getInstance()->getDisplay();
	
	_transform->children->add(this);
	dis->children->add( _transform );  

	/** dummy for test*/
	/*
#ifdef _DEBUG
	Shape * sh = new Shape;
	Cone * sph = new Cone;

	//FrictionalSurface * surf = new FrictionalSurface;
	sph->height->set(0.05);
	sph->bottom_radius->set(0.01);

	//surf->stiffness->set( 500 );

	Material * mat = new Material;
	mat->diffuse_color->set( RGB(0,1,0));

	Appearance * app = new Appearance;
	app->material->set(mat);
	//app->surface->set( surf );

	sh->appearance->set(app);
	sh->geometry->set(sph);

	_transform->children->add(sh);
#endif
	*/
	//en test

	sendMessage("Set stiffness = 1.5;");
	sendMessage("Set d = 0.02;");


	bindMessage( "^OUT FF3D GUIDE : point=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handlePointForceModel ));
	bindMessage( "^OUT FF3D GUIDE : sphere=\\((.*), (.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handleSphericalForceModel ));
	bindMessage( "^OUT FF3D GUIDE : line=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handleLinearForceModel ));
	bindMessage( "^OUT FF3D GUIDE : plan=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handlePlanarForceModel ));
	bindMessage( "^OUT FF3D GUIDE : disc=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\),(.*)\\);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handleDiscForceModel ));
	bindMessage( "^OUT FF3D GUIDE : none;$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handleNoForceModel ));
	bindMessage( "^OUT FF3D GUIDE : picobhg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\); bumps=\\((.*)\\); bumplength=(.*); pauselength=(.*);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handlePicobHgForceModel ));
	bindMessage( "^OUT FF3D GUIDE : picobg=\\((.*), (.*), (.*)\\); bumps=\\((.*)\\); bumplength=(.*); pauselength=(.*);$", 
		BUS_CALLBACK_OF(FF3DGuidanceAgent, handlePicobGForceModel ));
}

FF3DGuidanceAgent::~FF3DGuidanceAgent()
{
	delete _fm;
//	delete _transform;
}

void FF3DGuidanceAgent::handlePointForceModel ( MicoleBus *app, int argc, const char **argv )
{
	if (argc < 3)
		return;
	Vec3f position;
	try
	{
		position.x = atof(argv[0]);
		position.y = atof(argv[1]);
		position.z = atof(argv[2]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	_transform->translation->set(Vec3f(0,0,0));
	_transform->rotation->set( Rotation( Vec3f(0, 0, 0), 0 ) );

	setForceModel(new FF3DAttractivePointForceModel(position));
}

void FF3DGuidanceAgent::handleLinearForceModel ( MicoleBus *app, int argc, const char **argv )
{
		if (argc < 6)
		return;
	Vec3f pt1;
	Vec3f pt2;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);

		//_transform->translation->set(pt1);
		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	setForceModel(new FF3DLinearForceModel(pt1,pt2));

	//we compute the euclidian norma
	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( (pt1+pt2)/2 ); 
}

void FF3DGuidanceAgent::handlePlanarForceModel ( MicoleBus *app, int argc, const char **argv ) 
{
	if (argc < 6)
		return;

	Vec3f pt1;
	Vec3f pt2;
	Vec3f pt3;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);
		_transform->translation->set(pt1);

		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{ 
		return;
	}

	/*pt3.x = atof(argv[6]);
	pt3.y = atof(argv[7]);
	pt3.z = atof(argv[8]);*/

	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( pt1 ); 

	setForceModel(new FF3DPlanarForceModel(pt1,pt2));
}

void FF3DGuidanceAgent::handleSphericalForceModel ( MicoleBus *app, int argc, const char **argv )
{
	if (argc < 4)
		return;

	Vec3f position;
	mgFloat range;
	try
	{
		position.x = atof(argv[0]);
		position.y = atof(argv[1]);
		position.z = atof(argv[2]);
		range = atof(argv[3]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	_transform->translation->set(position);
	_transform->rotation->set( Rotation( Vec3f(0, 0, 0), 0 ) );

	//setForceModel(new FF3DPointForceModel(position));
	setForceModel( new FF3DSphericalForceModel(Vec3f(0,0,0),range) );
}

void FF3DGuidanceAgent::handleDiscForceModel ( MicoleBus *app, int argc, const char **argv ) 
{
	if (argc < 7)
		return;

	Vec3f pt1;
	Vec3f pt2;
	Vec3f pt3;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);
		_transform->translation->set(pt1);

		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	mgFloat range = atof(argv[6]);
	
	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( pt1 ); 

	setForceModel(new FF3DDiscForceModel(pt1,pt2,range));
}

void FF3DGuidanceAgent::handleNoForceModel ( MicoleBus *app, int argc, const char **argv ) 
{
	setForceModel(NULL);
}

void FF3DGuidanceAgent::handlePicobHgForceModel ( MicoleBus *app, int argc, const char **argv )
{
	if (argc < 9)
		return;

	Vec3f pt1;
	Vec3f pt2;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);

		//_transform->translation->set(pt1);
		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...)
	{ //can append for truncated packet or bad transmission
	}

	//we compute the euclidian norma
	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( (pt1+pt2)/2 ); 
	
	vector<Vec3f> bumpList;
	Picob::readPicobList(argv[6], bumpList);
	setForceModel(new FF3DPicobHGForceModel(pt1, pt2, bumpList, atof(argv[7]), atof(argv[8])));
}

void FF3DGuidanceAgent::handlePicobGForceModel ( MicoleBus *app, int argc, const char **argv )
{
	if (argc < 6)
		return;

	Vec3f pt;
	try {
		pt.x = atof(argv[0]);
		pt.y = atof(argv[1]);
		pt.z = atof(argv[2]);
	}
	catch (...)
	{ //can append for truncated packet or bad transmission
	}


	vector<Vec3f> bumpList;
	Picob::readPicobList(argv[3], bumpList);
	setForceModel(new FF3DPicobGForceModel(pt, bumpList, atof(argv[4]), atof(argv[5])));

	_transform->rotation->set( Rotation( Vec3f(0, 0, 0), 0 ) );
	_transform->translation->set(Vec3f(0,0,0));
}
