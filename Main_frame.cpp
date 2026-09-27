#include "main_frame.h"
#include "shutdown_engine.h"

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Smart Auto-Shutdown System", wxDefaultPosition, wxSize(400, 250)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    statusLabel = new wxStaticText(panel, wxID_ANY, "Status: Monitoring PC Idle State...", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont font = statusLabel->GetFont();
    font.SetPointSize(12);
    statusLabel->SetFont(font);

    countdownLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont largeFont = countdownLabel->GetFont();
    largeFont.SetPointSize(24);
    largeFont.SetWeight(wxFONTWEIGHT_BOLD);
    countdownLabel->SetFont(largeFont);

    abortButton = new wxButton(panel, wxID_ANY, "Abort Shutdown Sequence", wxDefaultPosition, wxSize(200, 40));
    abortButton->Disable(); 

    sizer->Add(statusLabel, 0, wxALL | wxALIGN_CENTER, 15);
    sizer->Add(countdownLabel, 0, wxALL | wxALIGN_CENTER, 15);
    sizer->Add(abortButton, 0, wxALL | wxALIGN_CENTER, 15);
    panel->SetSizer(sizer);

    abortButton->Bind(wxEVT_BUTTON, &MainFrame::OnAbort, this);

    idleLimitSeconds = 600; // 10 minutes tracking threshold
    countdownSeconds = 600; // 10 minutes visual ticking window
    currentCountdown = countdownSeconds;
    isCountingDown = false;

    m_timer = new wxTimer(this, TIMER_ID);
    Bind(wxEVT_TIMER, &MainFrame::OnTimerTick, this, TIMER_ID);
    m_timer->Start(1000); 
}

void MainFrame::OnTimerTick(wxTimerEvent& event) {
    if (!isCountingDown) {
        LASTINPUTINFO lii;
        lii.cbSize = sizeof(LASTINPUTINFO);
        
        if (GetLastInputInfo(&lii)) {
            DWORD currentTick = GetTickCount();
            DWORD elapsedTicks = currentTick - lii.dwTime;
            int idleSeconds = elapsedTicks / 1000;

            statusLabel->SetLabel(wxString::Format("System Idle for: %d / %d seconds", idleSeconds, idleLimitSeconds));

            if (idleSeconds >= idleLimitSeconds) {
                isCountingDown = true;
                abortButton->Enable();
                this->Raise(); 
            }
        }
    } else {
        statusLabel->SetLabel("WARNING: Inactivity Detected. Shutting down!");
        
        int mins = currentCountdown / 60;
        int secs = currentCountdown % 60;
        countdownLabel->SetLabel(wxString::Format("%02d:%02d", mins, secs));

        if (currentCountdown <= 0) {
            m_timer->Stop();
            countdownLabel->SetLabel("Shutting down...");
            ForceSystemShutdown();
        }
        currentCountdown--;
    }
}

void MainFrame::OnAbort(wxCommandEvent& event) {
    isCountingDown = false;
    currentCountdown = countdownSeconds; 
    countdownLabel->SetLabel("");
    statusLabel->SetLabel("Status: Monitoring PC Idle State...");
    abortButton->Disable();
    wxMessageBox("Shutdown sequence successfully aborted by user.", "System Notification", wxOK | wxICON_INFORMATION);
}
