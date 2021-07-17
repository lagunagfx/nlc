/*

    Title:	LaunchControl

    Author:	Jorge Barrientos Poisson

    Version:	0.1

    Description:


    ChangeLog:
    2021.04.16 | 1st draft based on RtMidi's MIDI OUT sketches
    2021.04.17 | Improved modularity, yet the code is still in a single file
    2021.04.18 | Got rid of "init" messages so they're invisible to the end user

*/

#include <unistd.h> // provides delay()
#include "midi.h"

int main( int argc, char* argv[] ) {

    std::vector<unsigned char> nlcTemplateMsg,nlcPadMsg;

    midiOutConstructor();
    searchMidiPort("Launch Control MIDI");
    
    // DEBUG point
    std::cout << "external nlcPadMsg memory address: " << &nlcPadMsg << "\n";

    nlcSetTemplate(nlcTemplateMsg,PRESET);    
    
    //nlcClearPads(nlcTemplateMsg,PRESET);

    for ( int s=0 ; s<1 ; s++ ) {
	for ( int pad=0 ; pad<2 ; pad++ ) {
	    if ( s == pad )
		nlcSetPad( nlcPadMsg,PRESET,pad,AMBER_HI);
	    else
		nlcSetPad( nlcPadMsg,PRESET,pad,RED_LOW);
	} 	
	sleep(1);
    }

    // DEBUG point
    std::cout << "external nlcPadMsg memory address: " << &nlcPadMsg << "\n";

    //nlcClearPads(nlcTemplateMsg,PRESET);

        //midiout->closePort();

    midiOutDestructor();

    return 0;
}
