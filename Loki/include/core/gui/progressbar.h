#pragma once

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

namespace gui
{

class ProgressBar	: public Widget
{
public:

	// _Progress has a range of [0, 1].
	// If called when a reference is still in use,
	// the reference value will be discarded and 
	// not used again until the user calls SetProgressReference.
	void SetProgress( float _Progress );

	// Stores a pointer to a value that is queried whenever
	// the bar's current progress needs to be known.
	// Range of the value of the reference is still [0, 1].
	void SetProgressVariable( float* _Reference );
private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete ProgressBar objects.

	ProgressBar( Container* _Parent );
	ProgressBar();
	virtual ~ProgressBar();

	float m_ManualProgress;
	float* m_ProgressReference;
};

}

#endif