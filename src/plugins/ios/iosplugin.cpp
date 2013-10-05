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

#include "iosplugin.h"

#include "iosconstants.h"
#include "iosconfigurations.h"
#include "iosdevicefactory.h"
#include "iossimulator.h"
#include "iossimulatorfactory.h"
#include "iosconfigurations.h"
#include "iosmanager.h"
#include "iosrunfactories.h"
#include "iossettingspage.h"
#include "iosqtversionfactory.h"
#include "iosbuildstep.h"
#include "iosdeploystepfactory.h"
#include "iosdeployconfiguration.h"
#include <projectexplorer/kitmanager.h>
#include <qtsupport/qtversionmanager.h>

#include <QtPlugin>

#include <projectexplorer/devicesupport/devicemanager.h>

namespace Ios {

IosPlugin::IosPlugin()
{ }

bool IosPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    Internal::IosConfigurations::instance();

    addAutoReleasedObject(new Internal::IosRunControlFactory);
    addAutoReleasedObject(new Internal::IosRunConfigurationFactory);
    addAutoReleasedObject(new Internal::IosSettingsPage);
    addAutoReleasedObject(new Internal::IosQtVersionFactory);
    addAutoReleasedObject(new Internal::IosDeviceFactory);
    addAutoReleasedObject(new Internal::IosSimulatorFactory);
    addAutoReleasedObject(new Internal::IosBuildStepFactory);
    addAutoReleasedObject(new Internal::IosDeployStepFactory);
    addAutoReleasedObject(new Internal::IosDeployConfigurationFactory);
    //ProjectExplorer::KitManager::instance()->registerKitInformation(new Internal::IosGdbServerKitInformation);
    return true;
}

void IosPlugin::extensionsInitialized()
{
    connect(ProjectExplorer::KitManager::instance(), SIGNAL(kitsLoaded()),
            this, SLOT(kitsRestored()));
}

void IosPlugin::kitsRestored()
{
    disconnect(ProjectExplorer::KitManager::instance(), SIGNAL(kitsLoaded()),
               this, SLOT(kitsRestored()));
    Internal::IosConfigurations::instance().updateAutomaticKitList();
    connect(QtSupport::QtVersionManager::instance(),
            SIGNAL(qtVersionsChanged(QList<int>,QList<int>,QList<int>)),
            &Internal::IosConfigurations::instance(),
            SLOT(updateAutomaticKitList()));
}

} // namespace Ios

Q_EXPORT_PLUGIN(Ios::IosPlugin)