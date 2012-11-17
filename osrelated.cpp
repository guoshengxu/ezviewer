/****************************************************************************
 * EZ Viewer
 * Copyright (C) 2012 huangezhao. CHINA.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#include "osrelated.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>


#ifdef Q_WS_WIN
#include <windows.h>
#include <ShellAPI.h>
#ifdef UNICODE
#   define _UNICODE
#endif // UNICODE
#include <tchar.h>  // need _UNICODE
#ifdef UNICODE
#   define QStringToTCHAR(x) (wchar_t*) x.utf16()   // need UNICODE
#else
#   define QStringToTCHAR(x) x.local8Bit().constData()
#endif // UNICODE
#endif // Q_WS_WIN


namespace OSRelated
{

void moveFile2Trash(const QString &filePath)
{
#ifdef Q_WS_WIN

    SHFILEOPSTRUCT FileOp;//����SHFILEOPSTRUCT�ṹ����;
    FileOp.hwnd = 0;
    FileOp.wFunc = FO_DELETE; //ִ���ļ�ɾ������;

    TCHAR buf[_MAX_PATH + 1];
    _tcscpy(buf, QStringToTCHAR(QDir::toNativeSeparators(filePath)));  // ����·��,end with '\0'.
    buf[_tcslen(buf)+1] = 0;  //! must end with 2 '\0'.

    FileOp.pFrom = buf; // source file path
    FileOp.pTo = NULL; //Ŀ���ļ�·��
    FileOp.fFlags |= FOF_ALLOWUNDO;//�˱�־ʹɾ���ļ����ݵ�Windows����վ
//    FileOp.fFlags &= ~FOF_ALLOWUNDO;    //ֱ��ɾ�������������վ

//    if(!needAsk)
        FileOp.fFlags |= FOF_NOCONFIRMATION;    //! ֱ��ɾ����������ȷ��

    // ����ļ���ռ�ã�����ֵ��32��1223
    if(SHFileOperation(&FileOp)) //���￪ʼɾ���ļ�
        QMessageBox::warning(0, QObject::tr("Delete Failed"),
                             QObject::tr("Delete file '%1' failed!")
                             .arg(QFileInfo(filePath).fileName()));

#else

    if(!QFile::remove(filePath))
        QMessageBox::warning(0, QObject::tr("Delete Failed"),
                             QObject::tr("Delete file '%1' failed!")
                             .arg(QFileInfo(filePath).fileName()));

#endif // Q_WS_WIN
}

}