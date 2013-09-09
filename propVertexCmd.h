#ifndef _propVertexCmd
#define _propVertexCmd
//
// Copyright (C) Punnavat
// 
// File: propVertexCmd.h
//
// MEL Command: propVertex
//
// Author: Maya Plug-in Wizard 2.0
//

#include <maya/MPxCommand.h>

class MArgList;

class propVertex : public MPxCommand
{

public:
				propVertex();
	virtual		~propVertex();

	MStatus		doIt( const MArgList& );
	MStatus		redoIt();
	MStatus		undoIt();
	bool		isUndoable() const;

	static		void* creator();
	MString		sh;
	double		sz;

private:
	// Store the data you will need to undo the command here
	//
};

#endif
