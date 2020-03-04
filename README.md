# MicoleLib
Micole library, for developping multimodal applications. This work is part of the [MICOLE project](http://micole.cs.uta.fi/), funded by the European Union through the [FP6 progam](https://ec.europa.eu/growth/sectors/space/research/fp6_en), grant agreement #511592.

# Authors
* Jérôme Wax
* Thomas Pietrzak
* Nicolas Noble

# Concept
The MicoleLib is based on a software bus which can transfer informations beetween different "agents". Each agent (MicoleAgent class) can provides differents "services" (constraint Phantom device on a point, display an object on a Braille cell) but it can have a simple task to do (send the Phantom's coordinates on the bus, send VTPlayer status).

All the commands and informations are sent on the bus like broadcasting on a network. When the system requires information, it can find it on the bus. Some agents need their own thread because they work in parallel with the main program (coordinates sender agent for example). The goal is to be able to render the same information with different devices.

The visual and haptic rendering use the Reachin API. The VTPlayer rendering uses an API developped at the University of Metz and uses the [libusb](https://libusb.info/).

# MicoleBus
Technically, the MicoleBus is simply a buffered messages system with a regular expressions parser that could broadcast messages on all MicoleAgent. You can easily create you own "catcher/bind" using regular expressions. MicoleBus supports "catch parenthesis" so you don't need to extract information (string) from messages.

If you are not familiar with regular expression, you can visit [this page](http://en.wikipedia.org/wiki/Regular_expression).
