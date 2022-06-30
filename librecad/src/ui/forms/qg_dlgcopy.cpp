/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/
#include<cmath>
#include "rs_math.h"
#include "qg_dlgcopy.h"

#include "rs_settings.h"
#include "rs_modification.h"

/*
    Constructs a QG_DlgCopy as a child of 'parent', with the
    name 'name' and widget flags set to 'f'.

    The dialog will by default be modeless, unless you set 'modal' to
    true to construct a modal dialog.
*/
QG_DlgCopy::QG_DlgCopy(QWidget *parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    init();
}

/*
    Destroys the object and frees any allocated resources
*/
QG_DlgCopy::~QG_DlgCopy()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
    Sets the strings of the subwidgets using the current
    language.
*/
void QG_DlgCopy::languageChange()
{
    retranslateUi(this);
}

void QG_DlgCopy::init()
{
    RS_SETTINGS->beginGroup("/Modify");
    copies = RS_SETTINGS->readEntry("/Copies", "1");
    useCurrentLayer = (bool)RS_SETTINGS->readNumEntry("/CopyUseCurrentLayer", 0);
    useCurrentAttributes = (bool)RS_SETTINGS->readNumEntry("/CopyUseCurrentAttributes", 0);
    RS_SETTINGS->endGroup();

    leNumber->setText(copies);
}

void QG_DlgCopy::destroy()
{
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/Copies", leNumber->text());
    RS_SETTINGS->endGroup();
}

void QG_DlgCopy::setData(RS_CopyData *d)
{
    data = d;
}

void QG_DlgCopy::updateData()
{
    bool ok;
    data->number = static_cast<int>(fabs(RS_Math::eval(leNumber->text(), &ok)));
    if (!ok || data->number < 1 || data->number > 100)
    {
        if(ok && data->number > 100)
        {
            data->number = -100; //max number of copies set to 100
        }
        else
        {
            data->number = -1;
        }
        leNumber->setText(QString::number(abs(data->number)));
    }
}

