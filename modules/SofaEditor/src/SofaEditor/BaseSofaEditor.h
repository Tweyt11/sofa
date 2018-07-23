/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/*****************************************************************************
 * Contributors:
 *     - damien.marchal@univ-lille1.fr
 ****************************************************************************/
#ifndef SOFAEDITOR_BASESOFAEDITOR_H

#include <vector>
#include <string>
#include <limits>

#include <SofaEditor/config.h>


namespace sofaeditor
{
    /// The SofaEditorStatic is holding information that sofa editors want to hold
    /// so they can be used to communicate this information to sofa component or
    /// controllers.
    class SOFAEDITOR_API SofaEditorState
    {
    public:
        std::string editorname ;

        SofaEditorState(const std::string& name="");
        ~SofaEditorState();

        void setSelectionFromPath(const std::vector<std::string>& paths );
        const std::vector<std::string>& getSelection() const ;

    private:

        std::vector<std::string> m_currentSelection;
    };

    /// Hold the editors properties. Each editors can attach its
    /// properties that can be queries by the client side.
    class SofaEditor
    {
    public:
        typedef size_t ID;

        /// Indicates an invalid ID.
        const static ID InvalidID;

        /// Returns the first editor matching the give editor's name
        /// returns InvalidId is none is found.
        static ID getIdFromEditorName(const std::string& s);

        static ID createId(const SofaEditorState* s=nullptr);
        static bool attachState(ID editorId, const SofaEditorState* s);
        static const SofaEditorState* getState(ID editorId=0);
    private:
        static std::vector<const SofaEditorState*> s_editorsstate ;
    };
}


#endif // SOFAEDITOR_BASESOFAEDITOR_H
