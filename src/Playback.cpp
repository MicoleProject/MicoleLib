/* 

     The contents of this file are subject to the Mozilla Public License
	 Version 1.1 (the "License"); you may not use this file except in
     compliance with the License. You may obtain a copy of the License at
     http://www.mozilla.org/MPL/

     Software distributed under the License is distributed on an "AS IS"
     basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
     License for the specific language governing rights and limitations
     under the License.

     The Original Code is the PID trajectory playback library.

     The Initial Developers of the Original Code are Andrew Crossan (ac@dcs.gla.ac.uk) 
	 and John Williamson (jhw@dcs.gla.ac.uk) from University of Glasgow.
	 
     Portions created by Andrew Crossan and John Williamson are Copyright (C) 2006.
     All Rights Reserved.

     Contributor(s): ______________________________________.

*/


// Playback.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Playback.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"


// Playback construction

Playback::Playback()
{
	beadSpeed = BEAD_SPEED;
	head = 0;
	cur = 0;
	end = 0;

	headToPt = 0;
	curToPt = 0;
	endToPt = 0;

	mode = PID_IDLE;

	playAtSampleRate = PLAY_AT_SAMPLE_RATE;
	absolutePositioning = ABSOLUTE_POSITIONING;
	err_thresh = ERROR_THRESHOLD;

	//debug_fp = fopen("debug.txt", "w");
}

Playback::~Playback()
{
	SetIdle();
	ClearPath();
	//fclose(debug_fp);
}

void Playback::CreateControllers(double p, double i, double d, double f, double out_filter,
									 double gain, double sat_low, double sat_high, double dead_zone)
{

	control_x = init_state(p, i, d, f, out_filter, gain, dead_zone, sat_low, sat_high);
	control_y = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);
	control_z = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);

	head = 0;
	cur = 0;
	end = 0;

	mode = PID_IDLE;
}

void Playback::SetIntegratorForceCap(double maxforce)
{
	double wind_up_max = 1/control_x->gain * 1/control_x->cn_i * maxforce;
	set_integrator_cap(control_x, wind_up_max);
	set_integrator_cap(control_y, wind_up_max);
	set_integrator_cap(control_z, wind_up_max);
}

void Playback::ResetControllerState(){
	reset_state(control_x);
	reset_state(control_y);
	reset_state(control_z);
}

void Playback::CreateControllersForOmni(){

	/* Position controller */
	double p = 0.3;
	double i = 0.0005;
	double d = 20.0;
	double f = 0.09;
	double out_filter = 0.09;
	double gain = 0.2;
	double sat_low = -3.0;
	double sat_high = 3.0;
	double dead_zone = 0.0;

	double wind_up_max = 1/gain * 1/i * sat_high;

	

	control_x = init_state(p, i, d, f, out_filter, gain, dead_zone, sat_low, sat_high);
	control_y = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);
	control_z = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);
	set_integrator_cap(control_x, wind_up_max);
	set_integrator_cap(control_y, wind_up_max);	
	set_integrator_cap(control_z, wind_up_max);


}

void Playback::CreatePDControllersForOmni(){

	/* Position controller */
	double p = 0.3;
	double i = 0.0;
	double d = 20.0;
	double f = 0.09;
	double out_filter = 0.09;
	double gain = 0.2;
	double sat_low = -3.0;
	double sat_high = 3.0;
	double dead_zone = 0.0;

	control_x = init_state(p, i, d, f, out_filter, gain, dead_zone, sat_low, sat_high);
	control_y = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);
	control_z = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);

	

}

void Playback::stickToPointAndGetForce(Vec3f pos, Vec3f dest, Vec3f &force)
{
	double ps[3] = {pos.x, pos.y, pos.z};
	double ds[3] = {dest.x, dest.y, dest.z};
	double fs[3] = {0, 0, 0};
	//StickToPoint(ps, ds);
	MoveToPoint(ps, ds, PID_PLAY);
	GetForce(ps, fs);
	force.x = fs[0]*_PLAY_MICOLE_RATE;
	force.y = fs[1]*_PLAY_MICOLE_RATE;
	force.z = fs[2]*_PLAY_MICOLE_RATE;
}

void Playback::CreateControllersForPremium15(){

	/* Position controller */
	double p = 0.3;
	double i = 0.00005;
	double d = 30.0;
	double f = 0.09;
	double out_filter = 0.09;
	double gain = 0.20;
	double sat_low = -1.5;
	double sat_high = 1.5;
	double dead_zone = 0.0;

	double wind_up_max = 1/gain * 1/i * sat_high;

	control_x = init_state(p, i, d, f, out_filter, gain, dead_zone, sat_low, sat_high);
	control_y = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);
	control_z = init_state(p, i, d, f, out_filter, gain, dead_zone,sat_low, sat_high);

	set_integrator_cap(control_x, wind_up_max);
	set_integrator_cap(control_y, wind_up_max);	
	set_integrator_cap(control_z, wind_up_max);


}

void Playback::SetPlaybackSpeed(double v){
	beadSpeed = v;
}

void Playback::StartPlayback(PtsList *pt, double curPos[3]){
// start playback of the path referenced by pt

	if(pt == 0)
		return;

	startTime = ((double)clock()/(double)CLOCKS_PER_SEC);

	ClearPath();

	head = pt;
	cur = head;

	//find and store the end of the list
	PtsList *tmp = pt;
	while(tmp->next != 0)
		tmp = tmp->next;
	
	// tmp is now the end of the list
	end = tmp;
	
	if(!absolutePositioning) // then just play the gesture from where we are
		mode = PID_PLAY;
	else{					// else, move to the start point first
		double pt[3];
		pt[0] = head->xPos; pt[1] = head->yPos; pt[2] = head->zPos; 

		MoveToPoint(curPos, pt, PID_PLAY);
	}

	prevTime = 0;

	startPos[0] = curPos[0];
	startPos[1] = curPos[1];
	startPos[2] = curPos[2];

	if(cur != 0){
		beadPos[0] = cur->xPos;
		beadPos[1] = cur->yPos;
		beadPos[2] = cur->zPos;	
	}

	//ResetControllerState();

}

void Playback::StartPlayback(double curPos[3]){

	if(head == 0)
		return;

	startTime = ((double)clock()/(double)CLOCKS_PER_SEC);
	prevTime = 0;
	cur = head;

	
	if(!absolutePositioning) // then just play the gesture from where we are
		mode = PID_PLAY;
	else{					// else, move to the start point first
		double pt[3];
		pt[0] = head->xPos; pt[1] = head->yPos; pt[2] = head->zPos; 

		MoveToPoint(curPos, pt, PID_PLAY);
	}
	
	//ResetControllerState();

	startPos[0] = curPos[0];
	startPos[1] = curPos[1];
	startPos[2] = curPos[2];

	if(cur != 0){
		beadPos[0] = cur->xPos;
		beadPos[1] = cur->yPos;
		beadPos[2] = cur->zPos;	
	}
}

bool Playback::GetForce(double pt[3], double force[3]){

	force[0] = 0;
	force[1] = 0;
	force[2] = 0;

	if(mode == PID_IDLE || mode == PID_PAUSE)
		return true;

	if(mode == PID_CONSTRAIN){
		bool continueConstrain = DoConstrain(pt, force);

		if(!continueConstrain)
			mode = PID_IDLE;

		return continueConstrain;
	}

	double theTime = ((double)clock()/(double)CLOCKS_PER_SEC) - startTime;
	double distToTravel;

	double dt = theTime - prevTime;
	if(dt < 0.001)
		dt = 0.001;
	
	prevTime = theTime;

	force[0] = 0; force[1] = 0; force[2] = 0;

	double magBeadToNext;

	if(mode == PID_STICK_TO_POINT || mode == PID_MOVE_TO_STICK_POINT 
		|| mode == PID_MOVE_TO_PLAY_POINT || mode == PID_MOVE_TO_CONSTRAIN_POINT){


		if(curToPt == 0){
			//then reached the stick point so hold there
			force[0] += control(control_x, pt[0], endToPt->xPos) /*+ control(control_vx, vel[0], 0)*/; 
			force[1] += control(control_y, pt[1], endToPt->yPos) /*+ control(control_vy, vel[1], 0)*/;
			force[2] += control(control_z, pt[2], endToPt->zPos) /*+ control(control_vz, vel[2], 0)*/;


		FILE *fp;
		fopen_s(&fp, "d.txt", "a");
		fprintf(fp, "%f, %f, %f, %f, %f, %f, %f, %f\n", control_x->p, control_x->i, control_x->d, 
			control_x->x, control_x->target_x, force[0], force[1], force[2]);
		fclose(fp);		
		
			//fprintf(debug_fp, "%f, %f, %f, %f, %f, %f, %f, 1\n", dt, pt[0], pt[1], pt[2], 
			//	endToPt->xPos, endToPt->yPos, endToPt->zPos);

			if(mode == PID_MOVE_TO_STICK_POINT) // if we were moving to the stick point, then we've reached it
				mode = PID_STICK_TO_POINT;

			if(mode == PID_MOVE_TO_PLAY_POINT) // if we were moving to the start of a gesture, then play gesture
				mode = PID_PLAY;

			if(mode == PID_MOVE_TO_CONSTRAIN_POINT) // if we were moving to the start of a gesture, then play gesture
				mode = PID_CONSTRAIN;

			return true;
		}

		if(curToPt->next == 0){
			//then reached the stick point so hold there
			force[0] += control(control_x, pt[0], endToPt->xPos);
			force[1] += control(control_y, pt[1], endToPt->yPos);
			force[2] += control(control_z, pt[2], endToPt->zPos);

			//fprintf(debug_fp, "%f, %f, %f, %f, %f, %f, %f, 2\n", dt, pt[0], pt[1], pt[2], 
			///endToPt->xPos, endToPt->yPos, endToPt->zPos);

			if(mode == PID_MOVE_TO_STICK_POINT) // if we were moving to the stick point, then we've reached it
				mode = PID_STICK_TO_POINT;

			if(mode == PID_MOVE_TO_PLAY_POINT) // if we were moving to the start of a gesture, then play gesture
				mode = PID_PLAY;

			if(mode == PID_MOVE_TO_CONSTRAIN_POINT) // if we were moving to the start of a gesture, then play gesture
				mode = PID_CONSTRAIN;

			return true;
		}

		
		if(!(abs(control_x->p)>err_thresh 
			|| abs(control_y->p)>err_thresh 
			|| abs(control_z->p)>err_thresh)) {

			// work out distance to the next point
			// check if it passes the current point
			// then move the bead onwards
			
			double stickBeadToNext[3];
			stickBeadToNext[0] = curToPt->next->xPos - stickBeadPos[0];
			stickBeadToNext[1] = curToPt->next->yPos - stickBeadPos[1];
			stickBeadToNext[2] = curToPt->next->zPos - stickBeadPos[2];

			magBeadToNext = sqrt(pow(stickBeadToNext[0], 2.0) + 
				pow(stickBeadToNext[1], 2.0) + pow(stickBeadToNext[2], 2.0));

			distToTravel = beadSpeed * dt;

			if(magBeadToNext < distToTravel){

				curToPt = curToPt->next;

				stickBeadPos[0] = endToPt->xPos;
				stickBeadPos[1] = endToPt->yPos;
				stickBeadPos[2] = endToPt->zPos;
			

				distToTravel = 0;
			
			}
			else if(magBeadToNext > 0){
				stickBeadPos[0] += distToTravel * (stickBeadToNext[0]/magBeadToNext);
				stickBeadPos[1] += distToTravel * (stickBeadToNext[1]/magBeadToNext);
				stickBeadPos[2] += distToTravel * (stickBeadToNext[2]/magBeadToNext);
			}
		}
	
		force[0] += control(control_x, pt[0], stickBeadPos[0]) /*+ control(control_vx, vel[0], 0)*/; 
		force[1] += control(control_y, pt[1], stickBeadPos[1]) /*+ control(control_vy, vel[1], 0)*/;
		force[2] += control(control_z, pt[2], stickBeadPos[2]) /*+ control(control_vz, vel[2], 0)*/;

		//fprintf(debug_fp, "%f, %f, %f, %f, %f, %f, %f, 0\n", dt, pt[0], pt[1], pt[2], 
		//	stickBeadPos[0], stickBeadPos[1], stickBeadPos[2]);

		return true; // end stick to point
	}
	
	// if we reach here then the mode is play

	if(cur == 0){
		mode  = PID_IDLE;

		return false;
	}

	if(cur->next == 0){
		mode  = PID_IDLE;

		return false;
	}

	// work out path of bead movement
	if(playAtSampleRate){
		beadPos[0] = cur->xPos;
		beadPos[1] = cur->yPos;
		beadPos[2] = cur->zPos;

		cur = cur->next;

	}
	else if(!(abs(control_x->p)>err_thresh 
		|| abs(control_y->p)>err_thresh
		|| abs(control_z->p)>err_thresh)) {

		// work out distance to the next point
		// check if it passes the current point
		// then move the bead onwards
		
		double beadToNext[3];
		beadToNext[0] = cur->next->xPos - beadPos[0];
		beadToNext[1] = cur->next->yPos - beadPos[1];
		beadToNext[2] = cur->next->zPos - beadPos[2];

		double magBeadToNext = sqrt(pow(beadToNext[0], 2.0) + 
			pow(beadToNext[1], 2.0) + pow(beadToNext[2], 2.0));

		double distToTravel = beadSpeed * dt;

		while(cur->next != 0 && magBeadToNext < distToTravel){

			cur = cur->next;

			beadPos[0] = cur->xPos;
			beadPos[1] = cur->yPos;
			beadPos[2] = cur->zPos;

			distToTravel -= magBeadToNext;

			if(cur->next == 0){					
				distToTravel = 0;
				break;
			}


			beadToNext[0] = cur->next->xPos - beadPos[0];
			beadToNext[1] = cur->next->yPos - beadPos[1];
			beadToNext[2] = cur->next->zPos - beadPos[2];

			magBeadToNext = sqrt(pow(beadToNext[0], 2.0) + 
				pow(beadToNext[1], 2.0) + pow(beadToNext[2], 2.0));

		}

		if(cur->next == 0){
			mode  = PID_IDLE;

			return false;
		}

		beadPos[0] += distToTravel * (beadToNext[0]/magBeadToNext);
		beadPos[1] += distToTravel * (beadToNext[1]/magBeadToNext);
		beadPos[2] += distToTravel * (beadToNext[2]/magBeadToNext);

	}
	else
	{
	
	}
	/* Do control */
	// Change the last zero to cur->xVel etc for vel. control as well
	if(absolutePositioning){
		force[0] += control(control_x, pt[0], beadPos[0]) /*+ control(control_vx, vel[0], 0)*/; 
		force[1] += control(control_y, pt[1], beadPos[1]) /*+ control(control_vy, vel[1], 0)*/;
		force[2] += control(control_z, pt[2], beadPos[2]) /*+ control(control_vz, vel[2], 0)*/;
	}
	else{
		force[0] += control(control_x, pt[0]-startPos[0], beadPos[0]-head->xPos) /*+ control(control_vx, vel[0], 0)*/; 
		force[1] += control(control_y, pt[1]-startPos[1], beadPos[1]-head->yPos) /*+ control(control_vy, vel[1], 0)*/;
		force[2] += control(control_z, pt[2]-startPos[2], beadPos[2]-head->zPos) /*+ control(control_vz, vel[2], 0)*/;
	}
	return true;
}

void Playback::ClearPath(){
	

	PtsList *pt = head, *next_pt;

	while(pt != 0){
		next_pt = pt->next;
		
		free(pt);
		pt = next_pt;
	}

	head = 0; end = 0;
	cur = 0;
}

bool Playback::LoadGestureFile(char *fName){

	FILE *fp;
	fopen_s(&fp, fName, "r");

	if(!fp)
		return false;

	int ret;
	float t, x, y, z;
	double pt[3];

	ClearPath();
	do{
		ret = fscanf_s(fp, "%f, %f, %f, %f\n", &t, &x, &y, &z);
		pt[0] = x; pt[1] = y; pt[2] = z;
		AddWaypoint(pt);
	}
	while(ret != EOF && ret == 4);

	fclose(fp);

	return true;
}

bool Playback::SaveGestureFile(char *fName)
{
	FILE *fp;
	fopen_s(&fp, fName, "w");

	if(!fp)
		return false;

	PtsList *tmp = head;
	while(tmp != 0){
		fprintf(fp, "0.0, %f, %f, %f\n",  tmp->xPos, tmp->yPos, tmp->zPos);
		tmp = tmp->next;
	}

	fclose(fp);

	return true;
}

void Playback::AddWaypoint(double pt[3]){

	// add to the end of the list
	PtsList *tmp = new PtsList;
	tmp->xPos = pt[0];
	tmp->yPos = pt[1];
	tmp->zPos = pt[2];

	tmp->next = 0;

	if(head == 0){ // then first value in the list
		head = tmp;
		end = tmp;
		tmp->prev = 0;
	}
	else{
		end->next = tmp;
		tmp->prev = end;
		end = tmp;
	}
}


void Playback::MoveToStartOfGesture(){
	cur = head;
}

void Playback::SetIdle(){
	
	mode = PID_IDLE;
	cur = head;

//	ResetControllerState();
}

void Playback::TogglePauseGesture(){
	
	if(mode == PID_PLAY)
		mode = PID_PAUSE;
	else if(mode == PID_PAUSE)
		mode = PID_PLAY;

}


Mode Playback::GetMode(){
	return mode;
}

void Playback::CreateGestureToPoint(double curPos[3], double thePoint[3]){

	FreeGestureToPoint();

	PtsList *tmp;
	tmp = (PtsList *) malloc(sizeof(PtsList));
	tmp->xPos = curPos[0]; 
	tmp->yPos = curPos[1]; 
	tmp->zPos = curPos[2];
	tmp->next = 0; 
	tmp->prev = 0;			

	headToPt = tmp; 
	endToPt = tmp;
	
	tmp = (PtsList *) malloc(sizeof(PtsList));
	tmp->xPos = thePoint[0];
	tmp->yPos = thePoint[1];
	tmp->zPos = thePoint[2];
	tmp->next = 0;
	tmp->prev = endToPt;			

	endToPt->next = tmp;
	endToPt = tmp;

	tmp = (PtsList *) malloc(sizeof(PtsList));
	tmp->xPos = thePoint[0];
	tmp->yPos = thePoint[1];
	tmp->zPos = thePoint[2];
	tmp->next = 0;
	tmp->prev = endToPt;			

	endToPt->next = tmp;
	endToPt = tmp;
	
	curToPt = headToPt;

	stickBeadPos[0] = curToPt->xPos;
	stickBeadPos[1] = curToPt->yPos;
	stickBeadPos[2] = curToPt->zPos;

//	reset_state(control_x);
//	reset_state(control_y);
//	reset_state(control_z);

}


void Playback::FreeGestureToPoint(){

	PtsList *pt = headToPt, *next_pt;

	while(pt != 0){
		next_pt = pt->next;
		
		free(pt);
		pt = next_pt;
	}

	headToPt = 0; endToPt = 0;
	curToPt = 0;

}

void Playback::StickToThisPoint(double curPos[3]){
	FreeGestureToPoint();

	startTime = ((double)clock()/(double)CLOCKS_PER_SEC);
	prevTime = 0;

	PtsList *tmp = (PtsList *) malloc(sizeof(PtsList));
	tmp->xPos = curPos[0];
	tmp->yPos = curPos[1];
	tmp->zPos = curPos[2];
	tmp->next = 0;
	tmp->prev = 0;			

	endToPt = tmp;
	curToPt = 0;

	mode = PID_STICK_TO_POINT;

	ResetControllerState();
}

void Playback::StickToPoint(double curPos[3], double pt[3]){
	FreeGestureToPoint();
	CreateGestureToPoint(curPos, pt);
	startTime = ((double)clock()/(double)CLOCKS_PER_SEC);
	prevTime = 0;
	
	mode = PID_MOVE_TO_STICK_POINT;

	ResetControllerState();
}

void Playback::MoveToPoint(double curPos[3], double pt[3], Mode m){
	
	CreateGestureToPoint(curPos, pt);
	
	startTime = ((double)clock()/(double)CLOCKS_PER_SEC);
	prevTime = 0;
	
	if(m == PID_PLAY)
		mode = PID_MOVE_TO_PLAY_POINT;
	else if(mode == PID_CONSTRAIN)
		mode = PID_MOVE_TO_CONSTRAIN_POINT;

	//ResetControllerState();
}

void Playback::SetPlayAtSampleRate(bool pasr){
	playAtSampleRate = pasr;
}

bool Playback::GetPlayAtSampleRate(){
	return playAtSampleRate;
}

void Playback::UseAbsolutePositioning(bool absPos){
	absolutePositioning = absPos;
}

bool Playback::GetPlaybackPos(double pPos[3]){
	if(mode == PID_IDLE)
		return false;

	if(mode == PID_PLAY || mode == PID_PAUSE){
		pPos[0] = beadPos[0];
		pPos[1] = beadPos[1];
		pPos[2] = beadPos[2];
	}
	else if(mode == PID_STICK_TO_POINT){
		pPos[0] = stickPoint[0];
		pPos[1] = stickPoint[1];
		pPos[2] = stickPoint[2];
	}
	else if(mode == PID_MOVE_TO_PLAY_POINT || mode == PID_MOVE_TO_CONSTRAIN_POINT
		|| mode == PID_MOVE_TO_STICK_POINT){
		pPos[0] = stickBeadPos[0];
		pPos[1] = stickBeadPos[1];
		pPos[2] = stickBeadPos[2];
	}

	return true;
}


double Playback::GetErrorThreshold(){
	return err_thresh;
}

void Playback::SetErrorThreshold(double e){
	err_thresh = e;
}


// this version of DoConstrain Allows the playback to move backwards
// and foorwards but struggles when the trajectory constains sharp turns
//bool Playback::DoConstrain(double pPos[3], double force[3]){
//	
//	force[0] = 0;
//	force[1] = 0;
//	force[2] = 0;
//
//	if(cur == 0)
//		return false;
//
//	if(cur->prev == 0){	// then at the start of the trajectory
//		beadPos[0] = cur->xPos;
//		beadPos[1] = cur->yPos;
//		beadPos[2] = cur->zPos;
//
//		cur = cur->next;
//
//	}
//	else{					//in the middle of the trajectory
//		
//		double prevToCur[3], prevToPhntm[3], magPrevToCur, magPrevToPhntm;
//
//		prevToCur[0] = cur->xPos - cur->prev->xPos;
//		prevToCur[1] = cur->yPos - cur->prev->yPos;
//		prevToCur[2] = cur->zPos - cur->prev->zPos;
//
//		magPrevToCur = sqrt(pow(prevToCur[0], 2.0) + pow(prevToCur[1], 2.0) + 
//			pow(prevToCur[2], 2.0));
//
//		prevToPhntm[0] = (pPos[0]/*-startPos[0]*/) - cur->prev->xPos;
//		prevToPhntm[1] = (pPos[1]/*-startPos[1]*/) - cur->prev->yPos;
//		prevToPhntm[2] = (pPos[2]/*-startPos[2]*/) - cur->prev->zPos;
//
//		magPrevToPhntm = sqrt(pow(prevToPhntm[0], 2.0) + pow(prevToPhntm[1], 2.0) + 
//			pow(prevToPhntm[2], 2.0));
//		
//		a = 0;	
//			
//		if(magPrevToCur != 0 && magPrevToPhntm != 0){
//			a = acos(((prevToCur[0] * prevToPhntm[0]) + (prevToCur[1] * prevToPhntm[1])+
//					(prevToCur[2] * prevToPhntm[2]))/(magPrevToCur * magPrevToPhntm));
//		}
//
//        distFromPrev = cos(a) * magPrevToPhntm;
//		double distToTravel = distFromPrev;// - magPrevToCur;
//		bool done = false;
//
//		if(a > PID_PI_BY_2 && a < 3.0 * PID_PI_BY_2 && cur != 0 && distFromPrev < magPrevToCur){ // then move backwards
//			if(cur->prev != 0){
//				if(cur->prev->prev)
//				cur = cur->prev;
//			}	
//			beadPos[0] = cur->xPos;
//			beadPos[1] = cur->yPos;
//			beadPos[2] = cur->zPos;
//			
//		}
//		else{
//			while(cur != 0 && !done){
//
//				if(distToTravel > magPrevToCur){
//					distToTravel -= magPrevToCur;
//		
//					if(a < PID_PI_BY_2 || a > 3.0 * PID_PI_BY_2)// then move forwards
//						cur = cur->next;
//					else 
//
//					if(cur == 0)
//						return false;
//
//					prevToCur[0] = cur->xPos - cur->prev->xPos;
//					prevToCur[1] = cur->yPos - cur->prev->yPos;
//					prevToCur[2] = cur->zPos - cur->prev->zPos;
//					
//					magPrevToCur = sqrt(pow(prevToCur[0], 2.0) + pow(prevToCur[1], 2.0) + 
//						pow(prevToCur[2], 2.0));
//				}
//				else{
//					done = true;
//				}
//				
//			}
//		
//			if(cur == 0)
//				return false;
//
//			if(a < PID_PI_BY_2 || a > 3.0 * PID_PI_BY_2){// then move forwards
//				beadPos[0] = cur->prev->xPos + (prevToCur[0]/magPrevToCur) * distToTravel;
//				beadPos[1] = cur->prev->yPos + (prevToCur[1]/magPrevToCur) * distToTravel;
//				beadPos[2] = cur->prev->zPos + (prevToCur[2]/magPrevToCur) * distToTravel;
//			}
//			else if(a > PID_PI_BY_2 && a < 3.0 * PID_PI_BY_2){ // then move backwards
//				beadPos[0] = cur->prev->xPos + (prevToCur[0]/magPrevToCur) * distToTravel;
//				beadPos[1] = cur->prev->yPos + (prevToCur[1]/magPrevToCur) * distToTravel;
//				beadPos[2] = cur->prev->zPos + (prevToCur[2]/magPrevToCur) * distToTravel;
//				beadPos[0] = cur->xPos - (prevToCur[0]/magPrevToCur) * distToTravel;
//				beadPos[1] = cur->yPos - (prevToCur[1]/magPrevToCur) * distToTravel;
//				beadPos[2] = cur->zPos - (prevToCur[2]/magPrevToCur) * distToTravel;
//			}
//			else{
//				beadPos[0] = cur->xPos;
//				beadPos[1] = cur->yPos;
//				beadPos[2] = cur->zPos;
//			}
//		}
//	}
//
//
//	force[0] = control(control_x, pPos[0]/*-startPos[0]*/, beadPos[0]); 
//	force[1] = control(control_y, pPos[1]/*-startPos[0]*/, beadPos[1]);
//	force[2] = control(control_z, pPos[2]/*-startPos[0]*/, beadPos[2]);
//
//	return true;
//}

bool Playback::DoConstrain(double pPos[3], double force[3]){
	
	force[0] = 0;
	force[1] = 0;
	force[2] = 0;

	if(cur == 0)
		return false;

	if(cur->prev == 0){	// then at the start of the trajectory
		beadPos[0] = cur->xPos;
		beadPos[1] = cur->yPos;
		beadPos[2] = cur->zPos;

		cur = cur->next;

	}
	else{					//in the middle of the trajectory
		
		double prevToCur[3], prevToPhntm[3], magPrevToCur, magPrevToPhntm;

		prevToCur[0] = cur->xPos - cur->prev->xPos;
		prevToCur[1] = cur->yPos - cur->prev->yPos;
		prevToCur[2] = cur->zPos - cur->prev->zPos;

		magPrevToCur = sqrt(pow(prevToCur[0], 2.0) + pow(prevToCur[1], 2.0) + 
			pow(prevToCur[2], 2.0));

		prevToPhntm[0] = (pPos[0]/*-startPos[0]*/) - cur->prev->xPos;
		prevToPhntm[1] = (pPos[1]/*-startPos[1]*/) - cur->prev->yPos;
		prevToPhntm[2] = (pPos[2]/*-startPos[2]*/) - cur->prev->zPos;

		magPrevToPhntm = sqrt(pow(prevToPhntm[0], 2.0) + pow(prevToPhntm[1], 2.0) + 
			pow(prevToPhntm[2], 2.0));
		
		a = 0;	
			
		if(magPrevToCur != 0 && magPrevToPhntm != 0){
			a = acos(((prevToCur[0] * prevToPhntm[0]) + (prevToCur[1] * prevToPhntm[1])+
					(prevToCur[2] * prevToPhntm[2]))/(magPrevToCur * magPrevToPhntm));
		}

        distFromPrev = cos(a) * magPrevToPhntm;
		double distToTravel = distFromPrev;
		bool done = false;

		while(cur != 0 && !done){

			if(distToTravel > magPrevToCur){
				distToTravel -= magPrevToCur;
	
				cur = cur->next;
				if(cur == 0)
					return false;

				curConstrainDist = 0;

				prevToCur[0] = cur->xPos - cur->prev->xPos;
				prevToCur[1] = cur->yPos - cur->prev->yPos;
				prevToCur[2] = cur->zPos - cur->prev->zPos;
				
				magPrevToCur = sqrt(pow(prevToCur[0], 2.0) + pow(prevToCur[1], 2.0) + 
					pow(prevToCur[2], 2.0));
			}
			else{
				done = true;
			}
			
			if(distToTravel > curConstrainDist)
				curConstrainDist = distToTravel;
		}
	
		if(cur == 0)
			return false;

		beadPos[0] = cur->prev->xPos + (prevToCur[0]/magPrevToCur) * curConstrainDist;
		beadPos[1] = cur->prev->yPos + (prevToCur[1]/magPrevToCur) * curConstrainDist;
		beadPos[2] = cur->prev->zPos + (prevToCur[2]/magPrevToCur) * curConstrainDist;
	}


	force[0] = control(control_x, pPos[0]/*-startPos[0]*/, beadPos[0]); 
	force[1] = control(control_y, pPos[1]/*-startPos[0]*/, beadPos[1]);
	force[2] = control(control_z, pPos[2]/*-startPos[0]*/, beadPos[2]);

	return true;
}


void Playback::StartConstrain(double curPos[3]){

	if(head == 0)
		return;

	startTime = ((double)clock()/(double)CLOCKS_PER_SEC);
	prevTime = 0;
	cur = head->next;

	
	if(!absolutePositioning) // then just play the gesture from where we are
		mode = PID_CONSTRAIN;
	else{					// else, move to the start point first
		double pt[3];
		pt[0] = head->xPos; pt[1] = head->yPos; pt[2] = head->zPos; 

		MoveToPoint(curPos, pt, PID_CONSTRAIN);
	}

	startPos[0] = curPos[0];
	startPos[1] = curPos[1];
	startPos[2] = curPos[2];

	if(cur != 0){
		beadPos[0] = cur->prev->xPos;
		beadPos[1] = cur->prev->yPos;
		beadPos[2] = cur->prev->zPos;	
	}

	curConstrainDist = 0;
}

//double Playback::AngleBetweenVecs(double p1[3], double p2[3]){
//
//	double magP1, magP2;
//
//	magP1 = sqrt(pow(p1[0], 2.0) + pow(p1[1], 2.0) + pow(p1[2], 2.0));
//
//	magP2 = sqrt(pow(p2[0], 2.0) + pow(p2[1], 2.0) + pow(p2[2], 2.0));
//	
//	double a = 0;
//		
//	if(magP1 != 0 && magP2 != 0){
//		a = acos(((p1[0] * p2[0]) + (p1[1] * p2[1]) + (p1[2] * p2[2]))/(magP1 * magP2));
//	}
//
//	return a;
//
//}
//
//double Playback::GetAdjacentDist(double ang, double hypot){
//
//}