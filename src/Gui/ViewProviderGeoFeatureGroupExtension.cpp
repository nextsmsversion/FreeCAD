/***************************************************************************
 *   Copyright (c) 2011 Juergen Riegel <FreeCAD@juergen-riegel.net>        *
 *   Copyright (c) 2015 Alexander Golubev (Fat-Zer) <fatzer2@gmail.com>    *
 *   Copyright (c) 2016 Stefan Tröger <stefantroeger@gmx.net>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
#endif

#include "ViewProviderGeoFeatureGroupExtension.h"
#include <App/GeoFeatureGroupExtension.h>
#include <Inventor/nodes/SoGroup.h>

using namespace Gui;

EXTENSION_PROPERTY_SOURCE(Gui::ViewProviderGeoFeatureGroupExtension, Gui::ViewProviderGroupExtension)

ViewProviderGeoFeatureGroupExtension::ViewProviderGeoFeatureGroupExtension()
{
    initExtension(ViewProviderGeoFeatureGroupExtension::getExtensionClassTypeId());
    
    pcGroupChildren = new SoGroup();
    pcGroupChildren->ref();
}

ViewProviderGeoFeatureGroupExtension::~ViewProviderGeoFeatureGroupExtension()
{
    pcGroupChildren->unref();
    pcGroupChildren = 0;
}


std::vector<App::DocumentObject*> ViewProviderGeoFeatureGroupExtension::extensionClaimChildren3D(void) const {
    auto* ext = getExtendedViewProvider()->getObject()->getExtensionByType<App::GeoFeatureGroupExtension>();
    return ext ? ext->getGeoSubObjects() : std::vector<App::DocumentObject*>();
}

void ViewProviderGeoFeatureGroupExtension::extensionAttach(App::DocumentObject* pcObject)
{
    ViewProviderGroupExtension::extensionAttach(pcObject);
    getExtendedViewProvider()->addDisplayMaskMode(pcGroupChildren, "Group");
}

void ViewProviderGeoFeatureGroupExtension::extensionSetDisplayMode(const char* ModeName)
{
    if ( strcmp("Group",ModeName)==0 )
        getExtendedViewProvider()->setDisplayMaskMode("Group");

    ViewProviderGroupExtension::extensionSetDisplayMode( ModeName );
}

std::vector<std::string> ViewProviderGeoFeatureGroupExtension::extensionGetDisplayModes(void) const
{
    // get the modes of the father
    std::vector<std::string> StrList = ViewProviderGroupExtension::extensionGetDisplayModes();

    // add your own modes
    StrList.push_back("Group");

    return StrList;
}

void ViewProviderGeoFeatureGroupExtension::extensionUpdateData(const App::Property* prop)
{
    auto obj = getExtendedViewProvider()->getObject()->getExtensionByType<App::GeoFeatureGroupExtension>();
    if (obj && prop == &obj->Placement) {
        getExtendedViewProvider()->setTransformation ( obj->Placement.getValue().toMatrix() );
    } else {
        ViewProviderGroupExtension::extensionUpdateData ( prop );
    }
}


namespace Gui {
EXTENSION_PROPERTY_SOURCE_TEMPLATE(Gui::ViewProviderGeoFeatureGroupExtensionPython, Gui::ViewProviderGeoFeatureGroupExtension)

// explicit template instantiation
template class GuiExport ViewProviderExtensionPythonT<ViewProviderGeoFeatureGroupExtension>;
}
