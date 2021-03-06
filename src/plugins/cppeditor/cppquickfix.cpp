/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "cppquickfix.h"
#include "cppeditor.h"
#include "cppquickfixassistant.h"

#include <AST.h>
#include <TranslationUnit.h>
#include <Token.h>

#include <cplusplus/ASTPath.h>
#include <cplusplus/CppDocument.h>
#include <cplusplus/ResolveExpression.h>
#include <cplusplus/Overview.h>
#include <cplusplus/TypeOfExpression.h>
#include <cplusplus/DependencyTable.h>
#include <cplusplus/CppRewriter.h>

#include <cpptools/cpprefactoringchanges.h>

#include <QTextBlock>
#include <QFileInfo>

using namespace CppEditor;
using namespace CppEditor::Internal;
using namespace CppTools;
using namespace TextEditor;
using namespace CPlusPlus;
using namespace Utils;

CppQuickFixOperation::CppQuickFixOperation(
        const QSharedPointer<const CppQuickFixAssistInterface> &interface, int priority)
    : QuickFixOperation(priority)
    , m_interface(interface)
{}

CppQuickFixOperation::~CppQuickFixOperation()
{}

void CppQuickFixOperation::perform()
{
    CppRefactoringChanges refactoring(m_interface->snapshot());
    CppRefactoringFilePtr current = refactoring.file(fileName());

    performChanges(current, refactoring);
}

const CppQuickFixAssistInterface *CppQuickFixOperation::assistInterface() const
{
    return m_interface.data();
}

QString CppQuickFixOperation::fileName() const
{
    return m_interface->document()->fileName();
}

CppQuickFixFactory::CppQuickFixFactory()
{
}

CppQuickFixFactory::~CppQuickFixFactory()
{
}

QList<QuickFixOperation::Ptr> CppQuickFixFactory::matchingOperations(
    const QSharedPointer<const TextEditor::IAssistInterface> &interface)
{
    QSharedPointer<const CppQuickFixAssistInterface> cppInterface =
            interface.staticCast<const CppQuickFixAssistInterface>();
    if (cppInterface->path().isEmpty())
        return QList<QuickFixOperation::Ptr>();
    return match(cppInterface);
}

QList<CppQuickFixOperation::Ptr> CppQuickFixFactory::singleResult(CppQuickFixOperation *operation)
{
    QList<CppQuickFixOperation::Ptr> result;
    result.append(CppQuickFixOperation::Ptr(operation));
    return result;
}

QList<CppQuickFixOperation::Ptr> CppQuickFixFactory::noResult()
{
    return QList<CppQuickFixOperation::Ptr>();
}
