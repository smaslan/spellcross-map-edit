#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/popupwin.h>
#include <wx/toolbar.h>
#include <wx/weakref.h>

#include <algorithm>
#include <memory>
#include <vector>

// Reloaded-style application chrome shared by the editor's menu model,
// custom menu bar and fullscreen shortcut.
namespace editor_chrome
{
    inline wxColour Background() { return wxColour(8, 24, 7); }
    inline wxColour Grid() { return wxColour(19, 43, 14); }
    inline wxColour Highlight() { return wxColour(24, 51, 15); }
    inline wxColour Accent() { return wxColour(61, 207, 32); }
    inline wxColour Text() { return wxColour(62, 224, 25); }

    inline wxFont MenuFont()
    {
        return wxFont(wxFontInfo(11).Family(wxFONTFAMILY_TELETYPE).Bold());
    }

    inline bool& Fullscreen()
    {
        static bool enabled = false;
        return enabled;
    }

    inline bool& ChangingMode()
    {
        static bool changing = false;
        return changing;
    }

    inline int FullscreenId()
    {
        static const int id = wxWindow::NewControlId();
        return id;
    }

    inline void PaintGrid(wxDC& dc, const wxSize& size, int spacing)
    {
        dc.SetPen(wxPen(Grid()));
        for (int x = spacing; x < size.x; x += spacing)
            dc.DrawLine(x, 0, x, size.y);
        for (int y = spacing; y < size.y; y += spacing)
            dc.DrawLine(0, y, size.x, y);
    }

    inline void ApplyFullscreen(wxFrame* frame)
    {
        if (ChangingMode() || frame->IsBeingDeleted() || !frame->IsShown() ||
            frame->IsFullScreen() == Fullscreen())
            return;

        struct TransitionGuard
        {
            TransitionGuard() { ChangingMode() = true; }
            ~TransitionGuard() { ChangingMode() = false; }
        } guard;

        const bool requested = Fullscreen();
        const wxWeakRef<wxWindow> previousFocus(wxWindow::FindFocus());
        const bool changed = frame->ShowFullScreen(requested,
            wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOSTATUSBAR);
        if (!changed)
            Fullscreen() = frame->IsFullScreen();

        frame->Layout();
        frame->Refresh();
        if (previousFocus && wxGetTopLevelParent(previousFocus.get()) == frame &&
            previousFocus->IsShownOnScreen() && previousFocus->IsEnabled())
            previousFocus->SetFocus();
        else
            frame->SetFocus();
    }

    inline void RequestFullscreen(wxFrame* frame)
    {
        if (ChangingMode())
            return;
        const wxWeakRef<wxFrame> window(frame);
        frame->CallAfter([window]() {
            if (window)
                ApplyFullscreen(window.get());
        });
    }

    inline void AddFullscreen(wxMenu* menu)
    {
        menu->AppendSeparator();
        menu->AppendCheckItem(FullscreenId(), "&Fullscreen\tAlt+Enter");
    }

    class Popup final : public wxPopupTransientWindow
    {
        wxFrame* m_frame;
        wxWeakRef<wxWindow> m_previousFocus;
        std::vector<wxMenu*> m_stack;
        std::vector<wxMenuItem*> m_rows;
        int m_selected = -1;
        bool m_closing = false;
        int m_rowHeight;
        int m_padding;

        void Rebuild()
        {
            m_rows.clear();
            if (m_stack.size() > 1)
                m_rows.push_back(nullptr);
            wxClientDC dc(this);
            dc.SetFont(GetFont());
            int width = FromDIP(260);
            for (auto* item : m_stack.back()->GetMenuItems())
            {
                m_rows.push_back(item);
                const auto label = item->GetItemLabel();
                width = std::max(width, dc.GetTextExtent(label).x + FromDIP(96));
            }
            SetClientSize(width,
                static_cast<int>(m_rows.size()) * m_rowHeight + 2 * m_padding);
            m_selected = -1;
            Refresh();
        }

        bool Selectable(int row) const
        {
            return row >= 0 && row < static_cast<int>(m_rows.size()) &&
                (!m_rows[row] ||
                    (!m_rows[row]->IsSeparator() && m_rows[row]->IsEnabled()));
        }

        void ReturnFocus()
        {
            if (m_previousFocus && m_previousFocus->IsShownOnScreen() &&
                m_previousFocus->IsEnabled())
                m_previousFocus->SetFocus();
        }

        void ClosePopup()
        {
            if (m_closing)
                return;
            m_closing = true;
            Dismiss();
            ReturnFocus();
            Destroy();
        }

        void Activate()
        {
            if (!Selectable(m_selected))
                return;
            auto* item = m_rows[m_selected];
            if (!item)
            {
                m_stack.pop_back();
                Rebuild();
            }
            else if (item->GetSubMenu())
            {
                m_stack.push_back(item->GetSubMenu());
                Rebuild();
            }
            else
            {
                if (item->IsCheckable())
                    item->Check(item->IsRadio() || !item->IsChecked());
                auto* command = new wxCommandEvent(wxEVT_MENU, item->GetId());
                command->SetInt(item->IsCheckable() && item->IsChecked());
                wxQueueEvent(m_frame, command);
                ClosePopup();
            }
        }

        void Paint(wxPaintEvent&)
        {
            wxAutoBufferedPaintDC dc(this);
            dc.SetBackground(wxBrush(Background()));
            dc.Clear();
            dc.SetFont(GetFont());
            PaintGrid(dc, GetClientSize(), FromDIP(12));
            dc.SetPen(wxPen(wxColour(121, 128, 115)));
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRectangle(GetClientRect());

            for (int row = 0; row < static_cast<int>(m_rows.size()); ++row)
            {
                const int y = m_padding + row * m_rowHeight;
                auto* item = m_rows[row];
                if (item && item->IsSeparator())
                {
                    dc.SetPen(wxPen(wxColour(65, 73, 77)));
                    dc.DrawLine(FromDIP(12), y + m_rowHeight / 2,
                        GetClientSize().x - FromDIP(12), y + m_rowHeight / 2);
                    continue;
                }
                if (row == m_selected && Selectable(row))
                {
                    dc.SetPen(*wxTRANSPARENT_PEN);
                    dc.SetBrush(wxBrush(Highlight()));
                    dc.DrawRectangle(FromDIP(3), y,
                        GetClientSize().x - FromDIP(6), m_rowHeight);
                }

                dc.SetTextForeground(!item || item->IsEnabled()
                    ? Text() : wxColour(115, 120, 124));
                const wxString title = item ? item->GetItemLabelText()
                    : wxString("Back");
                const int textY = y + (m_rowHeight - dc.GetTextExtent(title).y) / 2;
                dc.DrawText(title, FromDIP(30), textY);
                if (!item)
                    dc.DrawText("<", FromDIP(12), textY);
                if (item && item->IsCheckable() && item->IsChecked())
                {
                    dc.SetPen(wxPen(Accent()));
                    dc.SetBrush(wxBrush(Accent()));
                    dc.DrawCircle(FromDIP(15), y + m_rowHeight / 2, FromDIP(3));
                }
                if (item && item->GetSubMenu())
                    dc.DrawText(">", GetClientSize().x - FromDIP(20), textY);
                else if (item && item->GetItemLabel().Contains("\t"))
                {
                    const auto shortcut = item->GetItemLabel().AfterFirst('\t');
                    dc.SetTextForeground(Accent());
                    dc.DrawText(shortcut,
                        GetClientSize().x - FromDIP(16) - dc.GetTextExtent(shortcut).x,
                        textY);
                }
            }
        }

        void Key(wxKeyEvent& event)
        {
            const int key = event.GetKeyCode();
            if (event.AltDown() &&
                (key == WXK_RETURN || key == WXK_NUMPAD_ENTER))
            {
                wxQueueEvent(m_frame,
                    new wxCommandEvent(wxEVT_MENU, FullscreenId()));
                ClosePopup();
            }
            else if (key == WXK_ESCAPE)
                ClosePopup();
            else if (key == WXK_UP || key == WXK_DOWN)
            {
                const int count = static_cast<int>(m_rows.size());
                const int direction = key == WXK_DOWN ? 1 : -1;
                for (int step = 0; step < count; ++step)
                {
                    m_selected = (m_selected + direction + count) % count;
                    if (Selectable(m_selected))
                        break;
                }
                Refresh();
            }
            else if (key == WXK_LEFT && m_stack.size() > 1)
            {
                m_stack.pop_back();
                Rebuild();
            }
            else if (key == WXK_RETURN || key == WXK_NUMPAD_ENTER ||
                key == WXK_SPACE || key == WXK_RIGHT)
                Activate();
            else
                event.Skip();
        }

    protected:
        void OnDismiss() override { ClosePopup(); }

    public:
        Popup(wxFrame* frame, wxMenu* menu, wxWindow* anchor)
            : wxPopupTransientWindow(frame, wxBORDER_NONE),
              m_frame(frame), m_previousFocus(wxWindow::FindFocus()),
              m_stack{menu}, m_rowHeight(FromDIP(30)), m_padding(FromDIP(5))
        {
            SetBackgroundStyle(wxBG_STYLE_PAINT);
            SetFont(anchor->GetFont());
            Rebuild();
            Bind(wxEVT_PAINT, &Popup::Paint, this);
            Bind(wxEVT_CHAR_HOOK, &Popup::Key, this);
            Bind(wxEVT_MOTION, [this](wxMouseEvent& event) {
                m_selected = (event.GetY() - m_padding) / m_rowHeight;
                if (event.GetY() < m_padding)
                    m_selected = -1;
                Refresh();
            });
            Bind(wxEVT_LEFT_UP, [this](wxMouseEvent&) { Activate(); });
            Position(anchor->ClientToScreen(wxPoint(0, 0)),
                wxSize(0, anchor->GetSize().y));
            wxPopupTransientWindow::Popup();
            SetFocus();
        }
    };

    class Bar final : public wxToolBar
    {
        std::unique_ptr<wxMenuBar> m_model;

        void Open(size_t index, wxWindow* anchor)
        {
            if (!m_model->IsEnabledTop(index))
                return;
            if (auto* item = m_model->FindItem(FullscreenId()))
                item->Check(static_cast<wxFrame*>(GetParent())->IsFullScreen());
            new Popup(static_cast<wxFrame*>(GetParent()),
                m_model->GetMenu(index), anchor);
        }

        void AddAccelerators(wxMenu* menu,
            std::vector<wxAcceleratorEntry>& entries)
        {
            for (auto* item : menu->GetMenuItems())
            {
                if (item->GetSubMenu())
                    AddAccelerators(item->GetSubMenu(), entries);
                else
                {
                    std::unique_ptr<wxAcceleratorEntry> entry(item->GetAccel());
                    if (entry)
                    {
                        entry->Set(entry->GetFlags(), entry->GetKeyCode(),
                            item->GetId());
                        entries.push_back(*entry);
                    }
                }
            }
        }

    public:
        Bar(wxFrame* frame, wxMenuBar* model)
            : wxToolBar(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                wxTB_HORIZONTAL | wxTB_FLAT | wxTB_NODIVIDER | wxBORDER_NONE),
              m_model(model)
        {
            SetFont(MenuFont());
            SetBackgroundColour(Background());
            SetMargins(FromDIP(4), FromDIP(2));
            std::vector<wxAcceleratorEntry> entries;

            for (size_t index = 0; index < model->GetMenuCount(); ++index)
            {
                const auto label = model->GetMenuLabelText(index);
                wxClientDC dc(this);
                dc.SetFont(GetFont());
                auto* button = new wxControl(this, wxID_ANY,
                    wxDefaultPosition,
                    wxSize(dc.GetTextExtent(label).x + FromDIP(32), FromDIP(32)),
                    wxBORDER_NONE | wxWANTS_CHARS);
                button->SetFont(GetFont());
                button->SetBackgroundStyle(wxBG_STYLE_PAINT);
                button->SetName(label);
                button->SetToolTip(label);
                button->Bind(wxEVT_PAINT,
                    [this, button, label, index](wxPaintEvent&) {
                        wxAutoBufferedPaintDC paint(button);
                        const bool hover = button->GetClientRect().Contains(
                            button->ScreenToClient(wxGetMousePosition()));
                        paint.SetBackground(wxBrush(
                            hover ? Highlight() : Background()));
                        paint.Clear();
                        PaintGrid(paint, button->GetClientSize(), FromDIP(12));
                        paint.SetFont(GetFont());
                        paint.SetTextForeground(m_model->IsEnabledTop(index)
                            ? (hover ? wxColour(227, 233, 24) : Accent())
                            : wxColour(76, 104, 68));
                        const auto extent = paint.GetTextExtent(label);
                        const auto size = button->GetClientSize();
                        paint.DrawText(label, (size.x - extent.x) / 2,
                            (size.y - extent.y) / 2);
                        paint.SetPen(wxPen(Accent()));
                        paint.DrawLine(0, size.y - 1, size.x, size.y - 1);
                    });
                button->Bind(wxEVT_ENTER_WINDOW,
                    [button](wxMouseEvent&) { button->Refresh(); });
                button->Bind(wxEVT_LEAVE_WINDOW,
                    [button](wxMouseEvent&) { button->Refresh(); });
                button->Bind(wxEVT_LEFT_DOWN,
                    [this, index, button](wxMouseEvent&) {
                        Open(index, button);
                    });
                button->Bind(wxEVT_KEY_DOWN,
                    [this, index, button](wxKeyEvent& event) {
                        if (event.GetKeyCode() == WXK_SPACE ||
                            event.GetKeyCode() == WXK_RETURN ||
                            event.GetKeyCode() == WXK_DOWN)
                            Open(index, button);
                        else
                            event.Skip();
                    });
                AddControl(button);
                AddAccelerators(model->GetMenu(index), entries);

                const auto rawLabel = model->GetMenuLabel(index);
                const int mnemonic = rawLabel.Find('&');
                if (mnemonic != wxNOT_FOUND &&
                    mnemonic + 1 < static_cast<int>(rawLabel.length()))
                {
                    const int id = wxWindow::NewControlId();
                    entries.emplace_back(wxACCEL_ALT,
                        wxToupper(rawLabel[mnemonic + 1]), id);
                    frame->Bind(wxEVT_MENU,
                        [this, index, button](wxCommandEvent&) {
                            Open(index, button);
                        }, id);
                }
            }

            Realize();
            frame->SetToolBar(this);
            frame->SetAcceleratorTable(wxAcceleratorTable(
                static_cast<int>(entries.size()), entries.data()));
            frame->Bind(wxEVT_MENU, [frame](wxCommandEvent&) {
                Fullscreen() = !Fullscreen();
                RequestFullscreen(frame);
            }, FullscreenId());
        }

        wxMenuBar* Model() const { return m_model.get(); }
    };

    inline wxMenuBar* Model(wxFrame* frame)
    {
        auto* bar = dynamic_cast<Bar*>(frame->GetToolBar());
        return bar ? bar->Model() : nullptr;
    }

    inline void Install(wxFrame* frame, wxMenuBar* model)
    {
        new Bar(frame, model);
    }
}
