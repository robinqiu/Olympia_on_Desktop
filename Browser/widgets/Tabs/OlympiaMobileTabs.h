/*
 * Copyright (c) 2011, Torch Mobile (Beijing) Co. Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that
 * the following conditions are met:
 *
 *  -- Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *  -- Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *  -- Neither the name of the Torch Mobile (Beijing) Co. Ltd. nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef OlympiaMobileTabs_h
#define OlympiaMobileTabs_h

#include "Tabs.h"

class QStackedWidget;
namespace Olympia {

namespace Browser {

class OlympiaMobileTabs : public Tabs
{
    Q_OBJECT

public:
    OlympiaMobileTabs(QWidget* parent);
    virtual ~OlympiaMobileTabs();
    virtual QWidget* getCurrentWidget() const;
    int count() const;
    virtual void appendWidget(QWidget* w);
    virtual void setCurrentWidget(QWidget* w);
    virtual void setFixedShape(int w, int h);
    virtual void setPosition(int x, int y, int w, int h);
    virtual QWidget* getWidget(int index);
    virtual QSize getContentsTabSize();
public slots:
    virtual void slotRemoveWidget(int index);
    virtual void slotTabSwitchedTo(int index);
    virtual void slotSetTitle(const QString&, const QString&);
    virtual void slotSetTabIcon(const QString&, const QString&, const QIcon& icon);
    virtual QString slotGetCurPageUrl();
private slots:
    void slotCurrentChanged(int index);    
    

private:
    QStackedWidget* m_tabs;
    int m_oldWidgetIndex;
};

} // Browser
} // Olympia
#endif // OlympiaMobileTabs_h

