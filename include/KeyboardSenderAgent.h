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

#ifndef KeyboardSenderAgent_H
#define KeyboardSenderAgent_H

#include <Vrml.h>
#include <Keyboard.h>
using namespace Reachin;

#include "MicoleAgent.h"
#include "micolelib.h"

/*!
 * \brief
 * Send Keyboard Status on the bus
 * 
 * Keyboard status agent is directly linked with reachin KeyboardNode using a KeyboardListener Node.
 * 
 * \remark
 * without a Reachin context, you cannot use the KeyboardSenderAgent
 * 
 * \see
 * FF3DDeviceSenderAgent | VTPlayerSenderAgent | MouseSenderAgent
 */
class MICOLELIB_API KeyboardSenderAgent : public MicoleAgent
{
	public:
		/**
		 * default constructor
		 */
		KeyboardSenderAgent();
		/**
		 * default destructor
		 */
		~KeyboardSenderAgent();
		
	private:

		struct KeyboardListener : public Dependent< EvaldFField< KeyboardListener, MFInt32, MFInt32> >
		{
			void evaluate(MFInt32*);
			KeyboardSenderAgent *_kbssa;
		};
		auto_ptr< KeyboardListener > _keyboardListener;
		KeyboardSenderAgent(const KeyboardSenderAgent &);
		auto_ptr< Keyboard > _keyb;
};

#endif
