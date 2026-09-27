#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>

enum {
    TIMER_ID = 1001
};

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    wxStaticText* statusLabel;
    wxStaticText* countdownLabel;
    wxButton* abortButton;
    wxTimer* m_timer;

    int idleLimitSeconds;
    int countdownSeconds;
    int currentCountdown;
    bool isCountingDown;

    void OnTimerTick(wxTimerEvent& event);
    void OnAbort(wxCommandEvent& event);
};

#endif // MAIN_FRAME_H
