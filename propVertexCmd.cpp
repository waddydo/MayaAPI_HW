//
// Copyright (C) Punnavat
// 
// File: propVertexCmd.cpp
//
// MEL Command: propVertex
//
// Author: Maya Plug-in Wizard 2.0
//

#include "propVertexCmd.h"

#include <stdio.h>
#include <math.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItCurveCV.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MItMeshVertex.h>

MStatus propVertex::doIt( const MArgList& args) 
{
	MStatus status;
    // Parse the arguments.
    for ( int i = 0; i < args.length(); i++ )
	{
        if ( MString( "-sh" ) == args.asString( i, &status )
            && MS::kSuccess == status )
        {
            MString tmp = args.asString( ++i, &status );
            if ( MS::kSuccess == status )
			   sh=tmp;			
        }
        else if ( MString( "-sz" ) == args.asString( i, &status )
            && MS::kSuccess == status )
        {
            double tmp = args.asDouble( ++i, &status );
            if ( MS::kSuccess == status )
			   sz=tmp;			
        }
		else if ( MString( "-u" ) == args.asString( i, &status )
            && MS::kSuccess == status )
        {
            return undoIt();		
        }
        else
        {
            MString msg = "Invalid flag: ";
            msg += args.asString( i );
            displayError( msg );
            return MS::kFailure;
        }
	}
	return redoIt();
}

MStatus propVertex::redoIt()
{
	
	MDagPath node;
    MObject component;
    MSelectionList list;
    MFnDagNode nodeFn;
    MString msg = "";
	MStatus stat;
	MString cmdbuild,cmdmove;

	MGlobal::getActiveSelectionList( list );
	for ( MItSelectionList listIter( list); !listIter.isDone(); listIter.next() )
	{
		listIter.getDagPath( node, component );
		MItMeshVertex  meshlter(  node,  component,  &stat  ); 
		if(  stat  == MS::kSuccess  ) 
		{ 
			msg  +=  node.fullPathName()  +  "\n"; 
			int vertCount  =  meshlter.count(); 
			msg  +=  MString("#  Vertices:  ")  +  vertCount  +  "\n"; 
			for(  ;  !meshlter.isDone();  meshlter.next()  ) 
			{ 
				MPoint  pt  =  meshlter.position(  MSpace::kWorld  );
				if(sh == "Sphere") cmdbuild = MString("poly") + sh + " -r " + sz;
				else if(sh == "Cube") cmdbuild = MString("poly") + sh + " -w " + sz + " -h " + sz + " -d " + sz;
				else if(sh == "Helix") cmdbuild = MString("poly") + sh + " -c 3" + " -h " + (2*sz) + " -w " + sz + " -r " + sz;
				cmdmove = MString("move ") + pt.x + " " + pt.y + " " + pt.z;
				MGlobal::executeCommand(cmdbuild);
				MGlobal::executeCommand(cmdmove);			
			}
		}
	}
	setResult(cmdbuild);


	return MS::kSuccess;
}

MStatus propVertex::undoIt()
//
//	Description:
//		implements undo for the MEL propVertex command.  
//
//		This method is called to undo a previous command of this type.  The 
//		system should be returned to the exact state that it was it previous 
//		to this command being executed.  That includes the selection state.
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
//
{

	// You can also display information to the command window via MGlobal
	//
    MGlobal::displayInfo( "propVertex command undone!\n" );

	return MS::kSuccess;
}

void* propVertex::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new propVertex();
}

propVertex::propVertex()
//
//	Description:
//		propVertex constructor
//
{}

propVertex::~propVertex()
//
//	Description:
//		propVertex destructor
//
{
}

bool propVertex::isUndoable() const
//
//	Description:
//		this method tells Maya this command is undoable.  It is added to the 
//		undo queue if it is.
//
//	Return Value:
//		true if this command is undoable.
//
{
	return true;
}
