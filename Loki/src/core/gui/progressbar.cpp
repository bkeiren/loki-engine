#include "widget.h"
#include "progressbar.h"

namespace gui
{

ProgressBar::ProgressBar( Container* _Parent )	:
	Widget(_Parent),
	m_ManualProgress(0.0f),
	m_ProgressReference(&m_ManualProgress)
{

}

ProgressBar::ProgressBar()	:
	m_ManualProgress(0.0f),
	m_ProgressReference(&m_ManualProgress)
{

}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::SetProgress( float _Progress )
{
	m_ManualProgress = _Progress;
	m_ProgressReference = &m_ManualProgress;
}

void ProgressBar::SetProgressVariable( float* _Reference )
{
	m_ProgressReference = _Reference;
}

}