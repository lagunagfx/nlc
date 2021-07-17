#include <iostream>
#include <cstdlib>
// #include <unistd.h> // delay
#include <vector>

#include "RtMidi.h"

class midiDevice {
    public:
	std::string midiPortName;

    private:
	RtMidiOut* othermidiout = nullptr;

	void startMidiOut() {
	    try {
		othermidiout = new RtMidiOut();
	    }
	    catch (RtMidiError &error ) {
		error.printMessage();
		exit ( EXIT_FAILURE );
	    }	    
	}

	void stopMidiOut() {
	    othermidiout->closePort();
	    delete othermidiout;
	}
};

RtMidiOut *midiout = 0;

void midiOutConstructor() {

    try {
	midiout = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
	error.printMessage();
	exit ( EXIT_FAILURE );
    }
}

void midiOutDestructor() {
    midiout-> closePort();
    delete midiout;
}

void searchMidiPort( std::string myPortName ) {

    std::string midiPortName;
    
    unsigned int port,numOfPorts;
    std::size_t strPos;

    numOfPorts = midiout->getPortCount();

    for ( port=0 ; port<numOfPorts ; port++ ) {
	
	try {
	    midiPortName = midiout->getPortName(port);
	}
	catch ( RtMidiError &error ) {
	    error.printMessage();
	    midiOutDestructor();
	    exit(0);
	}
	
	strPos = midiPortName.find(myPortName);
	
	if ( strPos != std::string::npos ) {
	    std::cout << "  \u001b[32mCheck:\u001b[0m " << myPortName << " port found (" << port << ")\n";
	    midiout->openPort(port);
	}
    }

    if ( ! midiout->isPortOpen() ) {
	std::cout << "\u001b[31mError:\u001b[0m\tMIDI port \"" << myPortName << "\" not found or not available.\n";
	exit(0);
    }
}

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

void nlcSetTemplate(std::vector<unsigned char>& midiMsg, unsigned char templateNum ) {

    if ( midiMsg.empty() ) {

	std::cout << "\u001b[33mWarning:\u001b[0m The SysEx template message is not yet defined.\n";

	midiMsg.push_back(0xF0); // SysEx Start Msg
	midiMsg.push_back(0x00); // Manufacturer's ID (3 bytes)
	midiMsg.push_back(0x20);
	midiMsg.push_back(0x29); 
	midiMsg.push_back(0x02); // Change current template (3 bytes)
	midiMsg.push_back(0x0A);
	midiMsg.push_back(0x77);
	midiMsg.push_back(0x00); // Template (0 to 15)
	midiMsg.push_back(0xF7); // SysEx End Msg
    }

    midiMsg[7] = templateNum; // Set the 8th byte of the message (preset number)
    midiout->sendMessage( &midiMsg ); 
}

void nlcSetPad( std::vector<unsigned char> midiMsg , unsigned char preset, unsigned char button, unsigned char value ) {

    // DEBUG point: Are we replicating the message?
    std::cout << "internal midiMsg memory address: " << &midiMsg << "\n";

    if ( midiMsg.empty() ) {
	
	std::cout << "\u001b[33mWarning:\u001b[0m The SySex pad message is not yet defined.\n";

	midiMsg.push_back(0xF0); // SysEx Start Message
	midiMsg.push_back(0x00); // Manufacturer's ID (3 bytes)
	midiMsg.push_back(0x20);
	midiMsg.push_back(0x29); 
	midiMsg.push_back(0x02); // Change pad/button state (3 bytes)
	midiMsg.push_back(0x0A);
	midiMsg.push_back(0x78);
	midiMsg.push_back(0x00); // User Template
	midiMsg.push_back(0x00); // Pad (0-7) or Button (8-11) 
	midiMsg.push_back(0x00); // Value
	midiMsg.push_back(0xF7); // SysEx End Message
    }
    
    midiMsg[7] = preset;    // Set the 8th byte (preset number)
    midiMsg[8] = button;    // Set the 9th byte (pad/button position)
    midiMsg[9] = value;	    // Set the 10th byte (value)
    
    midiout->sendMessage( &midiMsg ); 
}

void nlcClearPads( std::vector <unsigned char> midiMsg , unsigned char preset ) {
    for ( int p=0 ; p<8 ; p++ ) {
	nlcSetPad( midiMsg, preset, p, OFF );
    }
} 
