//#include <cstdlib>
//#include <vector>

/*
 *  MIDI port handling using the RtMidi library
*/

#include "RtMidi.h"

class midiDevice {
    public:
	std::string midiPortName;

    private:
	RtMidiOut* othermidiout;

	void startMidiOut();
	void stopMidiOut();

};

void midiOutConstructor(); 

void midiOutDestructor();

void searchMidiPort( std::string myPortName );

/*
*  Novation LaunchControl section
*
*  Defining System Exclusice (SysEx) messages
*
*  SysEx structure:
*
*  Description:			    Bytes:
*   - SysEx Message Start	    0xF0
*   - Novation Manufacturer's ID    0x00 0x20 0x29
*   + Message types
*	- Set preset/template	    0x02 0x0A 0x77
*	- Set pad/button state	    0x02 0x0A 0x78
*   - SysEx Message End		    0xF7
*/

#define OFF	    0x0C
#define RED_LOW	    0x0D
#define RED_MID	    0x0E
#define RED_HI	    0x0F
#define GREEN_LOW   0x1C
#define GREEN_MID   0x2C
#define GREEN_HI    0x3C
#define AMBER_LOW   0x1D
#define AMBER_MID   0x2E
#define AMBER_HI    0x3F
#define YELLOW	    0x3E

#define PRESET	    0x0F

void nlcSetTemplate(std::vector<unsigned char>& midiMsg, unsigned char templateNum );

void nlcSetPad( std::vector<unsigned char> midiMsg , unsigned char preset, unsigned char button, unsigned char value );

void nlcClearPads( std::vector <unsigned char> midiMsg , unsigned char preset );
